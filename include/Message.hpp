#pragma once

#include <functional>

namespace Service
{
    /**
     * @brief Class, that describes message for
     * components communication. It support only move
     * semantics to prevent any unnecessary copies.
     */
    class Message
    {
    public:

        using DestructionFunction = void(*)(void*, std::size_t);

        /**
         * @brief Default constructor. Creates
         * empty message.
         */
        Message();

        /**
         * @brief Initializing constructor.
         * @param data Pointer to actual data.
         * @param size Size of data.
         * @param destr Operation, that will be executed for data after
         * message destructing. (If there any data left)
         */
        Message(void* data,
                std::size_t size,
                DestructionFunction destr=nullptr);

        /**
         * @brief Destructor. Evaluates destructing
         * function if there was any data.
         */
        ~Message();

        /**
         * @brief Move constructor. After
         * move, movable message is empty and
         * can be used again.
         * @param msg Movable message.
         */
        Message(Message&& msg) noexcept;

        /**
         * @brief Move operator. After
         * move, movable message is empty and
         * can be used again.
         * @param msg Movable message.
         * @return Reference to current message.
         */
        Message& operator=(Message&& msg) noexcept;

        // Explicit disable copying
        Message(const Message&) = delete;
        Message& operator=(const Message&) = delete;

        /**
         * @brief Method for receiving message data.
         * @return Pointer to message data.
         */
        void* data() const;

        /**
         * @brief Method for receiving message size.
         * @return Message size.
         */
        std::size_t size() const;

    private:

        void* m_data;
        std::size_t m_size;
        DestructionFunction m_destructor;

    };
}

