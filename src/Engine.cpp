#include "Engine.hpp"

Service::Engine::Engine() :
    m_components()
{

}

Service::Engine::~Engine()
{
    for (auto&& val : m_components)
    {
        delete val.second;
    }
}

void Service::Engine::sendMessage(Service::Component::IdType target,
                                  Service::Message message)
{
    auto component = m_components.find(target);

    if (component == m_components.end())
    {
        throw std::invalid_argument("There is no target component " + std::to_string(target));
    }

    component->second->addMessageToQueue(std::move(message));
}

std::size_t Service::Engine::sendMessageDelayed(Service::Component::IdType target,
                                                Service::Message message,
                                                std::chrono::microseconds mcs)
{
    auto component = m_components.find(target);

    if (component == m_components.end())
    {
        throw std::invalid_argument("There is no target component " + std::to_string(target));
    }

    return component->second->addDelayedMessageToQueue(std::move(message), mcs);
}

bool Service::Engine::deleteDelayedMessage(Service::Component::IdType target, std::size_t id)
{
    auto component = m_components.find(target);

    if (component == m_components.end())
    {
        throw std::invalid_argument("There is no target component " + std::to_string(target));
    }

    return component->second->deleteDelayedMessageFromQueue(id);
}

void Service::Engine::runComponents()
{
    for (auto&& component : m_components)
    {
        component.second->run();
    }
}
