#include "states/TitleState.hpp"
#include "states/StateIds.hpp"
#include "ResourceIds.hpp"

#include <SFML/Graphics/RenderWindow.hpp>

namespace cn::states
{

TitleState::TitleState(state::StateManager& _stateManagerRef)
    : State(_stateManagerRef)
{
    m_text.setFont(getContext().fontHolderRef.get(FontIds::Main));
    m_text.setString("Press any key to start");
    m_text.setPosition(sf::Vector2f(getContext().windowRef.getSize() / 2u));
}

state::State::Return TitleState::handleEvent(const sf::Event& _event)
{
    if (_event.type == sf::Event::KeyReleased)
    {
        pop();
        push(id::Create);
    }
	return Return::Break;
}

state::State::Return TitleState::update(sf::Time _dt)
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

} // namespace cn::states
