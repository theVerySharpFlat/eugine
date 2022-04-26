//
// Created by aiden on 8/17/21.
//

#ifndef EUGINE_EGPCH_H
#define EUGINE_EGPCH_H

#include <memory>
#include <functional>

#include <unordered_set>
#include <unordered_map>

#include <eugine/core/log.h>
#include <eugine/core/types.h>

#include <ghc/filesystem.hpp>

namespace eg {
    namespace fs = ghc::filesystem;
}

#ifdef EG_PLATFORM_WINDOWS
    #include <Windows.h>
#endif

#endif //EUGINE_EGPCH_H
