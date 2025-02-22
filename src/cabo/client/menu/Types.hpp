#pragma once

#include <SFML/System/Vector2.hpp>

#include <memory>

namespace cn::client::menu
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

    // For elements with the origin at the center
    sf::Vector2f calculateGlobalPos() const;
    // For elements with the origin at the top left corner pass a normal size
    sf::Vector2f calculateGlobalPos(sf::Vector2f _size) const;

    sf::Vector2f m_position{0.f, 0.f};
    sf::Vector2f m_parentPos{0.f, 0.f};
    sf::Vector2f m_parentSize{0.f, 0.f};
    Special m_specPositionX = Special::NORMAL; 
    Special m_specPositionY = Special::NORMAL; 
};

namespace item
{
    class Button;
    using ButtonPtr = std::shared_ptr<Button>;

    class SimpleImage;
    using SimpleImagePtr = std::shared_ptr<SimpleImage>;
} // namespace item

} // namespace cn::client::menu
