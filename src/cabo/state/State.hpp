#pragma once

#include "core/event/Dispatcher.hpp"
#include "core/Types.hpp"
#include "core/object/Container.hpp"
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

    // TODO rethink Return 
    Return update(sf::Time _dt);
    void draw();

    void activate();
    void deactivate();
    
    void registerEvents(core::event::Dispatcher& _dispatcher, bool _isBeingRegistered);

protected:
    virtual Return onUpdate(sf::Time _dt) { (void)_dt; return Return::Continue; }
    virtual void onDraw() {}
    virtual void onActivate() {}
    virtual void onDeactivate() {}
    virtual void onRegisterEvents(core::event::Dispatcher& _dispatcher, bool _isBeingRegistered) {}

    void push(StateId _stateId);
    void pop();
    void clear();

    core::Context& getContext();
    core::object::Container& getMenuContainer();

private:
    StateManager& m_stateManagerRef;
    core::object::Container m_menuContainer;
};
    
} // namespace cn::state
