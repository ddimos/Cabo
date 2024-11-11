#include "states/FinishState.hpp"

#include "states/StateIds.hpp"
#include "ResourceIds.hpp"

#include "menu/item/SimpleText.hpp"

#include <SFML/Graphics/RenderWindow.hpp>

namespace cn::states
{

FinishState::FinishState(state::StateManager& _stateManagerRef)
    : State(_stateManagerRef)
{
    auto text = std::make_shared<menu::item::SimpleText>(
        menu::Position{
            menu::Position::Special::CENTER_ALLIGNED, menu::Position::Special::OFFSET_FROM_CENTER, sf::Vector2f(0.f, -45.f)
        },
        "Finish",
        getContext().fontHolderRef.get(FontIds::Main),
        20,
        sf::Color::White,
        getContext().windowRef
    );
    getContainer().add(text);
}

} // namespace cn::states
