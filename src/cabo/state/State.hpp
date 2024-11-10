#pragma once

#include "state/StateTypes.hpp"

#include <SFML/System/Time.hpp>
#include <SFML/Window/Event.hpp>

#include "menu/Container.hpp"

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

    // TODO rethink Return 
    Return handleEvent(const sf::Event& _event);
    Return update(sf::Time _dt);
    void draw();

    void activate();
    void deactivate();
    
protected:
    virtual Return onHandleEvent(const sf::Event& _event) { (void)_event; return Return::Continue; }
    virtual Return onUpdate(sf::Time _dt) { (void)_dt; return Return::Continue; }
    virtual void onDraw() {}
    virtual void onActivate() {}
    virtual void onDeactivate() {}

    void push(StateId _stateId);
    void pop();
    void clear();

    Context& getContext();
    menu::Container& getContainer();

private:
    StateManager& m_stateManagerRef;
    menu::Container m_menuContainer;
};
    
} // namespace cn::state
