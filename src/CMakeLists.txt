set(FE_SRC
       fe/engine.cpp
       fe/dataStructures/tree.cpp
       fe/debug/debugDraw.cpp
       fe/debug/logger.cpp
       fe/debug/profilerLogger.cpp
       fe/entity/baseEntity.cpp
       fe/entity/baseEntityUtilities.cpp
       fe/entity/scriptObject.cpp
       fe/entity/component/component.cpp
       fe/entity/component/componentManager.cpp
       fe/gui/button.cpp
       fe/gui/checkBox.cpp
       fe/gui/checkList.cpp
       fe/gui/control.cpp
       fe/gui/dialog.cpp
       fe/gui/guiGraph.cpp
       fe/gui/inputBox.cpp
       fe/gui/listButton.cpp
       fe/gui/radioButton.cpp
       fe/gui/radioList.cpp
       fe/gui/slider.cpp
       fe/gui/text.cpp
       fe/localization/localizationStorage.cpp
       fe/math/matrix.cpp
       fe/math/polygon2.cpp
       fe/math/random.cpp
       fe/objectManagement/str.cpp
       fe/subsystems/ai/graph.cpp
       fe/subsystems/ai/graphNavigation.cpp
       fe/subsystems/collision/aabbTree.cpp
       fe/subsystems/collision/broadphaseAbstract.cpp
       fe/subsystems/collision/collisionWorld.cpp
       fe/subsystems/collision/bounds/AABB.cpp
       fe/subsystems/collision/bounds/aabbTests.cpp
       fe/subsystems/collision/bounds/circle.cpp
       fe/subsystems/collision/bounds/circleTests.cpp
       fe/subsystems/collision/bounds/mixedTests.cpp
       fe/subsystems/entitySpawner/entitySpawner.cpp
       fe/subsystems/entitySpawner/prefabObject.cpp
       fe/subsystems/entitySpawner/userEntityObject.cpp
       fe/subsystems/filesystem/fileUtilities.cpp
       fe/subsystems/gameState/entityWorld.cpp
       fe/subsystems/gameState/gameScreen.cpp
       fe/subsystems/gameState/gameState.cpp
       fe/subsystems/gameState/gameStateMachine.cpp
       fe/subsystems/gameState/gameWorld.cpp
       fe/subsystems/graphic/camera.cpp
       fe/subsystems/graphic/renderer.cpp
       fe/subsystems/graphic/renderGraph.cpp
       fe/subsystems/graphic/texturePacker.cpp
       fe/subsystems/graphic/tileMap.cpp
       fe/subsystems/graphic/animation/animationActor.cpp
       fe/subsystems/graphic/animation/animationTexture.cpp
       fe/subsystems/graphic/animation/animator.cpp
       fe/subsystems/graphic/renderObject/renderObject.cpp
       fe/subsystems/graphic/renderObject/sceneGraph.cpp
       fe/subsystems/graphic/renderObject/spriteBatch.cpp
       fe/subsystems/input/inputManager.cpp
       fe/subsystems/memory/dynamicMemoryAllocater.cpp
       fe/subsystems/memory/feNew.cpp
       fe/subsystems/memory/memoryManager.cpp
       fe/subsystems/memory/stackAllocater.cpp
       fe/subsystems/messaging/eventHandler.cpp
       fe/subsystems/messaging/eventSender.cpp
       fe/subsystems/messaging/gameEvent.cpp
       fe/subsystems/parsing/xmlParsing.cpp
       fe/subsystems/particle/particleBatch.cpp
       fe/subsystems/particle/particleSystem.cpp
       fe/subsystems/physics/physicsEngine.cpp
       fe/subsystems/physics/rigidBody.cpp
       fe/subsystems/physics/transformable.cpp
       fe/subsystems/scripting/commonScriptFunctions.cpp
       fe/subsystems/scripting/enumHandler.cpp
       fe/subsystems/scripting/functionHander.cpp
       fe/subsystems/scripting/luaFunctionReference.cpp
       fe/subsystems/scripting/scriptHelpers.cpp
       fe/subsystems/scripting/scriptManager.cpp
       fe/subsystems/scripting/tableHandler.cpp
       fe/subsystems/scripting/userTypeHandler.cpp
       fe/subsystems/serializer/feDataWriter.cpp
       fe/subsystems/serializer/serializerID.cpp
       fe/subsystems/threading/rayJob.cpp
       fe/subsystems/threading/threadJob.cpp
       fe/time/clock.cpp
       fe/time/countdown.cpp
       fe/time/time.cpp
       fe/utility/splitString.cpp
)
    
set(FE_LIB ${PROJECT_NAME})

add_library(${FE_LIB} SHARED ${FE_SRC})
set_target_properties(${FE_LIB} PROPERTIES DEBUG_POSTFIX -d)
fe_link_sfml(${FE_LIB})
fe_link_lua(${FE_LIB})

set_target_properties(${FE_LIB} PROPERTIES FILE "fe")

install(TARGETS ${FE_LIB}
                RUNTIME DESTINATION bin
                LIBRARY DESTINATION lib
                ARCHIVE DESTINATION lib)

