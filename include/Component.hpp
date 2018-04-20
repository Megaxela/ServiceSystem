#pragma once

#include <cstdint>
#include <queue>
#include <shared_mutex>
#include <atomic>
#include "Message.hpp"

namespace Service
{
    class Engine;

    /**
     * @brief Class, that describes service component.
     */
    class Component
    {
    public:

        /**
         * @brief Type of identifier.
         */
        using IdType = std::size_t;

        /**
         * @brief Constructor.
         * @param id Identifier value.
         */
        explicit Component(Engine* engine, IdType id);

        /**
         * @brief Destructor.
         */
        virtual ~Component() = default;

        /**
         * @brief Method for getting component
         * identifier value.
         * @return Component identifier.
         */
        IdType identifier() const;

        /**
         * @brief Method for receiving message queue
         * size.
         * @return Number of messages in queue.
         */
        std::size_t messageQueueSize() const;

        /**
         * @brief Method for adding new message to
         * queue.
         * @param mess Message.
         */
        void addMessageToQueue(Message mess);

        /**
         * @brief Pure virtual function, that's executed on
         * component running.
         */
        virtual void run() = 0;

        /**
         * @brief Method for stopping main thread.
         */
        virtual void stop();

        /**
         * @brief Method for checking is component running.
         * @return Running status.
         */
        bool isRunning() const;

    protected:

        /**
         * @brief Method for sending message to component.
         * @param target Identifier of target component.
         * @param data Pointer to data. Data will be copied.
         * @param size Size of sent data.
         */
        void sendMessage(IdType target,
                         Message mess);

        /**
         * @brief Method for receiving message from queue.
         * @param m Actual message.
         * @return Was any message received.
         */
        bool receiveMessage(Message& m);

        /**
         * @brief Blocking method for receiving message from queue.
         * @param m Message.
         */
        bool receiveMessageBlocking(Message& m);

        // Is current component running
        std::atomic_bool m_running;
    private:

        IdType m_id;

        Engine* m_engine;

        mutable std::mutex m_messageQueueMutex;
        std::queue<Message> m_messageQueue;
        std::condition_variable m_hasMessageCV;
    };
}

