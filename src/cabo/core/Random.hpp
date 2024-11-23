#pragma once
#include <random>

#include <vector>

namespace cn::core
{

class Random
{
public:
    Random() = default;
    ~Random() = default;

    void init(unsigned _seed);
    int rand(int _min, int _max);    // [_min, _max) _max is not included
    std::vector<unsigned> generateSequenceNoRepeats(unsigned _size); // [ 0 .. _size - 1 ]

private:
    std::mt19937 m_generator;
};

} // namespace cn::core
