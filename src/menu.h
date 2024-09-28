#pragma once

#include "vector2.h"

class AbstractButton
{
private:
    Vector2<int> loc;
    Vector2<int> size;

public:
    AbstractButton(Vector2<int> loc, Vector2<int> size);
    AbstractButton();
    bool isClicked(Vector2<int> touch_pos);
};