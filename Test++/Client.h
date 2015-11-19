#pragma once

using namespace KryNet;

using boost::format;

using std::cout;
using std::endl;

class Client : public ITCPClient {
	uint32_t identifier_;

	std::string target_host_;
	std::string target_service_;
public:
	// Constructors
	Client(uint32_t identifier) : identifier_(identifier) {

	}

	// Methods
	bool Connect(const std::string& szHost, uint16_t uPort) override {
		return Connect(szHost, std::to_string(uPort));
	}
	bool Connect(const std::string& szHost, const std::string& szService) override {
		target_host_ = szHost;
		target_service_ = szService;

		return ITCPClient::Connect(szHost, szService);
	}

	// Getters
	uint32_t Id(void) const { return identifier_; }
	std::string TargetAddress(void) const { return target_host_; }
	std::string TargetService(void) const { return target_service_; }

	// Callbacks
	void Client::Event_OnConnected(const ConnectError& error) override {
		if (error == ConnectError::SUCCESS) {
			cout << format("Client %1%: Connected to %2%:%3%!") % identifier_ % target_host_ % target_service_ << endl;
		} else {
			cout << format("Client %1%: Could not connect to %2%:%3%.") % identifier_ % target_host_ % target_service_ << endl;
		}
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