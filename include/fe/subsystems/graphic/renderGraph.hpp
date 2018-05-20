// renderGraph.hpp
// A generic render graph. Loops through nodes, transforms them, and then renders them
#pragma once
#define FLAT_ENGINE_EXPORT
#include "fe/flatEngineExport.hpp"
#include "fe/subsystems/physics/transformable.hpp"
#include "fe/dataStructures/tree.hpp"
#include <unordered_map>
#include <SFML/Graphics/RenderStates.hpp>

namespace sf
    {
        class Texture;
        class RenderTarget;
    }

namespace fe
    {
        class renderGraph
            {
                private:
                    unsigned int m_baseNode;
                    fe::transformable m_baseTransform;

                    fe::tree m_sceneRenderTree;
                    std::unordered_map<int, int> m_zOrderMap;

                    const unsigned int m_maxObjectsUntilThread;

                    FLAT_ENGINE_API void addZ(int z);
                    FLAT_ENGINE_API int getZ(int z);

                protected:
                    virtual void onObjectAdd(void *object, int node) {}
                    virtual void onObjectRemove(void *object, int node) {}

                    virtual int getNodeFromObject(void *object) = 0;
                    virtual fe::transformable *getNodeTransform(fe::priv::node *node) = 0;
                    virtual fe::transformable *getNodeTempTransform(fe::priv::node *node) = 0;
                    virtual const sf::Texture *getRenderTexture() const = 0;
                    virtual bool renderNode(fe::priv::node *node) const { return true; }
                    virtual void drawNode(fe::priv::node *node, sf::RenderTarget &target) = 0;

                    virtual void drawToScreen(sf::RenderTarget &target, sf::RenderStates states) = 0;

                    virtual void *createZOrderProxy() = 0;

                    virtual void onStartUp() {}
                    virtual void onShutDown() {}
                    virtual void onClear() {}

                public:
                    FLAT_ENGINE_API renderGraph(unsigned int maxObjectsUntilThread);

                    FLAT_ENGINE_API void startUp();
                    FLAT_ENGINE_API void shutDown();
                    FLAT_ENGINE_API void clear();

                    FLAT_ENGINE_API int addObjectToGraph(void *object, int connected = -1, int zPos = 0);
                    FLAT_ENGINE_API int removeObjectFromGraph(void *object);
                    FLAT_ENGINE_API int removeNodeFromGraph(int node);

                    virtual void preDraw() = 0;
                    FLAT_ENGINE_API void draw(sf::RenderTarget &window);

                    FLAT_ENGINE_API void setZOrder(int node, int z);
                    FLAT_ENGINE_API void connect(int a, int b); // connects object with node A to node B
                    FLAT_ENGINE_API void disconnect(int node); // Disconnects the node and attaches to the base scene node
                    FLAT_ENGINE_API int getConnected(int node); // Returns the node which the object is attached to

                    FLAT_ENGINE_API fe::transformable &getGlobalTransform();
            };
    }
