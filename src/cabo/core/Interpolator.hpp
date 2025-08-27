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

    Interpolator(sf::Time _duration, EasingFunc _func);

    void setDuration(sf::Time _duration);

    void start(T _value);
    void start(T _start, T _end);
    T get();

    bool doesInterpolate() const;

private:
    enum class State
    {
        None,
        Interpolates,
        Finished
    };

    sf::Clock m_clock;
    EasingFunc m_easingFunc{};
    sf::Time m_duration{};

    T m_start{};
    T m_end{};

    State m_state = State::None;
};

template <typename T>
Interpolator<T>::Interpolator(sf::Time _duration, EasingFunc _func)
    : m_easingFunc(_func)
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
    m_clock.restart();
    m_state = State::Interpolates;
}

template <typename T>
void Interpolator<T>::start(T _start, T _end)
{
    m_start = _start;
    m_end = _end;
    m_clock.restart();
    m_state = State::Interpolates;
}

template <typename T>
T Interpolator<T>::get()
{
    sf::Time time = m_clock.getElapsedTime();
    if (time > m_duration)
    {
        m_state = State::Finished;
        return m_end;
    }
    
    float ratio = time / m_duration;
    T delta = m_end - m_start;
    T value = m_start + delta * m_easingFunc(ratio);
    return value;
}

template <typename T>
bool Interpolator<T>::doesInterpolate() const
{
   return m_state == State::Interpolates;
}

} // namespace cn::core
