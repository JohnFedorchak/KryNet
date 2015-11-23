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

namespace KryNet {
	class Packet;

	enum class ConnectError {
		SUCCESS,
		FAILURE
	};

	namespace TCP {
		class IClient {
			// 
			struct Imp;
			std::unique_ptr<Imp> imp_;
		public:
			KRYNET_API IClient(void);

			/// <summary>Connects the client to a remote endpoint.</summary>
			/// <param name="host">The host to connect to.</param>
			/// <param name="port">The port to connect to.</param>
			/// <returns>Returns true if a connection was successfully established.</returns>
			KRYNET_API virtual bool Connect(const std::string& host, uint16_t port);

			/// <summary>Connects the client to a remote endpoint.</summary>
			/// <param name="host">The host to connect to.</param>
			/// <param name="service">The service to connect to.</param>
			/// <returns>Returns true if a connection was successfully established.</returns>
			KRYNET_API virtual bool Connect(const std::string& host, const std::string& service);

			/// <summary>Asynchronously connects the client to a remote endpoint.</summary>
			/// <param name="host">The host to connect to.</param>
			/// <param name="port">The port to connect to.</param>
			KRYNET_API void ConnectAsync(const std::string& host, uint16_t port);

			/// <summary>Asynchronously connects the client to a remote endpoint.</summary>
			/// <param name="host">The host to connect to.</param>
			/// <param name="service">The service to connect to.</param>
			KRYNET_API void ConnectAsync(const std::string& host, const std::string& service);

			/// <summary>Sends a packet to the remote endpoint.</summary>
			/// <param name="packet">The packet to send.</param>
			/// <returns>Returns true if the full packet was successfully sent.</returns>
			KRYNET_API bool Send(Packet& packet);

			/// <summary>Disconnects the client from the remote endpoint.</summary>
			KRYNET_API void Disconnect(void);

			/// <summary>Gets whether the client is currently connected to a remote endpoint.</summary>
			/// <returns>Returns true if the client is currently connected to a remote endpoint.</returns>
			KRYNET_API bool is_connected(void) const;

			/// <summary>Gets the local endpoint's port.</summary>
			/// <returns>Returns the local endpoint's port, or 0 if an error occurred.</returns>
			KRYNET_API uint16_t local_port(void) const;

			/// <summary>Gets the local endpoint's address.</summary>
			/// <returns>Returns the local endpoint's address, or an empty string if an error occurred.</returns>
			KRYNET_API std::string local_address(void) const;

			/// <summary>Gets the remote endpoint's port.</summary>
			/// <returns>Returns the remote endpoint's port, or 0 if an error occurred.</returns>
			KRYNET_API uint16_t remote_port(void) const;

			/// <summary>Gets the remote endpoint's address.</summary>
			/// <returns>Returns the remote endpoint's address, or an empty string if an error occurred.</returns>
			KRYNET_API std::string remote_address(void) const;

			KRYNET_API virtual ~IClient(void);
		private:
			void SetDisconnected(void);

			void ReadHeader(void);
			void ReadBody(void);

			/// <summary>Called when a client has connected to the remote endpoint.</summary>
			/// <param name="error">Set to indicate what error occurred, if any.</param>
			/// <remarks>This callback is called even if connection was not successful. <paramref name="error"> must be checked for success.</remarks>
			virtual void Event_OnConnected(const ConnectError& error) = 0;

			/// <summary>Called when a packet has been received from the remote endpoint.</summary>
			/// <param name="packet">The packet that was received.</param>
			virtual void Event_OnPacketReceived(const Packet& packet) = 0;

			/// <summary>Called when the client has been disconnected from the remote endpoint.</summary>
			virtual void Event_OnDisconnected(void) = 0;
		};
	}
}

