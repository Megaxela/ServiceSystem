#include <iostream>
#include <MessageData.hpp>
#include "ReceiverComponent.hpp"

ReceiverComponent::ReceiverComponent(Service::Engine* engine) :
    ThreadComponent(engine, typeid(ReceiverComponent).hash_code())
{

}

ReceiverComponent::~ReceiverComponent()
{

}

void ReceiverComponent::stop()
{
    m_running = false;
}

void ReceiverComponent::threadJob()
{
    m_running = true;

    Service::Message mess;

    while (m_running)
    {
        if (!receiveMessageBlocking(mess))
        {
            continue;
        }

        auto casted = static_cast<MessageData*>(mess.data());

        std::cout
            << "Received message: \""
            << casted->string
            << "\" after "
            << casted->ms.count()
            << "ms delay."
            << std::endl;
    }
}
