#pragma once

#ifdef KRYNET_EXPORTS
#define KRYNET_API __declspec(dllexport)
#else
#define KRYNET_API __declspec(dllimport)
#endif

namespace KryNet {
	int last_error_ = 0;

	KRYNET_API bool KryNetInitialize(void);
	KRYNET_API int KryNetGetLastError(void);
	KRYNET_API bool KryNetShutdown(void);
}