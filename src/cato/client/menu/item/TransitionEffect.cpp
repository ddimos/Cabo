#include "client/menu/item/TransitionEffect.hpp"

#include "core/math/Math.hpp"

namespace cn::client::menu::item
{

TransitionEffect::TransitionEffect(const sf::Texture& _texture, sf::IntRect _rectangle, float _speed)
    : m_speed(_speed)
{
    m_sprite.setTexture(_texture);
    m_sprite.setTextureRect(_rectangle);
    m_sprite.setOrigin(m_sprite.getLocalBounds().getSize() / 2.f);

    // TODO to think how to use m_transformable here
}

void TransitionEffect::startTransition(Position _initPos, Position _finalPos, OnFinishCallback _onFinishCallback)
{
    m_initPos = _initPos.calculateGlobalPos(sf::Vector2f{});
    m_finalPos = _finalPos.calculateGlobalPos(sf::Vector2f{});

    m_sprite.setPosition(m_initPos);

    m_direction = m_finalPos - m_initPos;
    m_distance = core::math::vectorLength(m_direction);
    m_direction = core::math::normalizeVector(m_direction);

    m_onFinishCallback = _onFinishCallback;

    requestActivated();
}

void TransitionEffect::onUpdate(sf::Time _dt)
{
    m_sprite.move(m_speed * m_direction * _dt.asSeconds());

    if (core::math::vectorLength(m_sprite.getPosition() - m_initPos) >= m_distance)
    {
        m_sprite.setPosition(m_finalPos);

        m_onFinishCallback();

        requestDeactivated();
    }
}

void TransitionEffect::onDraw(sf::RenderWindow& _window)
{
    _window.draw(m_sprite);
}

} // namespace cn::client::menu::item
