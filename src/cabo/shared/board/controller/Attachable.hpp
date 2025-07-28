#pragma once
#include "shared/board/controller/Controller.hpp"
#include "shared/board/component/Attachable.hpp"

namespace cn::shared::board::controller
{

class Attachable : public Controller<component::Attachable>
{
public:
    Attachable() = default;

};

} // namespace cn::shared::board::controller
