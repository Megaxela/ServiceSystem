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

void Service::Engine::runComponents()
{
    for (auto&& component : m_components)
    {
        component.second->run();
    }
}
