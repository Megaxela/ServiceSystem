#include <iostream>
#include "Component.hpp"
#include "Engine.hpp"

Service::Component::Component(Service::Engine* engine, Service::Component::IdType id) :
    m_id(id),
    m_engine(engine),
    m_messageQueue(),
    m_idCounter(0),
    m_delayedMessageQueue(),
    m_hasMessageCV(),
    m_running(false)
{

}

void Service::Component::stop()
{
    m_running = false;
    m_hasMessageCV.notify_all();
}

Service::Component::IdType Service::Component::identifier() const
{
    return m_id;
}

void Service::Component::sendMessage(Service::Component::IdType target, Service::Message mess)
{
    m_engine->sendMessage(target, std::move(mess));
}

bool Service::Component::deleteTimedMessage(Service::Component::IdType target, std::size_t id)
{
    return m_engine->deleteDelayedMessage(target, id);
}

std::size_t Service::Component::sendMessageImpl(Service::Component::IdType target,
                                                Service::Message mess,
                                                std::chrono::microseconds mcs)
{
    return m_engine->sendMessageDelayed(target, std::move(mess), mcs);
}

std::size_t Service::Component::addDelayedMessageToQueue(Service::Message mess,
                                                         std::chrono::microseconds delay)
{
    std::lock_guard lock(m_messageQueueMutex);

    m_delayedMessageQueue[{
        std::chrono::steady_clock::now() + delay,
        ++m_id
    }] = std::move(mess);

    m_hasMessageCV.notify_one();

    return m_id;
}

bool Service::Component::deleteDelayedMessageFromQueue(std::size_t id)
{
    std::lock_guard lock(m_messageQueueMutex);

    auto message = std::find_if(
        m_delayedMessageQueue.begin(),
        m_delayedMessageQueue.end(),
        [id](auto& val)
        {
            return val.first.second == id;
        }
    );

    if (message == m_delayedMessageQueue.end())
    {
        return false;
    }

    m_delayedMessageQueue.erase(message);

    return true;
}

std::size_t Service::Component::messageQueueSize() const
{
    std::unique_lock lock(m_messageQueueMutex);

    return m_messageQueue.size();
}

bool Service::Component::receiveMessage(Service::Message& m)
{
    std::unique_lock lock(m_messageQueueMutex);

    if (!m_delayedMessageQueue.empty() &&
        std::chrono::steady_clock::now() >= m_delayedMessageQueue.begin()->first.first)
    {
        m = std::move(m_delayedMessageQueue.begin()->second);
        m_delayedMessageQueue.erase(m_delayedMessageQueue.begin());
        return true;
    }

    if (m_messageQueue.empty())
    {
        return false;
    }

    m = std::move(m_messageQueue.back());
    m_messageQueue.pop();

    return true;
}

bool Service::Component::receiveMessageBlocking(Service::Message& m)
{
    std::unique_lock lock(m_messageQueueMutex);

    while (m_running &&
           m_messageQueue.empty() &&
           (m_delayedMessageQueue.empty() ||
            std::chrono::steady_clock::now() < m_delayedMessageQueue.begin()->first.first))
    {
        if (m_delayedMessageQueue.empty())
        {
            m_hasMessageCV.wait(lock);
        }
        else
        {
            m_hasMessageCV.wait_until(
                lock,
                m_delayedMessageQueue.begin()->first.first
            );
        }
    }


    if (!m_running)
    {
        return false;
    }

    // Is delayed message available
    if (!m_delayedMessageQueue.empty() &&
        std::chrono::steady_clock::now() >= m_delayedMessageQueue.begin()->first.first)
    {
        m = std::move(m_delayedMessageQueue.begin()->second);
        m_delayedMessageQueue.erase(m_delayedMessageQueue.begin());
        return true;
    }
    else if (!m_messageQueue.empty())
    {
        m = std::move(m_messageQueue.back());
        m_messageQueue.pop();
        return true;
    }

    throw std::runtime_error("Unknown state.");

    return true;
}

void Service::Component::addMessageToQueue(Service::Message mess)
{
    std::unique_lock lock(m_messageQueueMutex);

    m_messageQueue.emplace(std::move(mess));

    m_hasMessageCV.notify_one();
}

bool Service::Component::isRunning() const
{
    return m_running;
}
