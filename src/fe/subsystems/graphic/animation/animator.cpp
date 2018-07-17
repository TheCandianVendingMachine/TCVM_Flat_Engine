#include "fe/subsystems/graphic/animation/animator.hpp"
#include "fe/subsystems/graphic/animation/animationTexture.hpp"
#include "fe/subsystems/graphic/animation/animationActor.hpp"
#include "fe/typeDefines.hpp"
#include <algorithm>
#include <SFML/Graphics/VertexArray.hpp>

void fe::animator::startUp()
	{
		m_animationActorPool.startUp(FE_MAX_GAME_OBJECTS);
	}

void fe::animator::shutDown()
	{
		m_animationActorPool.clear();
	}

fe::animationActor *fe::animator::alloc(fe::renderObject *const object)
	{
		return m_animationActorPool.alloc(object);
	}

void fe::animator::free(animationActor *actor)
	{
		m_animationActorPool.free(actor);
	}

fe::Handle fe::animator::addAnimation(fe::Vector2<unsigned int> frameSize, fe::Vector2<unsigned int> animationSize, bool vertical, fe::Vector2<unsigned int> texturePosition)
    {
        return addObject(animationTexture(frameSize, texturePosition, animationSize, vertical));
    }

void fe::animator::removeAnimation(fe::Handle handle)
    {
        m_actors.erase(handle);
        removeObject(handle);
    }

void fe::animator::subscribe(animationActor *actor, fe::Handle animation)
    {
        m_actors[animation].push_back(actor);
        auto textureOffset = getObject(animation).getTexture(actor);
        actor->updateVerticies(textureOffset.first, textureOffset.second);
        actor->setCurrentFrame(0);
    }

void fe::animator::unsubscribe(animationActor *actor, fe::Handle animation)
    {
        auto &actorVector = m_actors[animation];
        actorVector.erase(std::remove(actorVector.begin(), actorVector.end(), actor), actorVector.end());
    }

void fe::animator::unsubscribe(animationActor *actor)
    {
        for (auto &handle : m_actors)
            {
                unsubscribe(actor, handle.first);
            }
    }

void fe::animator::updateTextures()
    {
        for (auto &animation : m_actors)
            {
                auto texture = getObject(animation.first);
                for (auto &actor : animation.second)
                    {
                        if (actor->needsUpdate(m_elapsedTime.getTime()))
                            {
                                auto textureOffset = texture.getTexture(actor);
                                actor->updateVerticies(textureOffset.first, textureOffset.second);
                                actor->iterateFrame(1);
                            }
                    }
            }
    }
