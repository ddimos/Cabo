#pragma once

#include "client/menu/component/MouseInteractable.hpp"
#include <SFML/Window/Event.hpp>
#include <functional>

namespace cn::client::menu::component
{

class Clickable final : public MouseInteractable
{
public:
    using Callback = std::function<void(void)>;

    void setCallback(Callback _callback);
    void setButton(sf::Mouse::Button _button);
    void onMouseReleased(sf::Event::MouseButtonEvent _event);

    bool isActive() const { return m_isActive; }

private:
    Callback m_onClickCallback{};
    sf::Mouse::Button m_button = sf::Mouse::Button::ButtonCount;

    bool m_isActive = false;
};

} // namespace cn::client::menu::component
