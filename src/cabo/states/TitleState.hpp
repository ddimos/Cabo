#pragma once

#include "core/state/State.hpp"

#include <SFML/Graphics/Sprite.hpp>

namespace core::object
{
    class Object;
}

namespace cn::states
{

class TitleState final : public core::state::State
{
public:
    TitleState(core::state::Manager& _stateManagerRef);

    core::state::Return onHandleEvent(const sf::Event& _event) override;
    core::state::Return onUpdate(sf::Time _dt) override;
    void onDraw() override;

private:
    sf::Sprite m_background;
    std::shared_ptr<core::object::Object> m_text;

    bool m_showText = true;
    sf::Time m_toggleEffectTimeS = sf::seconds(1.f);
    sf::Time m_toggleEffectTimeDt = sf::Time::Zero;
};
} // namespace cn::states
