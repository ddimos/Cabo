#pragma once

#include "shared/game/object/Discard.hpp"

namespace cn::client::game
{

class Discard final : public shared::game::object::Discard
{
public:
    Discard(shared::game::object::Id& _id)
        : shared::game::object::Discard(_id) {}

private:
};

} // namespace cn::client::game
