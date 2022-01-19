cmake_minimum_required(VERSION 2.8)
project(rfm69)

#aux_source_directory(. SRC_LIST)
#add_executable(${PROJECT_NAME} ${SRC_LIST})

# point to cosmos/source folder
set(COSMOS_SOURCE   "../../../")
set(USE_COSMOS_FROM "SOURCE")



set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic -std=c++11 -Wno-invalid-offsetof -lwiringPi")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -lwiringPi")

# -----------------------------------------------
# To use COSMOS from the source folder
#include(${CMAKE_SOURCE_DIR}/use_cosmos_from_source.cmake)
include(${COSMOS_SOURCE}/core/cmake/use_cosmos_from_source.cmake)

#add_executable(agent_cpu "${COSMOS_SOURCE}/core/programs/agents/agent_cpu.cpp")
#target_link_libraries(agent_cpu
#    CosmosSupport
#    CosmosAgent
#    CosmosDeviceGeneral
#    CosmosDeviceCpu
#    CosmosDeviceDisk
#    CosmosMath
#    )

set(COSMOS_LIBS
    CosmosSupport
    CosmosAgent
    CosmosDeviceGeneral
    CosmosDeviceI2C
    CosmosDeviceSerial
    CosmosDeviceCpu
    CosmosDeviceDisk
    CosmosMath
    )




# -----------------------------------------------
# Windows specific
if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    set(COSMOS_LIBS ${COSMOS_LIBS} ws2_32)
endif()




include_directories("../include")


set(RFM69_SRC	libraries/rfm69.cpp)
add_library(RFM69 STATIC ${RFM69_SRC})
target_link_libraries(RFM69 ${COSMOS_LIBS})

set(CUBESAT_LIBS RFM69)



# ============================== Agents ==============================

add_executable(agent_rfm69 agent_rfm69.cpp)
target_link_libraries(agent_rfm69 ${COSMOS_LIBS} ${CUBESAT_LIBS} wiringPi)
