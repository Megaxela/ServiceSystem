#pragma once

#include "Component.hpp"
#include <type_traits>
#include <map>

namespace Service
{
    /**
     * @brief Class, that describes abstract
     * component engine. It serves messaging
     * between components.
     */
    class Engine
    {
    public:

        /**
         * @brief Default constructor.
         */
        Engine();

        /**
         * @brief Virtual destructor.
         */
        virtual ~Engine();

        /**
         * @brief Method for creating components via
         * engine. May throw exception if not enough memory.
         * @tparam T Component type. Must be derived from
         * Service::Component class.
         * @return Pointer to created component.
         */
        template<typename T>
        typename std::enable_if<
            std::is_base_of<Service::Component, T>::value,
            T*
        >::type makeComponent()
        {
            auto component = new T(this);

            m_components[component->identifier()] = component;

            return component;
        }

        /**
         * @brief Running components.
         */
        void runComponents();

    protected:

        friend class Component;

        /**
         * @brief Method for routing message to target
         * component.
         * @param target Target component id.
         * @param message Message.
         */
        void sendMessage(Component::IdType target,
                         Message message);

        /**
         * @brief Method for routing message to target
         * component for delayed execution.
         * @param target Target component id.
         * @param message Message.
         * @param mcs Delay.
         * @return Message id.
         */
        std::size_t sendMessageDelayed(Component::IdType target,
                                       Message message,
                                       std::chrono::microseconds mcs);

        /**
         * @brief Method for deleting timed messages.
         * @param target Target component id.
         * @param id Message.
         * @return True - if success. False if there is
         * no message with such id.
         */
        bool deleteDelayedMessage(Component::IdType target,
                                  std::size_t id);

    private:
        std::map<
            Component::IdType,
            Component*
        > m_components;
    };
}

