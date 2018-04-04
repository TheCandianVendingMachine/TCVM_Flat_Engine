
set(FE_SRC
		fe/engine.cpp
		fe/dataStructures/tree.cpp
		fe/debug/debugDraw.cpp
		fe/debug/logger.cpp
		fe/debug/profilerLogger.cpp
		fe/entity/baseEntity.cpp
		fe/entity/scriptObject.cpp
		fe/gui/button.cpp
		fe/gui/guiElement.cpp
		fe/gui/guiPrefabricatedElements.cpp
		fe/gui/label.cpp
		fe/gui/panel.cpp
		fe/gui/square.cpp
		fe/gui/textBox.cpp
		fe/gui/toggleButton.cpp
		fe/localization/localizationStorage.cpp
		fe/math/matrix.cpp
		fe/math/random.cpp
		fe/subsystems/ai/graph.cpp
		fe/subsystems/ai/graphNavigation.cpp
		fe/subsystems/entitySpawner/entitySpawner.cpp
		fe/subsystems/entitySpawner/userEntityObject.cpp
		fe/subsystems/filesystem/fileUtilities.cpp
		fe/subsystems/gameState/entityWorld.cpp
		fe/subsystems/gameState/gameState.cpp
		fe/subsystems/gameState/gameStateMachine.cpp
		fe/subsystems/gameState/gameWorld.cpp
		fe/subsystems/graphic/animation/animationActor.cpp
		fe/subsystems/graphic/animation/animationTexture.cpp
		fe/subsystems/graphic/animation/animator.cpp
		fe/subsystems/graphic/renderObject/renderObject.cpp
		fe/subsystems/graphic/renderObject/sceneGraph.cpp
		fe/subsystems/graphic/renderObject/spriteBatch.cpp
		fe/subsystems/graphic/camera.cpp
		fe/subsystems/graphic/renderer.cpp
		fe/subsystems/graphic/texturePacker.cpp
		fe/subsystems/graphic/tileMap.cpp
		fe/subsystems/input/inputManager.cpp
		fe/subsystems/memory/dynamicMemoryAllocater.cpp
		fe/subsystems/memory/feNew.cpp
		fe/subsystems/memory/memoryManager.cpp
		fe/subsystems/memory/stackAllocater.cpp
		fe/subsystems/messaging/eventHandler.cpp
		fe/subsystems/messaging/eventSender.cpp
		fe/subsystems/messaging/gameEvent.cpp
		fe/subsystems/parsing/xmlParsing.cpp
		fe/subsystems/physics/collision/aabbTree.cpp
		fe/subsystems/physics/collision/broadphaseAbstract.cpp
		fe/subsystems/physics/collision/collisionBounds.cpp
		fe/subsystems/physics/collision/collisionWorld.cpp
		fe/subsystems/physics/physicsEngine.cpp
		fe/subsystems/physics/rigidBody.cpp
		fe/subsystems/physics/transformable.cpp
		fe/subsystems/scripting/commonScriptFunctions.cpp
		fe/subsystems/scripting/enumHandler.cpp
		fe/subsystems/scripting/functionHander.cpp
		fe/subsystems/scripting/scriptManager.cpp
		fe/subsystems/scripting/userTypeHandler.cpp
		fe/subsystems/serializer/serializerID.cpp
		fe/time/clock.cpp
		fe/time/countdown.cpp
		fe/time/time.cpp
		fe/engine.cpp)
	
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

