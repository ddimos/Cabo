#include "shared/game/controller/Controller.hpp"
#include "shared/game/component/Layerable.hpp"
#include "shared/game/Types.hpp"

#include <tuple>
#include <unordered_map>
#include <vector>

namespace cn::shared::game::controller
{

class Layerable final
{
    struct Layer
    {
        Layer(unsigned _min, unsigned _size)
            : min(_min), max(_min + _size), current(_min)
        {
            components.reserve(_size);
        }

        void reset()
        {
            current = min;
            components.clear();
        }

        const unsigned min = 0;
        const unsigned max = 0;
        unsigned current = 0;
        std::vector<component::Layerable*> components;
    };

public:
    using LayerSize = unsigned;
    Layerable(std::vector<std::tuple<layer::Id, LayerSize>> _layers);

    void addTolayer(layer::Id _layerId, component::Layerable& _component);
    void removeFromLayer(layer::Id _layerId, component::Layerable& _component);
    void promoteInLayer(layer::Id _layerId, component::Layerable& _component);
    void resetLayer(layer::Id _layerId);

private:
    std::unordered_map<layer::Id, Layer> m_currentLayers;
};

} // namespace cn::shared::game::controller
