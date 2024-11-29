#include "game/Player.hpp"
#include "game/Card.hpp"

#include "core/Types.hpp"
#include "core/math/Math.hpp"

#include "menu/item/Button.hpp"

#include "ResourceIds.hpp"

#include <SFML/Graphics/RenderWindow.hpp>

namespace cn::game
{

Player::Player(const core::Context& _context, std::vector<PlayerSlot>&& _slots, unsigned short _initialNumberOfSlots)
    : m_slots(std::move(_slots))
    , m_currentNumberOfSlots(_initialNumberOfSlots)
{
    m_nameText.setFont(_context.fontHolderRef.get(FontIds::Main));
    m_nameText.setString("Player");
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
    for (auto& slot : m_slots)
    {
        sf::Vector2f localPos = sf::Vector2f(i * offsetBetweenCardsX - 165, j * offsetBetweenCardsY + 20);
        localPos = core::math::rotateVector(localPos, m_spawnPoint.angleDeg + 90.f);
        slot.m_button->setPosition(cn::menu::Position{ .m_position = (m_spawnPoint.pos + localPos) });
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

void Player::deal(CardPtr _card)
{
    bool assigned = false;

    for (auto& slot : m_slots)
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

void Player::onDraw(sf::RenderWindow& _window)
{
    _window.draw(m_nameText);
}

} // namespace cn::game