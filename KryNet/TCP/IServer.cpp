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
#include "IServer.h"
#include "Packet.h"

using boost::asio::ip::tcp;

namespace KryNet {
	namespace TCP {
		struct IServer::Imp {
			IServer::Imp() :
				io_service(new boost::asio::io_service()),
				work(new boost::asio::io_service::work(*io_service)),
				resolver(new tcp::resolver(*io_service)),
				acceptor(new tcp::acceptor(*io_service)),
				socket(new tcp::socket(*io_service)) {

			}

			std::shared_ptr<boost::asio::io_service> io_service;
			std::shared_ptr<boost::asio::io_service::work> work;

			std::shared_ptr<tcp::resolver> resolver;
			std::shared_ptr<tcp::acceptor> acceptor;

			std::shared_ptr<tcp::socket> socket;
		};

		IServer::IServer() : imp_(new Imp()) {
		}

		bool IServer::Listen(uint16_t port) {
			try {
				imp_->acceptor->bind(tcp::endpoint(tcp::v4(), port));
			} catch (std::exception&) {
				return false;
			}

			return true;
		}

		IServer::~IServer() {
		}
	}
}

