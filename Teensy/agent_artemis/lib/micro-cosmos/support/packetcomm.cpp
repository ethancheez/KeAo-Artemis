#include <support/packetcomm.h>

namespace Cosmos
{
    namespace Support
    {
        // Thread-safe way of pushing onto the packet queue
        int32_t PacketComm::PushQueue(std::queue<PacketComm> &queue, Threads::Mutex &mtx)
        {
            Threads::Scope scope(mtx);
            queue.push(*this);
            return 1;
        }
        // Thread-safe way of pulling from the packet queue
        int32_t PacketComm::PullQueue(std::queue<PacketComm> &queue, Threads::Mutex &mtx)
        {
            Threads::Scope scope(mtx);
            if (queue.size())
            {
                *this = queue.front();
                queue.pop();
                return 1;
            }
            else
            {
                return 0;
            }
        }
    }
}