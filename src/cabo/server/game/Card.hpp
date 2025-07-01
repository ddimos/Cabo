#pragma once

#include "core/object/Object.hpp"
#include "shared/board/Card.hpp"

namespace cn::server::game
{

class Card final : public core::object::Object
{
public:
    Card(shared::board::Card& _boardCard) : m_boardCardRef(_boardCard) {}

    shared::board::Card& m_boardCardRef;
};

} // namespace cn::server::game
