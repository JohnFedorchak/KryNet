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

#pragma once

#ifdef KRYNET_EXPORTS
#define KRYNET_API __declspec(dllexport)
#else
#define KRYNET_API __declspec(dllimport)
#endif

namespace KryNet {
	namespace TCP {
		class IServer {
			struct Imp;
			std::unique_ptr<Imp> imp_;
		public:
			KRYNET_API IServer(void);

			KRYNET_API bool Listen(uint16_t port);

			KRYNET_API virtual ~IServer(void);
		};
	}
}

