#include "client/state/states/MainMenuState.hpp"
#include "client/state/StateIds.hpp"
#include "client/ResourceIds.hpp"

#include "client/menu/item/Button.hpp"
#include "client/menu/item/SimpleText.hpp"

#include "shared/net/Manager.hpp"

#include <SFML/Graphics/RenderWindow.hpp>

namespace
{
constexpr cn::core::object::Container::Id MenuContainerId = 1;
} // namespace

namespace cn::client::states
{

MainMenuState::MainMenuState(core::state::Manager& _stateManagerRef)
    : State(_stateManagerRef)
{
    createContainer(MenuContainerId);
    
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
    getContainer(MenuContainerId).add(joinButton);
}

void MainMenuState::connect()
{
    auto& save = getContext().get<SaveHolder>().get(SaveIds::ServerAddress);
    size_t pos = save.getValue().find(':');
    auto ip = sf::IpAddress(save.getValue().substr(0, pos));
    unsigned port = std::stoi(save.getValue().substr(pos + 1));

    getContext().get<net::Manager>().connect(nsf::NetworkAddress(ip, port));
    push(states::id::Joining);
}

} // namespace cn::client::states
