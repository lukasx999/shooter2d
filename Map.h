#pragma once

#include <algorithm>
#include <filesystem>
#include <ranges>

#include <gfx.h>

#include <tmxlite/Map.hpp>
#include <tmxlite/TileLayer.hpp>
#include <tmxlite/Layer.hpp>
#include <tmxlite/ObjectGroup.hpp>
#include <tmxlite/Object.hpp>

#include "GameObject.h"
#include "Entity.h"
#include "Emitter.h"
#include "misc.h"

class Map
: public GameObject
, public Emitter<CollisionData>
{
    tmx::Map m_map;
    std::unordered_map<const tmx::Tileset*, gfx::Texture> m_textures;

public:
    explicit Map(const char* path) {

        bool success = m_map.load(path);
        if (!success)
            throw std::runtime_error("failed to load map");

        load_tile_textures();
    }

    void update([[maybe_unused]] double dt) override { }

    void draw(gfx::Renderer& rd) const override {
        auto tile_size = m_map.getTileSize();

        // TODO: parse all layers
        auto& layer = m_map.getLayers().front();
        auto layer_size = layer->getSize();

        auto factor = get_map_scaling_factor(rd.get_window());

        auto color = m_map.getBackgroundColour();
        rd.draw_rectangle(0, 0, layer_size.x * tile_size.x, layer_size.y * tile_size.y, tmx_color_to_gfx_color(color));

        for_each_tile([&](uint32_t gid, gfx::Vec dest) {

            const tmx::Tileset& ts = find_tileset(gid);

            auto tileset_columns = ts.getColumnCount();
            int local_id = gid - ts.getFirstGID();
            int src_x = local_id % tileset_columns;
            int src_y = local_id / tileset_columns;

            float scaled_x = dest.x * factor.x;
            float scaled_y = dest.y * factor.y;
            float scaled_width = tile_size.x * factor.x;
            float scaled_height = tile_size.y * factor.y;

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
                m_textures.at(&ts)
            );

        });
    }

    void resolve_collisions(const gfx::Window& window, Entity& entity, double dt) {

        // TODO: parse all object layers
        auto& obj_layer = m_map.getLayers()[1];
        assert(obj_layer->getType() == tmx::Layer::Type::Object);
        auto objects = obj_layer->getLayerAs<tmx::ObjectGroup>().getObjects();

        auto factor = get_map_scaling_factor(window);

        for (auto& object : objects) {
            auto aabb = object.getAABB();

            float x = aabb.left * factor.x;
            float y = aabb.top * factor.y;
            float width = aabb.width * factor.x;
            float height = aabb.height * factor.y;

            // subtracted from the height of the collision hitbox, otherwise
            // the player would clip through the tile and trigger a wrong collision
            // it is set to the amount of pixels the player can move at the current frame
            float diff = entity.get_movement_speed() * dt;

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

            gfx::Vec pos = entity.get_position();
            gfx::Rect p = entity.get_hitbox();

            if (p.check_collision(left))
                notify({ x - p.width / 2.0f - 1, pos.y });

            if (p.check_collision(right))
                notify({ x + width + p.width / 2.0f + 1, pos.y });

            if (p.check_collision(top))
                notify({ pos.x, y - p.height / 2.0f - 1 });

            if (p.check_collision(bottom))
                notify({ pos.x, y + height + p.height / 2.0f + 1 });

        }

    }

private:
    // get the scaling factor so the map fits the window
    [[nodiscard]] gfx::Vec get_map_scaling_factor(const gfx::Window& window) const {

        auto tile_size = m_map.getTileSize();
        auto tile_count = m_map.getTileCount();

        float width = window.get_width();
        float height = window.get_height();
        float factor_x = width / (tile_count.x * tile_size.x);
        float factor_y = height / (tile_count.y * tile_size.y);

        return { factor_x, factor_y };
    }

    void for_each_tile(std::function<void(uint32_t gid, gfx::Vec dest)> fn) const {

        // TODO: use other layers than the first
        auto& layer = m_map.getLayers().front();
        auto layer_size = layer->getSize();
        assert(layer->getType() == tmx::Layer::Type::Tile);
        auto& tiles = layer->getLayerAs<tmx::TileLayer>().getTiles();
        auto tile_size = m_map.getTileSize();

        for (auto&& [idx, tile] : tiles | std::views::enumerate) {
            uint32_t gid = tile.ID;

            int dest_x = idx % layer_size.x;
            int dest_y = idx / layer_size.x;
            gfx::Vec dest {
                static_cast<float>(dest_x * tile_size.x),
                static_cast<float>(dest_y * tile_size.y),
            };

            if (gid == 0)
                // empty.
                continue;

            fn(gid, dest);
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
