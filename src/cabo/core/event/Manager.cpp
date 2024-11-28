#include "core/event/Manager.hpp"

namespace cn::core::event
{

Dispatcher& Manager::getDispatcher()
{
    return m_dispatcher;
}

void Manager::process()
{
    m_dispatcher.processDelayed();
}

} // namespace cn::core::event
