// graph.hpp
// graph class. Is a basic graph
#pragma once
#include "../../flatEngineExport.hpp"
#include "node.hpp"
#include "../../objectManagement/handleManager.hpp"
#include "../serializer/serializable.hpp"
#include <vector>
#include <memory>

namespace fe
    {
        class graph;
        namespace graphNav
            {
                std::vector<int> aStar(graph&, int, int, float);
            }

        class graph : public fe::serializable, private fe::handleManager<ai::node*, 0>
            {
                private:
                    friend std::vector<int> graphNav::aStar(graph&, int, int, float);

                    FLAT_ENGINE_API int addNode();
                    FLAT_ENGINE_API void onAdd(ai::node **object, fe::Handle objectHandle);

                public:
                    FLAT_ENGINE_API int addNode(float posX, float posY, float cost = 1.f);
                    FLAT_ENGINE_API void removeNode(int nodeHandle);

                    FLAT_ENGINE_API void addEdge(int nodeA, int nodeB);
                    FLAT_ENGINE_API void removeEdge(int nodeID, int edge);
                    FLAT_ENGINE_API ai::node *getNode(int nodeID);
                    FLAT_ENGINE_API const std::vector<int> getAllNodes();

                    FLAT_ENGINE_API void clear();

                    FLAT_ENGINE_API void serialize(fe::serializerID &serializer) const;
                    FLAT_ENGINE_API void deserialize(fe::serializerID &serializer);

            };
    }
