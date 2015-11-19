#pragma once

#include "stdafx.h"

using boost::format;

using std::cout;
using std::endl;

class Client : public KryNet::ITCPClient {
	uint32_t identifier_;
public:
	Client(uint32_t identifier) : identifier_(identifier) {

	}

	uint32_t GetId(void) const { return identifier_; }

	void Client::Event_OnConnected() override {
		cout << format("Client %1%: Connected!") % identifier_ << endl;
	}

	void Client::Event_OnPacketReceived() override {
		cout << format("Client %1%: Packet received!") % identifier_ << endl;
	}

	void Client::Event_OnPacketSent() override {
		cout << format("Client %1%: Packet sent!") % identifier_ << endl;
	}

	void Client::Event_OnDisconnected() override {
		cout << format("Client %1%: Disconnected!") % identifier_ << endl;
	}

	~Client() {}

	friend std::ostream& operator<<(std::ostream& os, const Client& client);
};

inline std::ostream& operator<<(std::ostream& os, const Client& client) {
	os << format("[Client (ID=%1%)]") % client.identifier_;
	return os;
}