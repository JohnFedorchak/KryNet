#pragma once

#include "stdafx.h"

namespace KryNet {
	static int last_error_ = 0;

	inline KRYNET_API int KryNetGetLastError(void) { return last_error_; };
}