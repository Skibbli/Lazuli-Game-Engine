#include "stdafx.h"

#include "Core.h"


int main(int argc, char* argv[])
{
	/*al_init();
	al_init_image_addon();
	al_install_audio();
	al_init_acodec_addon();
	al_init_font_addon();
	al_init_ttf_addon();

	ALLEGRO_BITMAP  *fuckingmap;
	fuckingmap = al_load_bitmap("Assets/Sprites/abcmap");*/

	Core theCore;

	theCore.Initialise();

	theCore.Run();

	theCore.Shutdown();

	printf("Woo main\n");

	return 0;
}