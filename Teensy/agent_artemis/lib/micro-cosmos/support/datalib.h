#ifndef _DATALIB_H
#define _DATALIB_H

#include <stdint.h>
#include <map>
#include <WString.h>

struct _node_list_struct
{
    uint16_t node_id;
    String node_name;
};

static const struct _node_list_struct _node_list[] = {
    {1, "ground"},
    {2, "teensy"},
    {3, "rpi"},
    {4, "pleiades"},
};

namespace Cosmos
{
    namespace Support
    {
        class NodeData
        {
        public:
            //! A table of node_id:node_names, used in node-to-node communications
            static std::map<String, uint8_t> node_ids;
            typedef uint8_t NODE_ID_TYPE;
            static int32_t load_node_ids();
            static int32_t check_node_id(NODE_ID_TYPE node_id);
            static int32_t lookup_node_id(String node_name);
            static String lookup_node_id_name(NODE_ID_TYPE node_id);

            static constexpr uint8_t NODEIDUNKNOWN = 0;
            static constexpr uint8_t NODEIDORIG = 254;
            static constexpr uint8_t NODEIDDEST = 255;
        };
    }
}

#endif // _DATALIB_H
