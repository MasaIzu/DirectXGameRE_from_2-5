#include"Vector2.h"

const Vector2 operator + (const Vector2& v1, const Vector2& v2)
{
    Vector2 temp(v1);
    return temp += v2;
}

const Vector2 operator - (const Vector2& v1, const Vector2& v2)
{
    Vector2 temp(v1);
    return temp -= v2;
}

const Vector2 operator *(const Vector2& v, float s)
{
    Vector2 temp(v);
    return temp * s;
}

const Vector2 operator *(float s, const Vector2& v)
{
    return v * s;
}

const Vector2 operator/(const Vector2& v, float s)
{
    Vector2 temp(v);
    if (v.x != 0) {
        temp.x = temp.x / s;
    }
    if (v.y != 0) {
        temp.y = temp.y / s;
    }

    return temp;
}

