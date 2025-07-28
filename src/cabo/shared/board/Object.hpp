#pragma once

#include "shared/board/Types.hpp"
#include "shared/player/Types.hpp"
#include "core/Identifier.hpp"

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Rect.hpp>

namespace cn::shared::board
{

class Object;
using ObjectId = core::Identifier<Object, uint8_t>;

class Object
{
public:
    Object(ObjectId _id);

    ObjectId getId() const { return m_id; }

    sf::Vector2f getPosition() const { return m_bounds.getPosition(); }
    void setPosition(sf::Vector2f _position) { m_bounds = sf::FloatRect(_position, m_bounds.getSize()); }

    sf::Vector2f getSize() const { return m_bounds.getSize(); }
    void setSize(sf::Vector2f _size) { m_bounds = sf::FloatRect(m_bounds.getPosition(), _size); }

    float getRotation() const { return m_rotation; }
    void setRotation(float _rotation) { m_rotation = _rotation; }

    unsigned getZIndex() const { return m_zIndex; }
    void setZIndex(unsigned _zIndex) { m_zIndex = _zIndex; }

    bool contains(sf::Vector2f _pos) const;

private:
    ObjectId m_id{};
    sf::FloatRect m_bounds{};
    float m_rotation{0};
    unsigned m_zIndex = 0;
};

} // namespace cn::shared::board
