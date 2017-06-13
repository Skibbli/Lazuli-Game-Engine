#include "stdafx.h"

#include "CustomCodeHandler.h"

#include "Camera.h"
#include "Light.h"
#include "Input.h"
#include "Time.h"
#include "BoxCollider.h"
#include "PlaneCollider.h"

CustomCodeHandler::CustomCodeHandler()
{

}

CustomCodeHandler::~CustomCodeHandler()
{

}

void CustomCodeHandler::Awake()
{
	m_gameObject = GetGameObj();
	m_gameObject.lock()->LoadModel("curuthers.obj");
}

void CustomCodeHandler::Start()
{
	weak<GameObject> floor = GameObject::CreateGameObj("Floor");
	//floor.lock()->AddComponent<PlaneCollider>().lock()->CreateCollider(Vec3(0,1,0), Vec3(0,-300,-300));
	floor.lock()->AddComponent<BoxCollider>().lock()->CreateCollider(50,1,50,0);
	floor.lock()->LoadModel("Plane2.obj");

	weak<GameObject> floor2 = GameObject::CreateGameObj("Floor2");
	floor2.lock()->AddComponent<BoxCollider>().lock()->CreateCollider(50,1,50,0);
	floor2.lock()->LoadModel("Plane2.obj");
	floor2.lock()->GetComponent<Transform>().lock()->SetWorldPosition(Vec3(0.0f,0.0f,-50));
	//floor2.lock()->GetComponent<Transform>().lock()->SetWorldRotation(Vec3(90.0f,0,0.0f));
}

void CustomCodeHandler::Update()
{
	/*if(Engine::Input::GetButton(0))
	{
		glm::vec2 mouseMove = Engine::Input::GetMouseMovement();
		m_gameObject.lock()->GetComponent<Transform>().lock()->Translate(Vec3(mouseMove.x * 10, 0.0f, mouseMove.y * 10));
	}

	if(Engine::Input::GetButton(1))
	{
		glm::vec2 mouseMove = Engine::Input::GetMouseMovement();
		m_gameObject.lock()->GetComponent<Transform>().lock()->Rotate(Vec3(0.0f, mouseMove.x * 10, 0.0f));
	}

	if(Engine::Input::GetKey(ALLEGRO_KEY_W))
	{
		m_gameObject.lock()->GetComponent<Transform>().lock()->Translate(glm::vec3(0,0,-60.2525) * Time::GetDeltaTime());
	}

	if(Engine::Input::GetKey(ALLEGRO_KEY_S))
	{
		m_gameObject.lock()->GetComponent<Transform>().lock()->Translate(glm::vec3(0,0,60.2525) * Time::GetDeltaTime());
	}

	if(Engine::Input::GetKey(ALLEGRO_KEY_A))
	{
		m_gameObject.lock()->GetComponent<Transform>().lock()->Translate(glm::vec3(-60.2525,0,0) * Time::GetDeltaTime());
	}

	if(Engine::Input::GetKey(ALLEGRO_KEY_D))
	{
		m_gameObject.lock()->GetComponent<Transform>().lock()->Translate(glm::vec3(60.2525,0,0) * Time::GetDeltaTime());
	}*/

	/*if(Engine::Input::GetKey(ALLEGRO_KEY_0))
	{
		activeObj = 0;
	}

	if(Engine::Input::GetKey(ALLEGRO_KEY_1))
	{
		activeObj = 1;
	}

	if(Engine::Input::GetKey(ALLEGRO_KEY_2))
	{
		activeObj = 2;
	}

	if(Engine::Input::GetKey(ALLEGRO_KEY_3))
	{
		activeObj = 3;
	}*/

	/*if(Engine::Input::GetKey(ALLEGRO_KEY_PAD_PLUS))
	{
		m_gameObject.lock()->GetComponent<Transform>().lock()->ChangeScale(1.05f);
	}

	if(Engine::Input::GetKey(ALLEGRO_KEY_PAD_MINUS))
	{
		m_gameObject.lock()->GetComponent<Transform>().lock()->ChangeScale(0.95f);
	}

	if(Engine::Input::GetKey(ALLEGRO_KEY_LEFT))
	{
		m_gameObject.lock()->GetComponent<Transform>().lock()->Rotate(Vec3(0,-1,0));
	}

	if(Engine::Input::GetKey(ALLEGRO_KEY_RIGHT))
	{
		m_gameObject.lock()->GetComponent<Transform>().lock()->Rotate(Vec3(0,1,0));
	}

	if(Engine::Input::GetKey(ALLEGRO_KEY_UP))
	{
		m_gameObject.lock()->GetComponent<Transform>().lock()->Rotate(Vec3(-1,0,0));
	}

	if(Engine::Input::GetKey(ALLEGRO_KEY_DOWN))
	{
		m_gameObject.lock()->GetComponent<Transform>().lock()->Rotate(Vec3(1,0,0));
	}*/
}

void CustomCodeHandler::Destroy()
{

}