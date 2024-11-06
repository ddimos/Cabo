#pragma once

#include "state/State.hpp"
#include "state/StateName.hpp"
#include "state/StateTypes.hpp"

#include <SFML/System/NonCopyable.hpp>
#include <SFML/System/Time.hpp>

#include <vector>
#include <functional>
#include <unordered_map>

// The core is taken from https://github.com/SFML/SFML-Game-Development-Book/blob/master/05_States/

namespace sf
{
	class Event;
	class RenderWindow;
}

namespace cn::state
{

class StateManager final    // TODO NonCopyable
{
public:		
    StateManager(Context _context);

    template <typename T>
    void registerState(Name _stateName);

    void handleEvent(const sf::Event& _event);
    void update(sf::Time _dt);
    void draw();

    void pushState(Name _stateName);
    void popState();
    void clearStates();

    bool isEmpty() const;

    Context& getContext() { return m_context; }

private:
    StatePtr createState(Name _stateName);
    void applyPendingChanges();

    struct PendingChange
    {
        enum class Action
        {
            Push,
            Pop,
            Clear,
        };

        explicit PendingChange(Action _action, Name _stateName = Name::None)
            : action(_action), stateName(_stateName) {}

        Action action;
        Name stateName;
    };

    std::vector<StatePtr> m_stack;
    std::vector<PendingChange> m_pendingList;

    Context m_context;
    std::unordered_map<Name, std::function<StatePtr()>>	m_factories;
};

template <typename T>
void StateManager::registerState(Name _stateName)
{
    m_factories.emplace(_stateName, [this]() {
		return std::make_unique<T>(*this);
	});
}

} // namespace cn::state
