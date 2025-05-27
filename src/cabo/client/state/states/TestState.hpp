#pragma once

#include "core/state/State.hpp"

#include <SFML/Graphics/Texture.hpp>

namespace cn::client::states
{

class TestState final : public core::state::State
{
public:
    TestState(core::state::Manager& _stateManagerRef);

private:
    void onDraw() override;

    sf::Texture m_texture1;
    bool m_loaded1 = false;
    sf::Texture m_texture2;
    bool m_loaded2 = false;
    sf::Texture m_texture3;
    bool m_loaded3 = false;
    sf::Texture m_texture4;
    bool m_loaded4 = false;
};

} // namespace cn::client::states
