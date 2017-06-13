#ifndef CUSTOM_CODE_HANDLER_H
#define CUSTOM_CODE_HANDLER_H

#include "stdafx.h"

#include "Component.h"
#include "GameObject.h"

/// A class for developers to put their own code in (similar to Unity scripts)
class CustomCodeHandler : public Component
{
	public:
		CustomCodeHandler();
		~CustomCodeHandler();

		void Awake();
		void Start();
		void Update();
		void Destroy();

	private:
		/// The game object this custom code is attached to
		weak<GameObject> m_gameObject;
};

#endif