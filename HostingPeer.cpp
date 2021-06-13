#pragma once
#include "HostingPeer.h"

/*
The WSAStartup function is called to initiate use of WS2_32.dll

The WSADATA structure contains information about the Windows Sockets implementation
The MAKEWORD(2,2) parameter of WSAStartup makes a request for version 2.2 of Winsock on the system,
and sets the passed version as the highest version of Windows Sockets support that the caller can use
*/
HostingPeer::HostingPeer() {
	int r = 0;
	r = WSAStartup(MAKEWORD(2, 2), &this->wsaData);
	if (r != 0) {
		std::cout << "Failure to initialize Windows Sockets: " << r << ".";
		exit(-1);
	}
}


void HostingPeer::ListenForOpponent() {
	GetAddressInfo();
	CreateSocket();
	BindSocket();
	Listen();

	std::cout << "Waiting for an Opponent...\n";

}

/* Private Functions */

/*
The sockaddr structure holds information regarding the address family, IP address, and port number
*/
void HostingPeer::BindSocket() {
	// Setup the TCP listening socket
	int r = bind(this->gameSocket, this->result->ai_addr, (int)this->result->ai_addrlen);
	if (r == SOCKET_ERROR) {
		std::cout << "bind failed with error: " << WSAGetLastError() << ".";
		freeaddrinfo(this->result);
		Disconnect();
		exit(-1);
	}

	freeaddrinfo(this->result);
}

/*
Call the listen function, passing as parameters the created socket and a value for the backlog => queueLength, maximum length of the queue of pending connections to accept
*/
void HostingPeer::Listen() {
	if (listen(this->gameSocket, this->queueLength) == SOCKET_ERROR) {
		std::cout << "Listen failed with error: " << WSAGetLastError() << ".";
		Disconnect();
		exit(-1);
	}
	return;
}

/*
Accept peer connections
*/
SOCKET HostingPeer::AcceptConnection() {
	SOCKET hostPeerSocket = INVALID_SOCKET;

	hostPeerSocket = accept(this->gameSocket, NULL, NULL);
	if (hostPeerSocket == INVALID_SOCKET) {
		std::cout << "Connection refused: " << WSAGetLastError() << ".";
		this->gameSocket = INVALID_SOCKET;
	}
	this->gameSocket = hostPeerSocket;
	return this->gameSocket;
}