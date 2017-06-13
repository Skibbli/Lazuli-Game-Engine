#include "stdafx.h"

#include "GameObject.h"

#include "Core.h"

GameObject::GameObject()
{
	m_isActive = true;
}

GameObject::GameObject(std::string _name)
{	
	m_name = _name;
	m_isActive = true;
}

GameObject::~GameObject()
{
	for(int i = 0; i < m_components.size(); i++)
	{
		m_components.clear();
	}
};

shared<GameObject> GameObject::CreateGameObj(std::string _name)
{
	shared<GameObject> obj = std::make_shared<GameObject>(_name);
	obj->AddComponent<Transform>();

	Core::GetGameObjects().push_back(obj);

	return obj;
}

void GameObject::Awake()
{
	for (Uint i = 0; i < m_components.size(); i++)
	{
		if(m_components.at(i)->IsActive())
			m_components.at(i)->Awake();
	}
}

void GameObject::Start()
{
	for (Uint i = 0; i < m_components.size(); i++)
	{
		if(m_components.at(i)->IsActive())
			m_components.at(i)->Start();
	}
}

void GameObject::Update()
{
	for (Uint i = 0; i < m_components.size(); i++)
	{
		if(m_components.at(i)->IsActive())
			m_components.at(i)->Update();
	}
}

void GameObject::Render()
{
	for (Uint i = 0; i < m_components.size(); i++)
	{
		if(m_components.at(i)->IsActive())
			m_components.at(i)->Render();
	}
}

void GameObject::Destroy()
{
	for(Uint i = 0; i < m_components.size(); i++)
	{
		if(m_components.at(i)->IsActive())
			m_components.at(i)->Destroy();
	}
}

void GameObject::LoadModel(std::string _model)
{
	StoredModel model = Resources::LoadModel(_model);

	std::vector<ModelPart> tempParts = model.GetModelParts();

	AddComponent<MeshRenderer>().lock()->LoadModelPart(tempParts.at(0));

	//for(Uint i = 0; i < model.GetNumMeshes(); i++)
	//{
	//	shared<GameObject> tempObj = GameObject::CreateGameObj(tempParts.at(i).name);
	//	tempObj->AddComponent<MeshRenderer>().lock()->LoadModelPart(tempParts.at(i));
	//	//tempObj->GetComponent<Transform>().lock()->SetLocalPosition(tempParts.at(i).mesh.lock()->GetCenter());

	//	AddChildObject(tempObj);
	//}
}

void GameObject::SetParentObject(weak<GameObject> _obj)
{
	GetComponent<Transform>().lock()->SetParent(_obj.lock()->GetComponent<Transform>());	
	_obj.lock()->GetComponent<Transform>().lock()->AddChild(GetComponent<Transform>());
}

///void GameObject::SetParentObject(GameObject _obj)
///{
///	GetComponent<Transform>().lock()->SetParent(_obj.GetComponent<Transform>());
///	_obj.GetComponent<Transform>().lock()->AddChild(GetComponent<Transform>());
///}

void GameObject::AddChildObject(weak<GameObject> _obj)
{
	_obj.lock()->GetComponent<Transform>().lock()->SetParent(GetComponent<Transform>());
	GetComponent<Transform>().lock()->AddChild(_obj.lock()->GetComponent<Transform>());
	m_children.push_back(_obj);
}

void GameObject::SetActive(bool _active)
{
	m_isActive = _active;
}