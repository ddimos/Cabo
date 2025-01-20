#include "game/object/Player.hpp"
#include "game/object/Card.hpp"

#include "core/math/Math.hpp"
#include "core/Context.hpp"

#include "client/menu/item/Button.hpp"

#include "client/ResourceIds.hpp"

#include <SFML/Graphics/RenderWindow.hpp>

namespace cn::game
{

Player::Player(const core::Context& _context, PlayerId _id, bool _isLocal, std::map<PlayerSlotId, PlayerSlot>&& _slots, unsigned short _initialNumberOfSlots)
    : m_id(_id)
    , m_isLocal(_isLocal)
    , m_slots(std::move(_slots))
    , m_currentNumberOfSlots(_initialNumberOfSlots)
{
    auto& fontHolderRef = _context.get<client::FontHolder>();

    m_nameText.setFont(fontHolderRef.get(client::FontIds::Main));
    m_nameText.setString("Player " + std::to_string(m_id));
    m_nameText.setFillColor(sf::Color::White);
    m_nameText.setCharacterSize(24);
    m_nameText.setOrigin(m_nameText.getGlobalBounds().getSize() / 2.f);
}

void Player::setSpawnPoint(PlayerSpawnPoint _spawnPoint)
{
    m_spawnPoint = _spawnPoint;
    m_nameText.setPosition(_spawnPoint.pos);

    float offsetBetweenCardsX = 85;
    float offsetBetweenCardsY = 115;
    int cardsInRow = 4;
    int i = 0;
    int j = 0;
    for (auto& [id, slot] : m_slots)
    {
        sf::Vector2f localPos = sf::Vector2f(i * offsetBetweenCardsX - 165, j * offsetBetweenCardsY + 20);
        localPos = core::math::rotateVector(localPos, m_spawnPoint.angleDeg + 90.f);
        slot.m_button->setPosition(client::menu::Position{ .m_position = (m_spawnPoint.pos + localPos) });
        slot.m_button->setRotation(m_spawnPoint.angleDeg + 90.f);
        ++i;
        if(i >= cardsInRow)
        {
            i = 0;
            j++;
        }
    }
}

void Player::addSlot()
{
    if (m_currentNumberOfSlots >= m_slots.size())
    {
        CN_ASSERT(false);
        return; // PlayerSlotIdInvalid
    }

    m_currentNumberOfSlots++;
}

void Player::removeSlot(PlayerSlotId _id)
{
    if (m_currentNumberOfSlots == 0)
    {
        CN_ASSERT(false);
        return;
    }
    m_currentNumberOfSlots--;

    m_slots.at(_id).m_button->requestDeactivated();
}

void Player::showCardInSlot(PlayerSlotId _id)
{
    auto slot = getSlot(_id);
    slot.card->requestActivated();
    slot.m_button->requestDeactivated();

    // TODO think about a better position
    sf::Vector2f localPos = sf::Vector2f(50, 200);
    localPos = core::math::rotateVector(localPos, m_spawnPoint.angleDeg + 90.f);
    
    slot.card->setPosition(m_spawnPoint.pos + localPos);
}

void Player::hideCardInSlot(PlayerSlotId _id)
{
    auto slot = getSlot(_id);
    slot.card->requestDeactivated();
    slot.m_button->requestActivated();
}

const PlayerSlot& Player::getSlot(PlayerSlotId _id) const
{
    CN_ASSERT(m_slots.contains(_id));
    return m_slots.at(_id);
}

CardPtr Player::getCard(PlayerSlotId _id) const
{
    return getSlot(_id).card;
}

CardPtr Player::replace(PlayerSlotId _id, CardPtr _card)
{
    auto prevCard = m_slots.at(_id).card;
    m_slots.at(_id).card = _card;
    return prevCard;
}

void Player::deal(CardPtr _card)
{
    bool assigned = false;

    for (auto& [id, slot] : m_slots)
    {
        if (slot.card)
            continue;

        slot.card = _card;
        slot.m_button->requestActivated();
        assigned = true;
        break;
    }
    CN_ASSERT(assigned);
}

void Player::visitSlots(std::function<void(PlayerSlot& _slot)> _visitor)
{
    for (auto& [id, slot] : m_slots)
        _visitor(slot);
}

void Player::onDraw(sf::RenderWindow& _window)
{
    _window.draw(m_nameText);
}

} // namespace cn::game