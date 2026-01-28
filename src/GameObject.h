#pragma once

#include <gfx/gfx.h>

class GameObject {
public:
    virtual ~GameObject() = default;
    virtual void update(double dt) = 0;
    virtual void draw(gfx::Renderer& rd) const = 0;
};
