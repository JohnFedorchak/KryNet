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

			//BYTE receive_data[Packet::header_size + Packet::max_body_length];
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

				boost::asio::connect(*imp_->socket, endpoint_iter);

				imp_->connected = true;

				ReadHeader();

				Event_OnConnected(ConnectError::SUCCESS);
			} catch (std::exception&) {
				//std::cerr << "Exception: " << e.what() << std::endl;
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

			boost::asio::async_connect(
				*imp_->socket,
				endpoint_iter,
				[this](const boost::system::error_code& ec, tcp::resolver::iterator iterator) {
					if (!ec) {
						imp_->connected = true;

						Event_OnConnected(ConnectError::SUCCESS);
					} else {
						Event_OnConnected(ConnectError::FAILURE);
						SetDisconnected();
					}
				}
			);
		}

		bool IClient::Send(Packet& packet) {
			try {
				boost::asio::write(*imp_->socket, boost::asio::buffer(packet.Data(), packet.Size()));
			} catch (std::exception&) {
				SetDisconnected();
				return false;
			}

			return true;
		}

		void IClient::Disconnect() {
			try {
				imp_->socket->shutdown(boost::asio::socket_base::shutdown_both);

				SetDisconnected();
			} catch (std::exception&) {

			}
		}

		bool IClient::Connected() const {
			return imp_->connected;
		}

		uint16_t IClient::LocalPort() const {
			try {
				return imp_->socket->local_endpoint().port();
			} catch (std::exception&) {
				return 0;
			}
		}

		std::string IClient::LocalAddress() const {
			try {
				return imp_->socket->local_endpoint().address().to_string();
			} catch (std::exception&) {
				return "";
			}
		}

		uint16_t IClient::RemotePort() const {
			try {
				return imp_->socket->remote_endpoint().port();
			} catch (std::exception&) {
				return 0;
			}
		}

		std::string IClient::RemoteAddress() const {
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

			Event_OnDisconnected();
		}

		void IClient::ReadHeader() {
			imp_->receive_packet->Clear();

			boost::asio::async_read(
				*imp_->socket,
				boost::asio::buffer(imp_->receive_packet->Data(), Packet::header_size),
				[this](const boost::system::error_code& ec, size_t bytes_transferred) {
					if (!ec && bytes_transferred == Packet::header_size) {
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
					if (!ec && bytes_transferred == imp_->receive_packet->DecodeHeader()) {
						Event_OnPacketReceived(*imp_->receive_packet);

						ReadHeader();
					} else {
						SetDisconnected();
					}
				}
			);
		}
	}
}

