#pragma once

#ifdef KRYNET_EXPORTS
#define KRYNET_API __declspec(dllexport)
#else
#define KRYNET_API __declspec(dllimport)
#endif

namespace KryNet {
	static int last_error_ = 0;

	inline KRYNET_API int KryNetGetLastError(void) {
		return last_error_;
	};
}

