#pragma once

#include <gfx.h>

class AnimatedTexture {
    const gfx::Texture& m_texture;
    const std::vector<gfx::Rect> m_frames;
    size_t m_current_frame = 0;
    double m_time_marker = 0.0f;
    const double m_animation_delay_secs;

public:
    explicit AnimatedTexture(const gfx::Texture& texture, double animation_delay_secs, std::vector<gfx::Rect> frames)
        : m_texture(texture)
        , m_frames(std::move(frames))
        , m_animation_delay_secs(animation_delay_secs)
    { }

    void update(const gfx::Window& window) {

        double time = window.get_time();

        if (time - m_time_marker >= m_animation_delay_secs) {
            m_current_frame++;
            m_current_frame %= m_frames.size();
            m_time_marker = time;
        }

    }

    void draw(gfx::Renderer& rd, gfx::Rect dest) const {
        gfx::Rect frame = m_frames[m_current_frame];
        rd.draw_texture_sub(dest, frame, m_texture);
    }

    // mirrors the sprite along the y-axis
    void draw_mirrored(gfx::Renderer& rd, gfx::Rect dest) const {
        dest.x += dest.width;
        dest.width *= -1;
        draw(rd, dest);
    }

};
