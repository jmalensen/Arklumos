#pragma once

#include <iostream>
#include <memory>
#include <utility>
#include <algorithm>
#include <functional>

#include <string>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <unordered_set>

#include "Arklumos/Log.h"

#ifdef AK_PLATFORM_WINDOWS
#include "Arklumos/Windows.h"
#endif

#ifdef AK_PLATFORM_LINUX
#include "Arklumos/Windows.h"
#endif