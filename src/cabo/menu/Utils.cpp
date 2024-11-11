#include "menu/Utils.hpp"

namespace cn::menu
{

float calculateCenter(float _parentSize, float _itemSize)
{
    return _parentSize / 2.f - _itemSize / 2.f;
}

} // namespace cn::menu