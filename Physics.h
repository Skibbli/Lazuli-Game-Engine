#ifndef ENGINE_PHYSICS_H
#define ENGINE_PHYSICS_H

#include "stdafx.h"
#include "DebugDrawer.h"
#include "MotionState.h"


class Collider;

class Physics
{
	public:
		Physics();
		~Physics();

		/// Initialises the physics world
		void Initialise();

		/// Runs the next simulation of the physics in the scene
		void Update(float _dt, int _maxStep);

		/// Adds a collider to the physics world, to be included in future simulations
		void AddCollider(weak<btRigidBody> _coll);
		/// Draws the collision boxes of the colliders in the scene
		void DrawDebugWorld();

	private:
		/// List of the colliders in the scene
		std::vector<weak<btCollisionObject>> m_colliders;

		btBroadphaseInterface *m_broadphase;
		btDefaultCollisionConfiguration *m_collisionConfiguration;
		btCollisionDispatcher *m_dispatcher;
		btSequentialImpulseConstraintSolver *m_solver;
		/// Stores the colliders in the scene and performs physics updates on them
		btDiscreteDynamicsWorld *m_dynamicsWorld;

		/// Stores the details to render the collision boxes of the colliders in the scene
		GLDebugDrawer m_debugDrawer;
};

#endif