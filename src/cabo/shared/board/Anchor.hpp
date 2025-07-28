#pragma once

#include "shared/board/Object.hpp"

namespace cn::shared::board
{

class Anchor : public Object
{
public:
    Anchor(ObjectId _id, int _radius);

    int getRadius() const { return m_radius; }

private:
    int m_radius = 0;
};

} // namespace cn::shared::board
