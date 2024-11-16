#include "game/Table.hpp"
#include "game/Constants.hpp"

#include "core/math/Math.hpp"
#include "core/Log.hpp"
#include "core/Types.hpp"

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

std::vector<cn::game::Table::PlayerSpawnPoint> generateSpawnPoints(float _r1, float _r2, unsigned _numberOfPoints)
{
    if (_numberOfPoints == 0)
        return {};

    std::vector<cn::game::Table::PlayerSpawnPoint> points;
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
                .pos = sf::Vector2f(x, y),
                .angle = cn::core::math::toDeg(theta)
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

Table::Table(const core::Context& _context, DeckPtr _deck, DiscardPtr _discard, unsigned _seed)
    : m_deck(_deck)
    , m_discard(_discard)
{
    m_sprite.setTexture(_context.textureHolderRef.get(TextureIds::Table));
    m_sprite.setScale(3.f, 1.8f);
    m_sprite.setPosition(menu::calculateCenter(sf::Vector2f(_context.windowRef.getSize()), m_sprite.getGlobalBounds().getSize()));

    m_spawnPoints = generateSpawnPoints(m_sprite.getGlobalBounds().getSize().x / 2.f, m_sprite.getGlobalBounds().getSize().y / 2.f, g_numberOfPoints);
}

void Table::onHandleEvent(const sf::Event& _event)
{
    if (_event.type == sf::Event::KeyPressed)
    {
        if (_event.key.code == sf::Keyboard::Q)
        {
            if (g_numberOfPoints > 0)
            {
                g_numberOfPoints--;
                m_spawnPoints = generateSpawnPoints(m_sprite.getGlobalBounds().getSize().x / 2.f, m_sprite.getGlobalBounds().getSize().y / 2.f, g_numberOfPoints);
            }
        }
        if (_event.key.code == sf::Keyboard::E)
        {
            g_numberOfPoints++;
            m_spawnPoints = generateSpawnPoints(m_sprite.getGlobalBounds().getSize().x / 2.f, m_sprite.getGlobalBounds().getSize().y / 2.f, g_numberOfPoints);
        }
    }
}

void Table::onUpdate(sf::Time _dt)
{
}

void Table::onDraw(sf::RenderWindow& _window)
{
    _window.draw(m_sprite);

    for (auto pos : m_spawnPoints)
    {
        sf::RectangleShape shape(sf::Vector2f{40.f, 60.f});
        shape.setFillColor(sf::Color::Red);
        shape.setOrigin(20, 30);
        shape.setPosition(pos.pos + sf::Vector2f(_window.getSize()) / 2.f);
        shape.rotate(pos.angle + 90.f);
        _window.draw(shape);
    }
}

void Table::addPlayer(PlayerPtr _player)
{
    m_players.push_back(_player);
}

} // namespace cn::game