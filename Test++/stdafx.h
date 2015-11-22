#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include <stdio.h>
#include <tchar.h>
#include <iostream>
#include <string>
#include <memory>
#include <vector>

// Temporary for converting BYTE array to hexadecimal string.
#include <sstream>
#include <iomanip>

#include <boost/format.hpp>

#include "Packet.h"
#include "TCP/IClient.h"

