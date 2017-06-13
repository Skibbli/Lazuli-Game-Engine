#ifndef NETWORK_INTERFACE_H
#define NETWORK_INTERFACE_H

#include "stdafx.h"

#include "Component.h"
#include "NetworkManager.h"


enum InitialState
{
	CHOOSING_TYPEZ, ENTER_IPZ, IN_GAME_PLAYINGZ, IN_GAME_TYPINGZ
};

/// Component that is used to hold the server/client when it's created
class NetworkInterface : public Component
{
	public:
		NetworkInterface();
		~NetworkInterface();

		void Awake();
		void Start();
		void Update();
		void Render();
		void Shutdown();

	private:
		/// Base variable used for creating the server/client
		shared<NetworkManager> m_networkManager;

		ALLEGRO_EVENT_QUEUE *m_eventQueue;
		ALLEGRO_EVENT m_event;

		InitialState m_state;
};

#endif