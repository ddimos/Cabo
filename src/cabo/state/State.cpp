#include "state/State.hpp"
#include "state/StateManager.hpp"

namespace cn::state
{

State::State(StateManager& _stateManagerRef)
    : m_stateManagerRef{_stateManagerRef}
{
}
    
State::~State() = default;

void State::push(Name _stateName)
{
    m_stateManagerRef.pushState(_stateName);
}

void State::pop()
{
    m_stateManagerRef.popState();
}

void State::clear()
{
    m_stateManagerRef.clearStates();
}

Context& State::getContext()
{
    return m_stateManagerRef.getContext();
}

} // namespace cn::state
