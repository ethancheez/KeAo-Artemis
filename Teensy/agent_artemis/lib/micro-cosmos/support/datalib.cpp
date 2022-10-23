#include <support/datalib.h>
#include <Arduino.h>

namespace Cosmos
{
    namespace Support
    {
        // Define the static member variables here
        std::map<String, uint8_t> NodeData::node_ids;

        //! Loads node table from nodeids.ini configuration file
        //! nodeids is a vector of node name strings indexed by a node_id
        int32_t NodeData::load_node_ids()
        {
            for (size_t i = 0; i < sizeof(_node_list) / sizeof(_node_list_struct); i++)
            {
                node_ids[_node_list[i].node_name] = _node_list[i].node_id;
            }

            return NodeData::node_ids.size();
        }

        //! Check if a node_id is in the node table
        //! \param node_id
        //! \return node_id on success, NODEIDUNKNOWN (0) if not found, negative on error
        int32_t NodeData::check_node_id(NODE_ID_TYPE node_id)
        {

            if (NodeData::load_node_ids() <= 0)
            {
                return NODEIDUNKNOWN;
            }

            for (auto it = node_ids.begin(); it != node_ids.end(); ++it)
            {
                if (it->second == node_id)
                {
                    return node_id;
                }
            }
            return NODEIDUNKNOWN;
        }

        //! Gets the node_id associated with a node name
        //! \return node_id on success, NODEIDUNKNOWN (0) if not found, negative on error
        int32_t NodeData::lookup_node_id(String node_name)
        {
            int32_t iretn = 0;

            if ((iretn = NodeData::load_node_ids()) <= 0)
            {
                return NODEIDUNKNOWN;
            }

            auto it = node_ids.find(node_name);
            if (it == node_ids.end())
            {
                return NODEIDUNKNOWN;
            }
            return it->second;
        }

        //! Find the node name associated with the given node id in the node table.
        //! \param node_id Node ID
        //! \return Node name on success, or empty string on failure
        String NodeData::lookup_node_id_name(NODE_ID_TYPE node_id)
        {
            if (node_id == NodeData::NODEIDORIG)
            {
                return "Origin";
            }
            else if (node_id == NodeData::NODEIDDEST)
            {
                return "Destination";
            }
            else if (node_id == NodeData::NODEIDUNKNOWN || NodeData::load_node_ids() <= 0)
            {
                return "";
            }

            for (auto it = node_ids.begin(); it != node_ids.end(); ++it)
            {
                if (it->second == node_id)
                {
                    return it->first;
                }
            }

            return "";
        }
    }
}
