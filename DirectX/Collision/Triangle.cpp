#include "Triangle.h"

Triangle::Triangle()
    : p0()
    , p1()
    , p2()
{
}

Triangle::Triangle(const Vector3& p0, const Vector3& p1, const Vector3& p2)
    : p0(p0)
    , p1(p1)
    , p2(p2)
{
}

Vector3 Triangle::normal() const {
    auto p0p1 = p1 - p0;
    auto p0p2 = p2 - p0;

    //外積により2辺に垂直なベクトルを算出する
    auto n = Vector3::cross(p0p1, p0p2);
    n.normalize();

    return n;
}
