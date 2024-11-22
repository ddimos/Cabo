#pragma once

#include "menu/component/MouseInteractable.hpp"
#include <SFML/Window/Event.hpp>
#include <functional>

namespace cn::menu::component
{

class Clickable final : public MouseInteractable
{
public:
    using Callback = std::function<void(bool)>;

    void setCallback(Callback _callback);
    void setButton(sf::Mouse::Button _button);
    void onMouseReleased(sf::Event::MouseButtonEvent _event);

private:
    Callback m_onClickCallback{};
    sf::Mouse::Button m_button = sf::Mouse::Button::ButtonCount;
};

} // namespace cn::menu::component
