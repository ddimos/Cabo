#pragma once

#include "core/object/Object.hpp"

#include "game/PlayerSlot.hpp"
#include "game/Types.hpp"

#include "menu/Types.hpp"

#include <SFML/Graphics/Text.hpp>

#include <vector>

namespace cn::core
{
    struct Context;
}

namespace cn::game
{

class Player final : public core::object::Object
{
public:
    Player(const core::Context& _context, std::vector<PlayerSlot>&& _slots, unsigned short _initialNumberOfSlots);

    void setSpawnPoint(PlayerSpawnPoint _spawnPoint);

    void addSlot();
    void removeSlot(PlayerSlotId _id);

    void deal(CardPtr _card);

private:
    void onDraw(sf::RenderWindow& _window) override;

    sf::Text m_nameText;

    std::vector<PlayerSlot> m_slots;
    PlayerSpawnPoint m_spawnPoint;
    unsigned short m_currentNumberOfSlots = 0;
};

} // namespace cn::game
