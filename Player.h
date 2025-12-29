#pragma once

#include "Entity.h"

class Player : public Entity {
    static constexpr double m_animation_delay = 0.1;
    static constexpr float m_texture_scale = 5;
    static constexpr int m_spritesheet_cell_size = 32;
    gfx::Spritesheet m_spritesheet { m_texture, m_spritesheet_cell_size, m_spritesheet_cell_size };
    gfx::AnimatedTexture m_sprite_attacking_south     { m_window, m_spritesheet.get_row(6, 4), m_animation_delay };
    gfx::AnimatedTexture m_sprite_attacking_north     { m_window, m_spritesheet.get_row(8, 4), m_animation_delay };
    gfx::AnimatedTexture m_sprite_attacking_sidewards { m_window, m_spritesheet.get_row(7, 4), m_animation_delay };
    gfx::AnimatedTexture m_sprite_idle_south          { m_window, m_spritesheet.get_row(0, 6), m_animation_delay };
    gfx::AnimatedTexture m_sprite_idle_sidewards      { m_window, m_spritesheet.get_row(1, 6), m_animation_delay };
    gfx::AnimatedTexture m_sprite_idle_north          { m_window, m_spritesheet.get_row(2, 6), m_animation_delay };
    gfx::AnimatedTexture m_sprite_walking_sidewards   { m_window, m_spritesheet.get_row(4, 6), m_animation_delay };
    gfx::AnimatedTexture m_sprite_walking_north       { m_window, m_spritesheet.get_row(5, 6), m_animation_delay };
    gfx::AnimatedTexture m_sprite_walking_south       { m_window, m_spritesheet.get_row(3, 6), m_animation_delay };

private:
    // const-agnostic code
    template <typename This> requires std::is_pointer_v<This>
    [[nodiscard]] static auto&
    get_sprite(This self, Direction direction, State state) {
        switch (state) {
            using enum State;
            using enum Direction;

            case Idle: switch (direction) {
                case North: return self->m_sprite_idle_north;
                case East:
                case West: return self->m_sprite_idle_sidewards;
                case South: return self->m_sprite_idle_south;
            }

            case Walking: switch (direction) {
                case North: return self->m_sprite_walking_north;
                case East:
                case West: return self->m_sprite_walking_sidewards;
                case South: return self->m_sprite_walking_south;
            }

            case Attacking: switch (direction) {
                case North: return self->m_sprite_attacking_north;
                case East:
                case West: return self->m_sprite_attacking_sidewards;
                case South: return self->m_sprite_attacking_south;
            }

        }
    }

public:
    Player(const gfx::Window& window, gfx::Vec position)
    : Entity(
        window,
        position,
        gfx::Texture("./assets/Cute_Fantasy_Free/Player/Player.png"),
        m_spritesheet_cell_size * m_texture_scale,
        m_spritesheet_cell_size * m_texture_scale
    )
    { }

    void draw(gfx::Renderer& rd) const override {

        auto& sprite = get_sprite(this, m_direction, m_state);

        if (m_direction == Direction::West)
            sprite.draw_mirrored(rd, get_hitbox());
        else
            sprite.draw(rd, get_hitbox());

        // rd.draw_rectangle(get_hitbox(), gfx::Color::red().set_alpha(0x7f));
    }

    void on_direction_change(Direction new_direction) override {
        auto& new_sprite = get_sprite(this, new_direction, m_state);
        new_sprite.reset();
        new_sprite.start();
    }

    void on_state_change(State new_state) override {
        auto& new_sprite = get_sprite(this, m_direction, new_state);
        new_sprite.reset();
        new_sprite.start();
    }

    bool is_attack_done() override {
        auto& sprite = get_sprite(this, m_direction, m_state);
        return sprite.is_done();
    }

};
