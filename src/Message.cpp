#include "Message.hpp"

Service::Message::Message() :
    m_data(nullptr),
    m_size(0),
    m_destructor(nullptr)
{

}

Service::Message::Message(void* data, std::size_t size, Service::Message::DestructionFunction destr) :
    m_data(data),
    m_size(size),
    m_destructor(destr)
{

}

Service::Message::~Message()
{
    if (m_data != nullptr &&
        m_destructor != nullptr)
    {
        m_destructor(m_data, m_size);
        m_data = nullptr;
        m_size = 0;
        m_destructor = nullptr;
    }
}

Service::Message::Message(Service::Message&& msg) noexcept :
    m_data(msg.m_data),
    m_size(msg.m_size),
    m_destructor(msg.m_destructor)
{
    msg.m_data = nullptr;
    msg.m_size = 0;
    msg.m_destructor = nullptr;
}

Service::Message& Service::Message::operator=(Service::Message&& msg) noexcept
{
    if (m_data != nullptr &&
        m_destructor != nullptr)
    {
        m_destructor(m_data, m_size);
        m_data = nullptr;
        m_size = 0;
        m_destructor = nullptr;
    }

    m_data = msg.m_data;
    m_size = msg.m_size;
    m_destructor = msg.m_destructor;

    msg.m_data = nullptr;
    msg.m_size = 0;
    msg.m_destructor = nullptr;

    return (*this);
}

void* Service::Message::data() const
{
    return m_data;
}

std::size_t Service::Message::size() const
{
    return m_size;
}
