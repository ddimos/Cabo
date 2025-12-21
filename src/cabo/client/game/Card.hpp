#pragma once

#include "core/object/Object.hpp"
#include "core/Context.hpp"
#include "core/Interpolator.hpp"

#include "shared/game/object/Card.hpp"

#include <SFML/Graphics/Sprite.hpp>

namespace cn::client::game
{

class Card final : public shared::game::object::Card
{
public:
    Card(const core::Context& _context, shared::game::object::Id _id);

    void startTransit(sf::Vector2f _pos);
    void startFlipping(bool _wantsToBeUp);

private:
    void onUpdate(sf::Time _dt) override;
    void onDraw(sf::RenderWindow& _window) override;

    void onMoved(sf::Vector2f _newPos) override;

    sf::Sprite m_sprite;
    core::Interpolator<sf::Vector2f> m_interpolatedPos;
    core::Interpolator<float> m_interpolatedFlip;
    bool m_wantsToBeUp = false;
};

} // namespace cn::client::game
