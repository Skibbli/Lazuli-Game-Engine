#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "stdafx.h"

#include "BaseObj.h"
#include "Component.h"


class Collider;

class GameObject : public BaseObj, public std::enable_shared_from_this<GameObject>
{
	public:
		GameObject();
		GameObject(std::string _name);
		~GameObject();

		/// Creates a game object and assigns it the given name. Automatically adds a transform and adds the object to the list of game objects
		static shared<GameObject> CreateGameObj(std::string _name);

		/// Adds a component of the given type
		template <class Comp>
		weak<Comp> AddComponent()
		{
			shared<Comp> comp = std::make_shared<Comp>();
			m_components.push_back(comp);
			comp->SetGameObject(shared_from_this());			

			return comp;
		}

		/// Returns the component of the given type. Returns the first one found if there is more than one
		template<class Comp>
		weak<Comp> GetComponent()
		{
			for(Uint i = 0; i < m_components.size(); i++)
			{
				{					
					weak<Comp> comp = std::dynamic_pointer_cast<Comp>(m_components.at(i));
					
					if(!comp.expired())
					{
						return comp;
					}
				}
			}

			return weak<Comp>();
		}

		/// Returns a vector of all of the given component
		template<class Comp>
		std::vector<weak<Comp>> GetComponents()
		{
			std::vector<weak<Comp>> comps;

			for(Uint i = 0; i < m_components.size(); i++)
			{
				weak<Comp> coll = std::dynamic_pointer_cast<Comp>(m_components.at(i));

				if(!coll.expired())
				{
					comps.push_back(std::dynamic_pointer_cast<Comp>(m_components.at(i)));
				}
			}

			return comps;
		}

		/// Loads the given model to this game object, creating and setting up the mesh renderer
		void LoadModel(std::string _model);

		/// Sets the parent game object to the given object
		void SetParentObject(weak<GameObject> _obj);
		/*void SetParentObject(GameObject _obj);*/

		/// Adds the given object as a child
		void AddChildObject(weak<GameObject> _obj);

		/// Sets whether this game object is active or not
		virtual void SetActive(bool _active);		

		virtual void Awake();
		virtual void Start();
		virtual void Update();
		virtual void Render();
		virtual void Destroy();

	private:
		std::string m_name;
		
		/// List of components attached to this game object
		std::vector<shared<Component>> m_components;
		/// List of child game objects attached to this object
		std::vector<weak<GameObject>> m_children;
};

#endif