#pragma once

#include "core/event/Types.hpp"
#include "core/state/State.hpp"

#include "client/menu/item/SimpleText.hpp"

#include <vector>

namespace cn::client::states
{

class LobbyState final : public core::state::State
{
public:
    LobbyState(core::state::Manager& _stateManagerRef);

private:
    void onRegisterEvents(core::event::Dispatcher& _dispatcher, bool _isBeingRegistered) override;

    core::event::ListenerId m_listenerId{};

    struct PlayerItem
    {
        menu::item::SimpleText& m_nameTextRef;
        menu::item::SimpleText& m_statusTextRef;
    };
    std::vector<PlayerItem> m_players;
};

} // namespace cn::client::states
