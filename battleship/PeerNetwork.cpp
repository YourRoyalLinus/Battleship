#pragma once
#include "PeerNetwork.h"
//#include "ConnectingPeer.h"
//#include "HostingPeer.h"

PeerNetwork::PeerNetwork() {
}

PeerNetwork::~PeerNetwork() {
	delete playerPeer;
	gameSocket = INVALID_SOCKET;
}

bool PeerNetwork::searchForOpponent(std::string networkStartIP) {
	cPeer = new ConnectingPeer();
	gameSocket = cPeer->SearchForOpponent(networkStartIP);
	if (gameSocket == INVALID_SOCKET) {
		//std::cout << WSAGetLastError() << std::endl;
		return false;
	}
	else {
		playerPeer = cPeer;
		playerPeer->setupWSAPollFD();
		playerPeer->setBlocking(false);
		peerType = Peer::PeerType::CONNECTING_PEER;
		std::cout << "CONNECTED TO OPPONENT IN QUEUE!\n";
		return true;
	}
}
void PeerNetwork::hostGame() {
	hPeer = new HostingPeer();
	hPeer->ListenForOpponent();
	// ~~~~ WSAPoll stuff for NON-Blocking functionality ~~~~
	hPeer->setupWSAPollFD();
	hPeer->setBlocking(false);
}

bool PeerNetwork::findOpponent() {
	WSAPOLLFD& hPeerFD = hPeer->getPollFD();
	if (WSAPoll(&hPeerFD, 1, 1) > 0) {
		if (hPeerFD.revents & POLLERR) {
			std::cout << WSAGetLastError() << std::endl;
		}
		if (hPeerFD.revents & POLLRDNORM) {
			gameSocket = hPeer->AcceptConnection();
			playerPeer = hPeer;
			peerType = Peer::PeerType::HOSTING_PEER;
			std::cout << "CONNECTED TO PLAYER SEARCHING FOR OPPONENET!\n";
			return true;
		}
	}
	return false;
}
/*
	Connect to a specific player
	Each player will have a game token created that will be a some sort of hash of their IP
	Sharing your token will allow another person to request a game
*/
//void PeerNetwork::ConnectToFriend(std::string gameToken) {
//	//TODO
//}

/*
	GC Peer pointer
	In the future, will remove peer from the Peer Routing Table as well
*/
//int PeerNetwork::RemovePeer(Peer* peer) {
//	if (peer != nullptr) {
//		delete peer;
//		return 0;
//	}
//
//	return -1;
//}

//NOTE: All send calls are blocking, all recv calls are non-blocking
int PeerNetwork::SendTurn(bool turn) {
	playerPeer->setBlocking(true);
	Payload::Buffer buffer;
	buffer.bufferType = Payload::BufferType::TURN;
	buffer.yourTurn = !turn;
	return playerPeer->SendGuess(buffer);
}
int PeerNetwork::SendNextGuess(int x, int y) {
	playerPeer->setBlocking(true);
	Payload::Buffer buffer;
	buffer.bufferType = Payload::BufferType::NEXT_GUESS;
	buffer.xChoordGuess = x;
	buffer.yChoordGuess = y;
	return playerPeer->SendGuess(buffer);
}

int PeerNetwork::SendGuessResult(int oppGuessX, int oppGuessY, bool res) {
	playerPeer->setBlocking(true);
	Payload::Buffer buffer;
	buffer.bufferType = Payload::BufferType::GUESS_RESULT;
	buffer.xChoordGuess = oppGuessX;
	buffer.yChoordGuess = oppGuessY;
	buffer.prevGuessHit = res;
	return playerPeer->SendGuess(buffer);
}

int PeerNetwork::SendGameOver() {
	playerPeer->setBlocking(true);
	Payload::Buffer buffer;
	buffer.bufferType = Payload::BufferType::GAME_OVER;
	return playerPeer->SendGuess(buffer);
}

Payload::Buffer PeerNetwork::ReceiveData() {
	playerPeer->setBlocking(false);
	playerPeer->setupWSAPollFD();
	//TODO: this WSAPoll checking should probably be abstracted to some other procedure but who cares at this point
	WSAPOLLFD& peerFD = playerPeer->getPollFD();
	if (WSAPoll(&peerFD, 1, 1) > 0) {
		if (peerFD.revents & POLLERR) {
			//Other player has disconnected and has forfeited 
			std::cout << "TIMEOUT: Failed to Recieve data from Opponent...\n";
			struct Payload::Buffer buf;
			buf.bufferType = Payload::BufferType::GAME_OVER;
			return buf;
		}
		if (peerFD.revents & POLLRDNORM) {
			return playerPeer->ReceiveData();
		}
	}
	Payload::Buffer emptyResult;
	emptyResult.bufferType = Payload::BufferType::EMPTY;
	return emptyResult;
};