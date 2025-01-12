#include "core/state/Manager.hpp"
#include "core/Assert.hpp"

namespace cn::core::state
{

Manager::Manager(core::Context& _contextRef)
    : m_contextRef(_contextRef)
{
}

void Manager::update(sf::Time _dt)
{
    for (auto it = m_stack.rbegin(); it != m_stack.rend(); ++it)
    {
        if ((*it)->update(_dt) == Return::Break)
            break;
    }
    applyPendingChanges();
}

void Manager::draw()
{
    for (auto &state : m_stack)
        state->draw();
}

void Manager::pushState(StateId _stateId)
{
    m_pendingList.push_back(PendingChange(PendingChange::Action::Push, _stateId));
}

void Manager::popState()
{
    m_pendingList.push_back(PendingChange(PendingChange::Action::Pop));
}

void Manager::clearStates()
{
    m_pendingList.push_back(PendingChange(PendingChange::Action::Clear));
}

bool Manager::isEmpty() const
{
    return m_stack.empty();
}

StatePtr Manager::createState(StateId _stateId)
{
    auto stateIt = m_factories.find(_stateId);
    CN_ASSERT_FRM(stateIt != m_factories.end(), "The state: {} wasn't found", (int)_stateId);
    return stateIt->second();
}

void Manager::applyPendingChanges()
{
    for(auto change : m_pendingList)
    {
        switch (change.action)
        {
        case PendingChange::Action::Push:
            if (!m_stack.empty())
                m_stack.back()->deactivate();
            m_stack.emplace_back(createState(change.stateId));
            m_stack.back()->activate();
            break;
        case PendingChange::Action::Pop:
            m_stack.back()->deactivate();
            m_stack.pop_back();
            if (!m_stack.empty())
                m_stack.back()->activate();
            break;
        case PendingChange::Action::Clear:
            for (auto it = m_stack.rbegin(); it != m_stack.rend(); ++it)
                (*it)->deactivate();
            m_stack.clear();
            break;
        }
    }
    m_pendingList.clear();
}

} // namespace cn::core::state
