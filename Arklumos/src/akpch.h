#pragma once

#include <iostream>
#include <memory>
#include <utility>
#include <algorithm>
#include <functional>

#include <string>
#include <sstream>
#include <array>
#include <vector>
#include <unordered_map>
#include <unordered_set>

#include "Arklumos/Core/Log.h"

#include "Arklumos/Debug/Instrumentor.h"

#ifdef AK_PLATFORM_WINDOWS
#include <Windows.h>
#endif

#ifdef AK_PLATFORM_LINUX
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/stat.h>
#endif