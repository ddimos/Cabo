#pragma once

#include "core/event/Types.hpp"
#include "core/state/State.hpp"

#include "client/menu/item/SimpleText.hpp"

#include "shared/player/Types.hpp"

#include <vector>

namespace cn::client::states
{

class LobbyState final : public core::state::State
{
public:
    LobbyState(core::state::Manager& _stateManagerRef);

private:
    void onRegisterEvents(core::event::Dispatcher& _dispatcher, bool _isBeingRegistered) override;

    bool hasPlayer(PlayerId _playerId) const;
    void addPlayer(PlayerId _playerId, const std::string& _name);
    void updatePlayer(PlayerId _playerId, bool _isReady);

    core::event::ListenerId m_listenerId = core::event::ListenerIdInvalid;

    struct PlayerItem
    {
        menu::item::SimpleText& m_nameTextRef;
        menu::item::SimpleText& m_statusTextRef;
        PlayerId m_playerId = PlayerIdInvalid;
    };
    std::vector<PlayerItem> m_players;
};

} // namespace cn::client::states
