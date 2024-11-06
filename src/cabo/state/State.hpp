#pragma once

#include "state/StateName.hpp"
#include "state/StateTypes.hpp"

#include <SFML/System/Time.hpp>
#include <SFML/Window/Event.hpp>

namespace sf
{
	class RenderWindow;
}

namespace cn::state
{

class StateManager;

class State
{
public:
    enum class Return
    {
        Continue,
        Break
    };

public:
    State(StateManager& _stateManagerRef);
    virtual ~State();

    virtual Return handleEvent(const sf::Event& _event) = 0;
    virtual Return update(sf::Time _dt) = 0;
    virtual void draw() = 0;

protected:
    void push(Name _stateName);
    void pop();
    void clear();

    Context& getContext();

private:
    StateManager& m_stateManagerRef;
};
    
} // namespace cn::state
