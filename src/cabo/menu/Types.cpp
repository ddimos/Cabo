#include "menu/Types.hpp"

namespace cn::menu
{

Position::Position() = default;

Position::Position(sf::Vector2f _position)
    : m_position{_position}
{
}

Position::Position(Special _specPositionX, Special _specPositionY)
    : m_specPositionX{_specPositionX}
    , m_specPositionY{_specPositionY}
{
}

Position::Position(Special _specPositionX, Special _specPositionY, sf::Vector2f _position)
    : m_position{_position}
    , m_specPositionX{_specPositionX}
    , m_specPositionY{_specPositionY}
{
}

} // namespace cn::menu
