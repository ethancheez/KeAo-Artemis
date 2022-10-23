#ifndef _PACKETCOMM_H
#define _PACKETCOMM_H

#include <queue>
#include <TeensyThreads.h>
#include <stdint.h>
#include <support/datalib.h>

namespace Cosmos
{
    namespace Support
    {
        class PacketComm
        {
        public:
            enum class TypeId : uint8_t
            {
                None = 0,
                DataBeacon = 10,
                DataPong = 41,
                DataEPSResponse = 43,
                DataADCSResponse = 45,
                DataResponse = 61,
                DataIP = 62,
                DataTest = 63,
                DataTime = 64,
                DataFileCommand = 71,
                DataFileMessage = 72,
                DataFileQueue = 79,
                DataFileCancel = 80,
                DataFileComplete = 81,
                DataFileReqMeta = 82,
                DataFileReqData = 83,
                DataFileMetaData = 84,
                DataFileChunkData = 85,
                DataFileReqComplete = 86,
                CommandReset = 128,
                CommandReboot = 129,
                CommandSendBeacon = 130,
                CommandClearQueue = 131,
                CommandExternalCommand = 132,
                CommandTestRadio = 133,
                CommandListDirectory = 134,
                CommandTransferFile = 135,
                CommandTransferNode = 136,
                CommandTransferRadio = 137,
                CommandTransferList = 138,
                CommandInternalRequest = 140,
                CommandPing = 141,
                CommandSetTime = 142,
                CommandGetTimeHuman = 143,
                CommandGetTimeBinary = 144,
                CommandAdcsCommunicate = 150,
                CommandAdcsState = 151,
                CommandAdcsSetRunMode = 152,
                CommandEpsCommunicate = 160,
                CommandEpsSwitchName = 161,
                CommandEpsSwitchNumber = 162,
                CommandEpsReset = 163,
                CommandEpsState = 164,
                CommandEpsWatchdog = 165,
                CommandEpsSetTime = 166,
                CommandExecLoadCommand = 170,
                CommandExecAddCommand = 171,
            };

            enum class RadioId: uint8_t {
                RFM23,
                RFM98,
                LITHIUM_3,
            };

            struct __attribute__ ((packed))  Header
            {
                uint16_t data_size;
                TypeId type;
                RadioId radio;
                NodeData::NODE_ID_TYPE orig = NodeData::NODEIDORIG;
                NodeData::NODE_ID_TYPE dest = NodeData::NODEIDDEST;
            } header;

            std::vector<uint8_t> data;

            int32_t PushQueue(std::queue<PacketComm> &queue, Threads::Mutex &mtx);
            int32_t PullQueue(std::queue<PacketComm> &queue, Threads::Mutex &mtx);
        };
    }
}

#endif // _PACKETCOMM_H