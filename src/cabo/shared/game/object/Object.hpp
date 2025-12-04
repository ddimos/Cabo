#pragma once
#include "core/object/Object.hpp"
#include "core/Identifier.hpp"

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Rect.hpp>

namespace cn::shared::game::object
{

class Object;
using Id = core::Identifier<Object, uint8_t>;

class Object : public core::object::Object
{
public:
    Object(Id _id);

    Id getId() const { return m_id; }

    sf::Vector2f getPosition() const { return m_bounds.getPosition(); }
    void setPosition(sf::Vector2f _position) { m_bounds = sf::FloatRect(_position, m_bounds.getSize()); }

    sf::Vector2f getSize() const { return m_bounds.getSize(); }
    void setSize(sf::Vector2f _size) { m_bounds = sf::FloatRect(m_bounds.getPosition(), _size); }

    float getRotation() const { return m_rotation; }
    void setRotation(float _rotation) { m_rotation = _rotation; }

    virtual unsigned getZIndex() const { return 0; }

    bool contains(sf::Vector2f _pos) const;

private:
    Id m_id{};
    sf::FloatRect m_bounds{};
    float m_rotation{0};
};

} // cn::shared::game::object
