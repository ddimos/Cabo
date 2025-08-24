#pragma once

#include "core/event/Types.hpp"
#include "core/state/State.hpp"

#include "shared/game/Board.hpp"
#include "shared/game/controller/Attachable.hpp"
#include "shared/game/controller/Flippable.hpp"
#include "shared/game/controller/Grabbable.hpp"
#include "shared/game/controller/PrivateZoneViewable.hpp"

#include "server/game/InputController.hpp"

namespace cn::server::states
{

class GameState final : public core::state::State
{
public:
    GameState(core::state::Manager& _stateManagerRef);

private:
    void onRegisterEvents(core::event::Dispatcher& _dispatcher, bool _isBeingRegistered) override;
    core::state::Return onUpdate(sf::Time _dt) override;

    std::unique_ptr<game::InputController> m_inputController;
    std::unique_ptr<shared::game::controller::Flippable> m_flipController;
    std::unique_ptr<shared::game::controller::Grabbable> m_grabController;
    std::unique_ptr<shared::game::controller::PrivateZoneViewable> m_privateZoneViewableController;
    std::unique_ptr<shared::game::Board> m_board;

    core::event::ListenerId m_listenerId{};
};

} // namespace cn::server::states
