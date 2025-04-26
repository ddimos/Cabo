#pragma once

#include "client/game/Card.hpp"
#include "client/game/Types.hpp"

#include "shared/player/Types.hpp"

#include "core/object/Object.hpp"

#include <SFML/Graphics/Text.hpp>

#include <functional>
#include <vector>

namespace cn::core
{
    class Context;
}

namespace cn::client::game
{

class Participant final : public core::object::Object
{
public:
    Participant(const core::Context& _context, PlayerId _id, bool _isLocal, std::vector<ParticipantSlot>&& _slots, unsigned short _initialNumberOfSlots);

    PlayerId getId() const { return m_id; }
    bool isLocal() const { return m_isLocal; }

    sf::Vector2f getOpenCardPosition() const { return m_openCardPosition; }
    float getOpenCardRotation() const { return m_openCardRotation; }

    void setSpawnPoint(PlayerSpawnPoint _spawnPoint);

    void addSlot(ParticipantSlotId _id);
    void removeSlot(ParticipantSlotId _id);

    void setCardInSlot(ParticipantSlotId _id, Card::Rank _rank, Card::Suit _suit); 
    Card* replaceCardInSlot(ParticipantSlotId _id, Card* _card);

    const ParticipantSlot& getSlot(ParticipantSlotId _id) const;
    ParticipantSlot& getSlot(ParticipantSlotId _id);
    void visitSlots(std::function<void(ParticipantSlot&)> _visitor);

private:
    void onDraw(sf::RenderWindow& _window) override;


    sf::Text m_nameText; // TODO simpleText

    PlayerId m_id = PlayerIdInvalid;
    bool m_isLocal = false;

    std::vector<ParticipantSlot> m_slots;
    sf::Vector2f m_openCardPosition{};
    float m_openCardRotation = 0.f;
    PlayerSpawnPoint m_spawnPoint;
    unsigned short m_currentNumberOfSlots = 0;
};

} // namespace cn::client::game
