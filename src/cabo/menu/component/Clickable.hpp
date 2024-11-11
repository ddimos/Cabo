#pragma once

#include <SFML/Graphics/Rect.hpp>
#include <SFML/Window/Event.hpp>

#include <functional>

namespace cn::menu::component
{

class Clickable final
{
public:
    using Callback = std::function<void(bool)>;

    void setCallback(Callback _callback);
    void setButton(sf::Mouse::Button _button);
    void setBounds(sf::FloatRect _rect);

    void onMouseReleased(sf::Event::MouseButtonEvent _event);

private:
    Callback m_onClickCallback{};

    sf::FloatRect m_bounds{};
    sf::Mouse::Button m_button = sf::Mouse::Button::ButtonCount;
};

} // namespace cn::menu::component
