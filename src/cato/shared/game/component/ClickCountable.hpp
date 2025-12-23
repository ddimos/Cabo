#pragma once
#include "shared/game/component/Component.hpp"
#include "shared/player/Types.hpp"

#include <functional>
#include <unordered_map>

namespace cn::shared::game::controller
{
class ClickCountable;
}

namespace cn::shared::game::component
{

class ClickCountable : public Component
{
public:
    using OnPlayerClickedFunc = std::function<void(PlayerId)>;
    using OnResetFunc = std::function<void(void)>;

    ClickCountable(object::Object& _parent, unsigned _numberOfPlayersToClick, 
        OnPlayerClickedFunc _playerClickedFunc, OnResetFunc _onResetFunc);

    bool isClicked() const;
    bool timeoutReached() const;
    unsigned getClicks(PlayerId _playerId) const;

private:
    friend class controller::ClickCountable;
    
    void click(PlayerId _playerId);
    void reset();
    void update(sf::Time _dt);

    const sf::Time m_resetTimeout{ sf::seconds(5.f) };
    static constexpr unsigned m_clicksPerPlayer = 1;
    const unsigned m_numberOfPlayersToClick = 1;
    OnPlayerClickedFunc m_playerClickedFunc{};
    OnResetFunc m_onResetFunc{};
    std::unordered_map<PlayerId, unsigned> m_clicks;
    sf::Time m_resetTimeoutDt{};
};

} // namespace cn::shared::game::component
