#pragma once
#define DEFAULT_PORT "1234"
#define DEFAULT_BUFFER_LEN 1024 //2^10 BYTES
#include <tuple>
#include <string>
#include <sstream>
#include <iostream>
#include <sys/types.h> 
#include <winsock2.h>
#include <ws2tcpip.h>
#include "Payload.h"
#include <assert.h>

class Peer {	
	public:
		enum class PeerType {
			CONNECTING_PEER,
			HOSTING_PEER
		};
		virtual ~Peer() {
			Disconnect();
			WSACleanup();
		}

		int SendGuess(struct Payload::Buffer buf) {
			std::ostringstream oss;
			int s;

			//Send an guess buffer
			if (this->gameSocket != INVALID_SOCKET) {

				// this block is used to make sure the destrcutor of archive is called
				// which flushes the output into string stream.
				{
					cereal::JSONOutputArchive oarchive(oss);
					oarchive(buf);
				}

				s = send(this->gameSocket, oss.str().c_str(), oss.str().length(), 0);
				if (s == SOCKET_ERROR && (WSAGetLastError() == WSAECONNRESET || WSAGetLastError() == WSAETIMEDOUT)) {
					std::cout << "TIMEOUT: Failed to Send data to Opponent...\n";
					s = 0;
				}
				else {
					std::cout << "Bytes sent: " << s << ".\n";
				}
			}
			else {
				std::cout << "Socket error: " << WSAGetLastError() << ".\n";
				Disconnect();
				s = INVALID_SOCKET;
			}

			return s;
		}

		struct Payload::Buffer ReceiveData() {
			char recvBuffer[DEFAULT_BUFFER_LEN];
			int recvBufferLen = DEFAULT_BUFFER_LEN;
			int r = SOCKET_ERROR;

			r = recv(this->gameSocket, recvBuffer, recvBufferLen, 0);
			if (r > 0) {
				std::cout << "Bytes recieved: " << r << ".\n";
				if (r == DEFAULT_BUFFER_LEN) {
					std::cout << "BUFFER OVERFLOW: Some data may be lost!\n";
				}
				recvBuffer[r] = '\0'; //Must place null terminator for inputarchive to function properly!!

				struct Payload::Buffer buf;
				std::istringstream iss(recvBuffer);
				std::cout << iss.str() << "\n";
				// this block is used to make sure the destrcutor of archive is called
				// ensure RAII (some archives can only safely finish flushing their contents upon destruction)

				{
					cereal::JSONInputArchive iarchive(iss);
					iarchive(buf);

				}

				std::cout << "GUESS = " << buf.xChoordGuess << " : " << buf.yChoordGuess << " prevHit = " << buf.prevGuessHit << ".\n";

				return buf;
			}
			else if (r == SOCKET_ERROR && (WSAGetLastError() == WSAECONNRESET || WSAGetLastError() == WSAETIMEDOUT)) {
				//Other player has disconnected and has forfeited 
				std::cout << "TIMEOUT: Failed to Recieve data from Opponent...\n";
				struct Payload::Buffer buf;
				buf.bufferType = Payload::BufferType::GAME_OVER;
				return buf;
			}
			else {
				std::cout << "recv failed: " << WSAGetLastError() << ".";
				Disconnect();
				exit(-1);
			}
		}

		void setBlocking(bool blocking) {
			assert(gameSocket != INVALID_SOCKET);
			u_long blockingMode = blocking? 0 : 1;  //0 for blocking, 1 for non-blocking
			int result = ioctlsocket(this->gameSocket, FIONBIO, &blockingMode);
			if (result != 0) {
				std::cout << "ioctlsocket failed with error:" << WSAGetLastError() << ".";
			}
		}

		void setupWSAPollFD() {
			this->socketFD = {};
			this->socketFD.fd = gameSocket;
			this->socketFD.events = POLLRDNORM | POLLWRNORM; //we care about reading and writing to this socket
		}

		WSAPOLLFD getPollFD() { return this->socketFD; }

	protected:
		void Disconnect() {
			//Shutdown the send half of the connection since no more data will be sent
			int r = shutdown(this->gameSocket, SD_SEND);
			closesocket(this->gameSocket);
		}
		/*
		AF_INET is used to specify the IPv4 address family
		SOCK_STREAM is used to specify a stream socket
		IPPROTO_TCP is used to specify the TCP protocol
		AI_PASSIVE flag indicates the caller intends to use the returned socket address structure in a call to the bind function
		When the AI_PASSIVE flag is set and nodename parameter to the getaddrinfo function is a NULL pointer,
		the IP address portion of the socket address structure is set to INADDR_ANY for IPv4 addressess
		*/
		void GetAddressInfo() {
			ZeroMemory(&this->hints, sizeof(this->hints));
			this->hints.ai_family = AF_INET;
			this->hints.ai_socktype = SOCK_STREAM;
			this->hints.ai_protocol = IPPROTO_TCP;
			this->hints.ai_flags = AI_PASSIVE;


			// Resolve the local address and port to be used by the server
			int r = getaddrinfo(NULL, DEFAULT_PORT, &this->hints, &this->result);
			if (r != 0) {
				std::cout << "getaddrinfo failed: " << r << ".";
				WSACleanup();
				exit(-1);
			}
		}

		void GetAddressInfo(std::string destinationAddress) {
			ZeroMemory(&this->hints, sizeof(this->hints));
			this->hints.ai_family = AF_INET;
			this->hints.ai_socktype = SOCK_STREAM;
			this->hints.ai_protocol = IPPROTO_TCP;
			this->hints.ai_flags = AI_PASSIVE;


			//Convert the hostAddress to PCStr type to pass to getaddrInfo func
			PCSTR destAddr = destinationAddress.c_str();

			// Resolve the local address and port to be used by the server
			int r = getaddrinfo(destAddr, DEFAULT_PORT, &this->hints, &this->result);
			if (r != 0) {
				std::cout << "getaddrinfo failed: " << r << ".";
				WSACleanup();
				exit(-1);
			}
		}

		/*
		Create a SOCKET object to interface with the other peer
		*/
		void CreateSocket() {
			SOCKET connectionSocket = INVALID_SOCKET;
			// Attempt to connect to the first address returned by the call to getaddrinfo
			for (this->ptr = this->result; this->ptr != NULL; this->ptr = this->ptr->ai_next) {
				connectionSocket = socket(this->ptr->ai_family, this->ptr->ai_socktype, this->ptr->ai_protocol);
				if (connectionSocket == INVALID_SOCKET) {
					std::cout << "Error at socket(): " << WSAGetLastError() << ".";
					freeaddrinfo(this->result);
					WSACleanup();
					exit(-1);
				}
				else {
					break; //Valid connection was made and the game socket was created;
				}

			}

		
			this->gameSocket = connectionSocket;
			//Set recv time out to 1 minute
			setsockopt(this->gameSocket, SOL_SOCKET, SO_RCVTIMEO, (char*)&this->recvTimeOut, sizeof(DWORD)); 
			//Set send time out to 1 minute
			setsockopt(this->gameSocket, SOL_SOCKET, SO_SNDTIMEO, (char*)&this->sendTimeOut, sizeof(DWORD));

		}

		void SetSocketTimeouts(DWORD rTO, DWORD sTO) {
			int r;
			r = setsockopt(this->gameSocket, SOL_SOCKET, SO_RCVTIMEO, (char*)&rTO, sizeof(DWORD));
			this->recvTimeOut = rTO;
			if (r != 0) {
				std::cout << "Error setting socket Recieve time out: " << WSAGetLastError() << "\n";
			}

			r = setsockopt(this->gameSocket, SOL_SOCKET, SO_SNDTIMEO, (char*)&sTO, sizeof(DWORD));
			if (r != 0) {
				std::cout << "Error setting socket Send time out: " << WSAGetLastError() << "\n";
			}
			this->sendTimeOut = sTO;
		}

	
		WSAData wsaData;
		int queueLength = 1;
		DWORD recvTimeOut = 60000; //Timeout in miliseconds
		DWORD sendTimeOut = 60000; //Timeout in miliseconds
		SOCKET gameSocket = INVALID_SOCKET;
		struct addrinfo* result = nullptr;
		struct addrinfo* ptr = nullptr;
		struct addrinfo hints;

		//WSAPOLL file-descriptor for polling non-blocking socket stuff
		WSAPOLLFD socketFD;
};