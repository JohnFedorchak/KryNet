#pragma once

#ifdef KRYNET_EXPORTS
#define KRYNET_API __declspec(dllexport)
#else
#define KRYNET_API __declspec(dllimport)
#endif

namespace KryNet {
	enum class ConnectError {
		SUCCESS,
		FAILURE
	};

	struct TargetAddress {
		std::string host;
		std::string port;
	};

	namespace TCP {
		class IClient {
			struct Imp;
			std::unique_ptr<Imp> imp_;

		public:
			// Constructors
			KRYNET_API IClient(void);

			// Methods
			KRYNET_API virtual bool Connect(const std::string& szHost, uint16_t uPort);
			KRYNET_API virtual bool Connect(const std::string& szHost, const std::string& szService);

			KRYNET_API void ConnectAsync(const std::string& szHost, uint16_t uPort);
			KRYNET_API void ConnectAsync(const std::string& szHost, const std::string& szService);

			// Getters
			KRYNET_API bool Connected(void) const;

			// Callbacks
			virtual void Event_OnConnected(const ConnectError& error) = 0;
			virtual void Event_OnPacketReceived(void) = 0;
			virtual void Event_OnPacketSent(void) = 0;
			virtual void Event_OnDisconnected(void) = 0;

			KRYNET_API virtual ~IClient(void);
		};
	}
}

