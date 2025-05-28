#include "client/state/states/TestState.hpp"

#include "core/Path.hpp"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/CircleShape.hpp>

#include <filesystem>
#include <fstream>
#include <iostream>

namespace cn::client::states
{

TestState::TestState(core::state::Manager& _stateManagerRef)
    : State(_stateManagerRef)
{
    auto path = core::getExecutablePath();
    std::cout << "Current path is " << std::filesystem::current_path() << '\n';
    std::cout << "Current path is " << path.parent_path().parent_path() << '\n';
    {
        sf::Texture texture;
        m_loaded1 = texture.loadFromFile("Contents/Resources/background.png");
    }
    {
        sf::Texture texture;
        m_loaded2 = texture.loadFromFile("../background.png");
    }
    {
        sf::Texture texture;
        m_loaded3 = texture.loadFromFile("../Resources/background.png");
    }
    {
        sf::Texture texture;
        m_loaded4 = texture.loadFromFile(std::string(std::filesystem::current_path().c_str())+"/Resources/background.png");
    }
    {
        sf::Texture texture;
        m_loaded5 = texture.loadFromFile(std::string(path.parent_path().parent_path().c_str())+"/Resources/background.png");
    }
    {
        sf::Texture texture;
        m_loaded6 = texture.loadFromFile(std::string(path.parent_path().c_str())+"/../Resources/background.png");
    }
    {
        // std::ofstream file("testtt.txt");
        // if (file.is_open())
        // {
        //     file << std::filesystem::current_path();
        // }
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
    if (m_loaded5)
    {
        sf::CircleShape circle;
        circle.setPosition(600.f, 100.f);
        circle.setRadius(50.f);
        circle.setFillColor(sf::Color::Yellow);
        windowRef.draw(circle);
    }
    if (m_loaded6)
    {
        sf::CircleShape circle;
        circle.setPosition(700.f, 100.f);
        circle.setRadius(50.f);
        circle.setFillColor(sf::Color::Cyan);
        windowRef.draw(circle);
    }
}

} // namespace cn::client::states
