#pragma once

#include "core/event/Dispatcher.hpp"
#include "core/Types.hpp"
#include "core/object/Container.hpp"
#include "core/state/StateTypes.hpp"

#include <SFML/System/Time.hpp>
#include <SFML/Window/Event.hpp>

#include <map>

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
    
    void registerEvents(event::Dispatcher& _dispatcher, bool _isBeingRegistered);

protected:
    virtual Return onUpdate(sf::Time _dt) { (void)_dt; return Return::Continue; }
    virtual void onDraw() {}
    virtual void onActivate() {}
    virtual void onDeactivate() {}
    virtual void onRegisterEvents(event::Dispatcher& _dispatcher, bool _isBeingRegistered) {}

    void push(StateId _stateId);
    void pop();
    void clear();

    Context& getContext();
    void createContainer(object::Container::Type _type);
    object::Container& getContainer(object::Container::Type _type);

private:
    Manager& m_stateManagerRef;
    std::map<object::Container::Type, object::Container> m_containers;
};
    
} // namespace cn::core::state
