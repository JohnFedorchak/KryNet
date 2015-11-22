#pragma once
#include "PacketType.h"

using namespace KryNet;

using boost::format;

using std::cout;
using std::endl;

class Client : public TCP::IClient {
	uint32_t identifier_;
public:
	// Constructors
	Client(uint32_t identifier) : identifier_(identifier) {
	}

	// Getters
	uint32_t Id(void) const {
		return identifier_;
	}

	// Callbacks
	void Client::Event_OnConnected(const ConnectError& error) override {
		if (error == ConnectError::SUCCESS) {
			cout << format("Client %1%: Connected to %2%:%3%!") % identifier_ % RemoteAddress() % RemotePort() << endl;

			// Create an info packet containing a string.
			InfoPacket packet(InfoPacket::InfoType::STRING);
			packet.PutBack("Hello!\r\n");

			// Send the packet.
			this->Send(packet);
		} else {
			cout << format("Client %1%: Could not connect.") % identifier_ << endl;
		}
	}

	static std::string hexStr(const BYTE* array, size_t len) {
		std::stringstream ss;

		ss << std::hex << std::uppercase << std::setfill('0');

		for (size_t i = 0; i < len; i++) {
			ss << std::setw(2) << static_cast<int>(array[i]) << " ";
		}

		return ss.str();
	}

	void Client::Event_OnPacketReceived(const Packet& packet) override {
		cout << format("Client %1%: Packet received!") % identifier_ << endl;
		cout << format("\tSize: %1% Bytes") % packet.Size() << endl;
		cout << format("\tData: %1%") % hexStr(packet.Body(), packet.BodySize()) << endl;

		MyPacket myPacket(packet);

		switch (myPacket.Type()) {
			case MyPacket::PacketType::INFO: {
				InfoPacket infoPacket(myPacket);

				switch (infoPacket.SubType()) {
					case InfoPacket::InfoType::STRING:
						cout << format("\tINFO::STRING = %1%") % (infoPacket.Data() + 2) << endl;
						break;
					default: break;
				}
				break;
			}
			case MyPacket::PacketType::REQUEST: break;
			case MyPacket::PacketType::RESPONSE: break;
			default: break;
		}
	}

	void Client::Event_OnDisconnected() override {
		cout << format("Client %1%: Disconnected!") % identifier_ << endl;
	}

	~Client() {
	}

	friend std::ostream& operator<<(std::ostream& os, const Client& client);
};

inline std::ostream& operator<<(std::ostream& os, const Client& client) {
	os << format("[Client (ID=%1%) (Connected=%2%)]") % client.identifier_ % client.Connected();
	return os;
}

