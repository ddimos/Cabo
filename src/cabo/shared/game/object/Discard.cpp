#include "shared/game/object/Discard.hpp"

namespace cn::shared::game::object
{

Discard::Discard(Id _id) 
    : Object(_id)
{}

void Discard::discard(Card* _card)
{
}

Card* Discard::getLast()
{
    return nullptr;
}

} // namespace cn::shared::game::object
