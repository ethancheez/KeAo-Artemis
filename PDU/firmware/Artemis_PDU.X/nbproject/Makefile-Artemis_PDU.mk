#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Include project Makefile
ifeq "${IGNORE_LOCAL}" "TRUE"
# do not include local makefile. User is passing all local related variables already
else
include Makefile
# Include makefile containing local settings
ifeq "$(wildcard nbproject/Makefile-local-Artemis_PDU.mk)" "nbproject/Makefile-local-Artemis_PDU.mk"
include nbproject/Makefile-local-Artemis_PDU.mk
endif
endif

# Environment
MKDIR=gnumkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=Artemis_PDU
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=elf
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=${DISTDIR}/Artemis_PDU.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=${DISTDIR}/Artemis_PDU.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

ifeq ($(COMPARE_BUILD), true)
COMPARISON_BUILD=-mafrlcsj
else
COMPARISON_BUILD=
endif

ifdef SUB_IMAGE_ADDRESS

else
SUB_IMAGE_ADDRESS_COMMAND=
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED=../src/config/Artemis_PDU/peripheral/clock/plib_clock.c ../src/config/Artemis_PDU/peripheral/cmcc/plib_cmcc.c ../src/config/Artemis_PDU/peripheral/eic/plib_eic.c ../src/config/Artemis_PDU/peripheral/evsys/plib_evsys.c ../src/config/Artemis_PDU/peripheral/nvic/plib_nvic.c ../src/config/Artemis_PDU/peripheral/nvmctrl/plib_nvmctrl.c ../src/config/Artemis_PDU/peripheral/port/plib_port.c ../src/config/Artemis_PDU/peripheral/sercom/i2c_master/plib_sercom4_i2c_master.c ../src/config/Artemis_PDU/peripheral/sercom/spi_master/plib_sercom2_spi_master.c ../src/config/Artemis_PDU/peripheral/sercom/usart/plib_sercom3_usart.c ../src/config/Artemis_PDU/stdio/xc32_monitor.c ../src/config/Artemis_PDU/initialization.c ../src/config/Artemis_PDU/interrupts.c ../src/config/Artemis_PDU/exceptions.c ../src/config/Artemis_PDU/startup_xc32.c ../src/config/Artemis_PDU/libc_syscalls.c ../src/main.c ../src/temp.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/_ext/1650922938/plib_clock.o ${OBJECTDIR}/_ext/1023086318/plib_cmcc.o ${OBJECTDIR}/_ext/659735671/plib_eic.o ${OBJECTDIR}/_ext/1648773452/plib_evsys.o ${OBJECTDIR}/_ext/1022749782/plib_nvic.o ${OBJECTDIR}/_ext/236948536/plib_nvmctrl.o ${OBJECTDIR}/_ext/1022696631/plib_port.o ${OBJECTDIR}/_ext/305419891/plib_sercom4_i2c_master.o ${OBJECTDIR}/_ext/796654533/plib_sercom2_spi_master.o ${OBJECTDIR}/_ext/1679430495/plib_sercom3_usart.o ${OBJECTDIR}/_ext/1186129630/xc32_monitor.o ${OBJECTDIR}/_ext/974488028/initialization.o ${OBJECTDIR}/_ext/974488028/interrupts.o ${OBJECTDIR}/_ext/974488028/exceptions.o ${OBJECTDIR}/_ext/974488028/startup_xc32.o ${OBJECTDIR}/_ext/974488028/libc_syscalls.o ${OBJECTDIR}/_ext/1360937237/main.o ${OBJECTDIR}/_ext/1360937237/temp.o
POSSIBLE_DEPFILES=${OBJECTDIR}/_ext/1650922938/plib_clock.o.d ${OBJECTDIR}/_ext/1023086318/plib_cmcc.o.d ${OBJECTDIR}/_ext/659735671/plib_eic.o.d ${OBJECTDIR}/_ext/1648773452/plib_evsys.o.d ${OBJECTDIR}/_ext/1022749782/plib_nvic.o.d ${OBJECTDIR}/_ext/236948536/plib_nvmctrl.o.d ${OBJECTDIR}/_ext/1022696631/plib_port.o.d ${OBJECTDIR}/_ext/305419891/plib_sercom4_i2c_master.o.d ${OBJECTDIR}/_ext/796654533/plib_sercom2_spi_master.o.d ${OBJECTDIR}/_ext/1679430495/plib_sercom3_usart.o.d ${OBJECTDIR}/_ext/1186129630/xc32_monitor.o.d ${OBJECTDIR}/_ext/974488028/initialization.o.d ${OBJECTDIR}/_ext/974488028/interrupts.o.d ${OBJECTDIR}/_ext/974488028/exceptions.o.d ${OBJECTDIR}/_ext/974488028/startup_xc32.o.d ${OBJECTDIR}/_ext/974488028/libc_syscalls.o.d ${OBJECTDIR}/_ext/1360937237/main.o.d ${OBJECTDIR}/_ext/1360937237/temp.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/_ext/1650922938/plib_clock.o ${OBJECTDIR}/_ext/1023086318/plib_cmcc.o ${OBJECTDIR}/_ext/659735671/plib_eic.o ${OBJECTDIR}/_ext/1648773452/plib_evsys.o ${OBJECTDIR}/_ext/1022749782/plib_nvic.o ${OBJECTDIR}/_ext/236948536/plib_nvmctrl.o ${OBJECTDIR}/_ext/1022696631/plib_port.o ${OBJECTDIR}/_ext/305419891/plib_sercom4_i2c_master.o ${OBJECTDIR}/_ext/796654533/plib_sercom2_spi_master.o ${OBJECTDIR}/_ext/1679430495/plib_sercom3_usart.o ${OBJECTDIR}/_ext/1186129630/xc32_monitor.o ${OBJECTDIR}/_ext/974488028/initialization.o ${OBJECTDIR}/_ext/974488028/interrupts.o ${OBJECTDIR}/_ext/974488028/exceptions.o ${OBJECTDIR}/_ext/974488028/startup_xc32.o ${OBJECTDIR}/_ext/974488028/libc_syscalls.o ${OBJECTDIR}/_ext/1360937237/main.o ${OBJECTDIR}/_ext/1360937237/temp.o

# Source Files
SOURCEFILES=../src/config/Artemis_PDU/peripheral/clock/plib_clock.c ../src/config/Artemis_PDU/peripheral/cmcc/plib_cmcc.c ../src/config/Artemis_PDU/peripheral/eic/plib_eic.c ../src/config/Artemis_PDU/peripheral/evsys/plib_evsys.c ../src/config/Artemis_PDU/peripheral/nvic/plib_nvic.c ../src/config/Artemis_PDU/peripheral/nvmctrl/plib_nvmctrl.c ../src/config/Artemis_PDU/peripheral/port/plib_port.c ../src/config/Artemis_PDU/peripheral/sercom/i2c_master/plib_sercom4_i2c_master.c ../src/config/Artemis_PDU/peripheral/sercom/spi_master/plib_sercom2_spi_master.c ../src/config/Artemis_PDU/peripheral/sercom/usart/plib_sercom3_usart.c ../src/config/Artemis_PDU/stdio/xc32_monitor.c ../src/config/Artemis_PDU/initialization.c ../src/config/Artemis_PDU/interrupts.c ../src/config/Artemis_PDU/exceptions.c ../src/config/Artemis_PDU/startup_xc32.c ../src/config/Artemis_PDU/libc_syscalls.c ../src/main.c ../src/temp.c

# Pack Options 
PACK_COMMON_OPTIONS=-I "${CMSIS_DIR}/CMSIS/Core/Include"



CFLAGS=
ASFLAGS=
LDLIBSOPTIONS=

############# Tool locations ##########################################
# If you copy a project from one host to another, the path where the  #
# compiler is installed may be different.                             #
# If you open this project with MPLAB X in the new host, this         #
# makefile will be regenerated and the paths will be corrected.       #
#######################################################################
# fixDeps replaces a bunch of sed/cat/printf statements that slow down the build
FIXDEPS=fixDeps

.build-conf:  ${BUILD_SUBPROJECTS}
ifneq ($(INFORMATION_MESSAGE), )
	@echo $(INFORMATION_MESSAGE)
endif
	${MAKE}  -f nbproject/Makefile-Artemis_PDU.mk ${DISTDIR}/Artemis_PDU.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=ATSAME51N19A
MP_LINKER_FILE_OPTION=,--script="..\src\config\Artemis_PDU\ATSAME51N19A.ld"
# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assembleWithPreprocess
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/_ext/1650922938/plib_clock.o: ../src/config/Artemis_PDU/peripheral/clock/plib_clock.c  .generated_files/flags/Artemis_PDU/3e3274a9e21871c085d1f73e23aaa328908ad398 .generated_files/flags/Artemis_PDU/50acabce23f44026a21b3861a7facc0a827a519
	@${MKDIR} "${OBJECTDIR}/_ext/1650922938" 
	@${RM} ${OBJECTDIR}/_ext/1650922938/plib_clock.o.d 
	@${RM} ${OBJECTDIR}/_ext/1650922938/plib_clock.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/Artemis_PDU" -I"../src/packs/ATSAME51N19A_DFP" -I"../src/packs/CMSIS/" -I"../src/packs/CMSIS/CMSIS/Core/Include" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1650922938/plib_clock.o.d" -o ${OBJECTDIR}/_ext/1650922938/plib_clock.o ../src/config/Artemis_PDU/peripheral/clock/plib_clock.c    -DXPRJ_Artemis_PDU=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/_ext/1023086318/plib_cmcc.o: ../src/config/Artemis_PDU/peripheral/cmcc/plib_cmcc.c  .generated_files/flags/Artemis_PDU/2c0f220954662006892bf1953def40306c4ba52d .generated_files/flags/Artemis_PDU/50acabce23f44026a21b3861a7facc0a827a519
	@${MKDIR} "${OBJECTDIR}/_ext/1023086318" 
	@${RM} ${OBJECTDIR}/_ext/1023086318/plib_cmcc.o.d 
	@${RM} ${OBJECTDIR}/_ext/1023086318/plib_cmcc.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/Artemis_PDU" -I"../src/packs/ATSAME51N19A_DFP" -I"../src/packs/CMSIS/" -I"../src/packs/CMSIS/CMSIS/Core/Include" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1023086318/plib_cmcc.o.d" -o ${OBJECTDIR}/_ext/1023086318/plib_cmcc.o ../src/config/Artemis_PDU/peripheral/cmcc/plib_cmcc.c    -DXPRJ_Artemis_PDU=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/_ext/659735671/plib_eic.o: ../src/config/Artemis_PDU/peripheral/eic/plib_eic.c  .generated_files/flags/Artemis_PDU/a79d21682fc9aeb00e6667bcc6c27e8c5f246266 .generated_files/flags/Artemis_PDU/50acabce23f44026a21b3861a7facc0a827a519
	@${MKDIR} "${OBJECTDIR}/_ext/659735671" 
	@${RM} ${OBJECTDIR}/_ext/659735671/plib_eic.o.d 
	@${RM} ${OBJECTDIR}/_ext/659735671/plib_eic.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/Artemis_PDU" -I"../src/packs/ATSAME51N19A_DFP" -I"../src/packs/CMSIS/" -I"../src/packs/CMSIS/CMSIS/Core/Include" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/659735671/plib_eic.o.d" -o ${OBJECTDIR}/_ext/659735671/plib_eic.o ../src/config/Artemis_PDU/peripheral/eic/plib_eic.c    -DXPRJ_Artemis_PDU=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/_ext/1648773452/plib_evsys.o: ../src/config/Artemis_PDU/peripheral/evsys/plib_evsys.c  .generated_files/flags/Artemis_PDU/4810031c9fd52394a3a33c000c1d27cf1ee3eb9c .generated_files/flags/Artemis_PDU/50acabce23f44026a21b3861a7facc0a827a519
	@${MKDIR} "${OBJECTDIR}/_ext/1648773452" 
	@${RM} ${OBJECTDIR}/_ext/1648773452/plib_evsys.o.d 
	@${RM} ${OBJECTDIR}/_ext/1648773452/plib_evsys.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/Artemis_PDU" -I"../src/packs/ATSAME51N19A_DFP" -I"../src/packs/CMSIS/" -I"../src/packs/CMSIS/CMSIS/Core/Include" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1648773452/plib_evsys.o.d" -o ${OBJECTDIR}/_ext/1648773452/plib_evsys.o ../src/config/Artemis_PDU/peripheral/evsys/plib_evsys.c    -DXPRJ_Artemis_PDU=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/_ext/1022749782/plib_nvic.o: ../src/config/Artemis_PDU/peripheral/nvic/plib_nvic.c  .generated_files/flags/Artemis_PDU/32df465e6a892bfac7b2ef0d72afd8bf32840b94 .generated_files/flags/Artemis_PDU/50acabce23f44026a21b3861a7facc0a827a519
	@${MKDIR} "${OBJECTDIR}/_ext/1022749782" 
	@${RM} ${OBJECTDIR}/_ext/1022749782/plib_nvic.o.d 
	@${RM} ${OBJECTDIR}/_ext/1022749782/plib_nvic.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/Artemis_PDU" -I"../src/packs/ATSAME51N19A_DFP" -I"../src/packs/CMSIS/" -I"../src/packs/CMSIS/CMSIS/Core/Include" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1022749782/plib_nvic.o.d" -o ${OBJECTDIR}/_ext/1022749782/plib_nvic.o ../src/config/Artemis_PDU/peripheral/nvic/plib_nvic.c    -DXPRJ_Artemis_PDU=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/_ext/236948536/plib_nvmctrl.o: ../src/config/Artemis_PDU/peripheral/nvmctrl/plib_nvmctrl.c  .generated_files/flags/Artemis_PDU/c8bb22ea557adde77890fe34690177307c223c8e .generated_files/flags/Artemis_PDU/50acabce23f44026a21b3861a7facc0a827a519
	@${MKDIR} "${OBJECTDIR}/_ext/236948536" 
	@${RM} ${OBJECTDIR}/_ext/236948536/plib_nvmctrl.o.d 
	@${RM} ${OBJECTDIR}/_ext/236948536/plib_nvmctrl.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/Artemis_PDU" -I"../src/packs/ATSAME51N19A_DFP" -I"../src/packs/CMSIS/" -I"../src/packs/CMSIS/CMSIS/Core/Include" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/236948536/plib_nvmctrl.o.d" -o ${OBJECTDIR}/_ext/236948536/plib_nvmctrl.o ../src/config/Artemis_PDU/peripheral/nvmctrl/plib_nvmctrl.c    -DXPRJ_Artemis_PDU=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/_ext/1022696631/plib_port.o: ../src/config/Artemis_PDU/peripheral/port/plib_port.c  .generated_files/flags/Artemis_PDU/8ce3477c3116d203bd3147dca9af089ae05b7c39 .generated_files/flags/Artemis_PDU/50acabce23f44026a21b3861a7facc0a827a519
	@${MKDIR} "${OBJECTDIR}/_ext/1022696631" 
	@${RM} ${OBJECTDIR}/_ext/1022696631/plib_port.o.d 
	@${RM} ${OBJECTDIR}/_ext/1022696631/plib_port.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/Artemis_PDU" -I"../src/packs/ATSAME51N19A_DFP" -I"../src/packs/CMSIS/" -I"../src/packs/CMSIS/CMSIS/Core/Include" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1022696631/plib_port.o.d" -o ${OBJECTDIR}/_ext/1022696631/plib_port.o ../src/config/Artemis_PDU/peripheral/port/plib_port.c    -DXPRJ_Artemis_PDU=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/_ext/305419891/plib_sercom4_i2c_master.o: ../src/config/Artemis_PDU/peripheral/sercom/i2c_master/plib_sercom4_i2c_master.c  .generated_files/flags/Artemis_PDU/1ea90bd5cfbd741cd5578f365248ba0e07554dae .generated_files/flags/Artemis_PDU/50acabce23f44026a21b3861a7facc0a827a519
	@${MKDIR} "${OBJECTDIR}/_ext/305419891" 
	@${RM} ${OBJECTDIR}/_ext/305419891/plib_sercom4_i2c_master.o.d 
	@${RM} ${OBJECTDIR}/_ext/305419891/plib_sercom4_i2c_master.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/Artemis_PDU" -I"../src/packs/ATSAME51N19A_DFP" -I"../src/packs/CMSIS/" -I"../src/packs/CMSIS/CMSIS/Core/Include" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/305419891/plib_sercom4_i2c_master.o.d" -o ${OBJECTDIR}/_ext/305419891/plib_sercom4_i2c_master.o ../src/config/Artemis_PDU/peripheral/sercom/i2c_master/plib_sercom4_i2c_master.c    -DXPRJ_Artemis_PDU=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/_ext/796654533/plib_sercom2_spi_master.o: ../src/config/Artemis_PDU/peripheral/sercom/spi_master/plib_sercom2_spi_master.c  .generated_files/flags/Artemis_PDU/249ddbe02109ccf04025e28e8ee742a665921b41 .generated_files/flags/Artemis_PDU/50acabce23f44026a21b3861a7facc0a827a519
	@${MKDIR} "${OBJECTDIR}/_ext/796654533" 
	@${RM} ${OBJECTDIR}/_ext/796654533/plib_sercom2_spi_master.o.d 
	@${RM} ${OBJECTDIR}/_ext/796654533/plib_sercom2_spi_master.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/Artemis_PDU" -I"../src/packs/ATSAME51N19A_DFP" -I"../src/packs/CMSIS/" -I"../src/packs/CMSIS/CMSIS/Core/Include" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/796654533/plib_sercom2_spi_master.o.d" -o ${OBJECTDIR}/_ext/796654533/plib_sercom2_spi_master.o ../src/config/Artemis_PDU/peripheral/sercom/spi_master/plib_sercom2_spi_master.c    -DXPRJ_Artemis_PDU=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/_ext/1679430495/plib_sercom3_usart.o: ../src/config/Artemis_PDU/peripheral/sercom/usart/plib_sercom3_usart.c  .generated_files/flags/Artemis_PDU/fd7bcc424bc362e657244d3f26116c08c97bc227 .generated_files/flags/Artemis_PDU/50acabce23f44026a21b3861a7facc0a827a519
	@${MKDIR} "${OBJECTDIR}/_ext/1679430495" 
	@${RM} ${OBJECTDIR}/_ext/1679430495/plib_sercom3_usart.o.d 
	@${RM} ${OBJECTDIR}/_ext/1679430495/plib_sercom3_usart.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/Artemis_PDU" -I"../src/packs/ATSAME51N19A_DFP" -I"../src/packs/CMSIS/" -I"../src/packs/CMSIS/CMSIS/Core/Include" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1679430495/plib_sercom3_usart.o.d" -o ${OBJECTDIR}/_ext/1679430495/plib_sercom3_usart.o ../src/config/Artemis_PDU/peripheral/sercom/usart/plib_sercom3_usart.c    -DXPRJ_Artemis_PDU=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/_ext/1186129630/xc32_monitor.o: ../src/config/Artemis_PDU/stdio/xc32_monitor.c  .generated_files/flags/Artemis_PDU/efe8c162fe406e3140cd83dc60e1f35cb86fc5d3 .generated_files/flags/Artemis_PDU/50acabce23f44026a21b3861a7facc0a827a519
	@${MKDIR} "${OBJECTDIR}/_ext/1186129630" 
	@${RM} ${OBJECTDIR}/_ext/1186129630/xc32_monitor.o.d 
	@${RM} ${OBJECTDIR}/_ext/1186129630/xc32_monitor.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/Artemis_PDU" -I"../src/packs/ATSAME51N19A_DFP" -I"../src/packs/CMSIS/" -I"../src/packs/CMSIS/CMSIS/Core/Include" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1186129630/xc32_monitor.o.d" -o ${OBJECTDIR}/_ext/1186129630/xc32_monitor.o ../src/config/Artemis_PDU/stdio/xc32_monitor.c    -DXPRJ_Artemis_PDU=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/_ext/974488028/initialization.o: ../src/config/Artemis_PDU/initialization.c  .generated_files/flags/Artemis_PDU/c619e8993e61efe33002dbfdb738af701cab4f4f .generated_files/flags/Artemis_PDU/50acabce23f44026a21b3861a7facc0a827a519
	@${MKDIR} "${OBJECTDIR}/_ext/974488028" 
	@${RM} ${OBJECTDIR}/_ext/974488028/initialization.o.d 
	@${RM} ${OBJECTDIR}/_ext/974488028/initialization.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/Artemis_PDU" -I"../src/packs/ATSAME51N19A_DFP" -I"../src/packs/CMSIS/" -I"../src/packs/CMSIS/CMSIS/Core/Include" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/974488028/initialization.o.d" -o ${OBJECTDIR}/_ext/974488028/initialization.o ../src/config/Artemis_PDU/initialization.c    -DXPRJ_Artemis_PDU=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/_ext/974488028/interrupts.o: ../src/config/Artemis_PDU/interrupts.c  .generated_files/flags/Artemis_PDU/1ae1e38c59394ee77a725e3cbe886e219c1bc93a .generated_files/flags/Artemis_PDU/50acabce23f44026a21b3861a7facc0a827a519
	@${MKDIR} "${OBJECTDIR}/_ext/974488028" 
	@${RM} ${OBJECTDIR}/_ext/974488028/interrupts.o.d 
	@${RM} ${OBJECTDIR}/_ext/974488028/interrupts.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/Artemis_PDU" -I"../src/packs/ATSAME51N19A_DFP" -I"../src/packs/CMSIS/" -I"../src/packs/CMSIS/CMSIS/Core/Include" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/974488028/interrupts.o.d" -o ${OBJECTDIR}/_ext/974488028/interrupts.o ../src/config/Artemis_PDU/interrupts.c    -DXPRJ_Artemis_PDU=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/_ext/974488028/exceptions.o: ../src/config/Artemis_PDU/exceptions.c  .generated_files/flags/Artemis_PDU/977253ef1121300600cc0a12bc2a0752a28d1500 .generated_files/flags/Artemis_PDU/50acabce23f44026a21b3861a7facc0a827a519
	@${MKDIR} "${OBJECTDIR}/_ext/974488028" 
	@${RM} ${OBJECTDIR}/_ext/974488028/exceptions.o.d 
	@${RM} ${OBJECTDIR}/_ext/974488028/exceptions.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/Artemis_PDU" -I"../src/packs/ATSAME51N19A_DFP" -I"../src/packs/CMSIS/" -I"../src/packs/CMSIS/CMSIS/Core/Include" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/974488028/exceptions.o.d" -o ${OBJECTDIR}/_ext/974488028/exceptions.o ../src/config/Artemis_PDU/exceptions.c    -DXPRJ_Artemis_PDU=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/_ext/974488028/startup_xc32.o: ../src/config/Artemis_PDU/startup_xc32.c  .generated_files/flags/Artemis_PDU/736fac7d322ed87ff9ac66dfd072ccd89a03ab53 .generated_files/flags/Artemis_PDU/50acabce23f44026a21b3861a7facc0a827a519
	@${MKDIR} "${OBJECTDIR}/_ext/974488028" 
	@${RM} ${OBJECTDIR}/_ext/974488028/startup_xc32.o.d 
	@${RM} ${OBJECTDIR}/_ext/974488028/startup_xc32.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/Artemis_PDU" -I"../src/packs/ATSAME51N19A_DFP" -I"../src/packs/CMSIS/" -I"../src/packs/CMSIS/CMSIS/Core/Include" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/974488028/startup_xc32.o.d" -o ${OBJECTDIR}/_ext/974488028/startup_xc32.o ../src/config/Artemis_PDU/startup_xc32.c    -DXPRJ_Artemis_PDU=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/_ext/974488028/libc_syscalls.o: ../src/config/Artemis_PDU/libc_syscalls.c  .generated_files/flags/Artemis_PDU/33c79d9bfe45a6a53d5f706cde1fa35559691851 .generated_files/flags/Artemis_PDU/50acabce23f44026a21b3861a7facc0a827a519
	@${MKDIR} "${OBJECTDIR}/_ext/974488028" 
	@${RM} ${OBJECTDIR}/_ext/974488028/libc_syscalls.o.d 
	@${RM} ${OBJECTDIR}/_ext/974488028/libc_syscalls.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/Artemis_PDU" -I"../src/packs/ATSAME51N19A_DFP" -I"../src/packs/CMSIS/" -I"../src/packs/CMSIS/CMSIS/Core/Include" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/974488028/libc_syscalls.o.d" -o ${OBJECTDIR}/_ext/974488028/libc_syscalls.o ../src/config/Artemis_PDU/libc_syscalls.c    -DXPRJ_Artemis_PDU=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/_ext/1360937237/main.o: ../src/main.c  .generated_files/flags/Artemis_PDU/9e13ab8cf57de6dba10edc6d0df141d18692a6e3 .generated_files/flags/Artemis_PDU/50acabce23f44026a21b3861a7facc0a827a519
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/main.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/main.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/Artemis_PDU" -I"../src/packs/ATSAME51N19A_DFP" -I"../src/packs/CMSIS/" -I"../src/packs/CMSIS/CMSIS/Core/Include" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/main.o.d" -o ${OBJECTDIR}/_ext/1360937237/main.o ../src/main.c    -DXPRJ_Artemis_PDU=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/_ext/1360937237/temp.o: ../src/temp.c  .generated_files/flags/Artemis_PDU/134826c06c32b7235108133ccca013d6edb2c18a .generated_files/flags/Artemis_PDU/50acabce23f44026a21b3861a7facc0a827a519
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/temp.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/temp.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/Artemis_PDU" -I"../src/packs/ATSAME51N19A_DFP" -I"../src/packs/CMSIS/" -I"../src/packs/CMSIS/CMSIS/Core/Include" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/temp.o.d" -o ${OBJECTDIR}/_ext/1360937237/temp.o ../src/temp.c    -DXPRJ_Artemis_PDU=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}" ${PACK_COMMON_OPTIONS} 
	
else
${OBJECTDIR}/_ext/1650922938/plib_clock.o: ../src/config/Artemis_PDU/peripheral/clock/plib_clock.c  .generated_files/flags/Artemis_PDU/2a9de361aff75bf6daa1edbe3c2a7b33e1f05ece .generated_files/flags/Artemis_PDU/50acabce23f44026a21b3861a7facc0a827a519
	@${MKDIR} "${OBJECTDIR}/_ext/1650922938" 
	@${RM} ${OBJECTDIR}/_ext/1650922938/plib_clock.o.d 
	@${RM} ${OBJECTDIR}/_ext/1650922938/plib_clock.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/Artemis_PDU" -I"../src/packs/ATSAME51N19A_DFP" -I"../src/packs/CMSIS/" -I"../src/packs/CMSIS/CMSIS/Core/Include" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1650922938/plib_clock.o.d" -o ${OBJECTDIR}/_ext/1650922938/plib_clock.o ../src/config/Artemis_PDU/peripheral/clock/plib_clock.c    -DXPRJ_Artemis_PDU=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/_ext/1023086318/plib_cmcc.o: ../src/config/Artemis_PDU/peripheral/cmcc/plib_cmcc.c  .generated_files/flags/Artemis_PDU/e2bca93d6660c036894c7ec7c65cbd30b7ec50b7 .generated_files/flags/Artemis_PDU/50acabce23f44026a21b3861a7facc0a827a519
	@${MKDIR} "${OBJECTDIR}/_ext/1023086318" 
	@${RM} ${OBJECTDIR}/_ext/1023086318/plib_cmcc.o.d 
	@${RM} ${OBJECTDIR}/_ext/1023086318/plib_cmcc.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/Artemis_PDU" -I"../src/packs/ATSAME51N19A_DFP" -I"../src/packs/CMSIS/" -I"../src/packs/CMSIS/CMSIS/Core/Include" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1023086318/plib_cmcc.o.d" -o ${OBJECTDIR}/_ext/1023086318/plib_cmcc.o ../src/config/Artemis_PDU/peripheral/cmcc/plib_cmcc.c    -DXPRJ_Artemis_PDU=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/_ext/659735671/plib_eic.o: ../src/config/Artemis_PDU/peripheral/eic/plib_eic.c  .generated_files/flags/Artemis_PDU/43b498b07ee3ff2edc118d612e90d8b8056ff893 .generated_files/flags/Artemis_PDU/50acabce23f44026a21b3861a7facc0a827a519
	@${MKDIR} "${OBJECTDIR}/_ext/659735671" 
	@${RM} ${OBJECTDIR}/_ext/659735671/plib_eic.o.d 
	@${RM} ${OBJECTDIR}/_ext/659735671/plib_eic.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/Artemis_PDU" -I"../src/packs/ATSAME51N19A_DFP" -I"../src/packs/CMSIS/" -I"../src/packs/CMSIS/CMSIS/Core/Include" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/659735671/plib_eic.o.d" -o ${OBJECTDIR}/_ext/659735671/plib_eic.o ../src/config/Artemis_PDU/peripheral/eic/plib_eic.c    -DXPRJ_Artemis_PDU=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/_ext/1648773452/plib_evsys.o: ../src/config/Artemis_PDU/peripheral/evsys/plib_evsys.c  .generated_files/flags/Artemis_PDU/436f69ecf1030d918fcca7ac7532db503bcd7b33 .generated_files/flags/Artemis_PDU/50acabce23f44026a21b3861a7facc0a827a519
	@${MKDIR} "${OBJECTDIR}/_ext/1648773452" 
	@${RM} ${OBJECTDIR}/_ext/1648773452/plib_evsys.o.d 
	@${RM} ${OBJECTDIR}/_ext/1648773452/plib_evsys.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/Artemis_PDU" -I"../src/packs/ATSAME51N19A_DFP" -I"../src/packs/CMSIS/" -I"../src/packs/CMSIS/CMSIS/Core/Include" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1648773452/plib_evsys.o.d" -o ${OBJECTDIR}/_ext/1648773452/plib_evsys.o ../src/config/Artemis_PDU/peripheral/evsys/plib_evsys.c    -DXPRJ_Artemis_PDU=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/_ext/1022749782/plib_nvic.o: ../src/config/Artemis_PDU/peripheral/nvic/plib_nvic.c  .generated_files/flags/Artemis_PDU/5e30a4948f6da17ab14205ab8a75788f9e1e2195 .generated_files/flags/Artemis_PDU/50acabce23f44026a21b3861a7facc0a827a519
	@${MKDIR} "${OBJECTDIR}/_ext/1022749782" 
	@${RM} ${OBJECTDIR}/_ext/1022749782/plib_nvic.o.d 
	@${RM} ${OBJECTDIR}/_ext/1022749782/plib_nvic.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/Artemis_PDU" -I"../src/packs/ATSAME51N19A_DFP" -I"../src/packs/CMSIS/" -I"../src/packs/CMSIS/CMSIS/Core/Include" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1022749782/plib_nvic.o.d" -o ${OBJECTDIR}/_ext/1022749782/plib_nvic.o ../src/config/Artemis_PDU/peripheral/nvic/plib_nvic.c    -DXPRJ_Artemis_PDU=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/_ext/236948536/plib_nvmctrl.o: ../src/config/Artemis_PDU/peripheral/nvmctrl/plib_nvmctrl.c  .generated_files/flags/Artemis_PDU/fd51a56d991450ebc42d76d1a20d5ebc0040299e .generated_files/flags/Artemis_PDU/50acabce23f44026a21b3861a7facc0a827a519
	@${MKDIR} "${OBJECTDIR}/_ext/236948536" 
	@${RM} ${OBJECTDIR}/_ext/236948536/plib_nvmctrl.o.d 
	@${RM} ${OBJECTDIR}/_ext/236948536/plib_nvmctrl.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/Artemis_PDU" -I"../src/packs/ATSAME51N19A_DFP" -I"../src/packs/CMSIS/" -I"../src/packs/CMSIS/CMSIS/Core/Include" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/236948536/plib_nvmctrl.o.d" -o ${OBJECTDIR}/_ext/236948536/plib_nvmctrl.o ../src/config/Artemis_PDU/peripheral/nvmctrl/plib_nvmctrl.c    -DXPRJ_Artemis_PDU=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/_ext/1022696631/plib_port.o: ../src/config/Artemis_PDU/peripheral/port/plib_port.c  .generated_files/flags/Artemis_PDU/54b6cbaffd12edf6d1cbec37d77c70c55cfed856 .generated_files/flags/Artemis_PDU/50acabce23f44026a21b3861a7facc0a827a519
	@${MKDIR} "${OBJECTDIR}/_ext/1022696631" 
	@${RM} ${OBJECTDIR}/_ext/1022696631/plib_port.o.d 
	@${RM} ${OBJECTDIR}/_ext/1022696631/plib_port.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/Artemis_PDU" -I"../src/packs/ATSAME51N19A_DFP" -I"../src/packs/CMSIS/" -I"../src/packs/CMSIS/CMSIS/Core/Include" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1022696631/plib_port.o.d" -o ${OBJECTDIR}/_ext/1022696631/plib_port.o ../src/config/Artemis_PDU/peripheral/port/plib_port.c    -DXPRJ_Artemis_PDU=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/_ext/305419891/plib_sercom4_i2c_master.o: ../src/config/Artemis_PDU/peripheral/sercom/i2c_master/plib_sercom4_i2c_master.c  .generated_files/flags/Artemis_PDU/fedbe19a4c2c01f147c33cd526716a83bcd93bc7 .generated_files/flags/Artemis_PDU/50acabce23f44026a21b3861a7facc0a827a519
	@${MKDIR} "${OBJECTDIR}/_ext/305419891" 
	@${RM} ${OBJECTDIR}/_ext/305419891/plib_sercom4_i2c_master.o.d 
	@${RM} ${OBJECTDIR}/_ext/305419891/plib_sercom4_i2c_master.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/Artemis_PDU" -I"../src/packs/ATSAME51N19A_DFP" -I"../src/packs/CMSIS/" -I"../src/packs/CMSIS/CMSIS/Core/Include" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/305419891/plib_sercom4_i2c_master.o.d" -o ${OBJECTDIR}/_ext/305419891/plib_sercom4_i2c_master.o ../src/config/Artemis_PDU/peripheral/sercom/i2c_master/plib_sercom4_i2c_master.c    -DXPRJ_Artemis_PDU=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/_ext/796654533/plib_sercom2_spi_master.o: ../src/config/Artemis_PDU/peripheral/sercom/spi_master/plib_sercom2_spi_master.c  .generated_files/flags/Artemis_PDU/6b4010d05c2d226d9250af9b8097ba88b73bf359 .generated_files/flags/Artemis_PDU/50acabce23f44026a21b3861a7facc0a827a519
	@${MKDIR} "${OBJECTDIR}/_ext/796654533" 
	@${RM} ${OBJECTDIR}/_ext/796654533/plib_sercom2_spi_master.o.d 
	@${RM} ${OBJECTDIR}/_ext/796654533/plib_sercom2_spi_master.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/Artemis_PDU" -I"../src/packs/ATSAME51N19A_DFP" -I"../src/packs/CMSIS/" -I"../src/packs/CMSIS/CMSIS/Core/Include" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/796654533/plib_sercom2_spi_master.o.d" -o ${OBJECTDIR}/_ext/796654533/plib_sercom2_spi_master.o ../src/config/Artemis_PDU/peripheral/sercom/spi_master/plib_sercom2_spi_master.c    -DXPRJ_Artemis_PDU=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/_ext/1679430495/plib_sercom3_usart.o: ../src/config/Artemis_PDU/peripheral/sercom/usart/plib_sercom3_usart.c  .generated_files/flags/Artemis_PDU/aa952e4dcd8ff0c97a2762dc31e4366c00e06b8c .generated_files/flags/Artemis_PDU/50acabce23f44026a21b3861a7facc0a827a519
	@${MKDIR} "${OBJECTDIR}/_ext/1679430495" 
	@${RM} ${OBJECTDIR}/_ext/1679430495/plib_sercom3_usart.o.d 
	@${RM} ${OBJECTDIR}/_ext/1679430495/plib_sercom3_usart.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/Artemis_PDU" -I"../src/packs/ATSAME51N19A_DFP" -I"../src/packs/CMSIS/" -I"../src/packs/CMSIS/CMSIS/Core/Include" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1679430495/plib_sercom3_usart.o.d" -o ${OBJECTDIR}/_ext/1679430495/plib_sercom3_usart.o ../src/config/Artemis_PDU/peripheral/sercom/usart/plib_sercom3_usart.c    -DXPRJ_Artemis_PDU=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/_ext/1186129630/xc32_monitor.o: ../src/config/Artemis_PDU/stdio/xc32_monitor.c  .generated_files/flags/Artemis_PDU/10659c9e178585d142aca80181cae53aecb5ff22 .generated_files/flags/Artemis_PDU/50acabce23f44026a21b3861a7facc0a827a519
	@${MKDIR} "${OBJECTDIR}/_ext/1186129630" 
	@${RM} ${OBJECTDIR}/_ext/1186129630/xc32_monitor.o.d 
	@${RM} ${OBJECTDIR}/_ext/1186129630/xc32_monitor.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/Artemis_PDU" -I"../src/packs/ATSAME51N19A_DFP" -I"../src/packs/CMSIS/" -I"../src/packs/CMSIS/CMSIS/Core/Include" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1186129630/xc32_monitor.o.d" -o ${OBJECTDIR}/_ext/1186129630/xc32_monitor.o ../src/config/Artemis_PDU/stdio/xc32_monitor.c    -DXPRJ_Artemis_PDU=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/_ext/974488028/initialization.o: ../src/config/Artemis_PDU/initialization.c  .generated_files/flags/Artemis_PDU/b4faa2bf4da368644316b8200b285862177d173e .generated_files/flags/Artemis_PDU/50acabce23f44026a21b3861a7facc0a827a519
	@${MKDIR} "${OBJECTDIR}/_ext/974488028" 
	@${RM} ${OBJECTDIR}/_ext/974488028/initialization.o.d 
	@${RM} ${OBJECTDIR}/_ext/974488028/initialization.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/Artemis_PDU" -I"../src/packs/ATSAME51N19A_DFP" -I"../src/packs/CMSIS/" -I"../src/packs/CMSIS/CMSIS/Core/Include" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/974488028/initialization.o.d" -o ${OBJECTDIR}/_ext/974488028/initialization.o ../src/config/Artemis_PDU/initialization.c    -DXPRJ_Artemis_PDU=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/_ext/974488028/interrupts.o: ../src/config/Artemis_PDU/interrupts.c  .generated_files/flags/Artemis_PDU/d3554039717464bb348ddef86eb956a3e97696bc .generated_files/flags/Artemis_PDU/50acabce23f44026a21b3861a7facc0a827a519
	@${MKDIR} "${OBJECTDIR}/_ext/974488028" 
	@${RM} ${OBJECTDIR}/_ext/974488028/interrupts.o.d 
	@${RM} ${OBJECTDIR}/_ext/974488028/interrupts.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/Artemis_PDU" -I"../src/packs/ATSAME51N19A_DFP" -I"../src/packs/CMSIS/" -I"../src/packs/CMSIS/CMSIS/Core/Include" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/974488028/interrupts.o.d" -o ${OBJECTDIR}/_ext/974488028/interrupts.o ../src/config/Artemis_PDU/interrupts.c    -DXPRJ_Artemis_PDU=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/_ext/974488028/exceptions.o: ../src/config/Artemis_PDU/exceptions.c  .generated_files/flags/Artemis_PDU/3af67b9dd99adb5c0375d050620cdebfc8e26ac5 .generated_files/flags/Artemis_PDU/50acabce23f44026a21b3861a7facc0a827a519
	@${MKDIR} "${OBJECTDIR}/_ext/974488028" 
	@${RM} ${OBJECTDIR}/_ext/974488028/exceptions.o.d 
	@${RM} ${OBJECTDIR}/_ext/974488028/exceptions.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/Artemis_PDU" -I"../src/packs/ATSAME51N19A_DFP" -I"../src/packs/CMSIS/" -I"../src/packs/CMSIS/CMSIS/Core/Include" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/974488028/exceptions.o.d" -o ${OBJECTDIR}/_ext/974488028/exceptions.o ../src/config/Artemis_PDU/exceptions.c    -DXPRJ_Artemis_PDU=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/_ext/974488028/startup_xc32.o: ../src/config/Artemis_PDU/startup_xc32.c  .generated_files/flags/Artemis_PDU/ec7ec634ce79c18893d75c8ff0918539ef8e81a1 .generated_files/flags/Artemis_PDU/50acabce23f44026a21b3861a7facc0a827a519
	@${MKDIR} "${OBJECTDIR}/_ext/974488028" 
	@${RM} ${OBJECTDIR}/_ext/974488028/startup_xc32.o.d 
	@${RM} ${OBJECTDIR}/_ext/974488028/startup_xc32.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/Artemis_PDU" -I"../src/packs/ATSAME51N19A_DFP" -I"../src/packs/CMSIS/" -I"../src/packs/CMSIS/CMSIS/Core/Include" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/974488028/startup_xc32.o.d" -o ${OBJECTDIR}/_ext/974488028/startup_xc32.o ../src/config/Artemis_PDU/startup_xc32.c    -DXPRJ_Artemis_PDU=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/_ext/974488028/libc_syscalls.o: ../src/config/Artemis_PDU/libc_syscalls.c  .generated_files/flags/Artemis_PDU/ba1dae8996bf2d2eb07413f24453e1a0963d0fb .generated_files/flags/Artemis_PDU/50acabce23f44026a21b3861a7facc0a827a519
	@${MKDIR} "${OBJECTDIR}/_ext/974488028" 
	@${RM} ${OBJECTDIR}/_ext/974488028/libc_syscalls.o.d 
	@${RM} ${OBJECTDIR}/_ext/974488028/libc_syscalls.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/Artemis_PDU" -I"../src/packs/ATSAME51N19A_DFP" -I"../src/packs/CMSIS/" -I"../src/packs/CMSIS/CMSIS/Core/Include" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/974488028/libc_syscalls.o.d" -o ${OBJECTDIR}/_ext/974488028/libc_syscalls.o ../src/config/Artemis_PDU/libc_syscalls.c    -DXPRJ_Artemis_PDU=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/_ext/1360937237/main.o: ../src/main.c  .generated_files/flags/Artemis_PDU/994a4b6d5b57e967873c7979efe2be47118bbe32 .generated_files/flags/Artemis_PDU/50acabce23f44026a21b3861a7facc0a827a519
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/main.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/main.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/Artemis_PDU" -I"../src/packs/ATSAME51N19A_DFP" -I"../src/packs/CMSIS/" -I"../src/packs/CMSIS/CMSIS/Core/Include" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/main.o.d" -o ${OBJECTDIR}/_ext/1360937237/main.o ../src/main.c    -DXPRJ_Artemis_PDU=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}" ${PACK_COMMON_OPTIONS} 
	
${OBJECTDIR}/_ext/1360937237/temp.o: ../src/temp.c  .generated_files/flags/Artemis_PDU/625a0b7cc5d6a1a3ef667408d1208aae0c2fd9e9 .generated_files/flags/Artemis_PDU/50acabce23f44026a21b3861a7facc0a827a519
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/temp.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/temp.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -ffunction-sections -fdata-sections -O1 -fno-common -I"../src" -I"../src/config/Artemis_PDU" -I"../src/packs/ATSAME51N19A_DFP" -I"../src/packs/CMSIS/" -I"../src/packs/CMSIS/CMSIS/Core/Include" -Werror -Wall -MP -MMD -MF "${OBJECTDIR}/_ext/1360937237/temp.o.d" -o ${OBJECTDIR}/_ext/1360937237/temp.o ../src/temp.c    -DXPRJ_Artemis_PDU=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}" ${PACK_COMMON_OPTIONS} 
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compileCPP
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${DISTDIR}/Artemis_PDU.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    ../src/config/Artemis_PDU/ATSAME51N19A.ld
	@${MKDIR} ${DISTDIR} 
	${MP_CC} $(MP_EXTRA_LD_PRE) -g   -mprocessor=$(MP_PROCESSOR_OPTION) -mno-device-startup-code -o ${DISTDIR}/Artemis_PDU.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}          -DXPRJ_Artemis_PDU=$(CND_CONF)    $(COMPARISON_BUILD)  -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--defsym=__ICD2RAM=1,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,-D=__DEBUG_D,--defsym=_min_heap_size=512,--gc-sections,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--memorysummary,${DISTDIR}/memoryfile.xml -mdfp="${DFP_DIR}"
	
else
${DISTDIR}/Artemis_PDU.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   ../src/config/Artemis_PDU/ATSAME51N19A.ld
	@${MKDIR} ${DISTDIR} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -mprocessor=$(MP_PROCESSOR_OPTION) -mno-device-startup-code -o ${DISTDIR}/Artemis_PDU.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}          -DXPRJ_Artemis_PDU=$(CND_CONF)    $(COMPARISON_BUILD)  -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--defsym=_min_heap_size=512,--gc-sections,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--memorysummary,${DISTDIR}/memoryfile.xml -mdfp="${DFP_DIR}"
	${MP_CC_DIR}\\xc32-bin2hex ${DISTDIR}/Artemis_PDU.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} 
endif


# Subprojects
.build-subprojects:


# Subprojects
.clean-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${OBJECTDIR}
	${RM} -r ${DISTDIR}

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(shell mplabwildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
