#pragma once

#include "shared/game/object/Object.hpp"
#include "shared/game/component/Layerable.hpp"
#include "shared/player/Types.hpp"

#include <SFML/System/Vector2.hpp>

namespace cn::shared::game::object
{

class Object;

class Participant : public Object
{
public:
    Participant(Id _id, PlayerId _playerId);

    virtual void setMousePosition(sf::Vector2f _position);
    void setObject(Object* _object);

    unsigned getZIndex() const override { return m_layerableComponent.getIndex(); }

    component::Layerable& getLayerableComponent() { return m_layerableComponent; }

private:
    PlayerId m_playerId{};
    Object* m_object = nullptr;
    component::Layerable m_layerableComponent;
};

} // namespace cn::shared::game::object
