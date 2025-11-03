#pragma once

#include <print>

#include <gfx.hh>

#include "player.hh"
#include "projectiles.hh"
#include "map.hh"

class Game {
    gfx::Renderer& m_renderer;
    gfx::Window& m_window;
    gfx::Font m_font;
    Map m_map;
    Player m_player;
    ProjectileSystem m_projectiles;

    double m_last_shot = 0.0;
    static constexpr double m_shot_delay = 0.25;

public:
    Game(gfx::Renderer& renderer, gfx::Window& window)
        : m_renderer(renderer)
        , m_window(window)
        , m_font(m_renderer.load_font("/usr/share/fonts/TTF/JetBrainsMonoNerdFont-Regular.ttf"))
        , m_map("./assets/map.tmx")
        , m_player({ m_window.get_width() / 2.0f, m_window.get_height() / 2.0f })
    { }

    void draw(gfx::Renderer& rd) const {

        rd.set_camera(m_player.get_position());
        rd.with_camera([&] {
            m_map.draw(rd);
            m_player.draw(rd);
            m_projectiles.draw(rd);
        });

        int size = 50;
        auto text = "Health: 0";
        int text_width = m_font.measure_text(text, size);
        rd.draw_text(m_window.get_width()/2.0 - text_width/2.0, 0, size, text, m_font, gfx::Color::red());

    }

    void update(double dt) {
        m_projectiles.update(dt);

        bool can_shoot = m_window.get_time() > m_last_shot + m_shot_delay;

        if (m_window.get_mouse_button_state(gfx::MouseButton::Left).pressed() && can_shoot) {
            m_last_shot = m_window.get_time();
            auto ppos = m_player.get_position();
            auto mpos = m_window.get_mouse_pos();

            auto direction = (mpos - ppos).normalized();
            m_projectiles.add(ppos, direction, 500.0);

        }

        if (m_window.get_key_state(gfx::Key::W).pressed())
            m_player.move(Direction::North, dt);

        if (m_window.get_key_state(gfx::Key::S).pressed())
            m_player.move(Direction::South, dt);

        if (m_window.get_key_state(gfx::Key::D).pressed())
            m_player.move(Direction::East, dt);

        if (m_window.get_key_state(gfx::Key::A).pressed())
            m_player.move(Direction::West, dt);

        if (m_window.get_key_state(gfx::Key::Escape).pressed())
            m_window.close();

        m_map.resolve_collisions(m_player, dt);

    }

};
