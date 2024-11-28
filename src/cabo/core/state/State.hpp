#pragma once

#include "core/event/Dispatcher.hpp"
#include "core/Types.hpp"
#include "core/object/Container.hpp"
#include "core/state/StateTypes.hpp"

#include <SFML/System/Time.hpp>
#include <SFML/Window/Event.hpp>

namespace cn::core::state
{

class Manager;

class State
{
public:

public:
    State(Manager& _stateManagerRef);
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
    Manager& m_stateManagerRef;
    core::object::Container m_menuContainer;
};
    
} // namespace cn::core::state
