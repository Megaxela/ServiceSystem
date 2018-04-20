#include <iostream>
#include "ThreadComponent.hpp"

Service::ThreadComponent::ThreadComponent(Service::Engine* engine,
                                          Service::Component::IdType id) :
    Component(engine, id),
    m_thread()
{}

Service::ThreadComponent::~ThreadComponent()
{
    stop();

    // Waiting until thread is stopped.
    if (m_thread.joinable())
    {
        m_thread.join();
    }
}

void Service::ThreadComponent::run()
{
    // Running thread
    m_thread = std::thread(
        [this](){ threadJob(); }
    );
}
