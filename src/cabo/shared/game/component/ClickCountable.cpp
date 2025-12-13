#include "shared/game/component/ClickCountable.hpp"

namespace cn::shared::game::component
{

ClickCountable::ClickCountable(object::Object& _parent, unsigned _numberOfPlayersToClick,
    std::function<void(PlayerId)> _playerClickedFunc, std::function<void(void)> _onResetFunc)
    : Component(_parent)
    , m_numberOfPlayersToClick(_numberOfPlayersToClick)
    , m_playerClickedFunc(_playerClickedFunc), m_onResetFunc(_onResetFunc)
{
    CN_ASSERT(m_clicksPerPlayer != 0);
}

bool ClickCountable::isClicked() const
{
    static_assert(m_clicksPerPlayer == 1, "Take clicksPerPlayer into account here");
    return m_clicks.size() >= m_numberOfPlayersToClick;
}

bool ClickCountable::timeoutReached() const
{
    return m_resetTimeoutDt.asSeconds() <= 0.f;
}

unsigned ClickCountable::getClicks(PlayerId _playerId) const
{
    auto it = m_clicks.find(_playerId);
    if (it != m_clicks.end())
        return it->second;
    return 0;
}

void ClickCountable::click(PlayerId _playerId)
{
    m_resetTimeoutDt = m_resetTimeout;
    auto it = m_clicks.find(_playerId);
    if (it != m_clicks.end())
    {
        if (it->second < m_clicksPerPlayer)
            it->second++;
    }
    else
        m_clicks.try_emplace(_playerId, 1);

    m_playerClickedFunc(_playerId);

    if (m_clicks.size() >= m_numberOfPlayersToClick)
        m_resetTimeoutDt = {};
}

void ClickCountable::reset()
{
    m_clicks.clear();
    m_resetTimeoutDt = {};

    m_onResetFunc();
}

void ClickCountable::update(sf::Time _dt)
{
    m_resetTimeoutDt -= _dt;
}

} // namespace cn::shared::game::component
