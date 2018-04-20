#include <chrono>
#include <ReceiverComponent.hpp>
#include "SenderComponent.hpp"
#include <cstddef>
#include <iostream>

SenderComponent::SenderComponent(Service::Engine* engine) :
    ThreadComponent(engine, typeid(SenderComponent).hash_code())
{

}

SenderComponent::~SenderComponent()
{

}

void SenderComponent::threadJob()
{
    m_running = true;

    Service::Message message;

    while (m_running)
    {
        // It can return status due to
        // thread can finish and no data
        // will be received.
        if (!receiveMessageBlocking(message))
        {
            continue;
        }

        sendMessage(
            typeid(ReceiverComponent).hash_code(),
            std::move(message)
        );
    }
}
