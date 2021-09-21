#pragma once

#include <glm.hpp>
#include "RigidBody.h"
#include <functional>

struct Manifold
{
	RigidBody* A;
	RigidBody* B;
	glm::vec2 Normal;
	float Penetration;
	glm::vec2 Point;
};

class CollisionTest
{
public:
	CollisionTest() = delete;

	static bool Intersect(Manifold* manifold);

public:
	static bool Circle_vs_Circle(Manifold* manifold);
	static bool AABB_vs_AABB(Manifold* manifold);

	static bool Circle_vs_AABB(Manifold* manifold);

	static bool NO_IMP(Manifold* manifold) { assert(0); return false; }
	static bool InvertIntersect(Manifold* manifold);

	static void ResolveCollision(Manifold* manifold);
	static void Solve(Manifold* manifold, float dt);

	//static std::function<CollisionResult(RigidBody*, RigidBody*)> IntersectFunctions[(int)RigidBody::TYPE::TYPE_COUNT][(int)RigidBody::TYPE::TYPE_COUNT]
	//{
	//	Circle_vs_Circle,	NO_IMP,				NO_IMP,
	//	NO_IMP,				AABB_vs_AABB,		NO_IMP,
	//	NO_IMP,				NO_IMP,				NO_IMP,
	//};
};