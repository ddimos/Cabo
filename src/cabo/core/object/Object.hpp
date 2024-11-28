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
    
    void activate();
    void deactivate();

    void setActivationOption(ActivationOption _activationOption);
    bool isAutoActivated() const;
    
    bool isActivated() const { return m_isActivated; }

protected:
    virtual void onUpdate(sf::Time _dt) { (void)_dt; }
    virtual void onDraw(sf::RenderWindow& _windowRef) { (void)_windowRef; }
    virtual void onActivate() {}
    virtual void onDeactivate() {}
    virtual void onRegisterEvents(core::event::Dispatcher& _dispatcher, bool _isBeingRegistered) {}

private:
    friend class Container;

    void update(sf::Time _dt);
    void draw(sf::RenderWindow& _window);

    void registerEvents(core::event::Dispatcher& _dispatcher, bool _isBeingRegistered);

    bool m_isActivated = false;
    ActivationOption m_activationOption = ActivationOption::Auto; 
};

} // namespace cn::core::object
