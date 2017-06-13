#include "stdafx.h"

#include "Input.h"
#include <iostream>

namespace Engine
{
	/// Static variable declaration
	float Input::m_mouseMoveLimit = 100.0f;
	Vec2 Input::m_mouseMovement;
	bool Input::mouseButtons[5];
	bool Input::mouseButtonsUp[5];
	bool Input::mouseButtonsDown[5];

	bool Input::keys[255];
	bool Input::keysUp[255];
	bool Input::keysDown[255];

	Vec2 Input::m_mousePos(0.0f,0.0f);
	
	std::map<int, Lazuli::Keybinding> Input::keyMaps;
	bool Input::events[Lazuli::Keybinding::MAX_KEYBINDINGS];

	ALLEGRO_EVENT_QUEUE* Input::m_eventQueue = nullptr;
	ALLEGRO_EVENT Input::m_event;

	Input::Input()
	{

	}

	void Input::Shutdown()
	{
		///m_eventQueue = nullptr;
		al_destroy_event_queue(m_eventQueue);		
	}

	Input::~Input()
	{
		
	}
	
	void Input::Initialise(ALLEGRO_DISPLAY &_display)
	{
		ResetButtons();
		ResetUpButtons();
		ResetDownButtons();
		ResetKeys();
		ResetUpKeys();
		ResetDownKeys();		
		ResetKeyBindStatus();

		keyMaps.insert(std::pair<int, Lazuli::Keybinding>(ALLEGRO_KEY_R, Lazuli::RELOAD));
		keyMaps.insert(std::pair<int, Lazuli::Keybinding>(ALLEGRO_KEY_E, Lazuli::INTERACT));
		keyMaps.insert(std::pair<int, Lazuli::Keybinding>(ALLEGRO_KEY_SPACE, Lazuli::JUMP));
		keyMaps.insert(std::pair<int, Lazuli::Keybinding>(ALLEGRO_KEY_UP, Lazuli::MOVE_FORWARD));
		keyMaps.insert(std::pair<int, Lazuli::Keybinding>(ALLEGRO_KEY_DOWN, Lazuli::MOVE_BACKWARD));
		keyMaps.insert(std::pair<int, Lazuli::Keybinding>(ALLEGRO_KEY_LEFT, Lazuli::STRAFE_LEFT));
		keyMaps.insert(std::pair<int, Lazuli::Keybinding>(ALLEGRO_KEY_RIGHT, Lazuli::STRAFE_RIGHT));
		keyMaps.insert(std::pair<int, Lazuli::Keybinding>(ALLEGRO_KEY_W, Lazuli::MOVE_FORWARD));
		keyMaps.insert(std::pair<int, Lazuli::Keybinding>(ALLEGRO_KEY_A, Lazuli::STRAFE_LEFT));
		keyMaps.insert(std::pair<int, Lazuli::Keybinding>(ALLEGRO_KEY_S, Lazuli::MOVE_BACKWARD));
		keyMaps.insert(std::pair<int, Lazuli::Keybinding>(ALLEGRO_KEY_D, Lazuli::STRAFE_RIGHT));
		keyMaps.insert(std::pair<int, Lazuli::Keybinding>(ALLEGRO_KEY_LCTRL, Lazuli::CROUCH));
		keyMaps.insert(std::pair<int, Lazuli::Keybinding>(ALLEGRO_KEY_ALT, Lazuli::PRONE));

		m_eventQueue = al_create_event_queue();

		al_install_keyboard();
		al_install_mouse();

		al_register_event_source(m_eventQueue, al_get_keyboard_event_source());
		al_register_event_source(m_eventQueue, al_get_mouse_event_source());
		al_register_event_source(m_eventQueue, al_get_display_event_source(&_display));
	}

	void Input::Update()
	{

	}

	bool Input::GetInput()
	{
		bool eventHappened = false;
		
		ResetUpButtons();
		ResetDownButtons();
		ResetUpKeys();
		ResetDownKeys();
		ResetKeyBindStatus();

		while (al_get_next_event(m_eventQueue, &m_event))
		{
			switch (m_event.type)
			{
				case ALLEGRO_EVENT_KEY_DOWN:
				{
					keysDown[m_event.keyboard.keycode] = true;
					keys[m_event.keyboard.keycode] = true;

					if (CheckIfBound(m_event.keyboard.keycode))
					{
						events[keyMaps[m_event.keyboard.keycode]] = true;
					}
				}
				break;
				case ALLEGRO_EVENT_KEY_UP:
				{
					keysUp[m_event.keyboard.keycode] = true;
					keys[m_event.keyboard.keycode] = false;
				}
				break;
				case ALLEGRO_EVENT_MOUSE_AXES:
				{
					m_mousePos.x = m_event.mouse.x;
					m_mousePos.y = m_event.mouse.y;

					m_mouseMovement.x = m_event.mouse.dx / m_mouseMoveLimit;
					m_mouseMovement.y = m_event.mouse.dy / m_mouseMoveLimit;
				}
				break;
				case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
				{
					mouseButtons[m_event.mouse.button - 1] = true;
					mouseButtonsDown[m_event.mouse.button - 1] = true;
				}
				break;
				case ALLEGRO_EVENT_MOUSE_BUTTON_UP:
				{
					mouseButtons[m_event.mouse.button - 1] = false;
					mouseButtonsUp[m_event.mouse.button - 1] = true;
				}
				break;
				case ALLEGRO_EVENT_DISPLAY_CLOSE:
				{
					return false;
				}
				break;
				default: ;
			}
		}

		return true;
	}

	Vec2 Input::GetMouseAxis()
	{
		return Vec2();
	}

	Vec2 Input::GetCursorScreenPos()
	{
		return Vec2();
	}

	Vec2 Input::GetCursorWindowPos()
	{
		return Vec2();
	}

	Vec2 Input::GetGamePadAxis()
	{
		return Vec2();
	}


	/// ++++++++++++++++++++++++
	/// Mouse functions
	/// ++++++++++++++++++++++++

	////
	/// Returns whether the given button is pressed
	bool Input::GetButton(int _button)
	{
		return mouseButtons[_button];
	}

	//// Returns whether the given button was released this frame
	bool Input::GetButtonUp(int _button)
	{
		return mouseButtonsUp[_button];
	}

	//// Returns whether the given button was pressed this frame
	bool Input::GetButtonDown(int _button)
	{
		return mouseButtonsDown[_button];
	}

	//// Returns normalised mouse movement for this frame
	glm::vec2 Input::GetMouseMovement()
	{
		return m_mouseMovement;
	}

	/// ++++++++++++++++++++++
	/// Keyboard functions
	/// ++++++++++++++++++++++

	//// Returns whether the given key is pressed
	bool Input::GetKey(int _keycode)
	{
		return keys[_keycode];
	}

	//// Returns whether the given key was released this frame
	bool Input::GetKeyUp(int _keycode)
	{
		return keysUp[_keycode];
	}

	//// Returns whether the given key was pressed this frame
	bool Input::GetKeyDown(int _keycode)
	{
		return keysDown[_keycode];
	}

	//// Returns whether the key linked to the given event was pressed this frame
	bool Input::CheckEvent(Lazuli::Keybinding _event)
	{
		return events[_event];
	}

	void Input::ResetButtons()
	{
		for (int i = 0; i < 5; i++)
		{
			mouseButtons[i] = false;
		}
	}

	void Input::ResetUpButtons()
	{
		for (int i = 0; i < 5; i++)
		{
			mouseButtonsUp[i] = false;
		}
	}

	void Input::ResetDownButtons()
	{
		for (int i = 0; i < 5; i++)
		{
			mouseButtonsDown[i] = false;
		}
	}

	void Input::ResetKeys()
	{
		for (int i = 0; i < 255; i++)
		{
			keys[i] = false;
		}
	}

	void Input::ResetUpKeys()
	{
		for (int i = 0; i < 255; i++)
		{
			keysUp[i] = false;
		}
	}

	void Input::ResetDownKeys()
	{
		for (int i = 0; i < 255; i++)
		{
			keysDown[i] = false;
		}
	}

	void Input::ResetKeyBindStatus()
	{
		for (int i = 0; i < Lazuli::MAX_KEYBINDINGS; i++)
		{
			events[i] = false;
		}
	}

	bool Input::CheckIfBound(int _keycode)
	{
		for each (std::pair<int, Lazuli::Keybinding> bind in keyMaps)
		{
			if (m_event.keyboard.keycode == bind.first)
			{
				events[bind.second] = true;
				return true;
			}
		}

		return false;
	}

	void Input::ReassignKeybind(Lazuli::Keybinding _bind, int _keycode)
	{
		if (CheckIfBound(_keycode))
		{
			keyMaps[_keycode] = _bind;
		}

		else
			keyMaps.insert(std::pair<int, Lazuli::Keybinding>(_keycode, _bind));
	}
}