#ifndef LAZULI_CORE_SCREEN_H
#define LAZULI_CORE_SCREEN_H

#include "stdafx.h"

#include "allegro5\allegro.h"

class Screen
{
	public:
		Screen();
		~Screen();

		/// Creates the display window with the given size
		static bool InitialiseScreen(int _width, int _height);

		/// Returns the width of the window
		static int GetWidth();
		/// Returns the height of the window
		static int GetHeight();
		/// Adjusts the size of the window to the given size
		static void ChangeScreenSize(int _width, int _height);

		/// Returns the window
		static ALLEGRO_DISPLAY& GetDisplay();

	private:
		/// The width of the screen
		static int m_width;
		/// The height of the screen
		static int m_height;

		/// The screen
		static ALLEGRO_DISPLAY *m_display;
};

#endif