#pragma once
#include "core/Context.hpp"
#include "core/event/Dispatcher.hpp"
#include "ResourceIds.hpp"

#include <SFML/Graphics/RenderWindow.hpp>

namespace cn
{
namespace context::id
{

enum : core::ContextObjectId
{
    EventDispatcher,
    FontHolder,
    TextureHolder,
    Window
};

} // namespace context::id

template <>
core::ContextObjectId core::getContextObjectId<sf::RenderWindow>()
{
    return context::id::Window;
}
template <>
core::ContextObjectId core::getContextObjectId<TextureHolder>()
{
    return context::id::TextureHolder;
}
template <>
core::ContextObjectId core::getContextObjectId<FontHolder>()
{
    return context::id::FontHolder;
}
template <>
core::ContextObjectId core::getContextObjectId<core::event::Dispatcher>()
{
    return context::id::EventDispatcher;
}

} // namespace cn
