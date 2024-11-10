#pragma once

#include "state/State.hpp"
#include <SFML/Graphics/Sprite.hpp>
#include <memory>

namespace cn::states
{

class MainMenuState : public state::State
{
public:
    MainMenuState(state::StateManager& _stateManagerRef);

private:
    Return onHandleEvent(const sf::Event& _event) override;
    Return onUpdate(sf::Time _dt) override;
    void onDraw() override;

    sf::Sprite m_background;
};
} // namespace cn::states
