#pragma once
#include "ConnectingPeer.h"

/*
The WSAStartup function is called to initiate use of WS2_32.dll

The WSADATA structure contains information about the Windows Sockets implementation
The MAKEWORD(2,2) parameter of WSAStartup makes a request for version 2.2 of Winsock on the system,
and sets the passed version as the highest version of Windows Sockets support that the caller can use
*/
ConnectingPeer::ConnectingPeer() {
	int r = 0;
	r = WSAStartup(MAKEWORD(2, 2), &this->wsaData);
	if (r != 0) {
		std::cout << "Failure to initialize Windows Sockets: " << r << ".";
		exit(-1);
	}
}


SOCKET ConnectingPeer::SearchForOpponent(std::string networkIp) {
	GetAddressInfo(networkIp);
	CreateSocket();

	ConnectToSocket(); 
	std::cout << "Searching for an Opponent in Queue...\n";
	if (this->gameSocket == INVALID_SOCKET) {
		return INVALID_SOCKET;
	}
	else {
		return this->gameSocket;
	}
}

/* Private Functions */

/*
Call the connection function
*/
void ConnectingPeer::ConnectToSocket() {
	//Connect to hosting peer
	int r = connect(this->gameSocket, this->ptr->ai_addr, (int)this->ptr->ai_addrlen);
	if (r == SOCKET_ERROR) {
		Disconnect();
		this->gameSocket = INVALID_SOCKET;
	}

	freeaddrinfo(this->result);

	if (this->gameSocket == INVALID_SOCKET) {
		std::cout << "No players currently queued in the network... Preparing to host new game!" << std::endl;
	}
}