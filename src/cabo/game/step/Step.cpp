#include "game/step/Step.hpp"

#include "events/GameEvents.hpp"

namespace cn::game::step
{

Step::Step()
{
    m_listenerId = core::event::getNewListenerId();
}

// ------------------------------------------



} // namespace cn::game::step
