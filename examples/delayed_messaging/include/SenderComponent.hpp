#pragma once

#include <ThreadComponent.hpp>
#include <atomic>

/**
 * @brief Class, that describes component, that performs sending
 * message to receiver component.
 */
class SenderComponent : public Service::ThreadComponent
{
public:

    /**
     * @brief Constructor.
     * @param engine Pointer to engine.
     */
    explicit SenderComponent(Service::Engine* engine);

    /**
     * @brief Destructor.
     * Stops thread.
     */
    ~SenderComponent() override;

protected:
    /**
     * @brief Thread for sending some amount of data.
     */
    void threadJob() override;

};

