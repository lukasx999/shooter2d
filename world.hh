#pragma once

#include <gfx.hh>

#include "player.hh"

enum class Tile {
    Bg,
    Fg,
};

class World {
    gfx::Window& m_window;
    using enum Tile;

    static constexpr float m_tile_size = 100.0;
    static constexpr int m_world_width = 16;

    static constexpr std::array m_tiles {
        Bg, Bg, Bg, Bg, Bg, Bg, Bg, Bg, Bg, Bg, Bg, Bg, Bg, Bg, Bg, Bg,
        Bg, Bg, Bg, Bg, Fg, Bg, Bg, Bg, Bg, Bg, Bg, Bg, Bg, Bg, Bg, Bg,
        Bg, Bg, Bg, Bg, Fg, Bg, Bg, Bg, Bg, Bg, Bg, Bg, Bg, Bg, Bg, Bg,
        Bg, Bg, Bg, Bg, Fg, Bg, Bg, Fg, Bg, Bg, Bg, Bg, Bg, Bg, Bg, Bg,
        Bg, Bg, Bg, Bg, Fg, Bg, Bg, Bg, Bg, Bg, Bg, Bg, Bg, Bg, Bg, Bg,
        Bg, Bg, Bg, Bg, Bg, Bg, Bg, Bg, Bg, Bg, Bg, Bg, Bg, Bg, Bg, Bg,
        Bg, Bg, Bg, Bg, Bg, Bg, Bg, Bg, Bg, Bg, Bg, Bg, Bg, Bg, Bg, Bg,
        Bg, Bg, Bg, Bg, Bg, Bg, Bg, Bg, Bg, Bg, Bg, Bg, Bg, Bg, Bg, Bg,
        Bg, Bg, Bg, Bg, Bg, Bg, Bg, Bg, Bg, Bg, Bg, Bg, Bg, Bg, Bg, Bg,
    };

    static_assert(m_tiles.size() % m_world_width == 0, "world must be aligned to width");

public:
    explicit World(gfx::Window& window)
    : m_window(window)
    { }

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

    void resolve_collisions(Player& player, gfx::Renderer& rd) {
        for (size_t i=0; i < m_tiles.size(); ++i) {
            float y = static_cast<int>(i / m_world_width) * m_tile_size;
            float x = (i % m_world_width) * m_tile_size;

            if (m_tiles[i] == Bg) continue;

            // size of the collision hitbox
            float diff = 500.0 * rd.get_frame_time();

            float collision_size = 1;

            // add a tiny collision rectangle for each side of the tile so we
            // know which tile was hit
            //
            // also make the side of each rectangle a bit smaller to avoid
            // overlaps in collisions
            gfx::Rect left {
                x-collision_size,
                y + diff,
                collision_size,
                m_tile_size - diff * 2,
            };

            gfx::Rect right {
                x + m_tile_size,
                y + diff,
                collision_size,
                m_tile_size - diff * 2,
            };

            gfx::Rect top {
                x + diff,
                y-collision_size,
                m_tile_size - diff * 2,
                collision_size,
            };

            gfx::Rect bottom {
                x + diff,
                y + m_tile_size,
                m_tile_size - diff * 2,
                collision_size,
            };

            rd.with_camera([&] {
                rd.draw_rectangle(left, gfx::Color::green());
                rd.draw_rectangle(right, gfx::Color::green());
                rd.draw_rectangle(top, gfx::Color::green());
                rd.draw_rectangle(bottom, gfx::Color::green());
            });

            gfx::Vec pos = player.get_position();
            float size = player.get_size();
            gfx::Rect p = player.get_hitbox();

            if (p.check_collision_rects(left))
                player.set_position({ x-size-1, pos.y });

            if (p.check_collision_rects(right))
                player.set_position({ x+m_tile_size+size+1, pos.y });

            if (p.check_collision_rects(top))
                player.set_position({ pos.x, y-size-1 });

            if (p.check_collision_rects(bottom))
                player.set_position({ pos.x, y+m_tile_size+size+1 });

        }
    }

};
