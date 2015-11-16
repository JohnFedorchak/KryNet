#include "stdafx.h"
#include <iostream>

using namespace KryNet;

using boost::format;

using std::cout;
using std::endl;

int _tmain(int argc, _TCHAR *argv[]) {
	if (!KryNetInitialize()) {
		cout << format("Could not initialize KryNet. Error = %1%") % KryNetGetLastError() << endl;
		return 0;
	}


	if (!KryNetShutdown()) {
		cout << format("Could not shutdown KryNet. Error = %1%") % KryNetGetLastError() << endl;
	}

	return 0;
}