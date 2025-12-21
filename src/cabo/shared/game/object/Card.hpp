#pragma once

#include "shared/game/object/Object.hpp"
#include "shared/game/component/Attachable.hpp"
#include "shared/game/component/Flippable.hpp"
#include "shared/game/component/Grabbable.hpp"
#include "shared/game/component/Layerable.hpp"
#include "shared/game/component/PrivateZoneViewable.hpp"

#include "core/Identifier.hpp"

namespace cn::shared::game::object
{

class Card : public Object
{
public:
    using Value = core::Identifier<Card, uint8_t>;

    Card(Id _id);

    Value getValue() const { return m_value; }
    void setValue(Value _value) { m_value = _value; }

    unsigned getZIndex() const override { return m_layerableComponent.getIndex(); }

    void move(sf::Vector2f _newPos);
    bool isInDeck() const { return m_isInDeck; }
    void addToDeck();
    void removeFromDeck();
    bool isInDiscard() const { return m_isInDiscard; }
    void addToDiscard();
    void removeFromDiscard();

    component::Attachable& getAttachableComponent() { return m_attachableComponent; }
    component::Flippable& getFlippableComponent() { return m_flippableComponent; }
    component::Grabbable& getGrabbableComponent() { return m_grabbableComponent; }
    component::Layerable& getLayerableComponent() { return m_layerableComponent; }
    component::PrivateZoneViewable& getPrivateZoneViewableComponent() { return m_privateZoneViewableComponent; }

private:
    virtual void onMoved(sf::Vector2f _newPos);

    Value m_value{};
    bool m_isInDeck = false;
    bool m_isInDiscard = false;
    component::Attachable m_attachableComponent;
    component::Flippable m_flippableComponent;
    component::Grabbable m_grabbableComponent;
    component::Layerable m_layerableComponent;
    component::PrivateZoneViewable m_privateZoneViewableComponent;
};

} // namespace cn::shared::game::object
