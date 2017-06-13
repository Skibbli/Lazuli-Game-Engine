#ifndef NETWORK_CLIENT_H
#define NETWORK_CLIENT_H

#include "stdafx.h"

#include "NetworkManager.h"

struct NetworkPlayerClient
{
	std::string name;
	weak<Transform> transform;
};


class NetworkClient : public NetworkManager
{
	public:
		NetworkClient();
		~NetworkClient();

		void Awake();
		void Start();
		void Update();

		/// Gets/processes input from the player
		void GetInput();

		/// Checks for and processes packets received
		void HandlePackets();

		/// Loads the scene sent from the server, creating a game object for each scene object
		void LoadScene(RakNet::Packet* _pack);
		/// Adds another object to the scene
		void AddSceneObject(RakNet::Packet* _pack);
		/// Loads the list of other players connected to the server
		void LoadPlayers(RakNet::Packet* _pack);
		/// Adds an additional player that has joined the server
		void AddPlayer(RakNet::Packet* _pack);
		/// Applies a position update for another player on the network
		void ProcessOtherPosUpdate(RakNet::Packet* _pack);

	private:
		RakNet::BitStream m_bsIn;
		RakNet::RakNetGUID m_serverID;
		RakNet::RakNetGUID m_thisID;

		/// List of other players connected to the network
		std::map<RakNet::RakNetGUID, NetworkPlayerClient> m_otherPlayers;

		ALLEGRO_EVENT_QUEUE *m_eventQueue;
		ALLEGRO_EVENT m_event;

		std::string m_ipEnter;
		std::string m_message;
};

#endif