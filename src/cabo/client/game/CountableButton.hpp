#pragma once

#include "core/Context.hpp"

#include "shared/game/object/CountableButton.hpp"
#include "shared/game/Types.hpp"

#include <SFML/Graphics/Sprite.hpp>

namespace cn::client::game
{

class CountableButton final : public shared::game::object::CountableButton
{
public:
    CountableButton(const core::Context& _context, shared::game::object::Id _id, shared::game::TableButtonType _type, unsigned _numberOfPlayersToClick);

private:
    void onDraw(sf::RenderWindow& _window) override;
    void onActivate() override;

    void onPlayerClicked(PlayerId _playerId);
    void onReset();

    shared::game::TableButtonType m_type = shared::game::TableButtonType::Deal;
    sf::Sprite m_sprite;

    sf::IntRect m_textureRectDefault = {};
    sf::IntRect m_textureRectClicked = {};
};

} // namespace cn::client::game
