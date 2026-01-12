#pragma once

#include <gfx.h>

#include "PhysicalObject.h"
#include "Player.h"

class HealthPickup : public PhysicalObject {
    static constexpr float m_radius = 50.0f;
    const int m_heal_amount;
    bool m_is_consumed = false;

    using PickupHandler = std::function<void()>;
    PickupHandler m_pickup_handler = [] { };

public:
    HealthPickup(gfx::Vec position, int heal_amount)
        : PhysicalObject(position, 100, 100)
        , m_heal_amount(heal_amount)
    { }

    void draw(gfx::Renderer& rd) const override {
        PhysicalObject::draw(rd);
        rd.draw_circle(m_position, m_radius, gfx::Color::green());
    }

    void update([[maybe_unused]] double dt) override { }

    void on_pickup(PickupHandler pickup_handler) {
        m_pickup_handler = pickup_handler;
    }

    [[nodiscard]] bool is_consumed() const {
        return m_is_consumed;
    }

    void check_collision_player(Player& player) {
        // TODO: use circle collision
        gfx::Rect rect {
            m_position.x - m_radius,
            m_position.y - m_radius,
            m_radius*2,
            m_radius*2,
        };

        if (player.get_hitbox().check_collision_rect(rect)) {
            m_is_consumed = true;
            player.set_health(player.get_health() + m_heal_amount);
            m_pickup_handler();
        }
    }

};
