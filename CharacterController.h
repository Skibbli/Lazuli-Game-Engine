#ifndef CHARACTER_CONTROLLER_H
#define CHARACTER_CONTROLLER_H

#include "stdafx.h"

#include "Transform.h"
#include "Component.h"
#include "BoxCollider.h"
#include "Camera.h"


/// Used for controlling the character in the scene when playing in single-player
class CharacterController : public Component
{
	public:
		CharacterController();
		~CharacterController();

		void Awake();
		void Start();
		void Update();

	private:
		/// The transform of the game object this controller is attached to
		weak<Transform> m_transform;
		/// The collider of the model on this game object
		weak<BoxCollider> m_collider;
		
		/// The transform of the camera in the scene
		weak<Transform> m_cameraTransform;

		bool m_forward, m_backward;

		/// The speed the character can move at
		float m_speed;
};

#endif