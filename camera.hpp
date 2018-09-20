#ifndef CAMERA_HPP
#define CAMERA_HPP

struct Camera
{
    void lock(float x, float y, float margin=0.45f);

    float x;
    float y;

    float w;
    float h;
    
    float zoom;
};

#endif