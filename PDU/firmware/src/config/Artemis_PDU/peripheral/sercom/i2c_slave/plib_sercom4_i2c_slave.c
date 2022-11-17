/*******************************************************************************
  Serial Communication Interface Inter-Integrated Circuit (SERCOM I2C) Library
  Source File

  Company:
    Microchip Technology Inc.

  File Name:
    plib_sercom4_i2c_slave.c

  Summary:
    SERCOM I2C PLIB Slave Implementation file

  Description:
    This file defines the interface to the SERCOM I2C Slave peripheral library.
    This library provides access to and control of the associated peripheral
    instance.

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

#include "interrupts.h"
#include "plib_sercom4_i2c_slave.h"

// *****************************************************************************
// *****************************************************************************
// Section: Global Data
// *****************************************************************************
// *****************************************************************************
// *****************************************************************************
// *****************************************************************************
// Section: SERCOM4 I2C Implementation
// *****************************************************************************
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Function:
    void SERCOM4_I2C_Initialize(void)

  Summary:
    Initializes the instance of the SERCOM peripheral operating in I2C mode.

  Description:
    This function initializes the given instance of the SERCOM I2C peripheral as
    configured by the user from the MHC.

  Remarks:
    Refer plib_SERCOM4_i2c.h for more information.
*/

void SERCOM4_I2C_Initialize(void)
{
    /* Reset the module */
    SERCOM4_REGS->I2CS.SERCOM_CTRLA = SERCOM_I2CS_CTRLA_SWRST_Msk ;

    /* Wait for synchronization */
    while((SERCOM4_REGS->I2CS.SERCOM_SYNCBUSY) != 0U)
    {
        /* Do nothing */
    }
     /* Set Operation Mode to I2C Slave */
    SERCOM4_REGS->I2CS.SERCOM_CTRLA = SERCOM_I2CS_CTRLA_MODE_I2C_SLAVE | SERCOM_I2CS_CTRLA_SDAHOLD_450NS | SERCOM_I2CS_CTRLA_SCLSM(0UL) | SERCOM_I2CS_CTRLA_SPEED_STANDARD_AND_FAST_MODE ;
    /* Wait for synchronization */
    while((SERCOM4_REGS->I2CS.SERCOM_SYNCBUSY) != 0U)
    {
        /* Do nothing */
    }

    /* Set SDA Setup time */
    SERCOM4_REGS->I2CS.SERCOM_CTRLC = SERCOM_I2CS_CTRLC_SDASETUP(1UL);

    /* Set the slave address */
    SERCOM4_REGS->I2CS.SERCOM_ADDR = SERCOM_I2CS_ADDR_ADDR(0x70UL) ;



    /* Enable peripheral */
    SERCOM4_REGS->I2CS.SERCOM_CTRLA |= SERCOM_I2CS_CTRLA_ENABLE_Msk ;

    /* Wait for synchronization */
    while((SERCOM4_REGS->I2CS.SERCOM_SYNCBUSY) != 0U)
    {
        /* Do nothing */
    }
}

SERCOM_I2C_SLAVE_INTFLAG SERCOM4_I2C_InterruptFlagsGet(void)
{
    return (SERCOM_I2C_SLAVE_INTFLAG)SERCOM4_REGS->I2CS.SERCOM_INTFLAG;
}

void SERCOM4_I2C_InterruptFlagsClear(SERCOM_I2C_SLAVE_INTFLAG intFlags)
{
    SERCOM4_REGS->I2CS.SERCOM_INTFLAG = (uint8_t)intFlags;
}

uint8_t SERCOM4_I2C_ReadByte(void)
{
    /* Wait for synchronization */
    while((SERCOM4_REGS->I2CS.SERCOM_SYNCBUSY) != 0U)
    {
        /* Do nothing */
    }

    return SERCOM4_REGS->I2CS.SERCOM_DATA;
}

void SERCOM4_I2C_WriteByte(uint8_t wrByte)
{
    SERCOM4_REGS->I2CS.SERCOM_DATA = wrByte;

    /* Wait for synchronization */
    while((SERCOM4_REGS->I2CS.SERCOM_SYNCBUSY) != 0U)
    {
        /* Do nothing */
    }
}

SERCOM_I2C_SLAVE_ERROR SERCOM4_I2C_ErrorGet(void)
{
    SERCOM_I2C_SLAVE_ERROR error;
    error = (SERCOM4_REGS->I2CS.SERCOM_STATUS & SERCOM_I2C_SLAVE_ERROR_ALL);

    /* Clear all error bits */
    SERCOM4_REGS->I2CS.SERCOM_STATUS = (uint16_t)SERCOM_I2C_SLAVE_ERROR_ALL;

    return error;
}

SERCOM_I2C_SLAVE_TRANSFER_DIR SERCOM4_I2C_TransferDirGet(void)
{
    return ((SERCOM4_REGS->I2CS.SERCOM_STATUS & SERCOM_I2CS_STATUS_DIR_Msk) != 0U) ? SERCOM_I2C_SLAVE_TRANSFER_DIR_READ: SERCOM_I2C_SLAVE_TRANSFER_DIR_WRITE;
}

SERCOM_I2C_SLAVE_ACK_STATUS SERCOM4_I2C_LastByteAckStatusGet(void)
{
    return ((SERCOM4_REGS->I2CS.SERCOM_STATUS & SERCOM_I2CS_STATUS_RXNACK_Msk) != 0U) ? SERCOM_I2C_SLAVE_ACK_STATUS_RECEIVED_NAK : SERCOM_I2C_SLAVE_ACK_STATUS_RECEIVED_ACK;
}

void SERCOM4_I2C_CommandSet(SERCOM_I2C_SLAVE_COMMAND command)
{
    if (command == SERCOM_I2C_SLAVE_COMMAND_SEND_ACK)
    {
        SERCOM4_REGS->I2CS.SERCOM_CTRLB = (SERCOM4_REGS->I2CS.SERCOM_CTRLB | SERCOM_I2CS_CTRLB_CMD(0x03UL)) & (~SERCOM_I2CS_CTRLB_ACKACT_Msk);
    }
    else if (command == SERCOM_I2C_SLAVE_COMMAND_SEND_NAK)
    {
        SERCOM4_REGS->I2CS.SERCOM_CTRLB |= (SERCOM_I2CS_CTRLB_CMD(0x03UL) | (SERCOM_I2CS_CTRLB_ACKACT_Msk));
    }
    else if (command == SERCOM_I2C_SLAVE_COMMAND_RECEIVE_ACK_NAK)
    {
        SERCOM4_REGS->I2CS.SERCOM_CTRLB = (SERCOM4_REGS->I2CS.SERCOM_CTRLB | SERCOM_I2CS_CTRLB_CMD(0x03UL));
    }
    else if (command == SERCOM_I2C_SLAVE_COMMAND_WAIT_FOR_START)
    {
        SERCOM4_REGS->I2CS.SERCOM_CTRLB = (SERCOM4_REGS->I2CS.SERCOM_CTRLB & ~SERCOM_I2CS_CTRLB_CMD_Msk) | SERCOM_I2CS_CTRLB_CMD(0x02UL);
    }
    else
    {
        /* Do nothing, return */
    }
}

