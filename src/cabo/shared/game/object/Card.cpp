#include "shared/game/object/Card.hpp"

namespace cn::shared::game::object
{

Card::Card(Id _id)
    : Object(_id)
    , m_attachableComponent(*this)
    , m_flippableComponent(*this)
    , m_grabbableComponent(*this)
    , m_layerableComponent(*this)
    , m_privateZoneViewableComponent(*this)
{
}

} // namespace cn::shared::game::object
