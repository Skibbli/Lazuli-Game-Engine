#include "stdafx.h"

#include "NetworkServer.h"
#include "MeshRenderer.h"
#include "Input.h"
#include "Core.h"


NetworkServer::NetworkServer()
{
	
}

NetworkServer::~NetworkServer()
{

}

void NetworkServer::Awake()
{

}

void NetworkServer::Start()
{
	/// Initialise Raknet variables

	m_peer = RakNet::RakPeerInterface::GetInstance();

	RakNet::SocketDescriptor sd(SERVER_PORT, 0);
	m_peer->Startup(MAX_CLIENTS, &sd, 1);
	m_peer->SetMaximumIncomingConnections(MAX_CLIENTS);


	/// Create the objects in the scene

	SceneObject floor1("curuthers.obj", Vec3(0,0,0), Quat(1,0,0,0), Vec3(1,1,1));
	m_sceneObjects.push_back(floor1);

	SceneObject floor2("curuthers.obj", Vec3(0,0,0), Quat(1,0,0,0), Vec3(1,1,1));
	m_sceneObjects.push_back(floor2);

	SceneObject floor3("curuthers.obj", Vec3(0,0,0), Quat(1,0,0,0), Vec3(1,1,1));
	m_sceneObjects.push_back(floor3);

	SceneObject floor4("curuthers.obj", Vec3(0,0,0), Quat(1,0,0,0), Vec3(1,1,1));
	m_sceneObjects.push_back(floor4);

	SceneObject floor5("curuthers.obj", Vec3(0,0,0), Quat(1,0,0,0), Vec3(1,1,1));
	m_sceneObjects.push_back(floor5);

	SceneObject floor6("curuthers.obj", Vec3(0,0,0), Quat(1,0,0,0), Vec3(1,1,1));
	m_sceneObjects.push_back(floor6);

	for(SceneObject obj : m_sceneObjects)
	{
		weak<GameObject> tempObj = GameObject::CreateGameObj("GameObject");
		tempObj.lock()->LoadModel(obj.model);
		tempObj.lock()->AddComponent<BoxCollider>().lock()->CreateColliderFromMesh(100);
		tempObj.lock()->GetComponent<Transform>().lock()->SetWorldPosition(obj.pos);
		tempObj.lock()->GetComponent<Transform>().lock()->SetWorldRotation(obj.orientation);
		tempObj.lock()->GetComponent<Transform>().lock()->SetLocalScale(obj.scale);

		tempObj.lock()->Awake();
		tempObj.lock()->Start();
	}

	m_camTransform = Core::GetCameras().at(0).lock()->GetGameObj().lock()->GetComponent<Transform>();

	m_moved = false;
	m_speed = 15.0f;
	m_state = IN_GAME_PLAYING;
}

/// Gets input from the player and processes received packets
void NetworkServer::Update()
{
	GetInput();
	HandlePackets();

	std::map<RakNet::RakNetGUID, NetworkPlayer>::iterator it3;

	/// If a player in the scene has moved, send it a position update
	for(it3 = m_otherPlayers.begin(); it3 != m_otherPlayers.end(); it3++)
	{
		if(it3->second.Update())
		{
			m_bsOut.Reset();
			m_packetID = POS_UPDATE;
			m_bsOut.Write((RakNet::MessageID)m_packetID);

			it3->second.GetPosPack(m_bsOut);
			m_peer->Send(&m_bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, it3->second.m_GUID, false);
		}
	}

	std::map<RakNet::RakNetGUID, NetworkPlayer>::iterator it;

	/// If a player in the scene has moved, notify the other players in the scene of its new position
	for(it = m_otherPlayers.begin(); it != m_otherPlayers.end(); it++)
	{
		if(it->second.moved)
		{
			std::map<RakNet::RakNetGUID, NetworkPlayer>::iterator it2;

			for(it2 = m_otherPlayers.begin(); it2 != m_otherPlayers.end(); it2++)
			{
				if(it2 != it)
				{
					m_bsOut.Reset();
					m_packetID = OTHER_POS_UPDATE;

					m_bsOut.Write((RakNet::MessageID)m_packetID);
					RakNet::RakString name = it->second.name.c_str();
					m_bsOut.Write(name);
					it->second.GetPosPack(m_bsOut);

					m_peer->Send(&m_bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, it2->second.m_GUID, false);
				}
			}
		}

		it->second.moved = false;
	}

	/// If the server player moved, notify the other players of its new position
	for(it3 = m_otherPlayers.begin(); it3 != m_otherPlayers.end(); it3++)
	{
		if(m_moved)
		{
			m_bsOut.Reset();
			m_packetID = OTHER_POS_UPDATE;
			m_bsOut.Write((RakNet::MessageID)m_packetID);

			RakNet::RakString name = m_name.c_str();
			m_bsOut.Write(name);
			GetPosPack(m_bsOut);
			m_peer->Send(&m_bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, it3->second.m_GUID, false);
		}
	}
}

/// Gets input from the player dependant on the state of the server
void NetworkServer::GetInput()
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
		m_collider.lock()->SetLinearVelocity(AXIS::Z_AXIS, m_speed);
	}

	if(Engine::Input::GetKey(ALLEGRO_KEY_S))
	{
		m_collider.lock()->SetLinearVelocity(AXIS::Z_AXIS, -m_speed);
	}

	if(Engine::Input::GetKey(ALLEGRO_KEY_A))
	{
		m_transform.lock()->YawLeft(0.05f);
	}

	if(Engine::Input::GetKey(ALLEGRO_KEY_D))
	{
		m_transform.lock()->YawRight(0.05f);
	}

	if(Engine::Input::GetKeyUp(ALLEGRO_KEY_W))
	{
		m_collider.lock()->SetLinearVelocity(AXIS::Z_AXIS, 0);
	}

	if(Engine::Input::GetKeyUp(ALLEGRO_KEY_S))
	{
		m_collider.lock()->SetLinearVelocity(AXIS::Z_AXIS, 0);
	}

	if(Engine::Input::GetKeyDown(ALLEGRO_KEY_SPACE))
	{
		m_collider.lock()->SetLinearVelocity(Y_AXIS, 5.0f);
	}

	al_wait_for_event(m_eventQueue, &m_event);

	switch(m_event.type)
	{
		case ALLEGRO_EVENT_KEY_CHAR:
			{
				switch(m_state)
				{
					case IN_GAME_TYPING:
					{
						switch(m_event.keyboard.keycode)
						{
							case ALLEGRO_KEY_ENTER:
									{
										if(m_message != "")
										{
											m_bsOut.Reset();

											m_packetID = CHAT_MESSAGE_SINGLE;

											RakNet::RakString rakSender;
											RakNet::RakString rakMessage;
											
											rakSender = m_name.c_str();
											rakMessage = m_message.c_str();

											m_bsOut.Write((RakNet::MessageID)m_packetID);
											m_bsOut.Write(rakSender);
											m_bsOut.Write(rakMessage);		

											std::map<RakNet::RakNetGUID, NetworkPlayer>::iterator it;

											for(it = m_otherPlayers.begin(); it != m_otherPlayers.end(); it++)
											{
												/*m_packetID = CHAT_MESSAGE_SINGLE;

												m_bsOut.Write((RakNet::MessageID)m_packetID);
												m_bsOut.Write(sender);
												m_bsOut.Write(msg);		*/								

												m_peer->Send(&m_bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, it->second.m_GUID, false);
											}

											std::string senderS = rakSender.C_String();
											std::string message = rakMessage.C_String();

											m_chatMessages.push_back(ChatMessage(senderS, message));
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

/// Check for packets and process them
void NetworkServer::HandlePackets()
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
					//m_onlinePlayers.erase(m_packet->guid);
					printf("A client has disconnected.\n");
				}
				break;
			case ID_CONNECTION_LOST:
					printf("A client lost the connection.\n");
				break;
			case CLIENT_DISCONNECT:
				{
					m_otherPlayers.erase(m_packet->guid);
					m_nameToGUID.erase(m_GUIDtoName[m_packet->guid]);
					m_GUIDtoName.erase(m_packet->guid);
					printf("A client has disconnected.\n");
				}
				break;
			case LOGIN_SUCCESS:
				{
					RakNet::BitStream bsIn(m_packet->data, m_packet->length, false);

					bsIn.IgnoreBytes(sizeof(RakNet::MessageID));

					RakNet::RakString rakName;
					std::string name;

					bsIn.Read(rakName);
					name = rakName.C_String();

					weak<GameObject> newObj = GameObject::CreateGameObj(name);
					newObj.lock()->LoadModel("curuthers.obj");
					newObj.lock()->AddComponent<BoxCollider>().lock()->CreateColliderFromMesh(100);
					newObj.lock()->GetComponent<Transform>().lock()->SetWorldPosition(Vec3(0,0,0));
					newObj.lock()->GetComponent<Transform>().lock()->SetWorldRotation(Quat(1,0,0,0));
					newObj.lock()->GetComponent<Transform>().lock()->SetLocalScale(Vec3(1,1,1));

					SendPlayerList(m_packet->guid);

					NetworkPlayer newPlayer;
					newPlayer.transform = newObj.lock()->GetComponent<Transform>();
					newPlayer.collider = newObj.lock()->GetComponent<BoxCollider>();
					newPlayer.m_GUID = m_packet->guid;
					newPlayer.name = name;

					m_nameToGUID[newPlayer.name] = newPlayer.m_GUID;
					m_GUIDtoName[newPlayer.m_GUID] = newPlayer.name;

					m_bsOut.Reset();

					RakNet::MessageID ID = LOGIN_SUCCESS;
					m_bsOut.Write(ID);

					newPlayer.GetInitPlayerPack(m_bsOut);

					m_peer->Send(&m_bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, m_packet->guid, false);
				}
				break;
			case CHAT_MESSAGE_SINGLE:
				{
					RakNet::BitStream bsIn(m_packet->data, m_packet->length, false);

					bsIn.IgnoreBytes(sizeof(RakNet::MessageID));

					RakNet::RakString destination;
					RakNet::RakString msg;
					RakNet::RakNetGUID sender;

					bsIn.Read(destination);			
					bsIn.Read(msg);
					sender = m_packet->guid;

					m_bsOut.Reset();

					if(PlayerOnline(destination.C_String()))
					{
						m_packetID = CHAT_MESSAGE_SINGLE;

						m_bsOut.Write((RakNet::MessageID)m_packetID);
						m_bsOut.Write(msg);						
						m_bsOut.Write(m_GUIDtoName[m_packet->guid]);

						m_peer->Send(&m_bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, m_nameToGUID[destination.C_String()], false);
					}

					else
					{
						m_packetID = CHAT_MESSAGE_ERROR;
						m_bsOut.Write((RakNet::MessageID)m_packetID);

						m_peer->Send(&m_bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, m_packet->guid, false);
					}				
				}
				break;
				case CHAT_MESSAGE_BROADCAST:
				{
					RakNet::BitStream bsIn(m_packet->data, m_packet->length, false);

					bsIn.IgnoreBytes(sizeof(RakNet::MessageID));

					RakNet::RakString msg;
					RakNet::RakString sender;

					bsIn.Read(msg);
					sender = m_GUIDtoName[m_packet->guid].c_str();

					m_bsOut.Reset();

					m_packetID = CHAT_MESSAGE_SINGLE;

					m_bsOut.Write((RakNet::MessageID)m_packetID);
					m_bsOut.Write(sender);
					m_bsOut.Write(msg);		

					std::map<RakNet::RakNetGUID, NetworkPlayer>::iterator it;

					for(it = m_otherPlayers.begin(); it != m_otherPlayers.end(); it++)
					{
						/*m_packetID = CHAT_MESSAGE_SINGLE;

						m_bsOut.Write((RakNet::MessageID)m_packetID);
						m_bsOut.Write(sender);
						m_bsOut.Write(msg);		*/								

						m_peer->Send(&m_bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, it->second.m_GUID, false);
					}

					std::string senderS = sender.C_String();
					std::string message = msg.C_String();

					m_chatMessages.push_back(ChatMessage(senderS, message));
				}
				break;
				case MOVE_FORWARD:
					{
						m_otherPlayers[m_packet->guid].forward = true;
						//printf("Forward received\n");
					}
					break;
				case MOVE_BACKWARD:
					{
						m_otherPlayers[m_packet->guid].backward = true;
						//printf("Backward received\n");
					}
					break;
				case TURN_LEFT:
					{
						m_otherPlayers[m_packet->guid].left = true;
						//printf("Left received\n");
					}
					break;
				case TURN_RIGHT:
					{
						m_otherPlayers.at(m_packet->guid).right = true;
						//printf("Right received\n");
					}
					break;
				case CANCEL_FORWARD:
					{
						m_otherPlayers[m_packet->guid].forward = false;
						//printf("Forward cancelled\n");
					}
					break;
				case CANCEL_BACKWARD:
					{
						m_otherPlayers[m_packet->guid].backward = false;
						//printf("Back cancelled\n");
					}
					break;
				case CANCEL_LEFT:
					{
						m_otherPlayers[m_packet->guid].left = false;
						//printf("Left cancelled\n");
					}
					break;
				case CANCEL_RIGHT:
					{
						m_otherPlayers[m_packet->guid].right = false;
						//printf("Right cancelled\n");
					}
					break;
		}
	}
}

/// Adds the players in the scene to the stream to be sent to a new joiner
void NetworkServer::SendPlayerList(RakNet::RakNetGUID _ID)
{
	m_bsOut.Reset();
	RakNet::MessageID ID = RECEIVE_PLAYER_LIST;

	m_bsOut.Write(ID);
	m_bsOut.Write(m_otherPlayers.size());

	RakNet::RakString rakName;
	std::string name;
	Vec3 position;
	Quat orientation;
	Vec3 scale;
	
	m_bsOut.Write(RakNet::RakString(m_name.c_str()));

	position = m_transform.lock()->GetWorldPosition();
	m_bsOut.Write(position.x);
	m_bsOut.Write(position.y);
	m_bsOut.Write(position.z);

	orientation =  m_transform.lock()->GetWorldRotation();
	m_bsOut.Write(orientation.w);
	m_bsOut.Write(orientation.x);
	m_bsOut.Write(orientation.y);
	m_bsOut.Write(orientation.z);

	scale =  m_transform.lock()->GetWorldScale();
	m_bsOut.Write(scale.x);
	m_bsOut.Write(scale.y);
	m_bsOut.Write(scale.z);

	std::map<RakNet::RakNetGUID, NetworkPlayer>::iterator it;

	for(it = m_otherPlayers.begin(); it != m_otherPlayers.end(); it++)
	{
		it->second.GetInitPlayerPack(m_bsOut);
	}

	m_peer->Send(&m_bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, m_packet->guid, false);
}

/// Checks if the given player is online
bool NetworkServer::PlayerOnline(std::string _name)
{
	std::map<std::string, RakNet::RakNetGUID>::iterator it = m_nameToGUID.find(_name);

	if(it != m_nameToGUID.end())
		return true;

	else
		return false;
}

/// Add the transform details of the server player to the stream
void NetworkServer::GetPosPack(RakNet::BitStream &_bsOut)
{
	_bsOut.Write(m_transform.lock()->GetWorldPosition().x);
	_bsOut.Write(m_transform.lock()->GetWorldPosition().y);
	_bsOut.Write(m_transform.lock()->GetWorldPosition().z);
	_bsOut.Write(m_transform.lock()->GetWorldRotation().w);
	_bsOut.Write(m_transform.lock()->GetWorldRotation().x);
	_bsOut.Write(m_transform.lock()->GetWorldRotation().y);
	_bsOut.Write(m_transform.lock()->GetWorldRotation().z);
	_bsOut.Write(m_transform.lock()->GetWorldScale().x);
	_bsOut.Write(m_transform.lock()->GetWorldScale().y);
	_bsOut.Write(m_transform.lock()->GetWorldScale().z);
}