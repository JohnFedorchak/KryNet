#include "stdafx.h"
//#include "KryNet.h"
#include "ITCPClient.h"
#include <thread>

using boost::asio::ip::tcp;

namespace KryNet {
	struct ITCPClient::Imp {
		ITCPClient::Imp() :
			io_service_(new boost::asio::io_service()),
			resolver_(new tcp::resolver(*io_service_)),
			socket_(new tcp::socket(*io_service_)),
			connected_(false) {

		}

		std::shared_ptr<boost::asio::io_service> io_service_;
		std::shared_ptr<tcp::resolver> resolver_;
		std::shared_ptr<tcp::socket> socket_;
		std::atomic_bool connected_;

	};

	ITCPClient::ITCPClient() : imp_(new Imp) {
	}

	bool ITCPClient::Connect(const std::string& szHost, uint16_t uPort) {
		return Connect(szHost, std::to_string(uPort));
	}

	bool ITCPClient::Connect(const std::string& szHost, const std::string& szPort) {
		try {
			std::thread t([this]() { imp_->io_service_->run(); });

			tcp::resolver::query query(szHost, szPort);

			auto endpoint_iter = imp_->resolver_->resolve(query);

			boost::asio::connect(*imp_->socket_, endpoint_iter);
		} catch (std::exception& e) {
			std::cerr << "Exception: " << e.what() << std::endl;
			// TODO: Set last_error_?
			return false;
		}

		Event_OnConnected();

		return true;
	}

	void ITCPClient::ConnectAsync(const std::string& szHost, uint16_t uPort) {
		ConnectAsync(szHost, std::to_string(uPort));
	}

	void ITCPClient::ConnectAsync(const std::string& szHost, const std::string& szPort) {
		std::thread t([this]() { imp_->io_service_->run(); });

		tcp::resolver::query query(szHost, szPort);

		auto endpoint_iter = imp_->resolver_->resolve(query);

		boost::asio::async_connect(
			*imp_->socket_,
			endpoint_iter,
			[this](boost::system::error_code ec, const tcp::resolver::iterator& it) {
			if (!ec) {
				imp_->connected_ = true;
				Event_OnConnected();
			} else {
				printf("Could not connect to %s:%s.\r\n", it->host_name().c_str(), it->service_name().c_str());
			}
		});
	}

	bool ITCPClient::Connected() {
		return imp_->connected_;
	}

	ITCPClient::~ITCPClient() {

	}
}