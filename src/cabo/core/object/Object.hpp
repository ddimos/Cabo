#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Window/Event.hpp>

namespace cn::core::object
{

class Object
{
public:
    virtual ~Object();

    void handleEvent(const sf::Event& _event);
    void update(sf::Time _dt);
    void draw(sf::RenderWindow& _window);

    void activate();
    void deactivate();

    bool isActivated() const { return m_isActivated; }

protected:
    virtual void onHandleEvent(const sf::Event& _event) { (void)_event; }
    virtual void onUpdate(sf::Time _dt) { (void)_dt; }
    virtual void onDraw(sf::RenderWindow& _windowRef) { (void)_windowRef; }
    virtual void onActivate() {}
    virtual void onDeactivate() {}

    bool m_isActivated = false;
};

} // namespace cn::core::object
