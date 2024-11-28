#pragma once

#include "core/event/Types.hpp"
#include "core/object/Container.hpp"
#include "core/state/State.hpp"
#include "game/Types.hpp"

namespace cn::states
{

class GameState final : public core::state::State
{
public:
    GameState(core::state::Manager& _stateManagerRef);

private:
    core::state::Return onUpdate(sf::Time _dt) override;
    void onDraw() override;
    void onActivate() override;
    void onDeactivate() override;
    void onRegisterEvents(core::event::Dispatcher& _dispatcher, bool _isBeingRegistered) override;

    core::object::Container& getGameContainer() { return m_gameContainer; }

    core::object::Container m_gameContainer;
    game::TablePtr m_table;

    core::event::ListenerId m_listenerId = core::event::ListenerIdInvalid;
};

} // namespace cn::states
