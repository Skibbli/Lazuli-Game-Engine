#ifndef NETWORK_SERVER_H
#define NETWORK_SERVER_H

#include "stdafx.h"

#include "NetworkManager.h"
#include "BoxCollider.h"


/// Variables used by other characters that have joined
struct NetworkPlayer
{
	NetworkPlayer()
	{
		forward = false; backward = false; left = false; right = false; moved = false;
	}

	void GetInitPlayerPack(RakNet::BitStream &_stream)
	{
		RakNet::RakString rName = name.c_str();

		_stream.Write(rName);
		_stream.Write(transform.lock()->GetWorldPosition().x);
		_stream.Write(transform.lock()->GetWorldPosition().y);
		_stream.Write(transform.lock()->GetWorldPosition().z);
		_stream.Write(transform.lock()->GetWorldRotation().w);
		_stream.Write(transform.lock()->GetWorldRotation().x);
		_stream.Write(transform.lock()->GetWorldRotation().y);
		_stream.Write(transform.lock()->GetWorldRotation().z);
		_stream.Write(transform.lock()->GetWorldScale().x);
		_stream.Write(transform.lock()->GetWorldScale().y);
		_stream.Write(transform.lock()->GetWorldScale().z);
	}

	void GetPosPack(RakNet::BitStream &_stream)
	{
		_stream.Write(transform.lock()->GetWorldPosition().x);
		_stream.Write(transform.lock()->GetWorldPosition().y);
		_stream.Write(transform.lock()->GetWorldPosition().z);
		_stream.Write(transform.lock()->GetWorldRotation().w);
		_stream.Write(transform.lock()->GetWorldRotation().x);
		_stream.Write(transform.lock()->GetWorldRotation().y);
		_stream.Write(transform.lock()->GetWorldRotation().z);
		_stream.Write(transform.lock()->GetWorldScale().x);
		_stream.Write(transform.lock()->GetWorldScale().y);
		_stream.Write(transform.lock()->GetWorldScale().z);
	}

	bool Update()
	{
		if(forward)
		{
			collider.lock()->SetLinearVelocity(AXIS::Z_AXIS, 15.0f);

			moved = true;
		}

		else if(backward)
		{
			collider.lock()->SetLinearVelocity(AXIS::Z_AXIS, -15.0f);

			moved = true;
		}

		if(left)
		{
			transform.lock()->YawLeft(0.05f);
			moved = true;
		}

		else if(right)
		{
			transform.lock()->YawLeft(0.05f);
			moved = true;
		}

		return moved;
	}

	std::string name;
	RakNet::RakNetGUID m_GUID;

	bool forward, backward, left, right, moved;

	weak<Transform> transform;
	weak<BoxCollider> collider;
};


/// Implementation of the server side of the network
class NetworkServer : public NetworkManager
{
	public:
		NetworkServer();
		~NetworkServer();

		void Awake();
		void Start();
		void Update();

		/// Gets input from the user for character control
		void GetInput();

		/// Sends a list of the players in the scene to a player that has just connected
		void SendPlayerList(RakNet::RakNetGUID _ID);

		/// Checks whether the given player is online
		bool PlayerOnline(std::string _name);

		/// Adds the player's transform details to the stream to be sent in a packet to update other players
		void GetPosPack(RakNet::BitStream &_bsOut);

		/// Checks for and handles new packets
		void HandlePackets();

	protected:	
		

	private:
		/// List of the other players connected to this server
		std::map<RakNet::RakNetGUID, NetworkPlayer> m_otherPlayers;

		/// The collider of the player
		weak<BoxCollider> m_collider;

		/// Whether the player has moved this frame or not
		bool m_moved;

		/// The speed of the player
		float m_speed;

		/// The outgoing message
		std::string m_message;
		State m_state;

		ALLEGRO_EVENT_QUEUE *m_eventQueue;
		ALLEGRO_EVENT m_event;
};

#endif