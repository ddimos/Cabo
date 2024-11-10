#pragma once

#include "state/State.hpp"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

namespace cn::states
{

class TitleState : public state::State
{
public:
    TitleState(state::StateManager& _stateManagerRef);

    Return onHandleEvent(const sf::Event& _event) override;
    Return onUpdate(sf::Time _dt) override;
    void onDraw() override;

private:
    sf::Sprite m_background;
    sf::Text m_text;

    bool m_showText = true;
    sf::Time m_toggleEffectTimeS = sf::seconds(1.f);
    sf::Time m_toggleEffectTimeDt = sf::Time::Zero;
};
} // namespace cn::states