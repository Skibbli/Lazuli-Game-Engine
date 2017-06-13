#ifndef ENGINE_INPUT_H
#define ENGINE_INPUT_H

#include "stdafx.h"

#include "allegro5\allegro.h"
#include <map>


namespace Lazuli
{
	/// Key bindings for typical game actions
	enum Keybinding
	{
		RELOAD, INTERACT, CROUCH, PRONE, JUMP, MOVE_FORWARD, MOVE_BACKWARD, TURN_LEFT, TURN_RIGHT, STRAFE_LEFT, STRAFE_RIGHT,
		MAX_KEYBINDINGS
	};
};

namespace Engine
{
	class Input
	{
	public:
		Input();
		~Input();

		/// Initialises the Input module
		static void Initialise(ALLEGRO_DISPLAY &_display);
		/// Shuts down the Input module
		static void Shutdown();

		/// Checks for any input this frame
		static bool GetInput();

		/// Returns how much the mouse has moved since the last frame, between -1 and 1 for each axis
		static Vec2 GetMouseAxis();
		/// Returns the mouse position on the screen
		static Vec2 GetCursorScreenPos();
		/// Returns the mouse position on the Allegro window
		static Vec2 GetCursorWindowPos();

		/// Returns how much the mouse has moved since the last frame, between -1 and 1 for each axis
		static Vec2 GetGamePadAxis();

		/// Returns whether the given button is pressed
		static bool GetButton(int _button);
		/// Returns whether the given button was released that frame
		static bool GetButtonUp(int _button);
		/// Returns whether the given button was pressed that frame
		static bool GetButtonDown(int _button);
		/// Returns normalised mouse movement for the frame
		static glm::vec2 GetMouseMovement();
		/// Returns whether the given key is pressed
		static bool GetKey(int _keycode);
		/// Returns whether the given key was released that frame
		static bool GetKeyUp(int _keycode);
		/// Returns whether the given key was pressed that frame
		static bool GetKeyDown(int _keycode);

		/// Returns whether the key linked to the given event was pressed this frame
		static bool CheckEvent(Lazuli::Keybinding _event);
		/// Reassign a keybinding
		static void ReassignKeybind(Lazuli::Keybinding _bind, int _keycode);

	private:
		/// Resets the Button array
		static void ResetButtons();
		/// Resets the Up Button array
		static void ResetUpButtons();
		/// Resets the Down Buttton array
		static void ResetDownButtons();

		/// Resets the Key array
		static void ResetKeys();
		/// Resets the Up Key array
		static void ResetUpKeys();
		/// Resets the Down Key array
		static void ResetDownKeys();

		/// Resets the array tracking if a keybind has been input
		static void ResetKeyBindStatus();

		/// Checks if the given key is bound to an event
		static bool CheckIfBound(int _keycode);

		static void Update();

		static Vec2 m_mousePos;
		static float m_mouseMoveLimit;
		static Vec2 m_mouseMovement;
		static bool mouseButtons[5];
		static bool mouseButtonsUp[5];
		static bool mouseButtonsDown[5];

		static bool keys[255];
		static bool keysUp[255];
		static bool keysDown[255];
		static std::map<int, Lazuli::Keybinding> keyMaps;
		static bool events[Lazuli::Keybinding::MAX_KEYBINDINGS];

		static ALLEGRO_EVENT_QUEUE* m_eventQueue;
		static ALLEGRO_EVENT m_event;
	};
}

#endif