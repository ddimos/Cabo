#include "client/state/states/MainMenuState.hpp"
#include "client/state/StateIds.hpp"

#include "menu/item/Button.hpp"

#include "ResourceIds.hpp"

#include <SFML/Graphics/RenderWindow.hpp>

namespace cn::client::states
{

MainMenuState::MainMenuState(core::state::Manager& _stateManagerRef)
    : State(_stateManagerRef)
{
    createContainer(core::object::Container::Type::Menu);
    
    auto& windowRef = getContext().get<sf::RenderWindow>();
    auto& textureHolderRef = getContext().get<TextureHolder>();

    auto startButton = std::make_shared<menu::item::Button>(
        menu::Position{
            .m_position = sf::Vector2f(0.f, -45.f), .m_parentSize = sf::Vector2f(windowRef.getSize()),
            .m_specPositionX = menu::Position::Special::CENTER_ALLIGNED, .m_specPositionY = menu::Position::Special::OFFSET_FROM_CENTER
        },
        textureHolderRef.get(TextureIds::MainMenuStartButton),
        sf::IntRect{0,   0, 200, 62},
        sf::IntRect{200, 0, 200, 62},
        [this](){
            pop();
            push(id::Game);
        },
        sf::Mouse::Button::Left
    );
    getContainer(core::object::Container::Type::Menu).add(startButton);

    auto joinButton = std::make_shared<menu::item::Button>(
        menu::Position{
            .m_position = sf::Vector2f(0.f, 45.f), .m_parentSize = sf::Vector2f(windowRef.getSize()),
            .m_specPositionX = menu::Position::Special::CENTER_ALLIGNED, .m_specPositionY = menu::Position::Special::OFFSET_FROM_CENTER
        },
        textureHolderRef.get(TextureIds::MainMenuJoinButton),
        sf::IntRect{0,   0, 200, 62},
        sf::IntRect{200, 0, 200, 62},
        [this](){

        },
        sf::Mouse::Button::Left
    );
    
    getContainer(core::object::Container::Type::Menu).add(joinButton);
}

} // namespace cn::client::states
