#pragma once

#include "client/menu/component/MouseInteractable.hpp"
#include <SFML/Window/Event.hpp>
#include <functional>

namespace cn::client::menu::component
{

class Hoverable final : public MouseInteractable
{
public:
    using Callback = std::function<void(bool)>;

    void setCallback(Callback _callback);
    void onMouseMoved(sf::Event::MouseMoveEvent _event);

private:
    Callback m_onHoverCallback{};
    bool m_isHovered = false;
};

} // namespace cn::client::menu::component