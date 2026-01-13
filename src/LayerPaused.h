#pragma once

#include <gfx.h>

#include "LayerPlaying.h"
#include "Layer.h"

class LayerPaused : public Layer {
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

    void update([[maybe_unused]] double dt) override {
        if (m_window.get_key_state(gfx::Key::P).pressed())
            m_on_state_change(State::Playing);
    }

};
