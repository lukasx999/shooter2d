#pragma once

#include <gfx.hh>

#include "player.hh"

enum class Tile {
    Bg,
    Fg,
};

class World {
    using enum Tile;

    static constexpr float m_tile_size = 100.0;
    static constexpr int m_world_width = 10;

    static constexpr std::array m_tiles {
        Bg, Bg, Bg, Bg, Bg, Bg, Bg, Bg, Bg, Bg,
        Bg, Bg, Bg, Bg, Fg, Bg, Bg, Bg, Bg, Bg,
        Bg, Bg, Bg, Bg, Bg, Bg, Bg, Bg, Bg, Bg,
        Bg, Bg, Bg, Bg, Bg, Bg, Bg, Bg, Bg, Bg,
        Bg, Bg, Bg, Bg, Bg, Bg, Bg, Bg, Bg, Bg,
        Bg, Bg, Bg, Bg, Bg, Bg, Bg, Bg, Bg, Bg,
    };

    static_assert(m_tiles.size() % m_world_width == 0, "world must be aligned to width");

public:
    World() = default;

    void draw(gfx::Renderer& rd) const {
        for (size_t i=0; i < m_tiles.size(); ++i) {
            float y = static_cast<int>(i / m_world_width) * m_tile_size;
            float x = (i % m_world_width) * m_tile_size;

            gfx::Color color;
            switch (m_tiles[i]) {
                case Bg:
                    color = gfx::Color::grey();
                    break;

                case Fg:
                    color = gfx::Color::orange();
                    break;
            }

            rd.draw_rectangle(x, y, m_tile_size, m_tile_size, color);
        }
    }

    void resolve_collisions(Player& player) {
        for (size_t i=0; i < m_tiles.size(); ++i) {
            float y = static_cast<int>(i / m_world_width) * m_tile_size;
            float x = (i % m_world_width) * m_tile_size;

            if (m_tiles[i] == Bg) continue;

            auto pos = player.get_position();
            auto size = player.get_size();

            auto p = player.get_hitbox();
            float diff = 10;

            // add a tiny collision rectangle for each side of the tile so we
            // know which tile was hit
            //
            // also make the side of each rectangle a bit smaller to avoid
            // overlaps in collisions
            gfx::Rect left { x, y+diff, diff, m_tile_size-diff*2 };
            gfx::Rect right { x+m_tile_size-diff, y+diff, diff, m_tile_size-diff*2 };
            gfx::Rect top { x+diff, y, m_tile_size-diff*2, diff };
            gfx::Rect bottom { x+diff, y+m_tile_size-diff, m_tile_size-diff*2, diff };

            if (check_collision_rects(p, left)) {
                player.set_position({ x-size, pos.y });
            }

            if (check_collision_rects(p, right)) {
                player.set_position({ x+m_tile_size+size, pos.y });
            }

            if (check_collision_rects(p, top)) {
                player.set_position({ pos.x, y-size });
            }

            if (check_collision_rects(p, bottom)) {
                player.set_position({ pos.x, y+m_tile_size+size });
            }

        }
    }

};
