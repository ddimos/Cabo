#pragma once

#include "core/event/Types.hpp"
#include "core/state/State.hpp"
#include "server/game/Board.hpp"

namespace cn::server::states
{

class GameState final : public core::state::State
{
public:
    GameState(core::state::Manager& _stateManagerRef);

private:
    void onRegisterEvents(core::event::Dispatcher& _dispatcher, bool _isBeingRegistered) override;
    core::state::Return onUpdate(sf::Time _dt) override;

    std::unique_ptr<game::Board> m_board;
    core::event::ListenerId m_listenerId = core::event::ListenerIdInvalid;
};

} // namespace cn::server::states
