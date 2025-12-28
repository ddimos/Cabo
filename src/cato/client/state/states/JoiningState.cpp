#include "client/state/states/JoiningState.hpp"
#include "client/state/StateIds.hpp"
#include "client/ResourceIds.hpp"

#include "client/menu/item/Button.hpp"
#include "client/menu/item/SimpleText.hpp"

#include "shared/events/ConnectionEvents.hpp"
#include "shared/net/Manager.hpp"

#include <SFML/Graphics/RenderWindow.hpp>

namespace
{
constexpr cn::core::object::Container::Id MenuContainerId = 1;
} // namespace

namespace cn::client::states
{

JoiningState::JoiningState(core::state::Manager& _stateManagerRef)
    : State(_stateManagerRef)
{
    createContainer(MenuContainerId);
    
    auto& windowRef = getContext().get<sf::RenderWindow>();
    auto& textureHolderRef = getContext().get<TextureHolder>();
    auto& fontHolderRef = getContext().get<FontHolder>();


    auto connectingText = std::make_shared<menu::item::SimpleText>(
        menu::Position{
            .m_position = sf::Vector2f(0.f, 0.f), .m_parentSize = sf::Vector2f(windowRef.getSize()),
            .m_specPositionX = menu::Position::Special::CENTER_ALLIGNED, .m_specPositionY = menu::Position::Special::CENTER_ALLIGNED
        },
        "Connecting...",
        fontHolderRef.get(FontIds::Main),
        24,
        sf::Color::White
    );
    getContainer(MenuContainerId).add(connectingText);

    auto backButton = std::make_shared<menu::item::Button>(
        menu::Position{
            .m_position = sf::Vector2f(100.f, 200.f), .m_parentSize = sf::Vector2f(windowRef.getSize()),
            .m_specPositionX = menu::Position::Special::OFFSET_FROM_CENTER, .m_specPositionY = menu::Position::Special::OFFSET_FROM_CENTER
        },
        textureHolderRef.get(TextureIds::BackButton),
        sf::IntRect{0,  0, 76, 28},
        sf::IntRect{0, 28, 76, 28},
        [this](){
            pop();
        },
        sf::Mouse::Button::Left
    );
    getContainer(MenuContainerId).add(backButton);

    m_listenerId = core::event::getNewListenerId();
}

void JoiningState::onActivate()
{
    auto& save = getContext().get<SaveHolder>().get(SaveIds::ServerAddress);
    size_t pos = save.getValue().find(':');
    auto ip = sf::IpAddress(save.getValue().substr(0, pos));
    unsigned port = std::stoi(save.getValue().substr(pos + 1));

    getContext().get<net::Manager>().connect(nsf::NetworkAddress(ip, port));
}

void JoiningState::onRegisterEvents(core::event::Dispatcher& _dispatcher, bool _isBeingRegistered)
{
    if (_isBeingRegistered)
    {
        _dispatcher.registerEvent<events::PeerConnectedEvent>(m_listenerId,
        [this](const events::PeerConnectedEvent& _event){
            clear();
            push(id::Lobby);
        });
    }
    else
    {
        _dispatcher.unregisterEvent<events::PeerConnectedEvent>(m_listenerId);
    }
}

} // namespace cn::client::states
