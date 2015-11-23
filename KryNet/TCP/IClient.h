/*
* Copyright 2015 John Fedorchak

* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at

* http://www.apache.org/licenses/LICENSE-2.0

* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*
*/

#pragma once

#ifdef KRYNET_EXPORTS
#define KRYNET_API __declspec(dllexport)
#else
#define KRYNET_API __declspec(dllimport)
#endif

//namespace boost {
//	namespace system {
//		class error_code;
//	}
//}

namespace KryNet {
	class Packet;

	enum class ConnectError {
		SUCCESS,
		FAILURE
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

			KRYNET_API bool Send(Packet& packet);

			KRYNET_API void Disconnect(void);

			// Getters
			KRYNET_API bool Connected(void) const;

			KRYNET_API uint16_t LocalPort(void) const;
			KRYNET_API std::string LocalAddress(void) const;

			KRYNET_API uint16_t RemotePort(void) const;
			KRYNET_API std::string RemoteAddress(void) const;

			// Destructor
			KRYNET_API virtual ~IClient(void);
		private:
			// Methods
			void SetDisconnected(void);

			void ReadHeader(void);
			void ReadBody(void);

			// Callbacks
			virtual void Event_OnConnected(const ConnectError& error) = 0;
			virtual void Event_OnPacketReceived(const Packet& packet) = 0;
			virtual void Event_OnDisconnected(void) = 0;
		};
	}
}

