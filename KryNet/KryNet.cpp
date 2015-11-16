#include "stdafx.h"
#include "KryNet.h"

namespace KryNet {
	bool KryNetInitialize() {
		WSADATA wsaData;

		last_error_ = WSAStartup(MAKEWORD(2, 2), &wsaData);

		if (last_error_ != 0 || LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2) {
			return false;
		}

		return true;
	}

	int KryNetGetLastError() {
		return last_error_;
	}

	bool KryNetShutdown() {
		last_error_ = WSACleanup();

		return (last_error_ == 0);
	}
}