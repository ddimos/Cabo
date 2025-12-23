#include "shared/game/controller/Layerable.hpp"

namespace cn::shared::game::controller
{

Layerable::Layerable(std::vector<std::tuple<layer::Id, LayerSize>> _layers)
{
    layer::Id prevLayer;
    unsigned prevMax = 0;
    for (auto& [id, size] : _layers)
    {
        prevMax += 1;
        auto it = m_currentLayers.emplace(id, Layer(prevMax, size));
        prevMax = it.first->second.max;
    }
}

void Layerable::addTolayer(layer::Id _layerId, component::Layerable& _component)
{
    CN_ASSERT(m_currentLayers.contains(_layerId));
    Layer& layer = m_currentLayers.at(_layerId);
    layer.components.push_back(&_component);
    _component.setIndex(layer.current);
    layer.current++;
    CN_ASSERT_FRM(layer.current <= layer.max, "The limit is reached for layer {}", _layerId.value()); 
}

void Layerable::removeFromLayer(layer::Id _layerId, component::Layerable& _component)
{
    // recalculate all indexes
    CN_ASSERT(m_currentLayers.contains(_layerId));
    Layer& layer = m_currentLayers.at(_layerId);
    unsigned compIndex = _component.getIndex();
    CN_ASSERT(compIndex >= layer.min && compIndex <= layer.max);
    unsigned vecIndex = compIndex - layer.min;
    CN_ASSERT(layer.current > 0);
    CN_ASSERT(layer.current > layer.min);
    layer.current--;
    auto it = layer.components.erase(layer.components.begin() + vecIndex);
    for (; it != layer.components.end(); ++it)
    {
        CN_ASSERT((*it)->getIndex() != 0);
        (*it)->setIndex((*it)->getIndex() - 1);
    }
}

void Layerable::promoteInLayer(layer::Id _layerId, component::Layerable& _component)
{
    CN_ASSERT(m_currentLayers.contains(_layerId));
    Layer& layer = m_currentLayers.at(_layerId);
    unsigned compIndex = _component.getIndex();
    CN_ASSERT(compIndex >= layer.min && compIndex <= layer.max);
    
    CN_ASSERT(!layer.components.empty());
    if (layer.components.size() == 1)
        return;

    for (unsigned i = compIndex - layer.min; i < layer.components.size() - 1; ++i)
    {
        unsigned j = i + 1;
        CN_ASSERT(layer.components[j]->getIndex() != 0);
        layer.components[j]->setIndex(layer.components[j]->getIndex() - 1);
        layer.components[i]->setIndex(layer.components[i]->getIndex() + 1);
        std::swap(layer.components[j], layer.components[i]);
    }
}

void Layerable::resetLayer(layer::Id _layerId)
{
    CN_ASSERT(m_currentLayers.contains(_layerId));
    m_currentLayers.at(_layerId).reset();
}

} // namespace cn::shared::game::controller
