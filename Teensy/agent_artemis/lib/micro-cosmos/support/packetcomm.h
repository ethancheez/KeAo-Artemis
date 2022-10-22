#ifndef _PACKETCOMM_H
#define _PACKETCOMM_H

#include <queue>
#include <TeensyThreads.h>
#include <stdint.h>

namespace Cosmos
{
    namespace Support
    {
        class PacketComm
        {
        public:
            int packetLen;
            char *packet;

            int32_t PushQueue(std::queue<PacketComm> &queue, Threads::Mutex &mtx);
            int32_t PullQueue(std::queue<PacketComm> &queue, Threads::Mutex &mtx);
        };
    }
}

#endif // _PACKETCOMM_H