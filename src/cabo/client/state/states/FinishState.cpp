#include "client/state/states/FinishState.hpp"
#include "client/state/StateIds.hpp"
#include "client/ResourceIds.hpp"

#include "client/menu/item/SimpleText.hpp"

#include "core/event/Dispatcher.hpp"
#include "shared/events/InputEvents.hpp"

#include <SFML/Graphics/RenderWindow.hpp>

namespace cn::client::states
{

FinishState::FinishState(core::state::Manager& _stateManagerRef)
    : State(_stateManagerRef)
{
    createContainer(core::object::Container::Type::Menu);

    auto& windowRef = getContext().get<sf::RenderWindow>();
    auto& fontHolderRef = getContext().get<FontHolder>();

    auto text = std::make_shared<menu::item::SimpleText>(
        menu::Position{
            .m_position = sf::Vector2f(0.f, -45.f), .m_parentSize = sf::Vector2f(windowRef.getSize()),
            .m_specPositionX = menu::Position::Special::CENTER_ALLIGNED, .m_specPositionY = menu::Position::Special::OFFSET_FROM_CENTER
        },
        "Finish",
        fontHolderRef.get(FontIds::Main),
        20,
        sf::Color::White
    );
    getContainer(core::object::Container::Type::Menu).add(text);

    m_listenerId = core::event::getNewListenerId();
}

void FinishState::onRegisterEvents(core::event::Dispatcher& _dispatcher, bool _isBeingRegistered)
{
    if (_isBeingRegistered)
    {
        _dispatcher.registerEvent<events::KeyReleasedEvent>(m_listenerId,
            [this](const events::KeyReleasedEvent& _event){
                (void)_event;
                pop();
                push(id::MainMenu);
            }
        );
    }
    else
    {
        _dispatcher.unregisterEvent<events::KeyReleasedEvent>(m_listenerId);
    }
}

} // namespace cn::client::states
