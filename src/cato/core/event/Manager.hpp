#pragma once

#include "core/event/Dispatcher.hpp"

namespace cn::core::event
{

class Manager final
{
public:
    Dispatcher& getDispatcher();
    void process();

private:
    Dispatcher m_dispatcher;
};

} // namespace cn::core::event
