// tree.hpp
// Generic tree structure
#pragma once
#define FLAT_ENGINE_EXPORT
#include "../flatEngineExport.hpp"
#include "../objectManagement/handleManager.hpp"
#include <memory>
#include <vector>

namespace fe
    {
        namespace priv
            {
                struct node
                    {
                        std::vector<int> m_children;
                        int m_parent = -1;
                        void *m_userData = nullptr;
                    };
            }

        class tree : private fe::handleManager<priv::node*, 0>
            {
                public:
                    FLAT_ENGINE_API tree();

                    FLAT_ENGINE_API int addNode();
                    FLAT_ENGINE_API void removeNode(int node);

                    FLAT_ENGINE_API void addChild(int node, int child);
                    FLAT_ENGINE_API void removeChild(int node, int child);

                    FLAT_ENGINE_API priv::node *getNode(int node);

            };
    }