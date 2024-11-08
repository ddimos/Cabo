#include "state/StateManager.hpp"

#include "core/Assert.hpp"

namespace cn::state
{

StateManager::StateManager(Context _context)
    : m_context(_context)
{
}

void StateManager::handleEvent(const sf::Event& _event)
{
    for (auto it = m_stack.rbegin(); it != m_stack.rend(); ++it)
    {
        if ((*it)->handleEvent(_event) == State::Return::Break)
            break;
    }
    applyPendingChanges();
}

void StateManager::update(sf::Time _dt)
{
    for (auto it = m_stack.rbegin(); it != m_stack.rend(); ++it)
    {
        if ((*it)->update(_dt) == State::Return::Break)
            break;
    }
    applyPendingChanges();
}

void StateManager::draw()
{
    for (auto &state : m_stack)
        state->draw();
}

void StateManager::pushState(StateId _stateId)
{
    m_pendingList.push_back(PendingChange(PendingChange::Action::Push, _stateId));
}

void StateManager::popState()
{
    m_pendingList.push_back(PendingChange(PendingChange::Action::Pop));
}

void StateManager::clearStates()
{
    m_pendingList.push_back(PendingChange(PendingChange::Action::Clear));
}

bool StateManager::isEmpty() const
{
    return m_stack.empty();
}

StatePtr StateManager::createState(StateId _stateId)
{
    auto stateIt = m_factories.find(_stateId);
    CN_ASSERT_FRM(stateIt != m_factories.end(), "The state: {} wasn't found", (int)_stateId);
    return stateIt->second();
}

void StateManager::applyPendingChanges()
{
    for(auto change : m_pendingList)
    {
        switch (change.action)
        {
        case PendingChange::Action::Push:
            m_stack.push_back(createState(change.stateId));
            break;
        case PendingChange::Action::Pop:
            m_stack.pop_back();
            break;
        case PendingChange::Action::Clear:
            m_stack.clear();
            break;
        }
    }
    m_pendingList.clear();
}

} // namespace cn::state
