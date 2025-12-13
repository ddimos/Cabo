#include "client/game/CountableButton.hpp"
#include "client/game/SpriteSheet.hpp"
#include "client/ResourceIds.hpp"

#include <SFML/Graphics/RenderWindow.hpp>

namespace cn::client::game
{

CountableButton::CountableButton(const core::Context& _context, shared::game::object::Id _id, shared::game::TableButtonType _type, unsigned _numberOfPlayersToClick)
    : shared::game::object::CountableButton(
        _id, _numberOfPlayersToClick, 
        [this](PlayerId _id){
            onPlayerClicked(_id);
        },
        [this](){
            onReset();
        })
    , m_type(_type)
{
    switch (m_type)
    {
        case shared::game::TableButtonType::Deal:
        m_textureRectDefault = game::spriteSheet::getDealButton();
        m_textureRectClicked = game::spriteSheet::getDealButton(game::spriteSheet::Hover::Yes);
        break;
        case shared::game::TableButtonType::ResetAndShuffle:
        m_textureRectDefault = game::spriteSheet::getResetButton();
        m_textureRectClicked = game::spriteSheet::getResetButton(game::spriteSheet::Hover::Yes);
        break;
        case shared::game::TableButtonType::ShuffleFromDiscard:
        m_textureRectDefault = game::spriteSheet::getFromDisacrdButton();
        m_textureRectClicked = game::spriteSheet::getFromDisacrdButton(game::spriteSheet::Hover::Yes);
        break;
        
    }
    m_sprite.setTexture(_context.get<TextureHolder>().get(TextureIds::TableButtons));
    m_sprite.setTextureRect(m_textureRectDefault);
    m_sprite.setOrigin(m_sprite.getLocalBounds().getSize() / 2.f);
}

void CountableButton::onDraw(sf::RenderWindow& _windowRef)
{
    _windowRef.draw(m_sprite);
}

void CountableButton::onActivate()
{
    m_sprite.setPosition(getPosition());
}

void CountableButton::onPlayerClicked(PlayerId _playerId)
{
    m_sprite.setTextureRect(m_textureRectClicked);
}

void CountableButton::onReset()
{
    m_sprite.setTextureRect(m_textureRectDefault);
}

} // namespace cn::client::game
