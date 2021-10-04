#include "CollisionTest.h"

#include <iostream>

#include "Circle.h"
#include "AABB.h"

#include "Debug/ConsoleLog.h"

std::function<bool(Manifold*)> IntersectFunctions[(int)RigidBody::TYPE::TYPE_COUNT][(int)RigidBody::TYPE::TYPE_COUNT]
{
	CollisionTest::Circle_vs_Circle,	CollisionTest::Circle_vs_AABB,		CollisionTest::NO_IMP,
	CollisionTest::InvertIntersect,		CollisionTest::AABB_vs_AABB,		CollisionTest::NO_IMP,
	CollisionTest::NO_IMP,				CollisionTest::NO_IMP,				CollisionTest::NO_IMP,
};




bool CollisionTest::Intersect(Manifold* manifold)
{
	return IntersectFunctions[(int)manifold->A->GetType()][(int)manifold->B->GetType()](manifold);
}


bool CollisionTest::InvertIntersect(Manifold* manifold)
{
	std::swap(manifold->A, manifold->B);
	return Intersect(manifold);
}


bool CollisionTest::Circle_vs_Circle(Manifold* manifold)
{
	Circle* A = (Circle*)manifold->A;
	Circle* B = (Circle*)manifold->B;

	glm::vec2 A_B = B->GetPosition() - A->GetPosition();
	float AB = glm::dot(A_B, A_B);

	float RA_RB = A->GetRadius() + B->GetRadius();

	if (!(AB < RA_RB * RA_RB))
		return false;

	AB = sqrt(AB);
	manifold->Normal = A_B / AB;
	manifold->Penetration = RA_RB - AB;

	return true;
}


bool CollisionTest::AABB_vs_AABB(Manifold* manifold)
{
	AABB* A = (AABB*)manifold->A;
	AABB* B = (AABB*)manifold->B;
	glm::vec2 N = B->GetPosition() - A->GetPosition();
	glm::vec2 D = A->GetHalfSize() + B->GetHalfSize() - glm::abs(N);


	if (D.x <= 0.0f || D.y <= 0.0f)
		return false;

	if (D.x < D.y)
	{
		manifold->Penetration = D.x;
		manifold->Normal = glm::vec2(N.x < 0.0f ? -1.0f : 1.0f, 0.0f);
	}
	else
	{
		manifold->Penetration = D.y;
		manifold->Normal = glm::vec2(0.0f, N.y < 0.0f ? -1.0f : 1.0f);
	}

	return true;
}


bool CollisionTest::Circle_vs_AABB(Manifold* manifold)
{
	Circle* A = (Circle*)manifold->A;
	AABB* B = (AABB*)manifold->B;
	
	glm::vec2 Nearest = glm::clamp(A->GetPosition(), B->GetPosition() - B->GetHalfSize(), B->GetPosition() + B->GetHalfSize());

	glm::vec2 AN = Nearest - A->GetPosition();
	float D = glm::dot(AN, AN);

	if (!(D < A->GetRadius() * A->GetRadius()))
		return false;

	D = sqrt(D);
	manifold->Normal = AN / D;
	manifold->Penetration = D - A->GetRadius();

	return true;
}


void CollisionTest::ResolveCollision(Manifold* manifold)
{
	float Restitution = 1.0f;

	glm::vec2 Vr = manifold->B->GetVelocity() - manifold->A->GetVelocity();

	float VelAlongNormal = glm::dot(Vr, manifold->Normal);

	if (VelAlongNormal > 0.0f)
		return;

	glm::vec2 I = (1.0f + Restitution) * manifold->Normal * VelAlongNormal / (manifold->A->GetInvMass() + manifold->B->GetInvMass());

	LOGI(manifold->Normal.x, " ", manifold->Normal.y);

	manifold->A->AddVelocity( I * manifold->A->GetInvMass());
	manifold->B->AddVelocity(-I * manifold->B->GetInvMass());

	//manifold->A->AddVelocity(manifold->Normal * (-2.0f * glm::dot(manifold->Normal, manifold->A->GetVelocity())));
	//manifold->B->AddVelocity(manifold->Normal * (-2.0f * glm::dot(manifold->Normal, manifold->B->GetVelocity())));
}


void CollisionTest::Solve(Manifold* manifold, float dt)
{
	glm::vec2 UnitAxis = -manifold->Normal;
 
	float RelVel = glm::dot(manifold->B->GetVelocity() - manifold->A->GetVelocity(), UnitAxis);
 
	float Remove = RelVel + manifold->Penetration / dt;
	float Impulse = Remove / (manifold->A->GetInvMass() + manifold->B->GetInvMass());
 
	glm::vec2 I = UnitAxis * Impulse;
 
	manifold->A->AddVelocity( I * manifold->A->GetInvMass());
	manifold->B->AddVelocity(-I * manifold->B->GetInvMass());

	//ApplyImpulse(I);
}