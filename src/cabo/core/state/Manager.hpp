#pragma once

#include "core/Types.hpp"
#include "core/state/State.hpp"
#include "core/state/StateTypes.hpp"

#include <SFML/System/NonCopyable.hpp>
#include <SFML/System/Time.hpp>

#include <vector>
#include <functional>
#include <unordered_map>

// The core is taken from https://github.com/SFML/SFML-Game-Development-Book/blob/master/05_States/

namespace sf
{
	class Event;
}

namespace cn::core::state
{

class Manager final    // TODO NonCopyable
{
public:		
    Manager(core::Context _context);

    template <typename T>
    void registerState(StateId _stateId);

    void update(sf::Time _dt);
    void draw();

    void pushState(StateId _stateId);
    void popState();
    void clearStates();

    bool isEmpty() const;

    core::Context& getContext() { return m_context; }

private:
    StatePtr createState(StateId _stateId);
    void applyPendingChanges();

    struct PendingChange
    {
        enum class Action
        {
            Push,
            Pop,
            Clear,
        };

        explicit PendingChange(Action _action, StateId _stateId = StateIdInvalid)
            : action(_action), stateId(_stateId) {}

        Action action;
        StateId stateId;
    };

    std::vector<StatePtr> m_stack;
    std::vector<PendingChange> m_pendingList;

    core::Context m_context;
    std::unordered_map<StateId, std::function<StatePtr()>> m_factories;
};

template <typename T>
void Manager::registerState(StateId _stateId)
{
    m_factories.emplace(_stateId, [this]() {
		return std::make_unique<T>(*this);
	});
}

} // namespace cn::core::state
