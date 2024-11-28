#pragma once

#include "core/object/Container.hpp"
#include "core/state/State.hpp"
#include "game/Types.hpp"

namespace cn::states
{

class GameState final : public core::state::State
{
public:
    GameState(core::state::Manager& _stateManagerRef);

    core::state::Return onHandleEvent(const sf::Event& _event) override;
    core::state::Return onUpdate(sf::Time _dt) override;
    void onDraw() override;
    void onActivate() override;
    void onDeactivate() override;

private:
    core::object::Container& getGameContainer() { return m_gameContainer; }

    core::object::Container m_gameContainer;
    game::TablePtr m_table;
};

} // namespace cn::states
