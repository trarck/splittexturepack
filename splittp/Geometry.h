#ifndef SPLITTP_GEOMETRY_H_
#define SPLITTP_GEOMETRY_H_

#include "json/json.h"
#include "pugixml.hpp"


struct Rect
{
    float x;
    float y;
    float width;
    float height;
};

struct Point
{
    float x;
    float y;
};

struct Size
{
    float width;
    float height;
};



Rect RectFromString(const char* pszContent);

Point PointFromString(const char* pszContent);

Size SizeFromString(const char* pszContent);


#endif // SPLITTP_GEOMETRY_H_
