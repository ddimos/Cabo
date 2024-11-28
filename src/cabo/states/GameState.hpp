#pragma once

#include "core/event/Types.hpp"
#include "core/object/Container.hpp"
#include "game/Types.hpp"
#include "state/State.hpp"

#include <SFML/Graphics/Sprite.hpp>

namespace cn::states
{

class GameState final : public state::State
{
public:
    GameState(state::StateManager& _stateManagerRef);

private:
    Return onUpdate(sf::Time _dt) override;
    void onDraw() override;
    void onActivate() override;
    void onDeactivate() override;
    void onRegisterEvents(core::event::Dispatcher& _dispatcher, bool _isBeingRegistered) override;

    core::object::Container& getGameContainer() { return m_gameContainer; }

    core::object::Container m_gameContainer;
    sf::Sprite m_background;
    game::TablePtr m_table;

    core::event::ListenerId m_listenerId = core::event::ListenerIdInvalid;
};

} // namespace cn::states
