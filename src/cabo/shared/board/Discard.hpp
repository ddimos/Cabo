#pragma once

#include <vector>

namespace cn::shared::board
{

class Card;

class Discard final
{
public:
    Discard()=default;

    void discard(Card* _card);
    Card* getLast();
    
private:
    std::vector<Card*> m_cards;
};

} // namespace cn::shared::board
