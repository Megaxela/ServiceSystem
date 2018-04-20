#include <Engine.hpp>
#include <SenderComponent.hpp>
#include <ReceiverComponent.hpp>
#include <iostream>
#include <cstring>

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
        std::getline(std::cin, input);

        if (input == "exit")
        {
            break;
        }

        // Copying
        auto copy = new char[input.size()];
        std::memcpy(copy, input.c_str(), input.size());

        processor->addMessageToQueue(
            std::move(
                Service::Message(
                    copy,
                    input.size(),
                    [](void* data, std::size_t size)
                    {
                        std::cout
                            << "Deleting message \""
                            << std::string((char*) data, size)
                            << "\"."
                            << std::endl;

                        delete[] ((char*) data);
                    }
                )
            )
        );
    }

    std::cout << "Stopping threads..." << std::endl;
}