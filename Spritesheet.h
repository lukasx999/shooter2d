#pragma once

#include <gfx.h>

class Spritesheet {
    const gfx::Texture& m_texture;
    const int m_cell_width;
    const int m_cell_height;

public:
    Spritesheet(const gfx::Texture& texture, int cell_width, int cell_height)
        : m_texture(texture)
        , m_cell_width(cell_width)
        , m_cell_height(cell_height)
    { }

    [[nodiscard]] int get_cell_width() const {
        return m_cell_width;
    }

    [[nodiscard]] int get_cell_height() const {
        return m_cell_height;
    }

    [[nodiscard]] std::vector<gfx::Rect> get_row(int y, int n) const {
        std::vector<gfx::Rect> row;

        for (int x = 0; x < n; ++x)
            row.push_back(get_sprite(x, y));

        return row;
    }

    [[nodiscard]] gfx::Rect get_sprite(int x, int y) const {
        return {
            static_cast<float>(x * m_cell_width),
            static_cast<float>(y * m_cell_height),
            static_cast<float>(m_cell_width),
            static_cast<float>(m_cell_height),
        };
    }

};
