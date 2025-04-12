#include "client/game/Step.hpp"

#include "core/Assert.hpp"
#include "core/Log.hpp"

namespace cn::client::game
{
    Step::Step(PlayerId _managedPlayerId, StateMap&& _states)
    : m_managedPlayerId(_managedPlayerId)
    , m_states(std::move(_states))
{
    m_nextStateId = m_states.begin()->first;
}

void Step::update(sf::Time _dt)
{
    if (m_nextStateId != m_stateId)
    {
        StateId currentStateId = m_nextStateId;
        CN_LOG_FRM("Next step's state {}, player {}", currentStateId, m_managedPlayerId);
        auto& nextState = m_states.at(currentStateId);
        if (nextState.onEnter)
            nextState.onEnter();

        m_stateId = currentStateId;
    }

    auto& state = m_states.at(m_stateId);
        if (state.onUpdate)
            state.onUpdate(_dt);
}

void Step::requestFollowingState()
{
    m_nextStateId++;
    CN_ASSERT(m_states.contains(m_nextStateId));
}

void Step::requestState(StateId _id)
{
    m_nextStateId = _id;
    CN_ASSERT(m_states.contains(m_nextStateId));
}
} // namespace cn::client::game
