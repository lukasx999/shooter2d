#pragma once

#include <print>

#include <gfx.h>

#include "GameObject.h"
#include "LayerPaused.h"
#include "LayerPlaying.h"
#include "LayerTitlescreen.h"

class Game : public GameObject {
    gfx::Window& m_window;
    gfx::Font m_font;

    LayerTitlescreen m_layer_titlescreen;
    LayerPlaying m_layer_playing;
    LayerPaused m_layer_paused;

    std::reference_wrapper<GameObject> m_active_layer = m_layer_titlescreen;

public:
    explicit Game(gfx::Window& window)
        : m_window(window)
        , m_font(m_window.load_font("/usr/share/fonts/TTF/JetBrainsMonoNerdFont-Regular.ttf"))
        , m_layer_titlescreen(m_window, m_font)
        , m_layer_playing(m_window, m_font)
        , m_layer_paused(m_window, m_font, m_layer_playing)
    {
        auto thunk = [&](State state) { handle_state_change(state); };
        m_layer_titlescreen.on_state_change(thunk);
        m_layer_playing.on_state_change(thunk);
        m_layer_paused.on_state_change(thunk);
    }

    void draw(gfx::Renderer& rd) const override {
        m_active_layer.get().draw(rd);
    }

    void update(double dt) override {
        handle_inputs(dt);
        m_active_layer.get().update(dt);
    }

private:
    void handle_inputs([[maybe_unused]] double dt) {

        // TODO: use the event system for this

        if (m_window.get_key_state(gfx::Key::Escape).pressed())
            m_window.close();
    }

    void handle_state_change(State state) {

        switch (state) {
            using enum State;

            case Titlescreen:
                m_active_layer = m_layer_titlescreen;
                break;

            case Playing:
                m_active_layer = m_layer_playing;
                break;

            case Paused:
                m_active_layer = m_layer_paused;
                break;
        }
    }

};
