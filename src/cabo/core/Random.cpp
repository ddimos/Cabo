#include "core/Random.hpp"

#include <numeric>

namespace cn::core
{

void Random::init(unsigned _seed)
{
    m_generator.seed(_seed);
}

int Random::rand(int _min, int _max)
{
    double requiredRange = _max - _min;
    double generatorRange = m_generator.max() - m_generator.min();
    double scale = requiredRange / (generatorRange + 1.0);
    return (m_generator() - m_generator.min()) * scale + _min;
}

std::vector<unsigned> Random::generateSequenceNoRepeats(unsigned _size)
{
    std::vector<unsigned> arr;
    arr.resize(_size);
    std::iota(arr.begin(), arr.end(), 0);
    for (int i = _size - 1; i > 0; --i)
    {
        int j = rand(0, i + 1);
        std::swap(arr[j], arr[i]);
    }
    return arr;
}

} // namespace cn::core
