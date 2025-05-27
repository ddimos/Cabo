#include "client/state/states/TestState.hpp"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/CircleShape.hpp>

namespace cn::client::states
{

TestState::TestState(core::state::Manager& _stateManagerRef)
    : State(_stateManagerRef)
{
    {
        sf::Texture texture;
        m_loaded1 = texture.loadFromFile("res/textures/background.png");
    }
    {
        sf::Texture texture;
        m_loaded2 = texture.loadFromFile("background.png");
    }
    {
        sf::Texture texture;
        m_loaded3 = texture.loadFromFile("Resources/background.png");
    }
    {
        sf::Texture texture;
        m_loaded4 = texture.loadFromFile("Resources/res/textures/background.png");
    }
}

void TestState::onDraw()
{
    auto& windowRef = getContext().get<sf::RenderWindow>();
    {
        sf::CircleShape circle;
        circle.setPosition(100.f, 100.f);
        circle.setRadius(50.f);
        circle.setFillColor(sf::Color::White);
        windowRef.draw(circle);
    }
    if (m_loaded1)
    {
        sf::CircleShape circle;
        circle.setPosition(200.f, 100.f);
        circle.setRadius(50.f);
        circle.setFillColor(sf::Color::Red);
        windowRef.draw(circle);
    }
    if (m_loaded2)
    {
        sf::CircleShape circle;
        circle.setPosition(300.f, 100.f);
        circle.setRadius(50.f);
        circle.setFillColor(sf::Color::Blue);
        windowRef.draw(circle);
    }
    if (m_loaded3)
    {
        sf::CircleShape circle;
        circle.setPosition(400.f, 100.f);
        circle.setRadius(50.f);
        circle.setFillColor(sf::Color::Green);
        windowRef.draw(circle);
    }
    if (m_loaded4)
    {
        sf::CircleShape circle;
        circle.setPosition(500.f, 100.f);
        circle.setRadius(50.f);
        circle.setFillColor(sf::Color::Magenta);
        windowRef.draw(circle);
    }
}

} // namespace cn::client::states
