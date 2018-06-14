#include <iostream>
#include "ThreadComponent.hpp"

Service::ThreadComponent::ThreadComponent(Service::Engine* engine,
                                          Service::Component::IdType id,
                                          uint32_t numberOfThreads) :
    Component(engine, id),
    m_threads(),
    m_numberOfThreads(numberOfThreads)
{}

Service::ThreadComponent::~ThreadComponent()
{
    stop();

    // Waiting until thread is stopped.
    for (auto&& thread : m_threads)
    {
        if (thread.joinable())
        {
            thread.join();
        }
    }
}

void Service::ThreadComponent::run()
{
    // Running thread
    for (uint32_t i = 0; i < m_numberOfThreads; ++i)
    {
        m_threads.push_back(std::thread([this](){ threadJob(); }));
    }
}
