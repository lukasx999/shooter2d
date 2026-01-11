#pragma once

#include "mixins.h"
#include "Player.h"

class TrackingEntity {
    // [[maybe_unused]], because the compiler is confused
    [[maybe_unused]] const Player& m_target;

public:
    explicit TrackingEntity(const Player& target) : m_target(target) { }

    void track_target(this auto& entity, double dt) {
        check_entity_mixin(entity);

        auto diff = entity.m_target.get_position() - entity.get_position();
        gfx::Vec base(1.0, 0.0);
        auto angle = gfx::rad_to_deg(diff.angle(base));

        if (entity.m_target.get_position().y > entity.get_position().y)
            angle = 180 + (180 - angle);

        if (diff.length() >= 100)
            entity.walk(entity.angle_to_direction(angle), dt);

    }

private:
    [[nodiscard]] constexpr Direction angle_to_direction(int angle) const {

        if (angle >= 45 && angle <= 135) return Direction::North;
        if (angle >= 135 && angle <= 225) return Direction::West;
        if (angle >= 225 && angle <= 315) return Direction::South;
        if (angle >= 315 || angle <= 45) return Direction::East;

        throw std::runtime_error("invalid angle");
    }

};
