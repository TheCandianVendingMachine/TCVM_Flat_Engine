#include "eventSender.hpp"
#include "eventHandler.hpp"
#include <algorithm>

void fe::eventSender::sendEvent(gameEvent &msg)
    {
        msg.eventSent = m_elapsedTime.getTime().asMilliseconds();
        switch (msg.sendType)
            {
                case fe::sendType::SEND_TO_ALL:
                    for (auto &handler : m_handlers)
                        {
                            handler.second->handleEvent(msg);
                        }
                    break;
                case fe::sendType::SEND_TO_EVENT:
                    {
                        auto event = msg.eventType;
                        for (auto &observer : m_observers[event])
                            {
                                observer->handleEvent(msg);
                            }
                    }
                    break;
                case fe::sendType::SEND_TO_TARGET:
                    {
                        auto handler = m_handlers[msg.target];
                        if (handler)
                            {
                                handler->handleEvent(msg);
                            }
                    }
                    break;
                default:
                    break;
            }
    }

fe::eventSender::eventSender() : m_currentFrame(0)
    {
    }

void fe::eventSender::subscribe(eventHandler *handler)
    {
        m_handlers[handler->id()] = handler;
    }

void fe::eventSender::subscribe(eventHandler *handler, int messageType)
    {
        m_handlers[handler->id()] = handler;
        m_observers[messageType].push_back(handler);
    }

void fe::eventSender::unsubscribe(eventHandler *handler)
    {
        m_handlers.erase(handler->id());
    }

void fe::eventSender::unsubscribe(eventHandler *handler, int messageType)
    {
        m_observers[messageType].erase(std::remove(m_observers[messageType].begin(), m_observers[messageType].end(), handler), m_observers[messageType].end());
    }

void fe::eventSender::unsubscribeAll(eventHandler *handler)
    {
        m_handlers.erase(handler->id());
        for (auto &observer : m_observers)
            {
                observer.second.erase(std::remove(observer.second.begin(), observer.second.end(), handler), observer.second.end());
            }
    }

void fe::eventSender::clear()
    {
        m_handlers.clear();
        m_observers.clear();
    }


void fe::eventSender::send(gameEvent event, unsigned int to)
    {
        event.target = to;
        event.sendType = fe::sendType::SEND_TO_TARGET;

        event.sendTimeType = fe::gameEvent::timeVar::NONE;
        sendEvent(event);
    }

void fe::eventSender::send(gameEvent event, unsigned int to, float time)
    {
        event.target = to;
        event.sendType = fe::sendType::SEND_TO_TARGET;

        event.sendTimeType = fe::gameEvent::timeVar::TIME;
        event.sendTimeVar.TIME = time + m_elapsedTime.getTime().asMilliseconds();
                
        m_eventQueueTimed.push(event);
    }

void fe::eventSender::send(gameEvent event, unsigned int to, int frame)
    {
        event.target = to;
        event.sendType = fe::sendType::SEND_TO_TARGET;

        event.sendTimeType = fe::gameEvent::timeVar::FRAME;
        event.sendTimeVar.FRAME = frame + m_currentFrame;
                
        m_eventQueueFrame.push(event);
    }

void fe::eventSender::send(gameEvent event, int eventType)
    {
        event.eventType = eventType;
        event.sendType = fe::sendType::SEND_TO_EVENT;

        event.sendTimeType = fe::gameEvent::timeVar::NONE;

        sendEvent(event);
    }


void fe::eventSender::send(gameEvent event, int eventType, float time)
    {
        event.sendType = fe::sendType::SEND_TO_EVENT;

        event.sendTimeType = fe::gameEvent::timeVar::TIME;
        event.sendTimeVar.TIME = time + m_elapsedTime.getTime().asMilliseconds();

        m_eventQueueTimed.push(event);
    }


void fe::eventSender::send(gameEvent event, int eventType, int frame)
    {
        event.sendType = fe::sendType::SEND_TO_EVENT;

        event.sendTimeType = fe::gameEvent::timeVar::FRAME;
        event.sendTimeVar.FRAME = frame + m_currentFrame;

        m_eventQueueFrame.push(event);
    }


void fe::eventSender::sendGlobal(gameEvent event)
    {
        event.sendType = fe::sendType::SEND_TO_ALL;

        event.sendTimeType = fe::gameEvent::timeVar::NONE;
        sendEvent(event);
    }

void fe::eventSender::sendGlobal(gameEvent event, float time)
    {
        event.sendType = fe::sendType::SEND_TO_ALL;

        event.sendTimeType = fe::gameEvent::timeVar::TIME;
        event.sendTimeVar.TIME = time + m_elapsedTime.getTime().asMilliseconds();

        m_eventQueueTimed.push(event);
    }

void fe::eventSender::sendGlobal(gameEvent event, int frame)
    {
        event.sendType = fe::sendType::SEND_TO_ALL;

        event.sendTimeType = fe::gameEvent::timeVar::FRAME;
        event.sendTimeVar.FRAME = frame + m_currentFrame;

        m_eventQueueFrame.push(event);
    }

void fe::eventSender::sendEvents()
    {
        m_currentFrame++;

        while (!m_eventQueueFrame.empty() && m_eventQueueFrame.top().sendTimeVar.FRAME - m_currentFrame <= 0)
            {
                auto event = m_eventQueueFrame.top();
                sendEvent(event);
                m_eventQueueFrame.pop();
            }

        while (!m_eventQueueTimed.empty() && m_elapsedTime.getTime().asMilliseconds() >= m_eventQueueTimed.top().sendTimeVar.TIME)
            {
                auto event = m_eventQueueTimed.top();
                sendEvent(event);
                m_eventQueueTimed.pop();
            }
    }