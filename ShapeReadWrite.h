/**
 * 说明：gdal读写shp
 * date: 2020-07-23
 */

#ifndef _SHAPE_READ_WRITE_H
#define _SHAPE_READ_WRITE_H

#include <fstream>
#include <iostream>

#include ".\GDAL\include\gdal.h"
#include ".\GDAL\include\gdal_priv.h"
#include ".\GDAL\include\ogrsf_frmts.h"

typedef struct double_point
{
    double x;
    double y;
    double z;

    double_point() : x(0), y(0), z(0) {}
    double_point(double dX, double dY) : x(dX), y(dY), z(0) {}
    double_point(double dX, double dY, doule dZ) : x(dX), y(dY), z(dZ) {}
} dPoint;

class ShapeReadWrite
{
public:
    ShapeReadWrite();
    virtual ~ShapeReadWrite();

public:
};

#endif