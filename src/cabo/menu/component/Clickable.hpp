#pragma once

#include <SFML/Graphics/Rect.hpp>
#include <SFML/Window/Event.hpp>

#include <functional>

namespace cn::menu::component
{

class Clickable
{
public:
    using Callback = std::function<void(bool)>;

    void setOnClickCallback(Callback _callback);
    void setClickableButton(sf::Mouse::Button _button);
    void setClickableBounds(sf::FloatRect _rect);

protected:
    void onMouseReleased(sf::Event::MouseButtonEvent _event);

private:
    Callback m_onClickCallback{};

    sf::FloatRect m_bounds{};
    sf::Mouse::Button m_button = sf::Mouse::Button::ButtonCount;
};

} // namespace cn::menu::component
