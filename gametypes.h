#ifndef GAMETYPES_H
#define GAMETYPES_H

#include <QString>

enum class CropType
{
    None = 0,
    Wheat,
    Rice
};

inline QString cropName(CropType type)
{
    switch (type)
    {
    case CropType::Wheat:
        return "小麦";
    case CropType::Rice:
        return "水稻";
    default:
        return "无作物";
    }
}

#endif
