#pragma once

#include "../Math/Math.h"

struct AABB;
struct Circle;
struct Sphere;
struct Triangle;
struct Ray;
class Transform3D;
class IMesh;

namespace Intersect {
//円同士の衝突判定を行う
bool intersectCircle(const Circle& a, const Circle& b);

//球同士の衝突判定を行う
bool intersectSphere(const Sphere& a, const Sphere& b);

//AABB同士の衝突判定を行う
bool intersectAABB(const AABB& a, const AABB& b);

//無限平面とレイの衝突判定を行う
bool intersectRayPlane(const Ray& ray, const Plane& p, Vector3* intersectPoint = nullptr);

//ポリゴンとレイの衝突判定を行う
bool intersectRayPolygon(const Ray& ray, const Vector3& p0, const Vector3& p1, const Vector3& p2, Vector3* intersectPoint = nullptr);
bool intersectRayPolygon(const Ray& ray, const Triangle& polygon, Vector3* intersectPoint = nullptr);

//球とレイの衝突判定を行う
bool intersectRaySphere(const Ray& ray, const Sphere& sphere, Vector3& intersectPoint);
//球とレイの衝突判定を複数回に分けて行う
//かなり重い
bool intersectRaySphere(const Ray& ray, const Sphere& sphere, int numDivision);

//AABBとレイの衝突判定を行う
bool intersectRayAABB(const Ray& ray, const AABB& aabb);
bool intersectRayAABB(const Ray& ray, const AABB& aabb, Vector3& intersectPoint);

//メッシュとレイの衝突判定を行う
//めちゃめちゃ重い
bool intersectRayMesh(const Ray& ray, const IMesh& mesh, const Transform3D& transform, Vector3* intersectPoint = nullptr, Triangle* intersectPolygon = nullptr);
};
