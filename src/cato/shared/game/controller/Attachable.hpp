#pragma once
#include "shared/game/controller/Controller.hpp"
#include "shared/game/component/Attachable.hpp"

namespace cn::shared::game::controller
{

class Attachable : public Controller<component::Attachable>
{
public:
    Attachable() = default;

};

} // namespace cn::shared::game::controller
