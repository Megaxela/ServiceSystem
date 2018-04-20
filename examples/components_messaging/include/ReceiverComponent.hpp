
#pragma once


#include <ThreadComponent.hpp>
#include <atomic>

/**
 * @brief Class, that describes component
 * that's receives all messages.
 */
class ReceiverComponent : public Service::ThreadComponent
{
public:

    /**
     * @brief Constructor.
     * @param engine Pointer to engine.
     */
    explicit ReceiverComponent(Service::Engine* engine);

    /**
     * @brief Receiver component.
     */
    ~ReceiverComponent() override;

    /**
     * @brief Method for stopping receive messages.
     */
    void stop() override;

protected:

    /**
     * @brief Thread for receiving all messages.
     */
    void threadJob() override;

};

