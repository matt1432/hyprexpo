#pragma once

#include "globals.hpp"

#include <hyprland/src/config/ConfigValue.hpp>
#include <hyprland/src/output/Monitor.hpp>
#include <hyprland/src/state/WorkspaceState.hpp>
#include <hyprland/src/state/workspace/Resolver.hpp>
#include <hyprland/src/workspace/AbstractWorkspace.hpp>
#include <hyprland/src/workspace/HLWorkspace.hpp>

#include <string>

constexpr int64_t WORKSPACE_INVALID = -1L;

inline int64_t workspaceNumber(const PHLWORKSPACE& ws) {
    if (!ws)
        return WORKSPACE_INVALID;
    const auto n = ws->numberedID();
    return n ? static_cast<int64_t>(*n) : WORKSPACE_INVALID;
}

inline int64_t monitorActiveWorkspaceID(const PHLMONITOR& m) {
    if (!m || !m->m_activeWorkspace)
        return WORKSPACE_INVALID;
    return workspaceNumber(m->m_activeWorkspace);
}

inline int64_t resolveWorkspaceID(const std::string& selector, std::optional<PHLMONITOR> mon = std::nullopt) {
    auto target = State::Workspace::resolver()->getWorkspaceTargetFromString(selector, mon);
    if (!target.id)
        return WORKSPACE_INVALID;
    if (auto* num = std::get_if<Workspace::SWorkspaceNumberedID>(&*target.id))
        return static_cast<int64_t>(num->value);
    return WORKSPACE_INVALID;
}

namespace CompatHyprlandAPI {
struct SConfigValueCompat {
    Config::INTEGER integer = 0;
    Config::INTEGER* integerPtr = &integer;
    Config::FLOAT floating = 0;
    Config::FLOAT* floatingPtr = &floating;
    Config::STRING string;
    void* ptr = nullptr;

    void* getDataStaticPtr() {
        return ptr;
    }
};

SConfigValueCompat* getConfigValue(HANDLE handle, const std::string& name);
bool                configValueSetByUser(const std::string& name);

Config::INTEGER intValue(const std::string& name);
Config::STRING  stringValue(const std::string& name);
}
