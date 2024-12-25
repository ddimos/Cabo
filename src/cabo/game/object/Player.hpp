#pragma once

#include "core/object/Object.hpp"

#include "game/PlayerSlot.hpp"
#include "game/Types.hpp"

#include "menu/Types.hpp"

#include <SFML/Graphics/Text.hpp>

#include <functional>
#include <map>

namespace cn::core
{
    struct Context;
}

namespace cn::game
{

class Player final : public core::object::Object
{
public:
    Player(const core::Context& _context, PlayerId _id, bool _isLocal, std::map<PlayerSlotId, PlayerSlot>&& _slots, unsigned short _initialNumberOfSlots);

    PlayerId getId() const { return m_id; }
    bool isLocal() const { return m_isLocal; }

    void setSpawnPoint(PlayerSpawnPoint _spawnPoint);

    void addSlot();
    void removeSlot(PlayerSlotId _id);

    void showCardInSlot(PlayerSlotId _id);
    void hideCardInSlot(PlayerSlotId _id);

    const PlayerSlot& getSlot(PlayerSlotId _id) const;
    CardPtr getCard(PlayerSlotId _id) const;

    CardPtr replace(PlayerSlotId _id, CardPtr _card);

    void deal(CardPtr _card);

    void visitSlots(std::function<void(PlayerSlot& _slot)> _visitor);

private:
    void onDraw(sf::RenderWindow& _window) override;

    sf::Text m_nameText;

    PlayerId m_id = PlayerIdInvalid;
    bool m_isLocal = false;

    std::map<PlayerSlotId, PlayerSlot> m_slots;
    PlayerSpawnPoint m_spawnPoint;
    unsigned short m_currentNumberOfSlots = 0;
};

} // namespace cn::game
