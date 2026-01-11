#pragma once

#include <gfx.h>

#include "Entity.h"

inline consteval void check_entity_mixin(const auto& deduced_this) {
    using This = std::remove_reference_t<decltype(deduced_this)>;
    static_assert(std::derived_from<This, Entity>, "this mixin may only be applied to classes derived from Entity");
}

// this mixin should be a friend of the derived class, so it can access all of its
// private/protected members
class ControllableEntityMixin {
public:
    void handle_input(this auto& entity, double dt) {
        check_entity_mixin(entity);

        using enum Direction;

        bool up    = entity.m_window.get_key_state(gfx::Key::W).pressed();
        bool right = entity.m_window.get_key_state(gfx::Key::D).pressed();
        bool down  = entity.m_window.get_key_state(gfx::Key::S).pressed();
        bool left  = entity.m_window.get_key_state(gfx::Key::A).pressed();

        if (up && right) {
            entity.walk(NorthEast, dt);
            return;
        }

        if (up && left) {
            entity.walk(NorthWest, dt);
            return;
        }

        if (down && right) {
            entity.walk(SouthEast, dt);
            return;
        }

        if (down && left) {
            entity.walk(SouthWest, dt);
            return;
        }

        if (up) {
            entity.walk(North, dt);
            return;
        }

        if (down) {
            entity.walk(South, dt);
            return;
        }

        if (right) {
            entity.walk(East, dt);
            return;
        }

        if (left) {
            entity.walk(West, dt);
            return;
        }

    }

};


