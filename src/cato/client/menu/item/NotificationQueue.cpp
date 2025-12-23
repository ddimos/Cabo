#include "client/menu/item/NotificationQueue.hpp"

namespace cn::client::menu::item
{

NotificationQueue::NotificationQueue(Position _firstElementPos, sf::Time _visibleTimeS, float _offsetBetween, const std::vector<SimpleText*>& _textItems)
    : m_firstElementPos(_firstElementPos), m_visibleTimeS(_visibleTimeS), m_offsetBetween(_offsetBetween)
{
    for (auto* text : _textItems)
    {
        m_notifications.emplace_back(Notification{ .m_timeLeft = {}, .m_textRef = *text, .m_isActive = false });
    }
}

void NotificationQueue::push(const std::string& _text)
{
    if (m_visibleSizeCount >= m_notifications.size())
    {
        size_t index = 0;
        float leastTimeLeftS = m_visibleTimeS.asSeconds();
        for (size_t i = 0; i < m_notifications.size(); ++i)
        {
            auto& element = m_notifications[i];
            float timeLeftS = element.m_timeLeft.asSeconds();
            if (timeLeftS < leastTimeLeftS)
            {
                leastTimeLeftS = timeLeftS;
                index = i;
            }
        }
        m_notifications[index].m_timeLeft = m_visibleTimeS;
        m_notifications[index].m_textRef.updateText(_text);
        
        return;
    }

    moveElements();
    for (auto& element : m_notifications)
    {
        if (element.m_isActive)
        continue;
        
        element.m_isActive = true;
        element.m_timeLeft = m_visibleTimeS;
        element.m_textRef.updateText(_text);
        element.m_textRef.requestActivated();

        element.m_textRef.setPosition(m_firstElementPos);
        break;
    }
    m_visibleSizeCount++;
}

void NotificationQueue::onUpdate(sf::Time _dt)
{
    for (auto& element : m_notifications)
    {
        if (!element.m_isActive)
            continue;

        element.m_timeLeft -= _dt;
        if (element.m_timeLeft.asSeconds() <= 0.f)
        {
            element.m_isActive = false;
            element.m_textRef.requestDeactivated();
            --m_visibleSizeCount;
        }
    }
}

void NotificationQueue::moveElements()
{
    for (auto& element : m_notifications)
    {
        if (!element.m_isActive)
            continue;

        auto oldPos = element.m_textRef.getPosition();
        oldPos.m_position.y -= m_offsetBetween;
        element.m_textRef.setPosition(oldPos);
    }
}

} // namespace cn::client::menu::item
