#include "stdafx.h"
//#include "KryNet.h"
#include "IClient.h"

using boost::asio::ip::tcp;

namespace KryNet {
	namespace TCP {
		struct IClient::Imp {
			IClient::Imp() :
				io_service(new boost::asio::io_service()),
				work(new boost::asio::io_service::work(*io_service)),
				resolver(new tcp::resolver(*io_service)),
				socket(new tcp::socket(*io_service)),
				connected(false) {
				boost::thread thread(boost::bind(&boost::asio::io_service::run, io_service.get()));
			}

			std::shared_ptr<boost::asio::io_service> io_service;
			std::shared_ptr<boost::asio::io_service::work> work;
			std::shared_ptr<tcp::resolver> resolver;
			std::shared_ptr<tcp::socket> socket;
			std::atomic_bool connected;

		};

		IClient::IClient() : imp_(new Imp) {
		}

		bool IClient::Connect(const std::string& szHost, uint16_t uPort) {
			return Connect(szHost, std::to_string(uPort));
		}

		bool IClient::Connect(const std::string& szHost, const std::string& szService) {
			try {
				tcp::resolver::query query(szHost, szService);

				auto endpoint_iter = imp_->resolver->resolve(query);

				boost::asio::connect(*imp_->socket, endpoint_iter);

				Event_OnConnected(ConnectError::SUCCESS);

			} catch (std::exception& e) {
				//std::cerr << "Exception: " << e.what() << std::endl;
				// TODO: Set last_error_?
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
				[this](boost::system::error_code ec, const tcp::resolver::iterator& it) {
					if (!ec) {
						imp_->connected = true;
						Event_OnConnected(ConnectError::SUCCESS);
					} else {
						Event_OnConnected(ConnectError::FAILURE);
					}
				}
			);
		}

		bool IClient::Connected() const {
			return imp_->connected;
		}

		IClient::~IClient() {

		}
	}
}

