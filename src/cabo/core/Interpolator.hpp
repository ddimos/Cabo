#pragma once

#include "core/Assert.hpp"

#include <SFML/System/Clock.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/System/Vector2.hpp>

#include <functional>

// Inspired by https://github.com/johnBuffer/Interpolated/blob/main/src/interpolated/interpolated.hpp

namespace cn::core
{

class Easing
{
public:
    static float linear(float _t);
    static float easeInOutExpo(float _t);
    static float easeOutBack(float _t);
    static float easeInBack(float _t);
};

template <typename T>
class Interpolator
{
public:
    using EasingFunc = std::function<float(float)>;

    Interpolator(sf::Clock& _clockRef, sf::Time _duration, EasingFunc _func);

    void setDuration(sf::Time _duration);

    void start(T _value);
    void start(T _start, T _end);
    T get() const;

    bool isFinished() const;

private:
    sf::Clock& m_clockRef;
    EasingFunc m_easingFunc{};
    sf::Time m_duration{sf::seconds(1.f)};

    T m_start{};
    T m_end{};

    sf::Time m_startTime{};
};

template <typename T>
Interpolator<T>::Interpolator(sf::Clock& _clockRef, sf::Time _duration, EasingFunc _func)
    : m_clockRef(_clockRef)
    , m_easingFunc(_func)
{
    setDuration(_duration);
}

template <typename T>
void Interpolator<T>::setDuration(sf::Time _duration)
{
    CN_ASSERT(_duration.asSeconds() > 0.f);
    m_duration = _duration;
}

template <typename T>
void Interpolator<T>::start(T _value)
{
    m_start = get();
    m_end = _value;
    m_startTime = m_clockRef.getElapsedTime();
}

template <typename T>
void Interpolator<T>::start(T _start, T _end)
{
    m_start = _start;
    m_end = _end;
    m_startTime = m_clockRef.getElapsedTime();
}

template <typename T>
T Interpolator<T>::get() const
{
    sf::Time time = m_clockRef.getElapsedTime() - m_startTime;
    if (time > m_duration)
        return m_end;
    
    float ratio = time / m_duration;
    CN_LOG_FRM("ratio: {}, {}", ratio , m_easingFunc(ratio));
    T delta = m_end - m_start;
    T value = m_start + delta * m_easingFunc(ratio);
    return value;
}

template <typename T>
bool Interpolator<T>::isFinished() const
{
   sf::Time time = m_clockRef.getElapsedTime() - m_startTime;
   return time > m_duration;
}

} // namespace cn::core
