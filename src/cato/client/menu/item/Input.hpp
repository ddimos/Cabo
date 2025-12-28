#pragma once

#include "client/menu/Item.hpp"
#include "client/menu/item/SimpleText.hpp"
#include "client/menu/component/Clickable.hpp"

#include "core/event/Types.hpp"

#include <SFML/Graphics/Sprite.hpp>

namespace cn::client::menu::item
{

class Input final : public Item
{
public:
    using ValidateEnteredTextCallback = std::function<bool(uint32_t, const std::string&)>;
    using FinishEnterTextCallback = std::function<void(const std::string&)>;

    Input(Position _position, const sf::Texture& _texture, SimpleText& _text, const std::string& _helpText,
        ValidateEnteredTextCallback _onValidateEnteredTextCallback, FinishEnterTextCallback _onFinishEnterTextCallback,
        unsigned _maxSize);

    const std::string& getEnteredText() const { return m_enteredStr; }
    
private:
    void onDraw(sf::RenderWindow& _windowRef) override;
    void onRegisterEvents(core::event::Dispatcher& _dispatcher, bool _isBeingRegistered) override;

    void enterInputMode();
    void exitInputMode();

    void processChar(uint32_t _char);

    component::Clickable m_clickable;

    sf::Sprite m_sprite;
    SimpleText& m_textRef;
    std::string m_enteredStr;
    std::string m_helpText;

    bool m_isInInputMode = false;
    ValidateEnteredTextCallback m_onValidateEnteredTextCallback = {};
    FinishEnterTextCallback m_onFinishEnterTextCallback = {};

    const unsigned m_maxSize = 32;

    core::event::ListenerId m_listenerId{};
};

} // namespace cn::client::menu::item
