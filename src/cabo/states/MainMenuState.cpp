#include "states/MainMenuState.hpp"

#include "states/StateIds.hpp"
#include "ResourceIds.hpp"

#include "menu/item/Button.hpp"

#include <SFML/Graphics/RenderWindow.hpp>

namespace cn::states
{

MainMenuState::MainMenuState(state::StateManager& _stateManagerRef)
    : State(_stateManagerRef)
{
    auto startButton = std::make_shared<menu::item::Button>(
        menu::Position{
             menu::Position::Special::CENTER_ALLIGNED, menu::Position::Special::CENTER_ALLIGNED
        },
        getContext().textureHolderRef.get(TextureIds::MainMenuStartButton),
        sf::IntRect{0,   0, 200, 62},
        sf::IntRect{200, 0, 200, 62},
        [this](bool _isPressed){

        },
        sf::Mouse::Button::Left,
        getContext().windowRef
    );
    
    getContainer().add(startButton);
}

state::State::Return MainMenuState::onHandleEvent(const sf::Event& _event)
{
	return Return::Break;
}

state::State::Return MainMenuState::onUpdate(sf::Time _dt)
{
    return Return::Break;
}

void MainMenuState::onDraw()
{
}

} // namespace cn::states
