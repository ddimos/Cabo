#pragma once

#include "shared/board/Object.hpp"
#include <vector>

namespace cn::shared::board
{

class Card;

class Discard : public Object
{
public:
    Discard(ObjectId _id) : Object(_id) {}

    void discard(Card* _card);
    Card* getLast();
    
private:
    std::vector<Card*> m_cards;
};

} // namespace cn::shared::board
