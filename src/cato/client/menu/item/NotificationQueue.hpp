#pragma once

#include "core/event/Types.hpp"

#include "client/menu/Item.hpp"
#include "client/menu/item/SimpleText.hpp"
// #include "client/menu/component/Clickable.hpp"
// #include "client/menu/component/Hoverable.hpp"

// #include <SFML/Graphics/Sprite.hpp>

#include <vector>

namespace cn::client::menu::item
{

class NotificationQueue : public Item
{
public:
    NotificationQueue(Position _firstElementPos, sf::Time _visibleTimeS, float _offsetBetween, const std::vector<SimpleText*>& _textItems);
    
    void push(const std::string& _text);

protected:
    void onUpdate(sf::Time _dt) override;
    void moveElements();

    struct Notification
    {
        sf::Time m_timeLeft = {};
        SimpleText& m_textRef;
        bool m_isActive = false;
    };
    
    std::vector<Notification> m_notifications;
    Position m_firstElementPos = {};
    unsigned m_visibleSizeCount = 0;
    const sf::Time m_visibleTimeS = {};
    const float m_offsetBetween = 0.f;
};

} // namespace cn::client::menu::item
