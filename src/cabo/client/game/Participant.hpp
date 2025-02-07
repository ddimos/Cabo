#pragma once

#include "core/object/Object.hpp"

#include "shared/player/Types.hpp"

#include "client/game/Card.hpp"
#include "client/game/Types.hpp"

#include "client/menu/Types.hpp"

#include <SFML/Graphics/Text.hpp>

#include <functional>
#include <map>

namespace cn::core
{
    class Context;
}

namespace cn::client::game
{

class Participant final : public core::object::Object
{
public:
    Participant(const core::Context& _context, PlayerId _id, bool _isLocal,
        std::map<ParticipantSlotId, ParticipantSlot>&& _slots,
        unsigned short _initialNumberOfSlots);

    PlayerId getId() const { return m_id; }
    bool isLocal() const { return m_isLocal; }

    void setSpawnPoint(PlayerSpawnPoint _spawnPoint);

    void addSlot();
    void removeSlot(ParticipantSlotId _id);

    void showCardInSlot(ParticipantSlotId _id);
    void hideCardInSlot(ParticipantSlotId _id);

    const ParticipantSlot& getSlot(ParticipantSlotId _id) const;
    Card* getCard(ParticipantSlotId _id) const;

    Card* replace(ParticipantSlotId _id, Card* _card);

    void deal(Card* _card);

private:
    void onDraw(sf::RenderWindow& _window) override;

    sf::Text m_nameText; // TODO simpleText

    PlayerId m_id = PlayerIdInvalid;
    bool m_isLocal = false;

    std::map<ParticipantSlotId, ParticipantSlot> m_slots;
    PlayerSpawnPoint m_spawnPoint;
    unsigned short m_currentNumberOfSlots = 0;
};

} // namespace cn::client::game
