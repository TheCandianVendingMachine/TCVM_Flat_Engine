// tree.hpp
// Generic tree structure
#pragma once
#include "../flatEngineExport.hpp"
#include "../objectManagement/handleManager.hpp"
#include <memory>
#include <functional>
#include <vector>

namespace fe
    {
        namespace priv
            {
                struct node
                    {
                        std::vector<int> m_children;
                        int m_parent = -1;
                        bool m_leaf = true;

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

                    // Returns if the node is avaliable in the highest-root
                    FLAT_ENGINE_API bool nodeExists(int node, int nodeCheck);

                    // Sorts the current node by node children
                    FLAT_ENGINE_API void sort(int node);
                    // Sorts the current node by function
                    FLAT_ENGINE_API void sort(int node, std::function<bool(int, int)> check);

            };
    }