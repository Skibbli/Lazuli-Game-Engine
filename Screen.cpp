#include "stdafx.h"

#include "Screen.h"
#include <stdio.h>

int Screen::m_width;
int Screen::m_height;

ALLEGRO_DISPLAY* Screen::m_display;

/// Default constructor
Screen::Screen() {};

/// Default destructor
Screen::~Screen()
{
	al_destroy_display(m_display);	
}

bool Screen::InitialiseScreen(int _width, int _height)
{
	m_width = _width;
	m_height = _height;	

	al_set_new_display_flags(ALLEGRO_OPENGL);

	m_display = al_create_display(m_width, m_height);
	al_set_window_title(m_display, "Lazuli");

	return true;
}

/// Get the width of the window
int Screen::GetWidth()
{
	return m_width;
}

/// Get the height of the window
int Screen::GetHeight()
{
	return m_height;
}

/// Change the size of the window
void Screen::ChangeScreenSize(int _width, int _height)
{
	m_width = _width;
	m_height = _height;
}

ALLEGRO_DISPLAY& Screen::GetDisplay()
{
	return *m_display;
}