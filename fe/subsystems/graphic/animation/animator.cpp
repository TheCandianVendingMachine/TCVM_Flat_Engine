#include "animator.hpp"
#include "animationTexture.hpp"
#include "animationActor.hpp"
#include <algorithm>
#include <SFML/Graphics/VertexArray.hpp>

fe::Handle fe::animator::addAnimation(fe::Vector2<unsigned int> frameSize, fe::Vector2<unsigned int> animationSize, fe::Vector2<unsigned int> texturePosition, bool vertical)
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
