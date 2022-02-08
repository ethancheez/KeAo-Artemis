/*******************************************************************************
  Main Source File

  Company:
    Microchip Technology Inc.

  File Name:
    main.c

  Summary:
    This file contains the "main" function for a project.

  Description:
    This file contains the "main" function for a project.  The
    "main" function calls the "SYS_Initialize" function to initialize the state
    machines of all modules in the system
 *******************************************************************************/

// DOM-IGNORE-BEGIN
/*******************************************************************************
* Copyright (C) 2018 Microchip Technology Inc. and its subsidiaries.
*
* Subject to your compliance with these terms, you may use Microchip software
* and any derivatives exclusively with Microchip products. It is your
* responsibility to comply with third party license terms applicable to your
* use of third party software (including open source software) that may
* accompany Microchip software.
*
* THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
* EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
* WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
* PARTICULAR PURPOSE.
*
* IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
* INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
* WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
* BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
* FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
* ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
* THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
*******************************************************************************/
// DOM-IGNORE-END

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include <stddef.h>                     // Defines NULL
#include <stdbool.h>                    // Defines true
#include <stdlib.h>                     // Defines EXIT_FAILURE
#include "definitions.h"                // SYS function prototypes
#include <string.h>

// *****************************************************************************
// *****************************************************************************
// Section: Main Entry Point
// *****************************************************************************
// *****************************************************************************

// USART Definitions
// *****************************************************************************
#define RX_BUFFER_SIZE 256
char messageStart[] = "**** USART read and write from COM terminal ****\r\n\
**** Type a line of characters and press the Enter key. **** \r\n\
**** Entered line will be echoed back. ****\r\n\
**** Note that you won't be able to see what you are typing, but it's there. ****\r\n";
char newline[] = "\r\n";
char errorMessage[] = "\r\n**** USART error has occurred ****\r\n";
char receiveBuffer[RX_BUFFER_SIZE] = {};
char data = 0;
uint16_t rxCounter = 0;
// *****************************************************************************

// I2C Definitions
// *****************************************************************************
#define DEVICE_ADDR                     0x40
#define SHUNT_VOLTAGE_ADDR              0x01
#define BUS_VOLTAGE_ADDR                0x02
#define POWER_ADDR                      0x03
#define CURR_ADDR                       0x04
#define TRANSMIT_DATA_LENGTH            4
#define ACK_DATA_LENGTH                 1
#define RECEIVE_DUMMY_WRITE_LENGTH      2
#define RECEIVE_DATA_LENGTH             4

static uint8_t testTxData[TRANSMIT_DATA_LENGTH] =
{
    SHUNT_VOLTAGE_ADDR,
    BUS_VOLTAGE_ADDR,
    POWER_ADDR,
    CURR_ADDR,
};

static uint8_t  testRxData[RECEIVE_DATA_LENGTH];

typedef enum
{
    APP_STATE_STATUS_VERIFY,
    APP_STATE_READ,
    APP_STATE_WAIT_READ_COMPLETE,
    APP_STATE_VERIFY,
    APP_STATE_IDLE,
    APP_STATE_XFER_SUCCESSFUL,
    APP_STATE_XFER_ERROR

} APP_STATES;

typedef enum
{
    APP_TRANSFER_STATUS_IN_PROGRESS,
    APP_TRANSFER_STATUS_SUCCESS,
    APP_TRANSFER_STATUS_ERROR,
    APP_TRANSFER_STATUS_IDLE,

} APP_TRANSFER_STATUS;

APP_STATES i2c_state = APP_STATE_STATUS_VERIFY;
volatile APP_TRANSFER_STATUS transferStatus = APP_TRANSFER_STATUS_ERROR;
uint8_t ackData = 0;
// *****************************************************************************

void USART_READ(void);
void I2C_READ(void);
void enableGPIOs(void);
void disableGPIOs(void);

int main ( void )
{
    SYS_Initialize(NULL);
    SERCOM2_SPI_Initialize();
    SERCOM3_USART_Initialize();
    SERCOM4_I2C_Initialize();
    
    // GPIO
    //enableGPIOs();
    disableGPIOs();
       
    SERCOM3_USART_Write(&newline[0],sizeof(newline));
    SERCOM3_USART_Write(&messageStart[0], sizeof(messageStart));
    
    while ( true )
    {
        USART_READ();
        I2C_READ();
    }
    
    return EXIT_FAILURE;
}

void USART_READ(void) {
    /* Check if there is a received character */
        if(SERCOM3_USART_ReceiverIsReady() == true)
        {
            if(SERCOM3_USART_ErrorGet() == USART_ERROR_NONE)
            {
                SERCOM3_USART_Read(&data, 1);

                if((data == '\n') || (data == '\r'))
                {
                    //SERCOM3_USART_Write(&newline[0],sizeof(newline));
                    SERCOM3_USART_Write(&receiveBuffer[0],rxCounter);
                    SERCOM3_USART_Write(&newline[0],sizeof(newline));
                    rxCounter = 0;
                }
                else
                {
                    receiveBuffer[rxCounter++] = data;
                }
            }
            else
            {
                SERCOM3_USART_Write(&errorMessage[0],sizeof(errorMessage));
            }
        }
}

void APP_I2CCallback(uintptr_t context )
{
    APP_TRANSFER_STATUS* transferStatus = (APP_TRANSFER_STATUS*)context;

    if(SERCOM4_I2C_ErrorGet() == SERCOM_I2C_ERROR_NONE)
    {
        if (transferStatus)
        {
            *transferStatus = APP_TRANSFER_STATUS_SUCCESS;
        }
    }
    else
    {
        if (transferStatus)
        {
            *transferStatus = APP_TRANSFER_STATUS_ERROR;
        }
    }
}

void I2C_READ ( void )
{
    /* Check the application's current state. */
    switch (i2c_state)
    {
        case APP_STATE_STATUS_VERIFY:

            /* Register the TWIHS Callback with transfer status as context */
            SERCOM4_I2C_CallbackRegister( APP_I2CCallback, (uintptr_t)&transferStatus );

           /* Verify if EEPROM is ready to accept new requests */
            transferStatus = APP_TRANSFER_STATUS_IN_PROGRESS;
            SERCOM4_I2C_Write(DEVICE_ADDR, &ackData, ACK_DATA_LENGTH);

            i2c_state = APP_STATE_READ;
            break;

        case APP_STATE_READ:

            transferStatus = APP_TRANSFER_STATUS_IN_PROGRESS;
            /* Read the data from the page written earlier */
            SERCOM4_I2C_WriteRead(DEVICE_ADDR, &testTxData[0], RECEIVE_DUMMY_WRITE_LENGTH,  &testRxData[0], RECEIVE_DATA_LENGTH);

            i2c_state = APP_STATE_WAIT_READ_COMPLETE;

            break;

        case APP_STATE_WAIT_READ_COMPLETE:

            if (transferStatus == APP_TRANSFER_STATUS_SUCCESS)
            {
                i2c_state = APP_STATE_XFER_SUCCESSFUL;
            }
            else if (transferStatus == APP_TRANSFER_STATUS_ERROR)
            {
                i2c_state = APP_STATE_XFER_ERROR;
            }
            break;

        case APP_STATE_XFER_SUCCESSFUL:
        {
            SERCOM3_USART_Write(&testRxData[0], RECEIVE_DATA_LENGTH);
            i2c_state = APP_STATE_STATUS_VERIFY;
            break;
        }
        case APP_STATE_XFER_ERROR:
        {
            i2c_state = APP_STATE_STATUS_VERIFY;
            break;
        }
        default:
            break;
    }
}

void enableGPIOs(void) {
    BURN_5V_Set();
    BURN2_EN_Set();
    SW_12V_EN1_Set();
    SW_3V3_EN1_Set();
    SW_3V3_EN2_Set();
    SW_5V_EN2_Set();
    SW_5V_EN1_Set();
    SW_5V_EN3_Set();
    SW_5V_EN4_Set();
    SW_VBATT_EN_Set();
    SD_CS_Set();
    WDT_WDI_Set();
    BURN1_EN_Set();
}

void disableGPIOs(void) {
    BURN_5V_Clear();
    BURN2_EN_Clear();
    SW_12V_EN1_Clear();
    SW_3V3_EN1_Clear();
    SW_3V3_EN2_Clear();
    SW_5V_EN2_Clear();
    SW_5V_EN1_Clear();
    SW_5V_EN3_Clear();
    SW_5V_EN4_Clear();
    SW_VBATT_EN_Clear();
    SD_CS_Clear();
    WDT_WDI_Clear();
    BURN1_EN_Set();
}

/*******************************************************************************
 End of File
*/
