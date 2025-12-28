#include "client/state/states/MainMenuState.hpp"
#include "client/state/StateIds.hpp"
#include "client/ResourceIds.hpp"

#include "client/menu/item/Button.hpp"

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
            .m_position = sf::Vector2f(0.f, -70.f), .m_parentSize = sf::Vector2f(windowRef.getSize()),
            .m_specPositionX = menu::Position::Special::CENTER_ALLIGNED, .m_specPositionY = menu::Position::Special::OFFSET_FROM_CENTER
        },
        textureHolderRef.get(TextureIds::JoinButton),
        sf::IntRect{0,   0, 200, 62},
        sf::IntRect{200, 0, 200, 62},
        [this](){
            push(states::id::Joining);
        },
        sf::Mouse::Button::Left
    );
    getContainer(MenuContainerId).add(joinButton);

    auto optionButton = std::make_shared<menu::item::Button>(
        menu::Position{
            .m_position = sf::Vector2f(0.f, 70.f), .m_parentSize = sf::Vector2f(windowRef.getSize()),
            .m_specPositionX = menu::Position::Special::CENTER_ALLIGNED, .m_specPositionY = menu::Position::Special::CENTER_ALLIGNED
        },
        textureHolderRef.get(TextureIds::SettingsButton),
        sf::IntRect{0,   0, 200, 62},
        sf::IntRect{200, 0, 200, 62},
        [this](){
            push(states::id::Entering);
        },
        sf::Mouse::Button::Left
    );
    getContainer(MenuContainerId).add(optionButton);
}

void MainMenuState::onActivate()
{
    auto& saveName = getContext().get<SaveHolder>().get(SaveIds::PlayerName);
    auto& saveServer = getContext().get<SaveHolder>().get(SaveIds::ServerAddress);
    m_goToEntering = saveName.getValue().empty() || saveServer.getValue().empty();
}

core::state::Return MainMenuState::onUpdate(sf::Time _dt)
{
    if (m_goToEntering)
    {
        push(id::Entering);
    }
    return core::state::Return::Break;
}

} // namespace cn::client::states
