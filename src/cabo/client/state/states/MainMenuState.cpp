#include "client/state/states/MainMenuState.hpp"
#include "client/state/StateIds.hpp"
#include "client/ResourceIds.hpp"

#include "client/menu/item/Button.hpp"
#include "client/menu/item/SimpleText.hpp"

#include "net/Manager.hpp"

#include <SFML/Graphics/RenderWindow.hpp>

namespace cn::client::states
{

MainMenuState::MainMenuState(core::state::Manager& _stateManagerRef)
    : State(_stateManagerRef)
{
    createContainer(core::object::Container::Type::Menu);
    
    auto& windowRef = getContext().get<sf::RenderWindow>();
    auto& textureHolderRef = getContext().get<TextureHolder>();
    auto& fontHolderRef = getContext().get<FontHolder>();


    auto joinButton = std::make_shared<menu::item::Button>(
        menu::Position{
            .m_position = sf::Vector2f(0.f, 0.f), .m_parentSize = sf::Vector2f(windowRef.getSize()),
            .m_specPositionX = menu::Position::Special::CENTER_ALLIGNED, .m_specPositionY = menu::Position::Special::CENTER_ALLIGNED
        },
        textureHolderRef.get(TextureIds::MainMenuJoinButton),
        sf::IntRect{0,   0, 200, 62},
        sf::IntRect{200, 0, 200, 62},
        [this](){
            connect();
        },
        sf::Mouse::Button::Left
    );
    getContainer(core::object::Container::Type::Menu).add(joinButton);
}

void MainMenuState::connect()
{
    // TODO 
    getContext().get<net::Manager>().connect(nsf::NetworkAddress(sf::IpAddress("127.0.0.1"), 20475));
    push(states::id::Joining);
}

} // namespace cn::client::states
