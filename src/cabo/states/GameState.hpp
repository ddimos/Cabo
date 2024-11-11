#pragma once

#include "state/State.hpp"

#include <SFML/Graphics/Sprite.hpp>

namespace cn::states
{

class GameState : public state::State
{
public:
    GameState(state::StateManager& _stateManagerRef);

    Return onHandleEvent(const sf::Event& _event) override;
    Return onUpdate(sf::Time _dt) override;
    void onDraw() override;

private:
    sf::Sprite m_background;
};

} // namespace cn::states
