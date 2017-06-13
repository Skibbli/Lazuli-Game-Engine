#ifndef CORE_H
#define CORE_H

#include "stdafx.h"

#include "Input.h"
#include "MeshRenderer.h"
#include "Screen.h"
#include "GameObject.h"
#include "Transform.h"
#include "Camera.h"
#include "Resources.h"
#include "Physics.h"
#include "LightManager.h"
#include "UI.h"


class Core
{
	public:
		Core();
		~Core();

		/// Initialises the necessary parts of the engine
		void Initialise();
		/// Initiates the main game loop
		void Run();
		/// Shuts the engine down and cleans up
		void Shutdown();

		/// List of all the game objects in the scene
		static std::vector<shared<GameObject>>& GetGameObjects();
		/// List of all the cameras in the scene
		static std::vector<weak<Camera>>& GetCameras();
		/// List of all the lights in the scene
		static LightManager& GetLightManager();
		/// Add collider to physics engine
		static void AddCollider(weak<btRigidBody> _coll);

		/// Reloads the shaders while the engine is running, to speed up shader development
		void ReloadShaders();

	private:
		bool m_running;

		/// List of all game objects in the scene
		static std::vector<shared<GameObject>> m_gameObjects;
		/// List of cameras in the scene
		static std::vector<weak<Camera>> m_cameras;

		/// The screen
		Screen m_screen;

		ALLEGRO_EVENT_QUEUE* m_eventQueue;
		ALLEGRO_EVENT m_event;
		ALLEGRO_TIMER *m_timer;

		/// The static physics engine that handles colliders in the scene
		static Physics m_physicsEngine;
		/// The static light manager that manages the lights in the scene
		static LightManager m_lightManager;

		ALLEGRO_FONT* m_font;
		ALLEGRO_BITMAP* m_bitmap;

		GLint bmpGL;

		UI theUI;
};

#endif