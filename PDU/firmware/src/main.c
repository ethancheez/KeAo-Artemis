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
char messageStart[] = "**** USART Initialized ****\r\n";
char newline[] = "\r\n";
char errorMessage[] = "\r\n**** USART error has occurred ****\r\n";
char receiveBuffer[RX_BUFFER_SIZE] = {};
char data = 0;
uint16_t rxCounter = 0;
// *****************************************************************************

void USART_READ(void);
void enableGPIOs(void);
void disableGPIOs(void);

int main ( void )
{
    SYS_Initialize(NULL);
    //SERCOM2_SPI_Initialize();
    SERCOM3_USART_Initialize();
    //SERCOM4_I2C_Initialize();
    
    // GPIO
    //enableGPIOs();
    disableGPIOs();
       
    while(!(SERCOM3_USART_TransmitterIsReady()));
    //while(SERCOM3_USART_ErrorGet() == USART_ERROR_NONE);
    //SERCOM3_USART_Write(&newline[0],sizeof(newline));
    //SERCOM3_USART_Write(&messageStart[0], sizeof(messageStart));
    
    while ( true )
    {
        USART_READ();
        //I2C_READ();
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
                //SERCOM3_USART_Write(&errorMessage[0],sizeof(errorMessage));
            }
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
    IN1_Set();
    IN2_Set();
    IN3_Set();
    IN4_Set();
    IN5_Set();
    IN6_Set();
    IN7_Set();
    IN8_Set();
    TRQ1_Set();
    TRQ2_Set();
    FAULT1_Set();
    SLEEP1_Set();
    SLEEP2_Set();
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
    IN1_Clear();
    IN2_Clear();
    IN3_Clear();
    IN4_Clear();
    IN5_Clear();
    IN6_Clear();
    IN7_Clear();
    IN8_Clear();
    TRQ1_Clear();
    TRQ2_Clear();
    FAULT1_Clear();
    SLEEP1_Clear();
    SLEEP2_Clear();
}

/*******************************************************************************
 End of File
*/
