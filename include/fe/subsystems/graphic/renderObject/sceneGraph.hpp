// sceneGraph.hpp
// a graph for the scene. Contains all objects, and manages their positions
#pragma once
#include "../../../flatEngineExport.hpp"
#include "../../../typeDefines.hpp"
#include "../../memory/poolAllocater.hpp"
#include "spriteBatch.hpp"
#include "renderObject.hpp"
#include "../../threading/threadJob.hpp"
#include "../../../dataStructures/tree.hpp"
#include "../../resourceManager/fontData.hpp"
#include <type_traits>
#include <unordered_map>

namespace sf
    {
        class RenderTarget;
        class Font;
    }

namespace fe
    {
        class sceneGraph
            {
                private:
                    fe::sceneGraphObject m_baseNode;
                    // The immediate children of this tree are all Z Position nodes
                    fe::tree m_sceneRenderTree;
                    // A map in the form of { Z Position, Node the Z is represented by }
                    std::unordered_map<int, int> m_zOrderMap;

                    fe::poolAllocater<sceneGraphObject> m_sceneGraphObjects; // Generic scene graph allocater so we can have some Z-Order
                    fe::poolAllocater<renderObject> m_renderObjects;
                    fe::poolAllocater<renderText> m_renderTextObjects;
                    fe::spriteBatch m_batch;

                    const unsigned int m_maxObjectsUntilThread;

                    FLAT_ENGINE_API void createRenderTextObject(sceneGraphObject *obj, const fe::fontData &font);
                    FLAT_ENGINE_API int deleteRenderObject(renderObject *obj); // returns the parent node of the render object
                    FLAT_ENGINE_API int deleteRenderTextObject(renderText *obj); // returns the parent node of the render object

                    FLAT_ENGINE_API void addZ(int z);
                    // Returns the node that the Z positon is mapped to
                    FLAT_ENGINE_API int getZ(int z);
                    FLAT_ENGINE_API bool doesZExist(int z);

                public:
                    FLAT_ENGINE_API sceneGraph();

                    FLAT_ENGINE_API void startUp();
                    FLAT_ENGINE_API void shutDown();
                    FLAT_ENGINE_API void clear();

                    FLAT_ENGINE_API void preDraw();
                    FLAT_ENGINE_API void draw(sf::RenderTarget &window);
                    FLAT_ENGINE_API sceneGraphObject *allocateRenderObject();
                    FLAT_ENGINE_API sceneGraphObject *allocateRenderText();
                    FLAT_ENGINE_API void createSceneGraphObject(sceneGraphObject *obj, int connected, const fe::fontData &font = fe::fontData());
                    FLAT_ENGINE_API int deleteSceneObject(sceneGraphObject *obj); // returns the parent node of the render object

                    FLAT_ENGINE_API void setZOrder(int node, int z);
                    FLAT_ENGINE_API void connect(int a, int b); // connects object with node A to node B
                    FLAT_ENGINE_API void disconnect(int node); // Disconnects the node and attaches to the base scene node
                    FLAT_ENGINE_API int getConnected(int node); // Returns the node which the object is attached to

                    FLAT_ENGINE_API void setZOrder(sceneGraphObject *object, int z);
                    FLAT_ENGINE_API void connect(sceneGraphObject *objectA, sceneGraphObject *objectB); // connects object with node A to node B
                    FLAT_ENGINE_API void disconnect(sceneGraphObject *object); // Disconnects the node and attaches to the base scene node
                    FLAT_ENGINE_API int getConnected(sceneGraphObject *object); // Returns the node which the object is attached to

                    FLAT_ENGINE_API fe::transformable &getGlobalTransform();

            };
    }