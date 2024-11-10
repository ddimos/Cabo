#pragma once

#include <SFML/Graphics/Rect.hpp>
#include <SFML/Window/Event.hpp>

#include <functional>

namespace cn::menu::component
{

class Hoverable
{
public:
    using Callback = std::function<void(bool)>;

    void setOnHoverCallback(Callback _callback);
    void setHoverableBounds(sf::FloatRect _rect);

protected:
    void onMouseMoved(sf::Event::MouseMoveEvent _event);

private:
    Callback m_onHoverCallback{};

    sf::FloatRect m_bounds{};
    bool m_isHovered = false;
};

} // namespace cn::menu::component