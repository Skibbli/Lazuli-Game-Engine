#ifndef ENGINE_TRANSFORM_H
#define ENGINE_TRANSFORM_H

#include "stdafx.h"

#include "Component.h"
#include "glm.hpp"


class Transform : public Component
{
	public:
		Transform();
		~Transform();

		/// Returns the local position of the game object this transform is attached to
		Vec3 GetLocalPosition();
		/// Returns the local scale of the game object this transform is attached to
		Vec3 GetLocalScale();
		/// Returns the local rotation of the game object this transform is attached to
		Quat GetLocalRotation();

		/// Returns the world position of the game object this transform is attached to
		Vec3 GetWorldPosition();
		/// Returns the world scale of the game object this transform is attached to
		Vec3 GetWorldScale();
		/// Returns the world rotation of the game object this transform is attached to
		Quat GetWorldRotation();

		/// Sets the local position of the game object this transform is attached to
		void SetLocalPosition(Vec3 _pos);
		/// Sets the local scale of the game object this transform is attached to
		void SetLocalScale(Vec3 _scale);
		/// Sets the local rotation of the game object this transform is attached to
		void SetLocalRotation(Quat _rot);

		/// Sets the world position of the game object this transform is attached to
		void SetWorldPosition(Vec3 _pos);
		void SetWorldPositionFromPhysics(Vec3 _pos);
		/// Sets the world rotation of the game object this transform is attached to
		void SetWorldRotation(Quat _rot);
		void SetWorldRotation(Vec3 _rot);
		void SetWorldRotationFromPhysics(btQuaternion &_rot);

		/// Increases the scale of the transform
		void ChangeScale(float _scale);

		/// Translates object by given translation
		void Translate(Vec3 _translation);
		/// Rotates object by given rotation
		void Rotate(Quat _rot);
		/// Rotates object by given rotation
		void Rotate(float _angle, Vec3 _axis);
		/// Scales object by given scale
		void LookAt(Vec3 _worldPos);

		virtual void Awake();
		virtual void Start();
		virtual void Destroy();

		/// Sets parent transform object
		void SetParent(weak<Transform> _transform);
		/// Gets parent transform object
		weak<Transform> GetParent();
		/// Adds child transform to this object
		void AddChild(weak<Transform> _transform);
		/// Removes all child transform from this object
		void DetachChildren();

		void MoveForward(float _dist);
		void MoveBackward(float _dist);
		void MoveLeft(float _dist);
		void MoveRight(float _dist);
		void MoveUp(float _dist);
		void MoveDown(float _dist);

		void RollLeft(float _angle);
		void RollRight(float _angle);
		void PitchUp(float _angle);
		void PitchDown(float _angle);
		void YawLeft(float _angle);
		void YawRight(float _angle);

		Mat4 GetModelMatrix();

		bool ManuallyMoved();
		void SetManMoved(bool _moved);

	private:
		/// The local position of the object
		Vec3 m_localPosition;
		/// The local scale of the object
		Vec3 m_localScale;
		/// The orientation of the object
		glm::quat m_localRotation;

		/// The parent of this transform
		weak<Transform> m_parent;
		/// A list of the transforms that are children of this transform
		std::vector<weak<Transform>> m_children;

		/// The model matrix
		Mat4 m_modelMat;

		/// Whether this transform was manually moved outside of the physics engine
		bool m_manualMove;
};

#endif