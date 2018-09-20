#include "camera.hpp"
#include <iostream>
void Camera::lock(float x, float y, float margin)
{
    const float mx = (x - this->x) / (w / zoom);
    const float my = (y - this->y) / (h / zoom);

    if(mx < margin)
    {
        this->x = -margin * (w / zoom) + x;
    }
    else if(mx > 1 - margin)
    {
        this->x = -(1 - margin) * (w / zoom) + x;
    }

    if(my < margin)
    {
        this->y = -margin * (h / zoom) + y;
    }
    else if(my > 1 - margin)
    {
        this->y = -(1 - margin) * (h / zoom) + y;
    }
}