#include "states/MainMenuState.hpp"
#include "states/StateIds.hpp"

#include "menu/item/Button.hpp"

#include "ResourceIds.hpp"

#include <SFML/Graphics/RenderWindow.hpp>

namespace cn::states
{

MainMenuState::MainMenuState(core::state::Manager& _stateManagerRef)
    : State(_stateManagerRef)
{
    auto startButton = std::make_shared<menu::item::Button>(
        menu::Position{
            .m_position = sf::Vector2f(0.f, -45.f), .m_parentSize = sf::Vector2f(getContext().windowRef.getSize()),
            .m_specPositionX = menu::Position::Special::CENTER_ALLIGNED, .m_specPositionY = menu::Position::Special::OFFSET_FROM_CENTER
        },
        getContext().textureHolderRef.get(TextureIds::MainMenuStartButton),
        sf::IntRect{0,   0, 200, 62},
        sf::IntRect{200, 0, 200, 62},
        [this](bool _isPressed){
            pop();
            push(id::Game);
        },
        sf::Mouse::Button::Left
    );
    getMenuContainer().add(startButton);

    auto joinButton = std::make_shared<menu::item::Button>(
        menu::Position{
            .m_position = sf::Vector2f(0.f, 45.f), .m_parentSize = sf::Vector2f(getContext().windowRef.getSize()),
            .m_specPositionX = menu::Position::Special::CENTER_ALLIGNED, .m_specPositionY = menu::Position::Special::OFFSET_FROM_CENTER
        },
        getContext().textureHolderRef.get(TextureIds::MainMenuJoinButton),
        sf::IntRect{0,   0, 200, 62},
        sf::IntRect{200, 0, 200, 62},
        [this](bool _isPressed){

        },
        sf::Mouse::Button::Left
    );
    
    getMenuContainer().add(joinButton);
}

} // namespace cn::states
