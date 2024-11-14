#pragma once

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

    Return onHandleEvent(const sf::Event& _event) override;
    Return onUpdate(sf::Time _dt) override;
    void onDraw() override;
    void onActivate() override;
    void onDeactivate() override;

private:
    core::object::Container& getGameContainer() { return m_gameContainer; }

    core::object::Container m_gameContainer;
    sf::Sprite m_background;
    game::TablePtr m_table;
};

} // namespace cn::states
