#pragma once

#include <print>
#include <vector>

#include <gfx.h>

#include "Entity.h"
#include "Player.h"
#include "GameObject.h"
#include "Map.h"

class Game : public GameObject {
    gfx::Renderer& m_renderer;
    gfx::Font m_font;
    Map m_map;
    Player m_player;

    std::vector<std::reference_wrapper<GameObject>> m_objects {
        m_map,
        m_player,
    };

public:
    explicit Game(gfx::Renderer& renderer)
        : m_renderer(renderer)
        , m_font(m_renderer.load_font("/usr/share/fonts/TTF/JetBrainsMonoNerdFont-Regular.ttf"))
        , m_map("./assets/map.tmx")
        , m_player(m_renderer.get_window(), { m_renderer.get_window().get_width() / 2.0f, m_renderer.get_window().get_height() / 2.0f })
    {
        m_map.add_listener(m_player);
    }

    void draw(gfx::Renderer& rd) const override {

        rd.set_camera(m_player.get_position());
        rd.with_camera([&] {
            for (auto& obj : m_objects)
                obj.get().draw(rd);
        });

        auto text = std::format("Health: {}", m_player.get_health());
        rd.draw_text(0, 0, 50, text.c_str(), m_font, gfx::Color::white());
        rd.draw_text(0, 50, 50, m_player.stringify_state(m_player.get_state()), m_font, gfx::Color::white());
        rd.draw_text(0, 100, 50, stringify_direction(m_player.get_direction()), m_font, gfx::Color::white());

    }

    void update(double dt) override {

        for (auto& obj : m_objects)
            obj.get().update(dt);

        handle_inputs(dt);

        // TODO: this method should accept a span of const entities, and fire an event if a collision happens
        m_map.resolve_collisions(m_renderer.get_window(), m_player, dt);
    }

private:
    void handle_inputs(double dt) {
        auto& window = m_renderer.get_window();

        using enum Direction;

        if (window.get_key_state(gfx::Key::W).pressed())
            m_player.walk(North, dt);

        if (window.get_key_state(gfx::Key::S).pressed())
            m_player.walk(South, dt);

        if (window.get_key_state(gfx::Key::D).pressed())
            m_player.walk(East, dt);

        if (window.get_key_state(gfx::Key::A).pressed())
            m_player.walk(West, dt);

        if (window.get_key_state(gfx::Key::J).pressed())
            m_player.attack();

        if (window.get_key_state(gfx::Key::Escape).pressed())
            window.close();
    }

};
