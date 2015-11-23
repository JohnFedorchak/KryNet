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
#include "Utility.h"

namespace KryNet {
	Utility::Utility() {
	}

	std::string Utility::BytesToHex(const BYTE* bytes, size_t len, std::string separator) {
		std::stringstream ss;

		// Set the mode to uppercase hex, with values under 0x10 being padded with a zero.
		ss << std::hex << std::uppercase << std::setfill('0');

		for (size_t i = 0; i < len; i++) {
			// Write the byte as a hex string.
			ss << std::setw(2) << static_cast<int>(bytes[i]);

			// Write the separator after every byte except the last one.
			if (i < len - 1) {
				ss << separator;
			}
		}

		// Return the built string.
		return ss.str();
	}

	Utility::~Utility() {
	}
}

