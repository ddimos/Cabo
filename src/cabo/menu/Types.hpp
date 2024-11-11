#pragma once

#include <SFML/System/Vector2.hpp>

namespace cn::menu
{

struct Position
{
    enum class Special
    {
        NORMAL,
        CENTER_ALLIGNED,
        OFFSET_FROM_CENTER,
        OFFSET_FROM_START,
        OFFSET_FROM_END
    };

    sf::Vector2f calculateGlobalPos(sf::Vector2f _size) const;

    sf::Vector2f m_position{0.f, 0.f};
    sf::Vector2f m_parentPos{0.f, 0.f};
    sf::Vector2f m_parentSize{0.f, 0.f};
    Special m_specPositionX = Special::NORMAL; 
    Special m_specPositionY = Special::NORMAL; 
};

} // namespace cn::menu
