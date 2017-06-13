#include "stdafx.h"

#include "NetworkClient.h"
#include "BoxCollider.h"
#include "Input.h"
#include "Core.h"


NetworkClient::NetworkClient()
{
	

	
}

NetworkClient::~NetworkClient()
{

}

void NetworkClient::Awake()
{
	m_eventQueue = al_create_event_queue();	

	al_register_event_source(m_eventQueue, al_get_keyboard_event_source());

	m_camTransform = Core::GetCameras().at(0).lock()->GetGameObj().lock()->GetComponent<Transform>();
}

void NetworkClient::Start()
{

}

void NetworkClient::Update()
{
	HandlePackets();
	GetInput();
}

/// Loads the scene of the server that was connected to using the given packet
void NetworkClient::LoadScene(RakNet::Packet* _pack)
{
	/// Cycle through the packet specified by the number of objects in the scene and for each
	/// one create a game object for it, load a model to it and and it to the list of objects in the scene
	RakNet::BitStream bsIn(m_packet->data, m_packet->length, false);

	bsIn.IgnoreBytes(sizeof(RakNet::MessageID));

	int numObjs;

	bsIn.Read(numObjs);

	std::string modelName;
	RakNet::RakString rakModelName;
	Vec3 position;
	Quat orientation;
	Vec3 scale;

	for(int i = 0; i < numObjs; i++)
	{
		bsIn.Read(rakModelName);
		modelName = rakModelName.C_String();

		bsIn.Read(position.x);
		bsIn.Read(position.y);
		bsIn.Read(position.z);

		bsIn.Read(orientation.w);
		bsIn.Read(orientation.x);
		bsIn.Read(orientation.y);
		bsIn.Read(orientation.z);

		bsIn.Read(scale.x);
		bsIn.Read(scale.y);
		bsIn.Read(scale.z);

		weak<GameObject> newObj = GameObject::CreateGameObj(modelName);
		newObj.lock()->LoadModel(modelName);
		newObj.lock()->AddComponent<BoxCollider>().lock()->CreateColliderFromMesh(100);
		newObj.lock()->GetComponent<Transform>().lock()->SetWorldPosition(position);
		newObj.lock()->GetComponent<Transform>().lock()->SetWorldRotation(orientation);
		newObj.lock()->GetComponent<Transform>().lock()->SetLocalScale(scale);
	}
}

/// Adds an additional object to the scene
void NetworkClient::AddSceneObject(RakNet::Packet* _pack)
{
	/// Extracts each bit of data one at a time and uses it to create the object in the scene
	/// Once it has finished the object is added to the list of scene objects
	RakNet::BitStream bsIn(m_packet->data, m_packet->length, false);

	bsIn.IgnoreBytes(sizeof(RakNet::MessageID));

	std::string modelName;
	RakNet::RakString rakModelName;
	Vec3 position;
	Quat orientation;
	Vec3 scale;

	bsIn.Read(rakModelName);
	modelName = rakModelName.C_String();

	bsIn.Read(position.x);
	bsIn.Read(position.y);
	bsIn.Read(position.z);

	bsIn.Read(orientation.w);
	bsIn.Read(orientation.x);
	bsIn.Read(orientation.y);
	bsIn.Read(orientation.z);

	bsIn.Read(scale.x);
	bsIn.Read(scale.y);
	bsIn.Read(scale.z);

	weak<GameObject> newObj = GameObject::CreateGameObj(modelName);
	newObj.lock()->LoadModel(modelName);
	newObj.lock()->AddComponent<BoxCollider>().lock()->CreateColliderFromMesh(100);
	newObj.lock()->GetComponent<Transform>().lock()->SetWorldPosition(position);
	newObj.lock()->GetComponent<Transform>().lock()->SetWorldRotation(orientation);
	newObj.lock()->GetComponent<Transform>().lock()->SetLocalScale(scale);

}

/// Loads the players already on the server
void NetworkClient::LoadPlayers(RakNet::Packet* _pack)
{
	/// Iterates over the packet the number of times given by the number of players on the server,
	/// creates a game object for each object and adds the object to the list of other players
	RakNet::BitStream bsIn(m_packet->data, m_packet->length, false);

	bsIn.IgnoreBytes(sizeof(RakNet::MessageID));

	int numPlayers;
	std::string playerName;
	RakNet::RakString rakPlayerName;
	Vec3 position;
	Quat orientation;
	Vec3 scale;

	for(int i = 0; i < numPlayers; i++)
	{
		bsIn.Read(rakPlayerName);
		playerName = rakPlayerName.C_String();

		bsIn.Read(position.x);
		bsIn.Read(position.y);
		bsIn.Read(position.z);

		bsIn.Read(orientation.w);
		bsIn.Read(orientation.x);
		bsIn.Read(orientation.y);
		bsIn.Read(orientation.z);

		bsIn.Read(scale.x);
		bsIn.Read(scale.y);
		bsIn.Read(scale.z);

		weak<GameObject> newObj = GameObject::CreateGameObj(playerName);
		newObj.lock()->LoadModel("curuthers.obj");
		newObj.lock()->AddComponent<BoxCollider>().lock()->CreateColliderFromMesh(100);
		newObj.lock()->GetComponent<Transform>().lock()->SetWorldPosition(position);
		newObj.lock()->GetComponent<Transform>().lock()->SetWorldRotation(orientation);
		newObj.lock()->GetComponent<Transform>().lock()->SetLocalScale(scale);

		NetworkPlayerClient newPlayer;
		newPlayer.name = playerName;
		newPlayer.transform = newObj.lock()->GetComponent<Transform>();
	}
}

/// Adds an additional player to the list of online players and creates them in the scene
void NetworkClient::AddPlayer(RakNet::Packet* _pack)
{
	RakNet::BitStream bsIn(m_packet->data, m_packet->length, false);

	bsIn.IgnoreBytes(sizeof(RakNet::MessageID));

	std::string playerName;
	RakNet::RakString rakPlayerName;
	Vec3 position;
	Quat orientation;
	Vec3 scale;

	bsIn.Read(rakPlayerName);
	playerName = rakPlayerName.C_String();

	bsIn.Read(position.x);
	bsIn.Read(position.y);
	bsIn.Read(position.z);

	bsIn.Read(orientation.w);
	bsIn.Read(orientation.x);
	bsIn.Read(orientation.y);
	bsIn.Read(orientation.z);

	bsIn.Read(scale.x);
	bsIn.Read(scale.y);
	bsIn.Read(scale.z);

	weak<GameObject> newObj = GameObject::CreateGameObj(playerName);
	newObj.lock()->LoadModel("curuthers.obj");
	newObj.lock()->AddComponent<BoxCollider>().lock()->CreateColliderFromMesh(100);
	newObj.lock()->GetComponent<Transform>().lock()->SetWorldPosition(position);
	newObj.lock()->GetComponent<Transform>().lock()->SetWorldRotation(orientation);
	newObj.lock()->GetComponent<Transform>().lock()->SetLocalScale(scale);
}

/// Applies a transform update to the given player
void NetworkClient::ProcessOtherPosUpdate(RakNet::Packet* _pack)
{
	RakNet::BitStream bsIn(m_packet->data, m_packet->length, false);

	bsIn.IgnoreBytes(sizeof(RakNet::MessageID));

	Vec3 position;
	Quat orientation;
	Vec3 scale;

	RakNet::RakString rakName;
	bsIn.Read(rakName);
	bsIn.Read(position.x);
	bsIn.Read(position.y);
	bsIn.Read(position.z);
	bsIn.Read(orientation.w);
	bsIn.Read(orientation.x);
	bsIn.Read(orientation.y);
	bsIn.Read(orientation.z);
	bsIn.Read(scale.x);
	bsIn.Read(scale.y);
	bsIn.Read(scale.z);

	m_otherPlayers[m_nameToGUID[rakName.C_String()]].transform.lock()->SetWorldPosition(position);
	m_otherPlayers[m_nameToGUID[rakName.C_String()]].transform.lock()->SetWorldRotation(orientation);
	m_otherPlayers[m_nameToGUID[rakName.C_String()]].transform.lock()->SetWorldPosition(scale);
}

/// Checks for packets and processes them
void NetworkClient::HandlePackets()
{
	for (m_packet = m_peer->Receive(); m_packet; m_peer->DeallocatePacket(m_packet), m_packet = m_peer->Receive())
	{
		switch (m_packet->data[0])
		{
			case ID_REMOTE_DISCONNECTION_NOTIFICATION:
				{
					printf("Another client has disconnected.\n");
				}
				break;
			case ID_REMOTE_CONNECTION_LOST:
				{
					printf("Another client has lost the connection.\n");
				}
				break;
			case ID_REMOTE_NEW_INCOMING_CONNECTION:
				printf("Another client has connected.\n");
				break;
			case ID_CONNECTION_REQUEST_ACCEPTED:
				printf("Our connection request has been accepted.\n");
			break;
			case ID_NEW_INCOMING_CONNECTION:
					printf("A connection is incoming.\n");
				break;
			case ID_NO_FREE_INCOMING_CONNECTIONS:
				printf("The server is full.\n");
				break;
			case ID_DISCONNECTION_NOTIFICATION:
				{
					printf("A client has disconnected.\n");
				}
				break;
			case ID_CONNECTION_LOST:
					printf("A client lost the connection.\n");
				break;
			case CLIENT_DISCONNECT:
				{
					printf("A client has disconnected.\n");
				}
				break;
			case LOGIN_SUCCESS:
				{
					RakNet::BitStream bsIn(m_packet->data, m_packet->length, false);

					bsIn.IgnoreBytes(sizeof(RakNet::MessageID));

					Vec3 position;
					Quat orientation;
					Vec3 scale;

					bsIn.Read(position.x);
					bsIn.Read(position.y);
					bsIn.Read(position.z);

					bsIn.Read(orientation.w);
					bsIn.Read(orientation.x);
					bsIn.Read(orientation.y);
					bsIn.Read(orientation.z);

					bsIn.Read(scale.x);
					bsIn.Read(scale.y);
					bsIn.Read(scale.z);

					weak<GameObject> newObj = GameObject::CreateGameObj(m_name);
					newObj.lock()->LoadModel("curuthers.obj");
					newObj.lock()->GetComponent<Transform>().lock()->SetWorldPosition(position);
					newObj.lock()->GetComponent<Transform>().lock()->SetWorldRotation(orientation);
					newObj.lock()->GetComponent<Transform>().lock()->SetLocalScale(scale);

					m_state = IN_GAME_PLAYING;
				}
				break;
			case RECEIVE_SCENE:
				{
					LoadScene(m_packet);
				}
				break;
			case RECEIVE_PLAYER_LIST:
				{
					LoadPlayers(m_packet);
				}
				break;
			case ADD_SCENE_OBJECT:
				{
					AddSceneObject(m_packet);
				}
				break;
			case POS_UPDATE:
				{
					RakNet::BitStream bsIn(m_packet->data, m_packet->length, false);

					bsIn.IgnoreBytes(sizeof(RakNet::MessageID));

					Vec3 position;
					Quat orientation;
					Vec3 scale;

					bsIn.Read(position.x);
					bsIn.Read(position.y);
					bsIn.Read(position.z);

					bsIn.Read(orientation.w);
					bsIn.Read(orientation.x);
					bsIn.Read(orientation.y);
					bsIn.Read(orientation.z);

					bsIn.Read(scale.x);
					bsIn.Read(scale.y);
					bsIn.Read(scale.z);

					m_transform.lock()->SetWorldPosition(position);
					m_transform.lock()->SetWorldRotation(orientation);
					m_transform.lock()->SetWorldPosition(scale);
				}
				break;
			case ADD_OTHER_PLAYER:
				{
					AddPlayer(m_packet);
				}
				break;
			case OTHER_POS_UPDATE:
				{
					ProcessOtherPosUpdate(m_packet);
				}
				break;
			case CHAT_MESSAGE_SINGLE:
				{
					RakNet::BitStream bsIn(m_packet->data, m_packet->length, false);

					bsIn.IgnoreBytes(sizeof(RakNet::MessageID));

					RakNet::RakString msg, sender;
					bsIn.Read(sender);
					bsIn.Read(msg);

					ChatMessage message(sender.C_String(), msg.C_String());

					m_chatMessages.push_back(message);

					if(m_chatMessages.size() > 10)
					{
						m_chatMessages.erase(m_chatMessages.begin());
					}			
				}
				break;
		}
	}
}

/// Gets input from the player dependant on the state they are in
void NetworkClient::GetInput()
{
	if(m_state == IN_GAME_PLAYING)
	{
		if(Engine::Input::GetButton(0))
		{
			glm::vec2 mouseMove = Engine::Input::GetMouseMovement();
			m_transform.lock()->Translate(Vec3(mouseMove.x * 10, 0.0f, mouseMove.y * 10));
		}


		if(Engine::Input::GetButton(1))
		{
			glm::vec2 mouseMove = Engine::Input::GetMouseMovement();
			//m_gameObjects.at(activeObj)->GetComponent<Transform>().lock()->Rotate(Vec3(0.0f, mouseMove.x * 10, 0.0f));
		}

		if(Engine::Input::GetKey(ALLEGRO_KEY_W))
		{
			m_bsOut.Reset();
			m_packetID = MOVE_FORWARD;

			m_bsOut.Write((RakNet::MessageID)m_packetID);
			m_peer->Send(&m_bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, m_serverID, false);
		}

		if(Engine::Input::GetKey(ALLEGRO_KEY_S))
		{
			m_bsOut.Reset();
			m_packetID = MOVE_BACKWARD;

			m_bsOut.Write((RakNet::MessageID)m_packetID);
			m_peer->Send(&m_bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, m_serverID, false);
		}

		if(Engine::Input::GetKey(ALLEGRO_KEY_A))
		{
			m_bsOut.Reset();
			m_packetID = TURN_LEFT;

			m_bsOut.Write((RakNet::MessageID)m_packetID);
			m_peer->Send(&m_bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, m_serverID, false);m_transform.lock()->YawLeft(0.05f);
		}

		if(Engine::Input::GetKey(ALLEGRO_KEY_D))
		{
			m_bsOut.Reset();
			m_packetID = TURN_RIGHT;

			m_bsOut.Write((RakNet::MessageID)m_packetID);
			m_peer->Send(&m_bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, m_serverID, false);
		}

		if(Engine::Input::GetKeyUp(ALLEGRO_KEY_W))
		{
			m_bsOut.Reset();
			m_packetID = CANCEL_FORWARD;

			m_bsOut.Write((RakNet::MessageID)m_packetID);
			m_peer->Send(&m_bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, m_serverID, false);
		}

		if(Engine::Input::GetKeyUp(ALLEGRO_KEY_S))
		{
			m_bsOut.Reset();
			m_packetID = CANCEL_BACKWARD;

			m_bsOut.Write((RakNet::MessageID)m_packetID);
			m_peer->Send(&m_bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, m_serverID, false);
		}

		if(Engine::Input::GetKeyUp(ALLEGRO_KEY_A))
		{
			m_bsOut.Reset();
			m_packetID = CANCEL_LEFT;

			m_bsOut.Write((RakNet::MessageID)m_packetID);
			m_peer->Send(&m_bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, m_serverID, false);
		}

		if(Engine::Input::GetKeyUp(ALLEGRO_KEY_D))
		{
			m_bsOut.Reset();
			m_packetID = CANCEL_RIGHT;

			m_bsOut.Write((RakNet::MessageID)m_packetID);
			m_peer->Send(&m_bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, m_serverID, false);
		}

		if(Engine::Input::GetKeyDown(ALLEGRO_KEY_SPACE))
		{
			m_bsOut.Reset();
			m_packetID = JUMP;

			m_bsOut.Write((RakNet::MessageID)m_packetID);
			m_peer->Send(&m_bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, m_serverID, false);
		}
	}

	al_wait_for_event(m_eventQueue, &m_event);

	switch(m_event.type)
	{
		case ALLEGRO_EVENT_KEY_CHAR:
			{
				switch(m_state)
				{
					case ENTER_IP:
					{
						switch(m_event.keyboard.keycode)
						{
							case ALLEGRO_KEY_ENTER:
								{
									if(m_ipEnter.size() > 10)
									{
										m_peer = RakNet::RakPeerInterface::GetInstance();

										RakNet::SocketDescriptor sd;
										m_peer->Startup(1, &sd, 1);

										m_peer->Connect(m_ipEnter.c_str(), SERVER_PORT, 0, 0);

										m_state = WAITING_FOR_SERVER;
									}
								}
								break;
								default:
								{
									if((m_event.keyboard.unichar <= 125) && (m_event.keyboard.unichar >= 32))
									{
										if(m_ipEnter.size() < 15)
											m_ipEnter += m_event.keyboard.unichar;
									}
								}
								break;
						}
					}
					break;
					case IN_GAME_TYPING:
					{
						switch(m_event.keyboard.keycode)
						{
							case ALLEGRO_KEY_ENTER:
									{
										if(m_message != "")
										{
											m_bsOut.Reset();

											m_packetID = CHAT_MESSAGE_BROADCAST;

											m_bsOut.Write((RakNet::MessageID)m_packetID);

											RakNet::RakString msg;
											msg = m_message.c_str();
											m_bsOut.Write(msg);

											m_peer->Send(&m_bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, m_serverID, false);
											m_message = "";
											m_state = IN_GAME_PLAYING;
										}

										else
											m_state = IN_GAME_PLAYING;
									}
									break;
									case ALLEGRO_KEY_BACKSPACE:
									{
										if(m_message != "")
											m_message.pop_back();
									}
									break;
									default:
									{
										if((m_event.keyboard.unichar <= 125) && (m_event.keyboard.unichar >= 32))
										{
											if(m_message.size() < 100)
												m_message += m_event.keyboard.unichar;
										}
									}
									break;
						}
					}
					break;
				case IN_GAME_PLAYING:
					{
						switch(m_event.keyboard.keycode)
						{
							case ALLEGRO_KEY_ENTER:
								m_state = IN_GAME_TYPING;
							break;
						case ALLEGRO_KEY_ESCAPE:
						{
							m_bsOut.Reset();
							m_packetID = CLIENT_DISCONNECT;

							m_bsOut.Write((RakNet::MessageID)m_packetID);
							m_peer->Send(&m_bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, m_serverID, false);
						}
							break;
						}
					}
					break;
				}
			}
			break;
	}
}