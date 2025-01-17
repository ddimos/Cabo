#pragma once

#include "core/event/Dispatcher.hpp"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Window/Event.hpp>

namespace cn::core::object
{

class Container;

class Object
{
public:
    enum class ActivationOption
    {
        Manually,
        Auto
    };

    virtual ~Object();

    void requestActivated();
    void requestDeactivated();

    void setActivationOption(ActivationOption _activationOption);
    bool isAutoActivated() const;
    
    bool isActivated() const { return m_isActivated; }

protected:
    virtual void onUpdate(sf::Time) {}
    virtual void onDraw(sf::RenderWindow&) {}
    virtual void onActivate() {}
    virtual void onDeactivate() {}
    virtual void onRegisterEvents(core::event::Dispatcher&, bool) {}

private:
    friend class Container;

    bool wantsActivated() const;
    bool wantsDeactivated() const;

    void update(sf::Time _dt);
    void draw(sf::RenderWindow& _window);    
    void activate();
    void deactivate();

    void registerEvents(core::event::Dispatcher& _dispatcher, bool _isBeingRegistered);

    enum class DesiredState
    {
        None,
        Activated,
        Deactivated
    };
    bool m_isActivated = false;
    ActivationOption m_activationOption = ActivationOption::Auto; 
    DesiredState m_desiredState = DesiredState::None;
};

} // namespace cn::core::object
