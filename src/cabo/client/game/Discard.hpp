#pragma once

#include "shared/game/Discard.hpp"

namespace cn::client::game
{

class Discard final : public shared::game::Discard
{
public:
    Discard(shared::board::Discard& _boardDiscard)
        : shared::game::Discard(_boardDiscard) {}

private:
};

} // namespace cn::client::game
