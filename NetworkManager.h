#ifndef NETWORK_MANAGER_H
#define NETWORK_MANAGER_H

#include "stdafx.h"

#include "GameObject.h"
#include "Transform.h"
#include <map>
#include "RakPeerInterface.h"
#include "RakNetTypes.h"
#include "BitStream.h"
#include "RakNetTypes.h"
#include "MessageIdentifiers.h"

#define SERVER_PORT 60000
#define MAX_CLIENTS 500

/// Identifiers used by RakNet to identify packets sent
enum GameMessages
{
	POSITION_UPDATE = ID_USER_PACKET_ENUM + 1,
	MOVE_FORWARD = ID_USER_PACKET_ENUM + 2,
	MOVE_BACKWARD = ID_USER_PACKET_ENUM + 3,
	CANCEL_FORWARD = ID_USER_PACKET_ENUM + 4,
	CANCEL_BACKWARD = ID_USER_PACKET_ENUM + 5,
	TURN_LEFT = ID_USER_PACKET_ENUM + 6,
	TURN_RIGHT = ID_USER_PACKET_ENUM + 7,
	CANCEL_LEFT = ID_USER_PACKET_ENUM + 8,
	CANCEL_RIGHT = ID_USER_PACKET_ENUM + 9,

	LOGIN_ATTEMPT = ID_USER_PACKET_ENUM + 10,
	CHAT_MESSAGE_SINGLE = ID_USER_PACKET_ENUM + 11,
	CHAT_MESSAGE_BROADCAST = ID_USER_PACKET_ENUM + 12,
	CHAT_MESSAGE_ERROR = ID_USER_PACKET_ENUM + 13,
	LOGIN_SUCCESS = ID_USER_PACKET_ENUM + 14,
	LOGIN_FAILURE = ID_USER_PACKET_ENUM + 15,
	PING = ID_USER_PACKET_ENUM + 16,
	CLIENT_DISCONNECT = ID_USER_PACKET_ENUM + 17,
	OTHER_POS_UPDATE = ID_USER_PACKET_ENUM + 18,
	POS_UPDATE = ID_USER_PACKET_ENUM + 19,
	ADD_OTHER_PLAYER = ID_USER_PACKET_ENUM + 20,
	RECEIVE_SCENE = ID_USER_PACKET_ENUM + 21,
	ADD_SCENE_OBJECT = ID_USER_PACKET_ENUM + 22,
	RECEIVE_PLAYER_LIST = ID_USER_PACKET_ENUM + 23,
	JUMP = ID_USER_PACKET_ENUM + 24
};

enum State
{
	ENTER_IP, WAITING_FOR_SERVER, IN_GAME_PLAYING, IN_GAME_TYPING
};

/// Stores a message and the person that sent
struct ChatMessage
{
	ChatMessage(std::string _sender, std::string _message)
	{
		sender = _sender;
		message = _message;
	}

	/// The message
	std::string message;
	/// The sender of the message
	std::string sender;
};

/// Details about an object in the scene
struct SceneObject
{
	SceneObject(std::string _model, Vec3 _pos, Quat _rot, Vec3 _scale)
	{
		model = _model; pos = _pos; orientation = _rot; scale = _scale;
	}

	/// The model of the object
	std::string model;
	/// The position of the object
	Vec3 pos;
	/// The orientation of the object
	Quat orientation;
	/// The scale of the object
	Vec3 scale;
};

/// Base class for the client and server to inherit from, providing common functions and variables
class NetworkManager
{
	public:
		NetworkManager();
		~NetworkManager();

		void Awake();
		void Start();
		void Update();

		/// Checks for and handles new packets
		void HandlePackets();

	protected:
		RakNet::RakPeerInterface *m_peer;
		RakNet::Packet *m_packet;
		RakNet::BitStream m_bsOut;
		RakNet::MessageID m_packetID;

		/// The game object of the network character
		weak<GameObject> charObj;
		/// The transform of the network character
		weak<Transform> m_transform;
		/// The transform of the camera in the scene
		weak<Transform> m_camTransform;

		/// A list of all of the objects in the scene
		std::vector<SceneObject> m_sceneObjects;
		/// The name of the network character
		std::string m_name;

		/// List of chat messages received by the network character
		std::vector<ChatMessage> m_chatMessages;
		/// Map that converts a name to a network identifier (GUID)
		std::map<std::string, RakNet::RakNetGUID> m_nameToGUID;
		/// Map that converts a network identifier (GUID) to a name
		std::map<RakNet::RakNetGUID, std::string> m_GUIDtoName;

		State m_state;

	private:

};

#endif