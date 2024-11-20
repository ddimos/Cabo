#include "game/Discard.hpp"
#include "menu/item/Button.hpp"

namespace cn::game
{

Discard::Discard(menu::item::ButtonPtr _button)
    : m_button(_button)
{
    m_button->setPosition(cn::menu::Position{ .m_position = sf::Vector2f(650, 400) });
}

} // namespace cn::game
