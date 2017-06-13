#ifndef ENGINE_BASE_OBJ_H
#define ENGINE_BASE_OBJ_H

#include "stdafx.h"

/// Base class that provides the shared functions of game objects and components
class BaseObj
{
	public:
		BaseObj();
		virtual ~BaseObj();

		virtual void Awake() {};
		virtual void Start() {};
		virtual void Update() {};
		virtual void Render() {};
		virtual void Destroy() {};

		/// Returns whether the object is active or not
		bool IsActive();
		/// Sets the active flag of the object
		void SetActive(bool _active);

	protected:
		/// Whether the object is active or not
		bool m_isActive;
};

#endif
