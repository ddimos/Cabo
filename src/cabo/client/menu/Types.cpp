#include "client/menu/Types.hpp"
#include "client/menu/Utils.hpp"
#include "core/Assert.hpp"

#include <cmath>

namespace
{

float calculateSpecialPosition(cn::client::menu::Position::Special _special, float _position, float _itemSize, float _parentPosition, float _parentSize)
{
    if (_special == cn::client::menu::Position::Special::NORMAL)
        return _position + _parentPosition;
    
    if (_special == cn::client::menu::Position::Special::OFFSET_FROM_START)
        return _position + _parentPosition;

    if (_special == cn::client::menu::Position::Special::OFFSET_FROM_END)
        return (_parentPosition + _parentSize) - _position;

    if (_special == cn::client::menu::Position::Special::CENTER_ALLIGNED)
        return cn::client::menu::calculateCenter(_parentSize, _itemSize) + _parentPosition;

    if (_special == cn::client::menu::Position::Special::OFFSET_FROM_CENTER)
        return _parentSize / 2.f + _position + _parentPosition;
    
    CN_ASSERT(false);
    return 0.f;
}

sf::Vector2f calculateSpecialPosition(cn::client::menu::Position _position, sf::Vector2f _size, sf::Vector2f _parentPosition, sf::Vector2f _parentSize)
{
    return sf::Vector2f{
        roundf(
            calculateSpecialPosition(
                _position.m_specPositionX, _position.m_position.x, _size.x, _parentPosition.x, _parentSize.x
        )),
        roundf(
            calculateSpecialPosition(
                _position.m_specPositionY, _position.m_position.y, _size.y, _parentPosition.y, _parentSize.y
        ))
    };
}
} // namespace


namespace cn::client::menu
{

sf::Vector2f Position::calculateGlobalPos() const
{
    return calculateSpecialPosition(*this, sf::Vector2f{0.f, 0.f}, m_parentPos, m_parentSize);
}

sf::Vector2f Position::calculateGlobalPos(sf::Vector2f _size) const
{
    return calculateSpecialPosition(*this, _size, m_parentPos, m_parentSize);
}

} // namespace cn::client::menu
