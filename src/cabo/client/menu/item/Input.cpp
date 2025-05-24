#include "client/menu/item/Input.hpp"
#include "shared/events/InputEvents.hpp"
#include "core/Assert.hpp"

#include <SFML/Window/Keyboard.hpp>

namespace cn::client::menu::item
{

Input::Input(Position _position, const sf::Texture& _texture,
    SimpleText& _text, const std::string& _helpText,
    ValidateEnteredTextCallback _onValidateEnteredTextCallback, FinishEnterTextCallback _onFinishEnterTextCallback, unsigned _maxSize)
    : m_textRef(_text)
    , m_helpText(_helpText)
    , m_onValidateEnteredTextCallback(_onValidateEnteredTextCallback)
    , m_onFinishEnterTextCallback(_onFinishEnterTextCallback)
    , m_maxSize(_maxSize)
{
    m_transformable.setPosition(_position);
    sf::Vector2f globalPos = m_transformable.calculateGlobalPos(sf::Vector2f{});

    m_sprite.setTexture(_texture);
    m_sprite.setPosition(globalPos);
    m_sprite.setOrigin(m_sprite.getLocalBounds().getSize() / 2.f);

    m_clickable.setCallback([](){});
    m_clickable.setButton(sf::Mouse::Button::Left);
    m_clickable.setBounds(sf::FloatRect(m_sprite.getPosition(), m_sprite.getLocalBounds().getSize()));

    m_textRef.setPosition(Position{ 
        .m_position = sf::Vector2f(10.f, 10.f),
        .m_parentPos = m_transformable.calculateGlobalPos(m_sprite.getLocalBounds().getSize()), .m_parentSize = m_sprite.getLocalBounds().getSize(),
        .m_specPositionX = menu::Position::Special::OFFSET_FROM_START, .m_specPositionY = menu::Position::Special::OFFSET_FROM_START
    });
}

void Input::onDraw(sf::RenderWindow& _window)
{
    _window.draw(m_sprite);
}

void Input::onRegisterEvents(core::event::Dispatcher& _dispatcher, bool _isBeingRegistered)
{
    if (_isBeingRegistered)
    {
        if (!m_listenerId.isValid())
            m_listenerId = core::event::getNewListenerId();

        _dispatcher.registerEvent<events::MouseButtonReleasedEvent>(m_listenerId,
            [this](const events::MouseButtonReleasedEvent& _event){
                m_clickable.onMouseReleased(_event.mouseButton);

                if(m_isInInputMode && !m_clickable.isActive())
                    exitInputMode();
                else if (!m_isInInputMode && m_clickable.isActive())
                    enterInputMode();
            }
        );
        _dispatcher.registerEvent<events::KeyReleasedEvent>(m_listenerId,
            [this](const events::KeyReleasedEvent& _event){
                if (!m_isInInputMode)
                    return;

                if (_event.key.code == sf::Keyboard::Enter || _event.key.code == sf::Keyboard::Escape)
                {
                    exitInputMode();
                }
                else if (_event.key.code == sf::Keyboard::BackSpace)
                {
                    if (!m_enteredStr.empty())
                    {
                        m_enteredStr.pop_back();
                        m_textRef.updateText(m_enteredStr);
                    }
                }
            }
        );
        _dispatcher.registerEvent<events::TextEnteredEvent>(m_listenerId,
            [this](const events::TextEnteredEvent& _event){
                if (!m_isInInputMode)
                    return;

                if (_event.text.unicode == 8) // Backspace
                    return;

                if(m_enteredStr.size() < m_maxSize)
                {
                    m_enteredStr += _event.text.unicode;    // To be able to validate the full string
                    if (!m_onValidateEnteredTextCallback(_event.text.unicode, m_enteredStr))
                        m_enteredStr.pop_back();
                
                    m_textRef.updateText(m_enteredStr);
                }
            }
        );
    }
    else
    {
        if (!m_listenerId.isValid())
            return;

        _dispatcher.unregisterEvent<events::MouseButtonReleasedEvent>(m_listenerId);
        _dispatcher.unregisterEvent<events::KeyReleasedEvent>(m_listenerId);
        _dispatcher.unregisterEvent<events::TextEnteredEvent>(m_listenerId);
    }
}

void Input::enterInputMode()
{
    CN_ASSERT(!m_isInInputMode);
    m_isInInputMode = true;

    m_textRef.updateText(m_enteredStr);
}

void Input::exitInputMode()
{
    CN_ASSERT(m_isInInputMode);
    m_isInInputMode = false;
    
    if (!m_enteredStr.empty())
        m_onFinishEnterTextCallback(m_enteredStr);
    else
        m_textRef.updateText(m_helpText);
}

} // namespace cn::client::menu::item
