#include "states/TitleState.hpp"
#include "states/StateIds.hpp"
#include "ResourceIds.hpp"

#include "menu/item/SimpleText.hpp"

#include <SFML/Graphics/RenderWindow.hpp>

namespace cn::states
{

TitleState::TitleState(state::StateManager& _stateManagerRef)
    : State(_stateManagerRef)
{
    m_text = std::make_shared<menu::item::SimpleText>(
        menu::Position{
            .m_position = sf::Vector2f(0.f, -45.f), .m_parentSize = sf::Vector2f(getContext().windowRef.getSize()),
            .m_specPositionX = menu::Position::Special::CENTER_ALLIGNED, .m_specPositionY = menu::Position::Special::OFFSET_FROM_CENTER
        },
        "Press any key to start",
        getContext().fontHolderRef.get(FontIds::Main),
        24,
        sf::Color::White
    );
    getMenuContainer().add(m_text);
}

state::State::Return TitleState::onHandleEvent(const sf::Event& _event)
{
    if (_event.type == sf::Event::KeyReleased)
    {
        pop();
        push(id::MainMenu);
    }
	return Return::Break;
}

state::State::Return TitleState::onUpdate(sf::Time _dt)
{
    m_toggleEffectTimeDt += _dt;
    if (m_toggleEffectTimeDt >= m_toggleEffectTimeS)
    {
        m_showText = !m_showText;
        m_toggleEffectTimeDt = sf::Time::Zero;
    }
    return Return::Break;
}

void TitleState::onDraw()
{
    if (m_showText && !m_text->isActivated())
        m_text->activate();
    else if (!m_showText && m_text->isActivated())
        m_text->deactivate();
}

} // namespace cn::states
