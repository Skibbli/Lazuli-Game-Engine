#include "stdafx.h"

#include "Physics.h"
#include "Collider.h"
#include "Input.h"

#include "CustomMotionState.h"


Physics::Physics()
{

}

/// Sets up the physics world
void Physics::Initialise()
{
	// Specify the dynamic AABB tree broadphase algorithm to be used to work out what objects
	// to test collision for.
	m_broadphase = new btDbvtBroadphase();

	// The collision configuration allows you to fine tune the algorithms used
	// for the full (not broadphase) collision detection
	m_collisionConfiguration = new btDefaultCollisionConfiguration();

	m_dispatcher =	new btCollisionDispatcher(m_collisionConfiguration);

	// We also need a "solver". This is what causes the objects to interact
	// properly, taking into account gravity, game logic supplied forces,
	// collisions, and hinge constraints. It does a good job as long as you
	// don't push it to extremes, and is one of the bottlenecks in any high
	// performance simulation. There are parallel versions available for some
	// threading models.
	m_solver = new btSequentialImpulseConstraintSolver();

	// Now, we can finally instantiate the dynamics world.
	m_dynamicsWorld = new btDiscreteDynamicsWorld(m_dispatcher, m_broadphase, m_solver,
		m_collisionConfiguration);

	// Set the gravity. We have chosen the Y axis to be "up".
	m_dynamicsWorld->setGravity(btVector3(0, -9.8, 0));

	m_debugDrawer.Initialise();
	m_debugDrawer.setDebugMode(btIDebugDraw::DBG_DrawWireframe);
	m_dynamicsWorld->setDebugDrawer(&m_debugDrawer);
}

/// Checks for position changes and simulates the physics world
void Physics::Update(float _dt, int _maxStep)
{
	/// Check each collider to see if it was manually moved outside the collider. If it was moved, update the collider
	/// with the new transform details
	for(weak<btCollisionObject> colObj : m_colliders)
	{
		Transform *trans = (Transform*)colObj.lock()->getUserPointer();	

		if(trans->ManuallyMoved())
		{
			btRigidBody* body = btRigidBody::upcast(colObj.lock().get());
			btTransform transform;
			body->getMotionState()->getWorldTransform(transform);
			transform.setOrigin(GLMtoBT(trans->GetWorldPosition()));
			transform.setRotation(GLMtoBT(trans->GetWorldRotation()));
			body->getMotionState()->setWorldTransform(transform);
			body->setWorldTransform(transform);
			trans->SetManMoved(false);
		}
	}

	m_dynamicsWorld->stepSimulation(_dt, 10);
}

/// Draws the colliders
void Physics::DrawDebugWorld()
{
	m_dynamicsWorld->debugDrawWorld();
}

/// Adds a collider to the physics simulation
void Physics::AddCollider(weak<btRigidBody> _coll)
{
	m_dynamicsWorld->addRigidBody(_coll.lock().get());
	m_colliders.push_back(_coll);
}

Physics::~Physics()
{
	delete m_dynamicsWorld;
    delete m_solver;
    delete m_dispatcher;
    delete m_collisionConfiguration;
    delete m_broadphase;
}