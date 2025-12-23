#pragma once
#include <SFML/Graphics/Rect.hpp>

namespace cn::client::menu::component
{

class MouseInteractable 
{
public:
    void setBounds(sf::FloatRect _rect);
    void setRotation(float _rotation);

    bool contains(sf::Vector2f _mousePos) const;
    
private:
    sf::FloatRect m_bounds{};
    float m_rotation{0};
};

} // namespace cn::client::menu::component
