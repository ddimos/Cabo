#pragma once

#include "core/object/Object.hpp"

#include "game/Card.hpp"
#include "game/Types.hpp"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Time.hpp>

#include <vector>

namespace cn::core
{
    struct Context;
}

namespace cn::game
{

class Table final : public core::object::Object
{
public:
    Table(const core::Context& _context, DeckPtr _deck, DiscardPtr _discard);

    void addPlayer(PlayerPtr _player);

    void start();
    void onLocalPlayerClickDeck();
    void onLocalPlayerClickDiscard();

private:
    void onHandleEvent(const sf::Event& _event);
    void onUpdate(sf::Time _dt);
    void onDraw(sf::RenderWindow& _window);

    sf::Sprite m_sprite;

    const core::Context& m_contextRef;
    DeckPtr m_deck;
    DiscardPtr m_discard;
    std::vector<PlayerPtr> m_players;
    std::vector<PlayerSpawnPoint> m_spawnPoints;

    CardPtr m_currentCard;
};

} // namespace cn::game
