#ifndef ENGINE_COMPONENT_H
#define ENGINE_COMPONENT_H

#include "stdafx.h"


#include "BaseObj.h"
#include <memory>
#include <vector>

#define weak std::weak_ptr
#define shared std::shared_ptr


class GameObject;

class Component : public BaseObj, public std::enable_shared_from_this<Component>
{
public:
	Component();
	virtual ~Component();

	/// Sets the game object this component belongs to
	void SetGameObject(weak<GameObject> _obj);
	/// Gets the game object this component belongs to
	weak<GameObject> GetGameObj();	

protected:
	/// The game object the component is attached to
	weak<GameObject> m_gameObject;

};

#endif