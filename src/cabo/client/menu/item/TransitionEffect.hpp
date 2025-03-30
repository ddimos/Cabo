#pragma once

#include "client/menu/Item.hpp"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics.hpp>

#include "functional"

namespace cn::client::menu::item
{

class TransitionEffect : public Item
{
public:
    using OnFinishCallback = std::function<void(void)>;

    TransitionEffect(const sf::Texture& _texture, sf::IntRect _rectangle, float _speed);

    void startTransition(Position _initPos, Position _finalPos, OnFinishCallback _onFinishCallback);

private:
    void onUpdate(sf::Time _dt) override;
    void onDraw(sf::RenderWindow& _window) override;
    
    sf::Sprite m_sprite;
    float m_speed;
    
    sf::Vector2f m_initPos;
    sf::Vector2f m_finalPos;
    sf::Vector2f m_direction;
    float m_distance = 0.f;

    OnFinishCallback m_onFinishCallback;
};

} // namespace cn::client::menu::item
