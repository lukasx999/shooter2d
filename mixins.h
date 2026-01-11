#pragma once

#include "Entity.h"

inline consteval void check_entity_mixin(const auto& deduced_this) {
    using This = std::remove_reference_t<decltype(deduced_this)>;
    static_assert(std::derived_from<This, Entity>, "this mixin may only be applied to classes derived from Entity");
}
