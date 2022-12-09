/*******************************************************************************
  MPLAB Harmony Application Source File

  Company:
    Microchip Technology Inc.

  File Name:
    app.c

  Summary:
    This file contains the source code for the MPLAB Harmony application.

  Description:
    This file contains the source code for the MPLAB Harmony application.  It
    implements the logic of the application's state machine and it may call
    API routines of other MPLAB Harmony modules in the system, such as drivers,
    system services, and middleware.  However, it does not call any of the
    system interfaces (such as the "Initialize" and "Tasks" functions) of any of
    the modules in the system or make any assumptions about when those functions
    are called.  That is the responsibility of the configuration-specific system
    files.
 *******************************************************************************/

//DOM-IGNORE-BEGIN
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
//DOM-IGNORE-END

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include "app.h"
#include "definitions.h"
#include "ff.h"
#include "pdu_packet.h"

// *****************************************************************************
// *****************************************************************************
// Section: Global Data Definitions
// *****************************************************************************
// *****************************************************************************

#define SDCARD_MOUNT_NAME    "/mnt/myDrive1"
#define SDCARD_DEV_NAME      "/dev/mmcblka1"
#define SDCARD_FILE_NAME     "hello.txt"
#define SDCARD_DIR_NAME      "Dir1"

#define APP_DATA_LEN         512
    
char APP_INIT_MSG[] = "\r\nAPP INITIALIZED\r\n";
char SD_MOUNT_FAIL[] = "SD MOUNT FAIL\r\n";
char SD_SUCCESS_MSG[] = "SD SUCCESS\r\n";
char SD_FAILED_MSG[] = "SD FAIL\r\n";

// USART Definitions
// *****************************************************************************
#define RX_BUFFER_SIZE 256
char newline[] = "\r\n";
char errorMessage[] = "\r\n**** USART error has occurred ****\r\n";
char receiveBuffer[RX_BUFFER_SIZE] = {};
int data = 0;
uint16_t rxCounter = 0;
// *****************************************************************************

void read_CMD(char* cmd);
void delay_ms(int delay);
void USART_READ(void);
void I2C_READ(void);
void enableGPIOs(void);
void disableGPIOs(void);
void FATFS_APP(void);

FATFS FatFs;	/* FatFs work area needed for each volume */
FIL Fil;		/* File object needed for each open file */

/*******************************************************************************
  Function:
    void APP_Initialize ( void )

  Remarks:
    See prototype in app.h.
 */

void APP_Initialize ( void )
{
//    SERCOM3_USART_Write(&APP_INIT_MSG[0], sizeof(APP_INIT_MSG));
    disableGPIOs();
    RTC_Initialize();
    RTC_Timer32Start();
    SERCOM4_I2C_Initialize();
    //SERCOM2_SPI_Initialize();
}


/******************************************************************************
  Function:
    void APP_Tasks ( void )

  Remarks:
    See prototype in app.h.
 */

void APP_Tasks ( void )
{
    USART_READ();
//    I2C_READ();
    //FATFS_APP();
}

UINT bw;
void FATFS_APP(void) {
    if (f_mount(&FatFs, "", 1) == FR_OK) {	/* Mount SD */

		if (f_open(&Fil, "hello.txt", FA_OPEN_ALWAYS | FA_READ | FA_WRITE) == FR_OK) {	/* Open or create a file */

			if ((Fil.fptr != 0) && (f_lseek(&Fil, Fil.fptr) != FR_OK)) goto endSD;	/* Jump to the end of the file */

            SERCOM3_USART_Write(&SD_SUCCESS_MSG[0], sizeof(SD_SUCCESS_MSG));
			f_write(&Fil, "Hello world!\r\n", 14, &bw);	/* Write data to the file */

			endSD: f_close(&Fil);								/* Close the file */
		} else {
            SERCOM3_USART_Write(&SD_FAILED_MSG[0], sizeof(SD_FAILED_MSG));
        }
	} else {
        SERCOM3_USART_Write(&SD_MOUNT_FAIL[0], sizeof(SD_MOUNT_FAIL));
    }
}

void USART_READ(void) {    
    /* Check if there is a received character */
        if(SERCOM3_USART_ReceiverIsReady() == true)
        {
            if(SERCOM3_USART_ErrorGet() == USART_ERROR_NONE)
            {
                SERCOM3_USART_Read(&data, 1);
//                data = SERCOM3_USART_ReadByte();

                if(data == '\r' || data == '\n')
                {
                    SERCOM3_USART_Write("\0", 1);
//                    SERCOM3_USART_Write(&newline[0],sizeof(newline));
//                    SERCOM3_USART_Write(&receiveBuffer[0],rxCounter);
//                    SERCOM3_USART_Write("\r\n", 2);
//                    SERCOM3_USART_Write(&newline[0],sizeof(newline));
                    rxCounter = 0;
                    decode_pdu_packet(receiveBuffer);
                    
//                    read_CMD(receiveBuffer);
                }
                else
                {
                    receiveBuffer[rxCounter++] = data;
                }
            }
            else
            {
//                SERCOM3_USART_Write(&errorMessage[0],sizeof(errorMessage));
            }
        }
}

void I2C_READ(void) {
    uint8_t i2c_char = SERCOM4_I2C_ReadByte();
    SERCOM3_USART_WriteByte(i2c_char);
    //SERCOM4_I2C_WriteByte(i2c_char);
}

void enableGPIOs(void) {
    BURN_5V_Set();
    BURN2_EN_Set();
    SW_12V_EN1_Set();
    SW_3V3_EN1_Set();
    SW_3V3_EN2_Set();
    SW_5V_EN1_Set();
    SW_5V_EN2_Set();
    SW_5V_EN3_Set();
    SW_5V_EN4_Set();
    SW_VBATT_EN_Set();
//    WDT_WDI_Set();
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
    FAULT2_Set();
    SLEEP1_Set();
    SLEEP2_Set();
}

void disableGPIOs(void) {
    BURN_5V_Clear();
    BURN2_EN_Clear();
    SW_12V_EN1_Clear();
    SW_3V3_EN2_Clear();
    SW_3V3_EN1_Clear();
    SW_5V_EN2_Clear();
    SW_5V_EN1_Clear();
    SW_5V_EN3_Clear();
    SW_5V_EN4_Clear();
    SW_VBATT_EN_Clear();
//    WDT_WDI_Clear();
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
    FAULT2_Clear();
    SLEEP1_Clear();
    SLEEP2_Clear();
}

void read_CMD(char *cmd) {   
    if(strstr(cmd, "CMD: GPIO ON ALL"))
        enableGPIOs();
    else if(strstr(cmd, "CMD: GPIO OFF ALL"))
        disableGPIOs();
    else if(strstr(cmd, "CMD: SW_3V3_1 ENABLE"))
        SW_3V3_EN1_Set();
    else if(strstr(cmd, "CMD: SW_3V3_1 DISABLE"))
        SW_3V3_EN1_Clear();
    else if(strstr(cmd, "CMD: SW_3V3_2 ENABLE"))
        SW_3V3_EN2_Set();
    else if(strstr(cmd, "CMD: SW_3V3_2 DISABLE"))
        SW_3V3_EN2_Clear();
    else if(strstr(cmd, "CMD: SW_5V_1 ENABLE"))
        SW_5V_EN1_Set();
    else if(strstr(cmd, "CMD: SW_5V_1 DISABLE"))
        SW_5V_EN1_Clear();
    else if(strstr(cmd, "CMD: SW_5V_2 ENABLE"))
        SW_5V_EN2_Set();
    else if(strstr(cmd, "CMD: SW_5V_2 DISABLE"))
        SW_5V_EN2_Clear();
    else if(strstr(cmd, "CMD: SW_5V_3 ENABLE"))
        SW_5V_EN3_Set();
    else if(strstr(cmd, "CMD: SW_5V_3 DISABLE"))
        SW_5V_EN3_Clear();
    else if(strstr(cmd, "CMD: SW_5V_4 ENABLE"))
        SW_5V_EN4_Set();
    else if(strstr(cmd, "CMD: SW_5V_4 DISABLE"))
        SW_5V_EN4_Clear();
    else if(strstr(cmd, "CMD: SW_12V ENABLE")) {
        SW_12V_EN1_Set();
        SW_5V_EN4_Set();
    }
    else if(strstr(cmd, "CMD: SW_12V DISABLE")) {
        SW_12V_EN1_Clear();
        SW_5V_EN4_Clear();
    }
    else if(strstr(cmd, "CMD: VBATT ENABLE"))
        SW_VBATT_EN_Set();
    else if(strstr(cmd, "CMD: VBATT DISABLE"))
        SW_VBATT_EN_Clear();
    else if(strstr(cmd, "CMD: WDT ENABLE"))
        WDT_WDI_Set();
    else if(strstr(cmd, "CMD: WDT DISABLE"))
        WDT_WDI_Clear();
    else if(strstr(cmd, "CMD: BURN ENABLE")) {
        BURN1_EN_Set();
        BURN2_EN_Set();
        BURN_5V_Set();
    }
    else if(strstr(cmd, "CMD: BURN DISABLE")) {
        BURN1_EN_Clear();
        BURN2_EN_Clear();
        BURN_5V_Clear();
    }
    else if(strstr(cmd, "CMD: BURN1 ENABLE")) {
        BURN1_EN_Set();
        BURN_5V_Set();
    }
    else if(strstr(cmd, "CMD: BURN1 DISABLE")) {
        BURN1_EN_Clear();
    }
    else if(strstr(cmd, "CMD: BURN2 ENABLE")) {
        BURN2_EN_Set();
        BURN_5V_Set();
    }
    else if(strstr(cmd, "CMD: BURN2 DISABLE")) {
        BURN2_EN_Clear();
    }
    else if(strstr(cmd, "CMD: HBRIDGE1 ENABLE")) {
        FAULT1_Set();
        IN1_Set();
        IN2_Set();
        IN3_Set();
        IN4_Set();
        TRQ1_Set();
        SLEEP1_Set();
    } 
    else if(strstr(cmd, "CMD: HBRIDGE1 DISABLE")) {
        FAULT1_Clear();
        IN1_Clear();
        IN2_Clear();
        IN3_Clear();
        IN4_Clear();
        TRQ1_Clear();
        SLEEP1_Clear();
    } 
    else if(strstr(cmd, "CMD: HBRIDGE2 ENABLE")) {
        FAULT2_Set();
        IN5_Set();
        IN6_Set();
        IN7_Set();
        IN8_Set();
        TRQ2_Set();
        SLEEP2_Set();
    } 
    else if(strstr(cmd, "CMD: HBRIDGE2 DISABLE")) {
        FAULT2_Clear();
        IN5_Clear();
        IN6_Clear();
        IN7_Clear();
        IN8_Clear();
        TRQ2_Clear();
        SLEEP2_Clear();
    } 
    else if(strstr(cmd, "CMD: FATFS"))
        FATFS_APP();
}

void delay_ms(int delay) {
    for(uint8_t i = 0; i < delay; i++)
        asm("NOP");
}

/*******************************************************************************
 End of File
 */