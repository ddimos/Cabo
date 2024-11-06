#include "state/states/TitleState.hpp"
#include "ResourceIds.hpp"

#include <SFML/Graphics/RenderWindow.hpp>

namespace cn::state
{

TitleState::TitleState(StateManager& _stateManagerRef)
    : State(_stateManagerRef)
{
    m_text.setFont(getContext().fontHolderRef.get(FontIds::Main));
    m_text.setString("Press any key to start");
    m_text.setPosition(sf::Vector2f(getContext().windowRef.getSize() / 2u));
}

State::Return TitleState::handleEvent(const sf::Event& _event)
{
    if (_event.type == sf::Event::KeyReleased)
    {
        pop();
        push(Name::Create);
    }
	return Return::Break;
}

State::Return TitleState::update(sf::Time _dt)
{
    m_toggleEffectTimeDt += _dt;
    if (m_toggleEffectTimeDt >= m_toggleEffectTimeS)
    {
        m_showText = !m_showText;
        m_toggleEffectTimeDt = sf::Time::Zero;
    }
    return Return::Break;
}

void TitleState::draw()
{
    if (m_showText)
        getContext().windowRef.draw(m_text);
}

} // namespace cn::state
