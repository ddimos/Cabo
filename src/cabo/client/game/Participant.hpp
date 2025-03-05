#pragma once

#include "client/game/Card.hpp"
#include "client/game/Types.hpp"
#include "client/menu/Types.hpp"

#include "shared/player/Types.hpp"

#include "core/object/Object.hpp"

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

    void onStartShowingCardInSlot(ParticipantSlotId _id);
    void onCardRecievedInSlot(ParticipantSlotId _id, Card _card);
    void onFinishShowingCardInSlot(ParticipantSlotId _id);
    
    // TODO to think how to unite with onCardRecievedInSlot
    void onProvideCardInSlot(ParticipantSlotId _id, Card _card); 

    const ParticipantSlot& getSlot(ParticipantSlotId _id) const;

private:
    void onDraw(sf::RenderWindow& _window) override;

    ParticipantSlot& getSlot(ParticipantSlotId _id);

    sf::Text m_nameText; // TODO simpleText

    PlayerId m_id = PlayerIdInvalid;
    bool m_isLocal = false;

    std::map<ParticipantSlotId, ParticipantSlot> m_slots;
    PlayerSpawnPoint m_spawnPoint;
    unsigned short m_currentNumberOfSlots = 0;
};

} // namespace cn::client::game
