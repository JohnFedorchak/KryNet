#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include <WinSock2.h>
#include <iostream>
#include <string>
#include <memory>
#include <atomic>

#pragma comment(lib, "ws2_32")

#include <boost/asio.hpp>
#include <boost/thread.hpp>

