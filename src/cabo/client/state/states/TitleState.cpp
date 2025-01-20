#include "client/state/states/TitleState.hpp"
#include "client/state/StateIds.hpp"
#include "client/ResourceIds.hpp"

#include "client/menu/item/SimpleText.hpp"

#include "core/event/Dispatcher.hpp"
#include "events/InputEvents.hpp"

#include <SFML/Graphics/RenderWindow.hpp>

namespace cn::client::states
{

TitleState::TitleState(core::state::Manager& _stateManagerRef)
    : State(_stateManagerRef)
{
    createContainer(core::object::Container::Type::Menu);

    auto& windowRef = getContext().get<sf::RenderWindow>();
    auto& fontHolderRef = getContext().get<FontHolder>();
    
    m_text = std::make_shared<menu::item::SimpleText>(
        menu::Position{
            .m_position = sf::Vector2f(0.f, -45.f), .m_parentSize = sf::Vector2f(windowRef.getSize()),
            .m_specPositionX = menu::Position::Special::CENTER_ALLIGNED, .m_specPositionY = menu::Position::Special::OFFSET_FROM_CENTER
        },
        "Press any key to start",
        fontHolderRef.get(FontIds::Main),
        24,
        sf::Color::White
    );
    getContainer(core::object::Container::Type::Menu).add(m_text);

    m_listenerId = core::event::getNewListenerId();
}

core::state::Return TitleState::onUpdate(sf::Time _dt)
{
    m_toggleEffectTimeDt += _dt;
    if (m_toggleEffectTimeDt >= m_toggleEffectTimeS)
    {
        m_showText = !m_showText;
        m_toggleEffectTimeDt = sf::Time::Zero;
    }
    return core::state::Return::Break;
}

void TitleState::onDraw()
{
    if (m_showText && !m_text->isActivated())
        m_text->requestActivated();
    else if (!m_showText && m_text->isActivated())
        m_text->requestDeactivated();
}

void TitleState::onRegisterEvents(core::event::Dispatcher& _dispatcher, bool _isBeingRegistered)
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
