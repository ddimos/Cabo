#include "ContextObjectIds.hpp"

namespace cn
{
    
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
template <>
core::ContextObjectId core::getContextObjectId<net::Manager>()
{
    return context::id::NetManager;
}

} // namespace cn
