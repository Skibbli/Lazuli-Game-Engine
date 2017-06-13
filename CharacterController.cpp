#include "stdafx.h"

#include "CharacterController.h"
#include "GameObject.h"
#include "Input.h"


CharacterController::CharacterController()
{

}

CharacterController::~CharacterController()
{

}

void CharacterController::Awake()
{
	m_transform = m_gameObject.lock()->GetComponent<Transform>();

	m_collider = m_gameObject.lock()->AddComponent<BoxCollider>();
	m_collider.lock()->CreateCollider(2,3.5,2,150);

	weak<GameObject> camObj = GameObject::CreateGameObj("Camera Object");
	camObj.lock()->AddComponent<Camera>();
	m_cameraTransform = camObj.lock()->GetComponent<Transform>();

	m_gameObject.lock()->LoadModel("curuthers.obj");

	m_gameObject.lock()->AddChildObject(camObj);
	m_cameraTransform.lock()->SetLocalPosition(Vec3(0,0,-10));
	m_cameraTransform.lock()->Rotate(Quat(0,0,1,0));

	m_speed = 10.0f;
}

void CharacterController::Start()
{

}

void CharacterController::Update()
{
	if(Engine::Input::GetButton(0))
	{
		glm::vec2 mouseMove = Engine::Input::GetMouseMovement();
		m_transform.lock()->Translate(Vec3(mouseMove.x * 10, 0.0f, mouseMove.y * 10));
	}

	if(Engine::Input::GetButton(1))
	{
		glm::vec2 mouseMove = Engine::Input::GetMouseMovement();
		//m_gameObjects.at(activeObj)->GetComponent<Transform>().lock()->Rotate(Vec3(0.0f, mouseMove.x * 10, 0.0f));
	}

	if(Engine::Input::GetKey(ALLEGRO_KEY_W))
	{
		m_transform.lock()->MoveForward(0.75f);
	}

	if(Engine::Input::GetKey(ALLEGRO_KEY_S))
	{
		m_transform.lock()->MoveBackward(0.75f);
	}

	if(Engine::Input::GetKey(ALLEGRO_KEY_A))
	{
		m_transform.lock()->YawRight(0.05f);
	}

	if(Engine::Input::GetKey(ALLEGRO_KEY_D))
	{
		m_transform.lock()->YawLeft(0.05f);
	}

	if(Engine::Input::GetKeyUp(ALLEGRO_KEY_W))
	{
		m_collider.lock()->SetLinearVelocity(AXIS::Z_AXIS, 0);
	}

	if(Engine::Input::GetKeyUp(ALLEGRO_KEY_S))
	{
		m_collider.lock()->SetLinearVelocity(AXIS::Z_AXIS, 0);
	}

	/*if(Engine::Input::GetKeyUp(ALLEGRO_KEY_A))
	{
		m_collider.lock()->SetAngularVelocity(AXIS::Z_AXIS, 0);
	}

	if(Engine::Input::GetKeyUp(ALLEGRO_KEY_D))
	{
		m_collider.lock()->SetAngularVelocity(AXIS::Z_AXIS, 0);
	}*/

	if(Engine::Input::GetKeyDown(ALLEGRO_KEY_SPACE))
	{
		m_collider.lock()->SetLinearVelocity(Y_AXIS, 7.50f);
	}

	//m_transform.lock()->YawLeft(0.01f);
	
}