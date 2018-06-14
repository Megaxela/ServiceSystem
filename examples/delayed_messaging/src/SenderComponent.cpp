#include <chrono>
#include <ReceiverComponent.hpp>
#include "SenderComponent.hpp"
#include <cstddef>
#include <iostream>
#include <MessageData.hpp>

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

        auto casted = static_cast<MessageData*>(message.data());

        sendMessage(
            typeid(ReceiverComponent).hash_code(),
            std::move(message),
            casted->ms
        );
    }
}
