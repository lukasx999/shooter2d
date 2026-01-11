#pragma once

#include <gfx.h>

#include "GameObject.h"
#include "misc.h"

class Entity : public IGameObject {
protected:
    const int m_width;
    const int m_height;

    float m_movement_speed = 500;
    gfx::Vec m_position;
    Direction m_direction = Direction::South;

    static constexpr int m_max_health = 100;
    int m_health = m_max_health;

    bool m_is_idle = false;
    bool m_idle_lock = false;
    bool m_is_first_iteration = true;

public:
    enum class State { Idle, Walking, Attacking };

protected:
    State m_state = State::Idle;

public:
    Entity(gfx::Vec position, int width, int height);

    [[nodiscard]] gfx::Vec get_position() const {
        return m_position;
    }

    [[nodiscard]] int get_health() const {
        return m_health;
    }

    void set_health(int health) {
        m_health = health;
    }

    void set_movement_speed(float movement_speed) {
        m_movement_speed = movement_speed;
    }

    [[nodiscard]] float get_movement_speed() const {
        return m_movement_speed;
    }

    [[nodiscard]] Direction get_direction() const {
        return m_direction;
    }

    [[nodiscard]] State get_state() const {
        return m_state;
    }

    void set_position(gfx::Vec position) {
        m_position = position;
    }

    void update(double dt) override;
    void draw([[maybe_unused]] gfx::Renderer& rd) const override { }

    [[nodiscard]] gfx::Rect get_hitbox() const;
    void walk(Direction dir, double dt);
    void attack();

protected:
    virtual void on_direction_change([[maybe_unused]] Direction new_direction) { }
    virtual void on_state_change([[maybe_unused]] State new_state) { }
    [[nodiscard]] virtual bool is_attack_done() const = 0;

};

template <>
struct std::formatter<Entity::State> : std::formatter<std::string> {
    auto format(const Entity::State& state, std::format_context& ctx) const {

        auto fmt = [&] {
            switch (state) {
                using enum Entity::State;
                case Idle:      return STRINGIFY(Idle);
                case Walking:   return STRINGIFY(Walking);
                case Attacking: return STRINGIFY(Attacking);
            }
        }();

        return std::formatter<std::string>::format(fmt, ctx);
    }
};
