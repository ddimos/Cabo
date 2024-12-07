#include "game/object/Table.hpp"
#include "game/object/Deck.hpp"
#include "game/object/Discard.hpp"
#include "game/object/Player.hpp"
#include "game/Constants.hpp"

#include "core/math/Math.hpp"
#include "core/Assert.hpp"
#include "core/Log.hpp"
#include "core/Types.hpp"

#include "menu/item/Button.hpp"
#include "menu/Utils.hpp"

#include "ResourceIds.hpp"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics.hpp>

namespace
{

unsigned g_numberOfPoints = 10;

// https://stackoverflow.com/a/20510150
//
// dp(t) = sqrt( (r1*sin(t))^2 + (r2*cos(t))^2)
// circ = sum(dp(t), t=0..2*Pi step 0.0001)

// n = 20
// nextPoint = 0
// run = 0.0
// for t=0..2*Pi step 0.0001
//     if n*run/circ >= nextPoint then
//         set point (r1*cos(t), r2*sin(t))
//         nextPoint = nextPoint + 1
//     next
//     run = run + dp(t)
// next

std::vector<cn::game::PlayerSpawnPoint> generateSpawnPoints(float _r1, float _r2, unsigned _numberOfPoints, sf::Vector2f _positionOffset)
{
    if (_numberOfPoints == 0)
        return {};

    std::vector<cn::game::PlayerSpawnPoint> points;
    points.reserve(_numberOfPoints);

    const float twoPi = std::numbers::pi * 2.0;
    const float deltaTheta = 0.0001;
    const unsigned numIntegrals = static_cast<unsigned>(std::round(twoPi / deltaTheta));
    float theta = 0.0;
    float circ = 0.f;
    float dpt = 0.f;

    auto computeDpt = [](float _r1, float _r2, float _theta)
    {
        float dpt_sin = std::pow(_r1 * std::sin(_theta), 2);
        float dpt_cos = std::pow(_r2 * std::cos(_theta), 2);
        float dp = std::sqrt(dpt_sin + dpt_cos);
        return dp;
    };

    // integrate over the elipse to get the circumference
    for(unsigned i = 0; i < numIntegrals; i++)
    {
        theta += i * deltaTheta;
        dpt = computeDpt(_r1, _r2, theta);
        circ += dpt;
    }

    unsigned nextPoint = 0;
    float run = 0.0;
    // Start with an offset
    theta = std::numbers::pi / 2;

    for(unsigned i = 0; i < numIntegrals; i++)
    {
        theta += deltaTheta;
        float subIntegral = (_numberOfPoints * run) / circ;
        if(subIntegral >= nextPoint)
        {
            float x = _r1 * std::cos(theta);
            float y = _r2 * std::sin(theta);
            points.push_back({
                .pos = sf::Vector2f(x, y) + _positionOffset,
                .angleDeg = cn::core::math::toDeg(theta)
            });
            nextPoint++;
        }
        run += computeDpt(_r1, _r2, theta);
    }
    return points;
}

} // namespace


namespace cn::game
{

Table::Table(const core::Context& _context, DeckPtr _deck, DiscardPtr _discard,
        DecideButtons&& _decideButtons)
    : m_contextRef(_context)
    , m_deck(_deck)
    , m_discard(_discard)
    , m_decideButtons(std::move(_decideButtons))
{
    m_sprite.setTexture(m_contextRef.textureHolderRef.get(TextureIds::Table));
    m_sprite.setPosition(menu::calculateCenter(sf::Vector2f(m_contextRef.windowRef.getSize()), m_sprite.getGlobalBounds().getSize()));

    unsigned index = 0;
    for (auto& button : m_decideButtons)
    {
        button->setPosition(cn::menu::Position{ .m_position = sf::Vector2f(780, 510 + index * 22) });
        button->setClickCallback([this, index](bool _isPressed){
            onLocalPlayerClickDecideButton(index);
        });
        ++index;
    }
}

void Table::onUpdate(sf::Time _dt)
{
    if (m_playerTurn == PlayerTurn::End)
    {
        m_playerTurn = PlayerTurn::DrawCard;
    }
}

void Table::onDraw(sf::RenderWindow& _window)
{
    _window.draw(m_sprite);

    for (auto pos : m_spawnPoints)
    {
        sf::RectangleShape shape(sf::Vector2f{40.f, 60.f});
        shape.setFillColor(sf::Color::Red);
        shape.setOrigin(20, 30);
        shape.setPosition(pos.pos);
        shape.rotate(pos.angleDeg + 90.f);
        _window.draw(shape);
    }
}

void Table::addPlayer(PlayerPtr _player)
{
    m_players.push_back(_player);

    m_spawnPoints = generateSpawnPoints(
        m_sprite.getGlobalBounds().getSize().x / 2.f, m_sprite.getGlobalBounds().getSize().y / 2.f,
        m_players.size(), sf::Vector2f(m_contextRef.windowRef.getSize()) / 2.f
    );

    int i = 0;
    for (auto& player : m_players)
    {
        player->setSpawnPoint(m_spawnPoints.at(i));
        ++i;
    }
}

void Table::start()
{
    CN_ASSERT(!hasGameStarted());

    for (auto& player : m_players)
    {
        unsigned short numberOfCards = DefaultInitialNumberOfCards;
        while (numberOfCards)
        {
            // TODO if not empty
            player->deal(m_deck->getNextCard());
            --numberOfCards;
        }
    }

    m_playingState = State::LookingCard;
}

bool Table::hasGameStarted() const
{
    return m_playingState != State::None;
}

void Table::onLocalPlayerClickDeck()
{
    if (!hasGameStarted())
        return;
    
    if (m_playerTurn != PlayerTurn::DrawCard)
        return;
    m_playerTurn = PlayerTurn::DecideAction;

    // TODO if not empty
    auto card = m_deck->getNextCard();
    card->setPosition(sf::Vector2f(850, 500));
    card->requestActivated();

    m_currentCard = card;

    for (auto& button : m_decideButtons)
        button->requestActivated();
}

void Table::onLocalPlayerClickDiscard()
{

}

void Table::onLocalPlayerClickDecideButton(unsigned _butonIndex)
{
    if (m_playerTurn != PlayerTurn::DecideAction)
    {
        CN_ASSERT(false);
        return;
    }
    for (auto& button : m_decideButtons)
        button->requestDeactivated();

    switch (_butonIndex)
    {
    case 0: // Match
        m_playerTurn = PlayerTurn::Match;
        // Highlight player cards
        break;
    case 1: // Take
        m_playerTurn = PlayerTurn::Take;
        // Highlight player cards
        break;
    case 2: // Action
        if (!Card::hasAbility(*m_currentCard))
            return;
        m_playerTurn = PlayerTurn::CardAction;
        break;
    case 3: // Discard
        m_discard->discard(m_currentCard);
        m_currentCard.reset();
        m_playerTurn = PlayerTurn::End;
        break;
    default:
        CN_ASSERT(false);
        break;
    }

}

void Table::onLocalPlayerClickSlot(PlayerSlotId _slotId, Player& _player)
{
    if (m_playerTurn == PlayerTurn::CardAction)
    {
        // TODO
    }
    else if (m_playerTurn == PlayerTurn::Match)
    {
        if (!_player.isLocal())
            return;

        auto card = _player.get(_slotId);
        if (card->getRank() == m_currentCard->getRank())
        {
            m_discard->discard(m_currentCard);
            card->requestActivated();
            m_discard->discard(card);
            m_currentCard.reset();
            _player.removeSlot(_slotId);

        }
        else
        {
            card->requestActivated();
            _player.addSlot();
            _player.deal(m_deck->getNextCard());
        }
        m_playerTurn = PlayerTurn::End;
    } 
    else if (m_playerTurn == PlayerTurn::Take)
    {
        if (!_player.isLocal())
            return;
        
        auto prevCard = _player.replace(_slotId, m_currentCard);
        m_currentCard->requestDeactivated();
        m_currentCard.reset();
        prevCard->requestActivated();
        m_discard->discard(prevCard);
        m_playerTurn = PlayerTurn::End;
    }
    else
    {
        CN_ASSERT(false);
    }
}

} // namespace cn::game