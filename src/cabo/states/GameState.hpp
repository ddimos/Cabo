#pragma once

#include "core/event/Types.hpp"
#include "core/state/State.hpp"
#include "game/Types.hpp"

namespace cn::states
{

class GameState final : public core::state::State
{
public:
    GameState(core::state::Manager& _stateManagerRef);

private:
    void onRegisterEvents(core::event::Dispatcher& _dispatcher, bool _isBeingRegistered) override;

    game::TablePtr m_table;
    core::event::ListenerId m_listenerId = core::event::ListenerIdInvalid;
};

} // namespace cn::states
