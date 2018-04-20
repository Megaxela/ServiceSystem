#include "Component.hpp"
#include "Engine.hpp"

Service::Component::Component(Service::Engine* engine, Service::Component::IdType id) :
    m_id(id),
    m_engine(engine),
    m_messageQueue(),
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

std::size_t Service::Component::messageQueueSize() const
{
    std::unique_lock lock(m_messageQueueMutex);

    return m_messageQueue.size();
}

bool Service::Component::receiveMessage(Service::Message& m)
{
    std::unique_lock lock(m_messageQueueMutex);

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

    m_hasMessageCV.wait(
        lock,
        [this]() -> bool
        {
            return !m_messageQueue.empty() || !m_running;
        }
    );

    if (!m_running)
    {
        return false;
    }

    m = std::move(m_messageQueue.back());
    m_messageQueue.pop();

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
