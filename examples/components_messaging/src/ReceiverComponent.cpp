#include <iostream>
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

        std::cout
            << "Received message: "
            << std::string((const char*) mess.data(), mess.size())
            << std::endl;
    }
}
