#pragma once

#include <thread>
#include "Component.hpp"

namespace Service
{
    /**
     * @brief Class, that describes component with one
     * individual thread.
     */
    class ThreadComponent : public Component
    {
    public:
        /**
         * @brief Constructor.
         * @param engine Engine.
         * @param id Component identifier.
         */
        ThreadComponent(Engine* engine, IdType id, uint32_t numberOfThreads=1);

        /**
         * @brief Destructor.
         */
        ~ThreadComponent() override;

        /**
         * @brief Method for running main thread.
         */
        void run() override;

    protected:

        /**
         * @brief Thread job method.
         */
        virtual void threadJob() = 0;

    private:

        std::vector<std::thread> m_threads;
        uint32_t m_numberOfThreads;
    };
}

