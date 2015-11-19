#pragma once

namespace KryNet {
	class ITCPClient {
		struct Imp;
		std::unique_ptr<Imp> imp_;

	public:
		KRYNET_API ITCPClient();

		KRYNET_API bool Connect(const std::string& szHost, uint16_t uPort);
		KRYNET_API bool Connect(const std::string& szHost, const std::string& szPort);

		KRYNET_API void ConnectAsync(const std::string& szHost, uint16_t uPort);
		KRYNET_API void ConnectAsync(const std::string& szHost, const std::string& szPort);

		KRYNET_API bool Connected(void);

		virtual void Event_OnConnected(void) = 0;
		virtual void Event_OnPacketReceived(void) = 0;
		virtual void Event_OnPacketSent(void) = 0;
		virtual void Event_OnDisconnected(void) = 0;

		KRYNET_API virtual ~ITCPClient();
	};
}