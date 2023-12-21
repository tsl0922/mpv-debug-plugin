// Copyright (c) 2023 tsl0922. All rights reserved.
// SPDX-License-Identifier: GPL-2.0-only

#pragma once
#include <mpv/client.h>

#ifndef MPV_EXPORT
#ifdef _WIN32
#define MPV_EXPORT __declspec(dllexport)
#elif defined(__GNUC__) || defined(__clang__)
#define MPV_EXPORT __attribute__((visibility("default")))
#else
#define MPV_EXPORT
#endif
#endif

extern "C" MPV_EXPORT int mpv_open_cplugin(mpv_handle* handle);
