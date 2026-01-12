#pragma once

#include <gfx.h>

#include "GameObject.h"

class PhysicalObject : public GameObject {
protected:
    gfx::Vec m_position;

public:
    explicit PhysicalObject(gfx::Vec position)
    : m_position(position)
    { }

    [[nodiscard]] gfx::Vec get_position() const {
        return m_position;
    }

    void set_position(gfx::Vec position) {
        m_position = position;
    }

    void draw(gfx::Renderer& rd) const override { }
    void update([[maybe_unused]] double dt) override { }

};
