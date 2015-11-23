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

#include "stdafx.h"
#include "IClient.h"
#include "../Packet.h"

using boost::asio::ip::tcp;

//#include "boost/system/error_code.hpp"

namespace KryNet {
	namespace TCP {
		struct IClient::Imp {
			IClient::Imp(void) :
				io_service(new boost::asio::io_service()),
				work(new boost::asio::io_service::work(*io_service)),
				resolver(new tcp::resolver(*io_service)),
				socket(new tcp::socket(*io_service)),
				connected(false),
				receive_packet(new Packet()) {
				boost::thread thread(boost::bind(&boost::asio::io_service::run, io_service.get()));
			}

			std::shared_ptr<boost::asio::io_service> io_service;
			std::shared_ptr<boost::asio::io_service::work> work;

			std::shared_ptr<tcp::resolver> resolver;

			std::shared_ptr<tcp::socket> socket;
			std::atomic_bool connected; // TODO: Make pointer?

			std::shared_ptr<Packet> receive_packet;
		};

		IClient::IClient() : imp_(new Imp()) {
		}

		bool IClient::Connect(const std::string& szHost, uint16_t uPort) {
			return Connect(szHost, std::to_string(uPort));
		}

		bool IClient::Connect(const std::string& szHost, const std::string& szService) {
			try {
				tcp::resolver::query query(szHost, szService);

				auto endpoint_iter = imp_->resolver->resolve(query);

				// Connect to any of the resolved endpoints.
				boost::asio::connect(*imp_->socket, endpoint_iter);

				imp_->connected = true;

				// Asynchronously read a packet header.
				ReadHeader();

				// Call user-handled callback.
				Event_OnConnected(ConnectError::SUCCESS);
			} catch (std::exception&) {
				// Call user-handled callback.
				Event_OnConnected(ConnectError::FAILURE);
				return false;
			}

			return true;
		}

		void IClient::ConnectAsync(const std::string& szHost, uint16_t uPort) {
			ConnectAsync(szHost, std::to_string(uPort));
		}

		void IClient::ConnectAsync(const std::string& szHost, const std::string& szService) {
			tcp::resolver::query query(szHost, szService);

			auto endpoint_iter = imp_->resolver->resolve(query);

			// Connect to any of the resolved endpoints.
			boost::asio::async_connect(
				*imp_->socket,
				endpoint_iter,
				[this](const boost::system::error_code& ec, tcp::resolver::iterator iterator) {
					// Make sure there was no error.
					if (!ec) {
						imp_->connected = true;

						// Call user-handled callback.
						Event_OnConnected(ConnectError::SUCCESS);
					} else {
						// Call user-handled callback.
						Event_OnConnected(ConnectError::FAILURE);
						SetDisconnected();
					}
				}
			);
		}

		bool IClient::Send(Packet& packet) {
			try {
				// Write the full packet data to the socket.
				boost::asio::write(*imp_->socket, boost::asio::buffer(packet.Data(), packet.Size()));
			} catch (std::exception&) {
				SetDisconnected();
				return false;
			}

			return true;
		}

		void IClient::Disconnect() {
			try {
				// Shutdown the socket.
				imp_->socket->shutdown(boost::asio::socket_base::shutdown_both);

				SetDisconnected();
			} catch (std::exception&) {

			}
		}

		bool IClient::is_connected() const {
			return imp_->connected;
		}

		uint16_t IClient::local_port() const {
			try {
				return imp_->socket->local_endpoint().port();
			} catch (std::exception&) {
				return 0;
			}
		}

		std::string IClient::local_address() const {
			try {
				return imp_->socket->local_endpoint().address().to_string();
			} catch (std::exception&) {
				return "";
			}
		}

		uint16_t IClient::remote_port() const {
			try {
				return imp_->socket->remote_endpoint().port();
			} catch (std::exception&) {
				return 0;
			}
		}

		std::string IClient::remote_address() const {
			try {
				return imp_->socket->remote_endpoint().address().to_string();
			} catch (std::exception&) {
				return "";
			}
		}

		IClient::~IClient() {
		}

		void IClient::SetDisconnected() {
			try {
				imp_->connected = false;
				imp_->socket->close();
			} catch (std::exception&) {

			}

			// Call user-handled callback.
			Event_OnDisconnected();
		}

		void IClient::ReadHeader() {
			imp_->receive_packet->Clear();

			boost::asio::async_read(
				*imp_->socket,
				boost::asio::buffer(imp_->receive_packet->Data(), Packet::header_size),
				[this](const boost::system::error_code& ec, size_t bytes_transferred) {
					// Make sure there was no error and a full packet header has been received.
					if (!ec && bytes_transferred == Packet::header_size) {
						// Asynchronously read the rest of the packet.
						ReadBody();
					} else {
						SetDisconnected();
					}
				}
			);
		}

		void IClient::ReadBody() {
			boost::asio::async_read(
				*imp_->socket,
				boost::asio::buffer(imp_->receive_packet->Body(), imp_->receive_packet->DecodeHeader()),
				[this](const boost::system::error_code& ec, size_t bytes_transferred) {
					// Make sure there was no error and a full packet has been received.
					if (!ec && bytes_transferred == imp_->receive_packet->DecodeHeader()) {
						// Call user-handled callback.
						Event_OnPacketReceived(*imp_->receive_packet);

						// Asynchronously read a packet header.
						ReadHeader();
					} else {
						SetDisconnected();
					}
				}
			);
		}
	}
}

