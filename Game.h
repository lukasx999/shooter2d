#pragma once

#include <print>
#include <vector>

#include <gfx.h>

#include "Enemy.h"
#include "Entity.h"
#include "Player.h"
#include "GameObject.h"
#include "Map.h"

class Game : public GameObject {
    gfx::Renderer& m_renderer;
    gfx::Font m_font;
    Map m_map;
    Player m_player;
    Enemy m_enemy;

    std::vector<std::reference_wrapper<GameObject>> m_objects {
        m_map,
        m_player,
        m_enemy,
    };

public:
    explicit Game(gfx::Renderer& renderer)
        : m_renderer(renderer)
        , m_font(m_renderer.load_font("/usr/share/fonts/TTF/JetBrainsMonoNerdFont-Regular.ttf"))
        , m_map("./assets/map.tmx")
        , m_player({ m_renderer.get_window().get_width() / 2.0f, m_renderer.get_window().get_height() / 2.0f })
        , m_enemy({ m_renderer.get_window().get_width() / 2.0f, m_renderer.get_window().get_height() / 2.0f })
    { }

    void draw(gfx::Renderer& rd) const override {

        rd.set_camera(m_player.get_position());
        rd.with_camera([&] {
            for (auto& obj : m_objects) {
                obj.get().draw(rd);
            }
        });

        int size = 50;
        auto text = std::format("Health: {}", m_player.get_health());
        int text_width = m_font.measure_text(text.c_str(), size);
        rd.draw_text(m_renderer.get_window().get_width()/2.0 - text_width/2.0, 0, size, text.c_str(), m_font, gfx::Color::red());

    }

    void update(double dt) override {

        for (auto& obj : m_objects) {
            obj.get().update(dt);
        }

        handle_inputs(dt);
        // TODO: this method should accept a span of const entities, and fire an event if a collision happens
        m_map.resolve_collisions(m_renderer.get_window(), m_player, dt);
    }

private:
    void handle_inputs(double dt) {
        auto& window = m_renderer.get_window();

        if (window.get_key_state(gfx::Key::W).pressed())
            m_player.move(Direction::North, dt);

        if (window.get_key_state(gfx::Key::S).pressed())
            m_player.move(Direction::South, dt);

        if (window.get_key_state(gfx::Key::D).pressed())
            m_player.move(Direction::East, dt);

        if (window.get_key_state(gfx::Key::A).pressed())
            m_player.move(Direction::West, dt);

        if (window.get_key_state(gfx::Key::Escape).pressed())
            window.close();
    }

};
