#include "Intersect.h"
#include "AABB.h"
#include "Circle.h"
#include "Ray.h"
#include "Sphere.h"
#include "Square.h"
#include "Triangle.h"
#include "../Mesh/IMesh.h"
#include "../Transform/Transform3D.h"
#include <algorithm>
#include <vector>

bool Intersect::intersectCircle(const Circle& a, const Circle& b) {
    Vector2 dist = a.center - b.center;
    float distSq = dist.lengthSq();
    float sumRadius = a.radius + b.radius;
    return distSq <= (sumRadius * sumRadius);
}

bool Intersect::intersectSphere(const Sphere& a, const Sphere& b) {
    Vector3 dist = a.center - b.center;
    float distSq = dist.lengthSq();
    float sumRadii = a.radius + b.radius;
    return distSq <= (sumRadii * sumRadii);
}

bool Intersect::intersectAABB(const AABB& a, const AABB& b) {
    bool no = (
        a.max.x < b.min.x ||
        a.max.y < b.min.y ||
        a.max.z < b.min.z ||
        b.max.x < a.min.x ||
        b.max.y < a.min.y ||
        b.max.z < a.min.z
        );
    //いずれにも当てはまらなければ衝突している
    return !no;
}

bool Intersect::intersectRayPlane(const Ray& ray, const Plane& p, Vector3* intersectPoint) {
    //tの解決策があるかどうかの最初のテスト
    float denom = Vector3::dot(ray.end - ray.start, p.normal());
    if (Math::nearZero(denom)) {
        //交差するのは、開始が平面上の点(P dot N) == dである場合だけ
        return (Math::nearZero(Vector3::dot(ray.start, p.normal()) - p.d));
    }

    float numer = -Vector3::dot(ray.start, p.normal()) - p.d;
    float t = numer / denom;
    //tが線分の範囲内にあるか
    if (t >= 0.f && t <= 1.f) {
        //衝突点を取得する
        if (intersectPoint) {
            *intersectPoint = ray.pointOnSegment(t);
        }
        return true;
    }

    //衝突していない
    return false;
}

bool Intersect::intersectRayPolygon(const Ray& ray, const Vector3& p0, const Vector3& p1, const Vector3& p2, Vector3* intersectPoint) {
    //まずは無限平面でテストする
    Plane plane(p0, p1, p2);
    Vector3 interPoint{};
    if (!intersectRayPlane(ray, plane, &interPoint)) {
        return false;
    }

    //各辺ベクトルを求める
    auto ab = p1 - p0;
    auto bc = p2 - p1;
    auto ca = p0 - p2;

    //各辺ベクトルと各頂点から交点へ向かうベクトルの外積を求める
    auto crossAB = Vector3::cross(ab, interPoint - p0);
    auto crossBC = Vector3::cross(bc, interPoint - p1);
    auto crossCA = Vector3::cross(ca, interPoint - p2);

    //それぞれの外積ベクトルとの内積を計算する
    auto dotAB = Vector3::dot(plane.normal(), crossAB);
    auto dotBC = Vector3::dot(plane.normal(), crossBC);
    auto dotCA = Vector3::dot(plane.normal(), crossCA);

    //3つの内積結果のうち1つでもマイナス符号のものがあれば交点は外にある
    if (!(dotAB < 0.f || dotBC < 0.f || dotCA < 0.f)) {
        if (intersectPoint) {
            *intersectPoint = interPoint;
        }
        return true;
    }

    return false;
}

bool Intersect::intersectRayPolygon(const Ray& ray, const Triangle& polygon, Vector3* intersectPoint) {
    return intersectRayPolygon(ray, polygon.p0, polygon.p1, polygon.p2, intersectPoint);
}

bool Intersect::intersectRaySphere(const Ray& ray, const Sphere& sphere, Vector3& intersectPoint) {
    //方程式のX, Y, a, b, cを計算
    Vector3 X = ray.start - sphere.center;
    Vector3 Y = ray.end - ray.start;
    float a = Vector3::dot(Y, Y);
    float b = 2.f * Vector3::dot(X, Y);
    float c = Vector3::dot(X, X) - sphere.radius * sphere.radius;
    //判別式を計算
    float disc = b * b - 4.f * a * c;
    if (disc < 0.f) {
        return false;
    }

    disc = Math::sqrt(disc);
    //tの解(minとmax)を求める
    float tMin = (-b - disc) / (2.f * a);
    float tMax = (-b + disc) / (2.f * a);
    //tが線分の領域にあるのかチェック
    if (tMin >= 0.f && tMin <= 1.f) {
        intersectPoint = ray.pointOnSegment(tMin);
        return true;
    } else if (tMax >= 0.f && tMax <= 1.f) {
        intersectPoint = ray.pointOnSegment(tMax);
        return true;
    }

    return false;
}

bool Intersect::intersectRaySphere(const Ray& ray, const Sphere& sphere, int numDivision) {
    Vector3 intersectPoint;

    //分割数が0以下なら1回で終了
    if (numDivision <= 0) {
        return intersectRaySphere(ray, sphere, intersectPoint);
    }

    Ray r;
    //レイを分割する
    auto rayDiv = (ray.end - ray.start) / numDivision;
    for (int i = 0; i < numDivision; ++i) {
        r.start = ray.start + rayDiv * i;
        r.end = r.start + rayDiv;
        if (intersectRaySphere(r, sphere, intersectPoint)) {
            return true;
        }
    }

    return false;
}

bool testSidePlane(float start, float end, float negd, std::vector<float>& out) {
    float denom = end - start;
    if (Math::nearZero(denom)) {
        return false;
    }

    float numer = -start + negd;
    float t = numer / denom;
    //tが線分の範囲内にあるか
    if (t >= 0.f && t <= 1.f) {
        out.emplace_back(t);
        return true;
    }

    //範囲外
    return false;
}

bool Intersect::intersectRayAABB(const Ray& ray, const AABB& aabb) {
    Vector3 temp;
    return intersectRayAABB(ray, aabb, temp);
}

bool Intersect::intersectRayAABB(const Ray& ray, const AABB& aabb, Vector3& intersectPoint) {
    //すべてのt値を格納する
    std::vector<float> tValues;
    //x平面テスト
    testSidePlane(ray.start.x, ray.end.x, aabb.min.x, tValues);
    testSidePlane(ray.start.x, ray.end.x, aabb.max.x, tValues);
    //y平面テスト
    testSidePlane(ray.start.y, ray.end.y, aabb.min.y, tValues);
    testSidePlane(ray.start.y, ray.end.y, aabb.max.y, tValues);
    //z平面テスト
    testSidePlane(ray.start.z, ray.end.z, aabb.min.z, tValues);
    testSidePlane(ray.start.z, ray.end.z, aabb.max.z, tValues);

    //t値を昇順で並べ替える
    std::sort(tValues.begin(), tValues.end(), [](
        float a,
        float b
        ) {
            return a < b;
        });

    //ボックスに交点が含まれているか調べる
    for (const auto& t : tValues) {
        auto point = ray.pointOnSegment(t);
        if (aabb.contains(point)) {
            intersectPoint = point;
            return true;
        }
    }

    //衝突していない
    return false;
}

bool Intersect::intersectRayMesh(const Ray& ray, const IMesh& mesh, const Transform3D& transform, Vector3* intersectPoint, Triangle* intersectPolygon) {
    //ワールド行列を先に取得しておく
    const auto& world = transform.getWorldTransform();

    //衝突した点との最小距離、点
    float minDistance = FLT_MAX;
    Vector3 minPoint{};
    Triangle minPoly{};
    //一回でも衝突したか
    bool isIntersect = false;

    //すべてのメッシュとレイによる判定を行う
    for (unsigned i = 0; i < mesh.getMeshCount(); ++i) {
        const auto& meshVertices = mesh.getMeshVertices(i);
        const auto& meshIndices = mesh.getMeshIndices(i);
        const auto polygonCount = mesh.getPolygonCount(i);
        for (unsigned j = 0; j < polygonCount; ++j) {
            //ワールド行列乗算済みポリゴンを取得する
            const auto& polygon = mesh.getPolygon(i, j, world);

            //ポリゴンとレイの衝突判定
            if (Vector3 interPoint{}; Intersect::intersectRayPolygon(ray, polygon, &interPoint)) {
                float dist = (interPoint - ray.start).lengthSq();
                //既存の距離より近いなら
                if (dist < minDistance) {
                    //最小記録を更新
                    minDistance = dist;
                    minPoint = interPoint;
                    minPoly = polygon;
                }

                isIntersect = true;
            }
        }
    }

    //一回でも衝突してたら最小距離の点を設定し終了
    if (!Math::equal(minDistance, FLT_MAX)) {
        if (intersectPoint) {
            *intersectPoint = minPoint;
        }
        if (intersectPolygon) {
            *intersectPolygon = minPoly;
        }
    }

    return isIntersect;
}
