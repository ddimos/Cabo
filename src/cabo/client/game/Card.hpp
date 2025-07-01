#pragma once

#include "core/object/Object.hpp"
#include "core/Context.hpp"

#include "shared/board/Card.hpp"

#include <SFML/Graphics/Sprite.hpp>

namespace cn::client::game
{

class Card final : public core::object::Object
{
public:
    Card(const core::Context& _context, shared::board::Card& _boardCard);
    
    void onUpdate(sf::Time _dt) override;
    void onDraw(sf::RenderWindow& _window) override;

    shared::board::Card& getBoardCard() const { return m_boardCardRef; };

private:
    shared::board::Card& m_boardCardRef;
    sf::Sprite m_sprite;
};

} // namespace cn::client::game
