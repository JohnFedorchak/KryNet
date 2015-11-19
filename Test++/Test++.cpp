#include "stdafx.h"
#include "Client.h"

using namespace KryNet;

using boost::format;

using std::cout;
using std::endl;

int _tmain(int argc, _TCHAR *argv[]) {
	// Create a client with ID 0.
	Client clientAsync(0), clientSync(1);

	// (Asynchronous) Connect the client to a local server.
	clientAsync.ConnectAsync("localhost", 31412);
	while (!clientAsync.Connected()) {}

	// (Synchronous) Connect the client to a local server.
	if (!clientSync.Connect("localhost", 31412)) {
		cout << "Could not connect client." << endl;
		return 0;
	}

	cout << "Exiting..." << endl;

	return 0;
}