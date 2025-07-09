#pragma once

#include "shared/game/Participant.hpp"

namespace cn::client::game
{

class Participant final : public shared::game::Participant
{
public:
    Participant(shared::board::Participant& _boardParticipant)
        : shared::game::Participant(_boardParticipant) {}

private:
};

} // namespace cn::client::game
