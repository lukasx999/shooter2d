#pragma once

#include <print>

#include <gfx.h>

#include "GameObject.h"
#include "LayerPlaying.h"

using namespace std::chrono_literals;

class LayerTitlescreen : public GameObject {
    const gfx::Window& m_window;
    const gfx::Font& m_font;

    gfx::ui::Button m_button { m_window, m_font, "start game", { 0, 0, 500, 100 }, 50 };
    gfx::Animation<int> m_animation{ 0, m_window.get_width(), 5s, gfx::interpolators::ease_in_out_circ };

public:
    LayerTitlescreen(const gfx::Window& window, const gfx::Font& font)
        : m_window(window)
        , m_font(font)
    { }

    void draw(gfx::Renderer& rd) const override {

        rd.draw_text_centered(rd.get_window().get_width()/2.0, 0, 50, "epic game", m_font, gfx::Color::white());
        rd.draw_rectangle(0, 100, m_animation.get(), 50, gfx::Color::red());
        m_button.draw(rd);
    }

    void update([[maybe_unused]] double dt) override {
        m_button.update();

        if (m_button.is_pressed()) {
        }

    }

};

class LayerPaused : public GameObject {
    const gfx::Window& m_window;
    const gfx::Font& m_font;

    LayerPlaying& m_layer_playing;

public:
    LayerPaused(const gfx::Window& window, const gfx::Font& font, LayerPlaying& layer_playing)
        : m_window(window)
        , m_font(font)
        , m_layer_playing(layer_playing)
    { }

    void draw(gfx::Renderer& rd) const override {
        m_layer_playing.draw(rd);

        rd.draw_rectangle(0, 0, m_window.get_width(), m_window.get_height(), gfx::Color::black().set_alpha(128));
        rd.draw_text(0, 0, 50, "paused.", m_font, gfx::Color::white());
    }

    void update([[maybe_unused]] double dt) override { }

};

// TODO: implement state switching
class Game : public GameObject {

    gfx::Window& m_window;
    gfx::Font m_font;

    LayerTitlescreen m_layer_titlescreen;
    LayerPlaying m_layer_playing;
    LayerPaused m_layer_paused;

    GameObject& m_active_layer = m_layer_titlescreen;

    enum class State {
        Titlescreen,
        Playing,
        Paused
    } m_state = State::Paused;

public:
    explicit Game(gfx::Window& window)
        : m_window(window)
        , m_font(m_window.load_font("/usr/share/fonts/TTF/JetBrainsMonoNerdFont-Regular.ttf"))
        , m_layer_titlescreen(m_window, m_font)
        , m_layer_playing(m_window, m_font)
        , m_layer_paused(m_window, m_font, m_layer_playing)
    { }

    void draw(gfx::Renderer& rd) const override {
        m_active_layer.draw(rd);
    }

    void update(double dt) override {
        handle_inputs(dt);
        m_active_layer.update(dt);
    }

private:
    void handle_inputs([[maybe_unused]] double dt) {

        // TODO: use the event system for this

        if (m_window.get_key_state(gfx::Key::Escape).pressed())
            m_window.close();
    }

};
