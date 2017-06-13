#include "stdafx.h"

#include "NetworkInterface.h"
#include "NetworkServer.h"
#include "NetworkClient.h"


NetworkInterface::NetworkInterface()
{

}

NetworkInterface::~NetworkInterface()
{

}

void NetworkInterface::Awake()
{
	m_eventQueue = al_create_event_queue();	

	al_register_event_source(m_eventQueue, al_get_keyboard_event_source());

	m_state = CHOOSING_TYPEZ;
}

void NetworkInterface::Start()
{

}

/// Checks for input from the player. If the user enters C, create a client. If the user
/// enters S, create a server. Once either has been created it can then be polled for updates
void NetworkInterface::Update()
{
	if(m_state == ENTER_IPZ)
	{
		m_networkManager = std::make_shared<NetworkServer>();
		m_state = ENTER_IPZ;
		m_networkManager->Awake();
		m_networkManager->Start();
	}

	al_get_next_event(m_eventQueue, &m_event);

	switch(m_event.type)
	{
		case ALLEGRO_EVENT_KEY_CHAR:
			{
				switch(m_state)
				{
					case CHOOSING_TYPEZ:
					{
						switch(m_event.keyboard.keycode)
						{
							case ALLEGRO_KEY_C:
							{
									m_networkManager = std::make_shared<NetworkServer>();
									m_state = ENTER_IPZ;
									m_networkManager->Awake();
									m_networkManager->Start();
							}
							break;
							case ALLEGRO_KEY_S:
								{
									m_networkManager = std::make_shared<NetworkServer>();
									m_state = ENTER_IPZ;
									m_networkManager->Awake();
									m_networkManager->Start();
								}
								break;
						}
					}
					break;
				}
			}

	}

	if(m_networkManager)
	{
		m_networkManager->Update();
	}
}

void NetworkInterface::Render()
{
	int i = 5;
}

void NetworkInterface::Shutdown()
{

}