#pragma once

#include "shared/board/Object.hpp"
#include "shared/board/component/Attachable.hpp"
#include "shared/board/component/Flippable.hpp"
#include "shared/board/component/Grabbable.hpp"
#include "shared/board/component/PrivateZoneViewable.hpp"

#include "core/Identifier.hpp"

namespace cn::shared::board
{

class Card : public Object
{
public:
    using Value = core::Identifier<Card, uint8_t>;

    Card(ObjectId _id);

    Value getValue() const { return m_value; }
    void setValue(Value _value) { m_value = _value; }

    component::Attachable& getAttachableComponent() { return m_attachableComponent; }
    component::Flippable& getFlippableComponent() { return m_flippableComponent; }
    component::Grabbable& getGrabbableComponent() { return m_grabbableComponent; }
    component::PrivateZoneViewable& getPrivateZoneViewableComponent() { return m_privateZoneViewableComponent; }

private:
    Value m_value{};
    component::Attachable m_attachableComponent;
    component::Flippable m_flippableComponent;
    component::Grabbable m_grabbableComponent;
    component::PrivateZoneViewable m_privateZoneViewableComponent;
};

} // namespace cn::shared::board
