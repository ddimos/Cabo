#pragma once

#include "core/event/Types.hpp"
#include "core/state/State.hpp"

#include "shared/game/controller/Grabbable.hpp"
#include "shared/game/controller/PrivateZoneViewable.hpp"
#include "shared/game/Board.hpp"

#include "client/game/InputController.hpp"

namespace cn::client::states
{

class GameState final : public core::state::State
{
public:
    GameState(core::state::Manager& _stateManagerRef);

private:
    void onRegisterEvents(core::event::Dispatcher& _dispatcher, bool _isBeingRegistered) override;
    core::state::Return onUpdate(sf::Time _dt) override;

    std::unique_ptr<game::InputController> m_inputController;
    std::unique_ptr<shared::game::controller::Grabbable> m_grabController;
    std::unique_ptr<shared::game::controller::PrivateZoneViewable> m_privateZoneViewableController;
    std::unique_ptr<shared::game::Board> m_board;

    core::event::ListenerId m_listenerId{};
};

} // namespace cn::client::states
