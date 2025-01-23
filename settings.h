#ifndef SETTINGS_H
#define SETTINGS_H

#include <cstdint>

struct GridSettings
{
    // GridSettings() {}

    double minX = 0, minY = 0, minZ = 0;
    double maxX =  20, maxY =  20, maxZ =  20;
    uint32_t segmentCountX = 1;
    uint32_t segmentCountY = 1;
    uint32_t segmentCountZ = 1;

    bool isGridHide = false;
    bool isStarSkyHide = false;

    uint32_t starsNumber = 1000;
};

struct CameraSettings
{
    int linearSpeed = 20;
    int lookSpeed = 270;
    int fieldOfView = 65;
    int farPlane = 1000;
    double nearPlane = 0.1;
};

#endif // SETTINGS_H
