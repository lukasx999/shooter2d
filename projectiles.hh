#pragma once

#include <vector>

#include <gfx.hh>

class Projectile {
    gfx::Vec m_position;
    const gfx::Vec m_direction;
    const float m_travel_speed;

public:
    Projectile(gfx::Vec position, gfx::Vec direction, float travel_speed)
        : m_position(position)
        , m_direction(direction)
        , m_travel_speed(travel_speed)
    { }

    void update(float dt) {
        m_position += m_direction * m_travel_speed * dt;
    }

    void draw(gfx::Renderer& rd) const {
        rd.draw_circle(m_position, 5.0, gfx::Color::green());
    }

};

class ProjectileSystem {
    std::vector<Projectile> projectiles;

public:
    ProjectileSystem() = default;

    void draw(gfx::Renderer& rd) const {
        for (auto& projectile : projectiles) {
            projectile.draw(rd);
        }
    }

    void update(float dt) {
        for (auto& projectile : projectiles) {
            projectile.update(dt);
        }
    }

    void add(gfx::Vec position, gfx::Vec direction, float size) {
        projectiles.emplace_back(Projectile{ position, direction, size });
    }

};
