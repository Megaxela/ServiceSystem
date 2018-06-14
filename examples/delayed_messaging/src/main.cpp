#include <Engine.hpp>
#include <SenderComponent.hpp>
#include <ReceiverComponent.hpp>
#include <iostream>
#include <cstring>
#include <MessageData.hpp>

int main(int argc, char** argv)
{
    Service::Engine engine;

    auto processor = engine.makeComponent<SenderComponent>();
    engine.makeComponent<ReceiverComponent>();

    engine.runComponents();

    std::string input;

    std::cout << "Enter something. And it will be sent to sender component." << std::endl;

    while (true)
    {
        auto msg = new MessageData();

        std::cout << "Enter delay in ms: " << std::endl;
        std::getline(std::cin, input);

        if (input == "exit" || input == "quit")
        {
            delete msg;
            break;
        }

        msg->ms = std::chrono::milliseconds(std::stoi(input));

        std::cout << "Enter message: " << std::endl;
        std::getline(std::cin, input);

        if (input == "exit" || input == "quit")
        {
            delete msg;
            break;
        }

        msg->string = input;

        std::cout << "Sending entered text..." << std::endl;

        processor->addMessageToQueue(
            std::move(
                Service::Message(
                    msg,
                    input.size(),
                    [](void* data, std::size_t size)
                    {
                        std::cout
                            << "Deleting message \""
                            << ((MessageData*) data)->string
                            << "\"."
                            << std::endl;

                        delete ((MessageData*) data);
                    }
                )
            )
        );
    }

    std::cout << "Stopping threads..." << std::endl;
}