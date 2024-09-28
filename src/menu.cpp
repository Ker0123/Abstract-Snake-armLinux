#include "menu.h"

AbstractButton::AbstractButton(Vector2<int> loc, Vector2<int> size)
{
    this->loc = loc;
    this->size = size;
}

AbstractButton::AbstractButton()
{
    this->loc = Vector2<int>(0, 0);
    this->size = Vector2<int>(0, 0);
}

bool AbstractButton::isClicked(Vector2<int> touch_pos)
{
    if(loc.x < touch_pos.x && loc.x + size.x > touch_pos.x && loc.y < touch_pos.y && loc.y + size.y > touch_pos.y)
        return true;
    return false;
}
