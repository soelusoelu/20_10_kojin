#include "Collision.h"
#include <algorithm>
#include <array>
#include <vector>

Ray::Ray(const Vector3& origin, const Vector3& direction, float maxDistance) :
    start(origin),
    end(direction* maxDistance) {
}

Vector3 Ray::pointOnSegment(float t) const {
    return start + (end - start) * t;
}

float Ray::minDistanceSquare(const Vector3& point) const {
    //ベクトルの準備
    Vector3 ab = end - start;
    Vector3 ba = -1.0f * ab;
    Vector3 ac = point - start;
    Vector3 bc = point - end;

    // Case 1: Aの外側
    if (Vector3::dot(ab, ac) < 0.0f) {
        return ac.lengthSq();
    }
    // Case 2: Bの外側
    else if (Vector3::dot(ba, bc) < 0.0f) {
        return bc.lengthSq();
    }
    // Case 3:ABの間
    else {
        //pを計算
        float scalar = Vector3::dot(ac, ab) / Vector3::dot(ab, ab);
        Vector3 p = scalar * ab;
        //ac - pの長さの2乗を計算
        return (ac - p).lengthSq();
    }
}

float Ray::minDistanceSquare(const Ray& s1, const Ray& s2) {
    Vector3   u = s1.end - s1.start;
    Vector3   v = s2.end - s2.start;
    Vector3   w = s1.start - s2.start;
    float    a = Vector3::dot(u, u);         // always >= 0
    float    b = Vector3::dot(u, v);
    float    c = Vector3::dot(v, v);         // always >= 0
    float    d = Vector3::dot(u, w);
    float    e = Vector3::dot(v, w);
    float    D = a * c - b * b;        // always >= 0
    float    sc, sN, sD = D;       // sc = sN / sD, default sD = D >= 0
    float    tc, tN, tD = D;       // tc = tN / tD, default tD = D >= 0

                                   // compute the line parameters of the two closest points
    if (Math::nearZero(D)) { // the lines are almost parallel
        sN = 0.0;         // force using point P0 on segment S1
        sD = 1.0;         // to prevent possible division by 0.0 later
        tN = e;
        tD = c;
    } else {                 // get the closest points on the infinite lines
        sN = (b * e - c * d);
        tN = (a * e - b * d);
        if (sN < 0.0) {        // sc < 0 => the s=0 edge is visible
            sN = 0.0;
            tN = e;
            tD = c;
        } else if (sN > sD) {  // sc > 1  => the s=1 edge is visible
            sN = sD;
            tN = e + b;
            tD = c;
        }
    }

    if (tN < 0.0) {            // tc < 0 => the t=0 edge is visible
        tN = 0.0;
        // recompute sc for this edge
        if (-d < 0.0)
            sN = 0.0;
        else if (-d > a)
            sN = sD;
        else {
            sN = -d;
            sD = a;
        }
    } else if (tN > tD) {      // tc > 1  => the t=1 edge is visible
        tN = tD;
        // recompute sc for this edge
        if ((-d + b) < 0.0)
            sN = 0;
        else if ((-d + b) > a)
            sN = sD;
        else {
            sN = (-d + b);
            sD = a;
        }
    }
    // finally do the division to get sc and tc
    sc = (Math::nearZero(sN) ? 0.0f : sN / sD);
    tc = (Math::nearZero(tN) ? 0.0f : tN / tD);

    // get the difference of the two closest points
    Vector3   dP = w + (sc * u) - (tc * v);  // =  S1(sc) - S2(tc)

    return dP.lengthSq();   // return the closest distance squared
}



Circle::Circle() :
    center(Vector2::zero),
    radius(0.f) {
}

Circle::Circle(const Vector2& center, float radius) :
    center(center),
    radius(radius) {
}

void Circle::set(const Vector2& center, float radius) {
    this->center = center;
    this->radius = radius;
}

bool Circle::contains(const Vector2& point) const {
    //中心と点との距離の2乗を求める
    float distSq = (center - point).lengthSq();
    return distSq <= (radius * radius);
}



Sphere::Sphere() :
    center(Vector3::zero),
    radius(0.f) {
}

Sphere::Sphere(const Vector3& center, float radius) :
    center(center),
    radius(radius) {
}

void Sphere::set(const Vector3& center, float radius) {
    this->center = center;
    this->radius = radius;
}

bool Sphere::contains(const Vector3& point) const {
    //中心と点との距離の2乗を求める
    float distSq = (center - point).lengthSq();
    return distSq <= (radius * radius);
}



bool intersect(const Circle& a, const Circle& b) {
    Vector2 dist = a.center - b.center;
    float distSq = dist.lengthSq();
    float sumRadius = a.radius + b.radius;
    return distSq <= (sumRadius * sumRadius);
}

bool intersect(const Sphere& a, const Sphere& b) {
    Vector3 dist = a.center - b.center;
    float distSq = dist.lengthSq();
    float sumRadii = a.radius + b.radius;
    return distSq <= (sumRadii * sumRadii);
}

bool intersect(const Ray& r, const Sphere& s, float* outT) {
    //方程式のX, Y, a, b, cを計算
    Vector3 X = r.start - s.center;
    Vector3 Y = r.end - r.start;
    float a = Vector3::dot(Y, Y);
    float b = 2.0f * Vector3::dot(X, Y);
    float c = Vector3::dot(X, X) - s.radius * s.radius;
    //判別式を計算
    float disc = b * b - 4.0f * a * c;
    if (disc < 0.0f) {
        return false;
    } else {
        disc = Math::sqrt(disc);
        //tの解(minとmax)を求める
        float tMin = (-b - disc) / (2.0f * a);
        float tMax = (-b + disc) / (2.0f * a);
        //tが線分の領域にあるのかチェック
        if (0.f <= tMin && tMin <= 1.0f) {
            *outT = tMin;
            return true;
        } else if (0.f <= tMax && tMax <= 1.0f) {
            *outT = tMax;
            return true;
        } else {
            return false;
        }
    }
}

bool testSidePlane(float start, float end, float negd, const Vector3& norm, std::vector<std::pair<float, Vector3>>* out) {
    float denom = end - start;
    if (Math::nearZero(denom)) {
        return false;
    } else {
        float numer = -start + negd;
        float t = numer / denom;
        //tが範囲内にあることをテスト
        if (0.f <= t && t <= 1.0f) {
            out->emplace_back(t, norm);
            return true;
        } else {
            return false;
        }
    }
}

bool SweptSphere(const Sphere& P0, const Sphere& P1, const Sphere& Q0, const Sphere& Q1, float* outT) {
    //X, Y, a, b, cを計算
    Vector3 X = P0.center - Q0.center;
    Vector3 Y = P1.center - P0.center - (Q1.center - Q0.center);
    float a = Vector3::dot(Y, Y);
    float b = 2.0f * Vector3::dot(X, Y);
    float sumRadii = P0.radius + Q0.radius;
    float c = Vector3::dot(X, X) - sumRadii * sumRadii;
    //判別式を解く
    float disc = b * b - 4.0f * a * c;
    if (disc < 0.0f) {
        return false;
    } else {
        disc = Math::sqrt(disc);
        //小さい方の解だけが重要
        *outT = (-b - disc) / (2.0f * a);
        if (0.f <= *outT && *outT <= 0.0f) {
            return true;
        } else {
            return false;
        }
    }
}
