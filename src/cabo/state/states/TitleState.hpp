#pragma once

#include "state/State.hpp"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

namespace cn::state
{

class TitleState : public State
{
public:
    TitleState(StateManager& _stateManagerRef);

    Return handleEvent(const sf::Event& _event) override;
    void draw() override;
    Return update(sf::Time _dt) override;

private:
    sf::Sprite m_background;
    sf::Text m_text;

    bool m_showText = true;
    sf::Time m_toggleEffectTimeS = sf::seconds(1.f);
    sf::Time m_toggleEffectTimeDt = sf::Time::Zero;
};
} // namespace cn::state
