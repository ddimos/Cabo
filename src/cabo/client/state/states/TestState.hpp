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

    bool m_loaded1 = false;
    bool m_loaded2 = false;
    bool m_loaded3 = false;
    bool m_loaded4 = false;
    bool m_loaded5 = false;
    bool m_loaded6 = false;
};

} // namespace cn::client::states
