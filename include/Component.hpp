#pragma once

#include <cstdint>
#include <queue>
#include <shared_mutex>
#include <atomic>
#include <map>
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
         * @brief Method for adding delayed message to
         * queue.
         * @param mess Message.
         * @param delay Delay.
         * @return Message id.
         */
        std::size_t addDelayedMessageToQueue(Message mess,
                                             std::chrono::microseconds delay);

        /**
         * @brief Method for deleting delayed message from
         * queue.
         * @param id Message id.
         * @return Was message found and deleted?
         */
        bool deleteDelayedMessageFromQueue(std::size_t id);

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
         * @param data Data. Must be moved.
         */
        void sendMessage(IdType target,
                         Message mess);

        /**
         * @brief Method for sending message to component,
         * that will proceed this message after some duration.
         * @tparam Duration Duration type.
         * @param target Identifier of target component.
         * @param mess Data. Must be moved.
         * @param duration Duration of timeout, before
         * processing.
         * @return Message ID. It's unique for target.
         * Can be used for cancelling message.
         */
        template<typename Duration>
        std::size_t sendMessage(IdType target,
                                Message mess,
                                Duration duration)
        {
            return sendMessageImpl(
                target,
                std::move(mess),
                std::chrono::duration_cast<
                    std::chrono::microseconds
                >(duration)
            );
        }

        /**
         * @brief Method for deleting timed message
         * from components queue.
         * @param target Identifier of target component.
         * @param id Timed message id.
         * @return True if message was deleted
         * successfully, False if there is no timed message
         * with such id at target component (or it already
         * was executed)
         */
        bool deleteTimedMessage(IdType target,
                                std::size_t id);

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

        /**
         * @brief Internal implementation of timed message
         * sending.
         * @param target Identifier of target component.
         * @param mess Message. Must be moved.
         * @param mcs Duration.
         * @return Timed message id.
         */
        std::size_t sendMessageImpl(IdType target, Message mess, std::chrono::microseconds mcs);

        IdType m_id;

        Engine* m_engine;

        mutable std::mutex m_messageQueueMutex;

        std::queue<Message> m_messageQueue;

        std::size_t m_idCounter;
        std::map<
            std::pair<std::chrono::steady_clock::time_point, std::size_t>,
            Message
        > m_delayedMessageQueue;

        std::condition_variable m_hasMessageCV;
    };
}

