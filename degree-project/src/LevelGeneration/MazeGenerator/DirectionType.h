#pragma once
#include <cstdint>

enum class DirectionType : uint8_t
{
    None = 0,
    North = 1 << 0, // 1
    East = 1 << 1, // 2
    South = 1 << 2, // 4
    West = 1 << 3 // 8
};

inline DirectionType operator|(DirectionType lhs, DirectionType rhs)
{
    return static_cast<DirectionType>(static_cast<int>(lhs) | static_cast<int>(rhs));
}

inline DirectionType operator&(DirectionType lhs, DirectionType rhs)
{
    return static_cast<DirectionType>(static_cast<uint8_t>(lhs) & static_cast<uint8_t>(rhs));
}

inline DirectionType operator~(const DirectionType& value)
{
    return static_cast<DirectionType>(~static_cast<int>(value));
}