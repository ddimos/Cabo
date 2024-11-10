#include "menu/Utils.hpp"
#include <cmath>

#include "core/Assert.hpp"

namespace cn::menu
{

float calculateCenter(float _parentSize, float _itemSize)
{
    return _parentSize / 2.f - _itemSize / 2.f;
}

float calculateSpecialPosition(Position::Special _special, float _position, float _itemSize, float _parentPosition, float _parentSize)
{
    if (_special == Position::Special::NORMAL)
        return _position + _parentPosition;
    
    if (_special == Position::Special::OFFSET_FROM_START)
        return _position + _parentPosition;

    if (_special == Position::Special::OFFSET_FROM_END)
        return (_parentPosition + _parentSize) - _position;

    if (_special == Position::Special::CENTER_ALLIGNED)
        return calculateCenter(_parentSize, _itemSize) + _parentPosition;

    if (_special == Position::Special::OFFSET_FROM_CENTER)
        return _parentSize / 2.f + _position + _parentPosition;
    
    CN_ASSERT(false);
    return 0.f;
}

sf::Vector2f calculateSpecialPosition(Position _position, sf::Vector2f _size, sf::FloatRect _parentRect)
{
    return sf::Vector2f{
        roundf(
            calculateSpecialPosition(
                _position.m_specPositionX, _position.m_position.x, _size.x, _parentRect.getPosition().x, _parentRect.getSize().x
        )),
        roundf(
            calculateSpecialPosition(
                _position.m_specPositionY, _position.m_position.y, _size.y, _parentRect.getPosition().y, _parentRect.getSize().y
        ))
    };
}

sf::Vector2f calculateSpecialPosition(Position _position, sf::Vector2f _size, sf::Vector2u _windowSize)
{
    return calculateSpecialPosition(_position, _size, sf::FloatRect{ sf::Vector2f{ 0.f, 0.f }, sf::Vector2f{ _windowSize } });
}

} // namespace cn::menu