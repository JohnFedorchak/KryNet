#include "stdafx.h"
#include "Client.h"

using namespace KryNet;

using boost::format;

using std::cout;
using std::endl;

int _tmain(int argc, _TCHAR* argv[]) {
	// Create a client with ID 1.
	Client clientSync(1);
	
	// Connect the client to a local server.
	if (!clientSync.Connect("localhost", 31412)) {
		return 0;
	}

	// Wait for client to disconnect.
	while (clientSync.Connected()) {
		Sleep(10);
	}

	cout << "Exiting..." << endl;

	return 0;
}

