#include "stdafx.h"

#include "Core.h"
#include "MeshRenderer.h"
#include "SphereCollider.h"
#include "BoxCollider.h"
#include <iostream>
#include "Resources.h"
#include "Light.h"
#include "Time.h"
#include "CustomCodeHandler.h"
#include "AudioSource.h"
#include "CharacterController.h"
#include "NetworkInterface.h"
#include "NetworkServer.h"

std::vector<shared<GameObject>> Core::m_gameObjects;
std::vector<weak<Camera>> Core::m_cameras;
LightManager Core::m_lightManager;
Physics Core::m_physicsEngine;


Core::Core()
{
	
}

void Core::Initialise()
{
	/// Initialise Allegro 5
	m_running = true;
	al_init();
	al_init_image_addon();
	al_install_audio();
	al_init_acodec_addon();
	al_init_font_addon();
	al_init_ttf_addon();

	/// Create the screen, initialise the input and start the time class
	m_screen.InitialiseScreen(800, 800);
	Engine::Input::Initialise(m_screen.GetDisplay());
	Time::StartTime();

	/// Initialise OpenGL
	glewExperimental = GL_TRUE;

	GLenum err = glewInit();
	if( GLEW_OK != err )
	{
		/* Problem: glewInit failed, something is seriously wrong. */
		std::cerr<<"Error: GLEW failed to initialise with message: "<< glewGetErrorString(err) <<std::endl;
	}

	std::cout<<"INFO: Using GLEW "<< glewGetString(GLEW_VERSION)<<std::endl;

	std::cout<<"INFO: OpenGL Vendor: "<< glGetString( GL_VENDOR ) << std::endl;
	std::cout<<"INFO: OpenGL Renderer: "<< glGetString( GL_RENDERER ) << std::endl;
	std::cout<<"INFO: OpenGL Version: "<< glGetString( GL_VERSION ) << std::endl;
	std::cout<<"INFO: OpenGL Shading Language Version: "<< glGetString( GL_SHADING_LANGUAGE_VERSION ) << std::endl;

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_DEPTH);
	glEnable(GL_TEXTURE_2D);
    glViewport(0,0,800,800);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0,800,800,0,-200,200);
    glClearColor(0,0,0,1);

	Resources::Initialise();

	//Must init physics after resources
	m_physicsEngine.Initialise();

	m_eventQueue = al_create_event_queue();

	m_timer = nullptr;

	m_timer = al_create_timer(1.0 / 90);

	al_register_event_source(m_eventQueue, al_get_timer_event_source(m_timer));
	al_start_timer(m_timer);
}

/// Get the game objects in the scene
std::vector<shared<GameObject>>& Core::GetGameObjects()
{
	return m_gameObjects;
}

/// Get the cameras in the scene
std::vector<weak<Camera>>& Core::GetCameras()
{
	return m_cameras;
}

/// Game loop
void Core::Run()
{
	/// Create the scene
	shared<GameObject> curuthers = GameObject::CreateGameObj("Irving");
	//curuthers->AddComponent<NetworkInterface>();
	curuthers->AddComponent<CharacterController>();

	

	shared<GameObject> floor = GameObject::CreateGameObj("House1");
	floor->LoadModel("FloorBox.obj");
	floor->GetComponent<Transform>().lock()->SetWorldPosition(Vec3(0,-10,0));
	floor->GetComponent<Transform>().lock()->SetLocalScale(Vec3(0.1,0.1,0.1));
	floor->AddComponent<BoxCollider>().lock()->CreateCollider(600,0.5,600,0);

	shared<GameObject> table1 = GameObject::CreateGameObj("House1");
	table1->LoadModel("Table.obj");
	table1->GetComponent<Transform>().lock()->SetWorldPosition(Vec3(0,-10,0));
	table1->GetComponent<Transform>().lock()->SetLocalScale(Vec3(0.1,0.1,0.1));
	table1->AddComponent<BoxCollider>().lock()->CreateCollider(6,3,2, 0);

	shared<GameObject> table2 = GameObject::CreateGameObj("House1");
	table2->LoadModel("Table.obj");
	table2->GetComponent<Transform>().lock()->SetWorldPosition(Vec3(0,-10,0));
	table2->GetComponent<Transform>().lock()->SetLocalScale(Vec3(0.1,0.1,0.1));
	table2->AddComponent<BoxCollider>().lock()->CreateCollider(6,3,2, 0);

	shared<GameObject> table3 = GameObject::CreateGameObj("House1");
	table3->LoadModel("Table.obj");
	table3->GetComponent<Transform>().lock()->SetWorldPosition(Vec3(0,-10,0));
	table3->GetComponent<Transform>().lock()->SetLocalScale(Vec3(0.1,0.1,0.1));
	table3->AddComponent<BoxCollider>().lock()->CreateCollider(6,3,2, 0);

	shared<GameObject> pointLight = GameObject::CreateGameObj("PointLight");
	pointLight->AddComponent<PointLight>().lock()->SetLightParams(Vec3(1.0f), 1.0f, 0.09f, 0.032f);
	pointLight->GetComponent<Transform>().lock()->SetWorldPosition(Vec3(0,10,0));

	shared<GameObject> skybox = GameObject::CreateGameObj("Skybox");
	skybox->LoadModel("Skybox.obj");
	skybox->AddComponent<AudioSource>().lock()->LoadAudioSample("Background_Music.ogg", false, 0);
	
	/*shared<GameObject> camera = GameObject::CreateGameObj("Camera");
	camera->AddComponent<Camera>();*/


	// https://www.allegro.cc/forums/thread/616568
	
	/// Initialise the light manager
	m_lightManager.Initialise();

	/// Awake the game objects
	for(Uint i = 0; i < m_gameObjects.size(); i++)
	{
		if(m_gameObjects.at(i)->IsActive())
			m_gameObjects.at(i)->Awake();
	}

	/// Start the game objects
	for(Uint i = 0; i < m_gameObjects.size(); i++)
	{
		if(m_gameObjects.at(i)->IsActive())
			m_gameObjects.at(i)->Start();
	}

	/// Awake the light manager (after the lights are created)
	m_lightManager.Awake();

	int activeObj = 0;

	while(m_running)
	{
		ALLEGRO_EVENT ev;
		al_wait_for_event(m_eventQueue, &ev);
		{
			m_running = Engine::Input::GetInput();

			/// Reload the shader (speeds up shader development
			if(Engine::Input::GetKey(ALLEGRO_KEY_BACKSPACE))
			{
				ReloadShaders();
			}

			Time::Update();

			/// Get any updates from the game objects
			for(Uint i = 0 ; i < m_gameObjects.size(); i++)
			{
				if(m_gameObjects.at(i)->IsActive())
					m_gameObjects.at(i)->Update();
			}

			/// Send the light parameters to the GPU for shading
			m_lightManager.Update();

			/// Update the physics world based on how much time has passed since the las frame
			m_physicsEngine.Update(Time::GetDeltaTime(), 10);

			///Specify the colour to clear the framebuffer to
			glClearColor(1.0f,0.0f,0.0f,1.0f);
	
			///Clears the colour and depth buffers
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			/// Render any game objects that have rendererable components
			for(Uint i = 0 ; i < m_gameObjects.size(); i++)
			{
				if(m_gameObjects.at(i)->IsActive())
					m_gameObjects.at(i)->Render();
			}

			//glEnable(GL_DEPTH_TEST);
			//glEnable(GL_TEXTURE_2D);

			//glMatrixMode(GL_PROJECTION);    // current matrix stack (for subsequent matrix operations) = GL_PROJECTION
			//glLoadIdentity();               // replace the current matrix with the identity matrix
			//glOrtho(0.0, (GLdouble)800, (GLdouble)800, 0.0, 0.0, 1.0);

			///*al_set_target_bitmap(m_bitmap);
			//al_clear_to_color(al_map_rgb(100,100,100));*/

			//glBindTexture  (GL_TEXTURE_2D, bmpGL);
			//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			//glPushMatrix(); // push on stack

			//glBegin(GL_QUADS);
			//glTexCoord2f(0, 0); glVertex3f(-400,   -300, 0);
			//glTexCoord2f(0, 1); glVertex3f(-400,   192, 0);
			//glTexCoord2f(1, 1); glVertex3f(256,   192, 0);
			//glTexCoord2f(1, 0); glVertex3f(256,   -300, 0);
			//glEnd();

			//glPopMatrix();  // pop off stack

			//glDisable(GL_TEXTURE_2D);
			//glDisable(GL_DEPTH_TEST);
			//glFlush();

			//m_physicsEngine.DrawDebugWorld();

			al_flip_display();
		}
	}
}

/// Add a collider the physics world
void Core::AddCollider(weak<btRigidBody> _coll)
{
	m_physicsEngine.AddCollider(_coll);
}

/// Get the light manager
LightManager& Core::GetLightManager()
{
	return m_lightManager;
}

/// Reload the shader while the engine is running to speed up shader development
void Core::ReloadShaders()
{
	Resources::ReloadShader("monolith");

	m_lightManager.ReloadShaders();

	for (shared<GameObject> obj : m_gameObjects)
	{
		if(!obj->GetComponent<MeshRenderer>().expired())
		{
			obj->GetComponent<MeshRenderer>().lock()->ReloadShader();
		}
	}
}

void Core::Shutdown()
{
	Engine::Input::Shutdown();
	Resources::Shutdown();

	/*for(int i = 0; i < m_gameObjects.size(); i++)
	{
		m_gameObjects.at(i)->Destroy();
	}*/
}

Core::~Core()
{

}