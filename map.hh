#pragma once

#include <algorithm>
#include <filesystem>
#include <ranges>

#include <gfx.hh>

#include <tmxlite/Map.hpp>
#include <tmxlite/TileLayer.hpp>
#include <tmxlite/Layer.hpp>
#include <tmxlite/ObjectGroup.hpp>
#include <tmxlite/Object.hpp>

#include "player.hh"

class Map {
    tmx::Map m_map;
    std::unordered_map<const tmx::Tileset*, gfx::Texture> m_textures;

public:
    explicit Map(const char* path) {

        bool success = m_map.load(path);
        if (!success) {
            std::println("failed to load map");
            exit(1);
        }

        load_tile_textures();
    }

    void draw(gfx::Renderer& rd) const {

        auto tile_size = m_map.getTileSize();
        auto tile_count = m_map.getTileCount();
        // TODO: parse all layers
        auto& layer = m_map.getLayers().front();
        auto layer_size = layer->getSize();

        auto color = m_map.getBackgroundColour();
        rd.draw_rectangle(0, 0, layer_size.x * tile_size.x, layer_size.y * tile_size.y, tmx_color_to_gfx_color(color));

        for_each_tile([&](uint32_t gid, int dest_x, int dest_y) {

            const tmx::Tileset& ts = find_tileset(gid);

            auto tileset_columns = ts.getColumnCount();
            int local_id = gid - ts.getFirstGID();
            int src_x = local_id % tileset_columns;
            int src_y = local_id / tileset_columns;

            auto& tex = m_textures.at(&ts);

            // scale up the map to fit the window resolution
            // TODO: we also need to apply scaling in collision detection system
            float factor_x = static_cast<float>(rd.get_window().get_width()) / (tile_count.x * tile_size.x);
            float factor_y = static_cast<float>(rd.get_window().get_height()) / (tile_count.y * tile_size.y);
            float scaled_x = dest_x * factor_x;
            float scaled_y = dest_y * factor_y;
            float scaled_width = tile_size.x * factor_x;
            float scaled_height = tile_size.y * factor_y;

            rd.draw_texture_sub(
                scaled_x,
                scaled_y,
                scaled_width,
                scaled_height,
                src_x * tile_size.x,
                src_y * tile_size.y,
                tile_size.x,
                tile_size.y,
                0_deg,
                tex
            );

        });
    }

    void resolve_collisions(Player& player, double dt) {

        // TODO: parse all object layers
        auto& obj_layer = m_map.getLayers()[1];
        assert(obj_layer->getType() == tmx::Layer::Type::Object);
        auto objects = obj_layer->getLayerAs<tmx::ObjectGroup>().getObjects();

        for (auto& object : objects) {
            auto aabb = object.getAABB();

            float x = aabb.left;
            float y = aabb.top;
            float width = aabb.width;
            float height = aabb.height;

            // subtracted from the height of the collision hitbox, otherwise
            // the player would clip through the tile and trigger a wrong collision
            // it is set to the amount of pixels the player can move at the current frame
            float diff = player.get_movement_speed() * dt;

            // width of the collision hitbox
            float collision_size = 1;

            // add a tiny collision rectangle for each side of the tile so we
            // know which tile was hit
            gfx::Rect left {
                x - collision_size,
                y + diff,
                collision_size,
                height - diff * 2,
            };

            gfx::Rect right {
                static_cast<float>(x) + width,
                y + diff,
                collision_size,
                height - diff * 2,
            };

            gfx::Rect top {
                x + diff,
                y - collision_size,
                width - diff * 2,
                collision_size,
            };

            gfx::Rect bottom {
                x + diff,
                static_cast<float>(y) + height,
                width - diff * 2,
                collision_size,
            };

            gfx::Vec pos = player.get_position();
            gfx::Rect p = player.get_hitbox();

            if (p.check_collision(left))
                player.set_position({ x - p.width / 2.0f - 1, pos.y });

            if (p.check_collision(right))
                player.set_position({ x + width + p.width / 2.0f + 1, pos.y });

            if (p.check_collision(top))
                player.set_position({ pos.x, y - p.height / 2.0f - 1 });

            if (p.check_collision(bottom))
                player.set_position({ pos.x, y + height + p.height / 2.0f + 1 });

        }

    }

private:
    void for_each_tile(std::function<void(uint32_t gid, int x, int y)> fn) const {

        // TODO: use other layers than the first
        auto& layer = m_map.getLayers().front();
        auto layer_size = layer->getSize();
        assert(layer->getType() == tmx::Layer::Type::Tile);
        auto& tiles = layer->getLayerAs<tmx::TileLayer>().getTiles();
        auto tile_size = m_map.getTileSize();

        for (auto&& [i, tile] : tiles | std::views::enumerate) {
            uint32_t gid = tile.ID;

            int dest_x = i % layer_size.x;
            int dest_y = i / layer_size.x;

            if (gid == 0)
                // empty.
                continue;

            fn(gid, dest_x * tile_size.x, dest_y * tile_size.y);
        }
    }

    void load_tile_textures() {
        auto& tilesets = m_map.getTilesets();
        for (auto& tileset : tilesets) {

            auto tex_path = tileset.getImagePath();
            if (!std::filesystem::exists(tex_path)) {
                std::println("image doesnt exist");
                exit(1);
            }

            m_textures.emplace(&tileset, gfx::Texture(tex_path));

        }
    }

    [[nodiscard]] const tmx::Tileset& find_tileset(uint32_t gid) const {

        auto& tilesets = m_map.getTilesets();
        auto ts = std::ranges::find_if(tilesets, [&](const tmx::Tileset& ts) {
            return gid >= ts.getFirstGID() && gid <= ts.getLastGID();
        });

        assert(ts != tilesets.cend());
        return *ts;
    }

    [[nodiscard]] static constexpr
    gfx::Color tmx_color_to_gfx_color(tmx::Colour color) {
        // HACK: use a cleaner approach
        return *reinterpret_cast<gfx::Color*>(&color);
    }

};
