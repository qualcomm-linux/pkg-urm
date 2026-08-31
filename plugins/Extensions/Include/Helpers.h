// Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
// SPDX-License-Identifier: BSD-3-Clause-Clear

#ifndef URM_EXT_HELPERS_H
#define URM_EXT_HELPERS_H

#include <string>

#include "Logger.h"
#include "Resource.h"
#include "Extensions.h"
#include "UrmPlatformAL.h"
#include "SignalInternal.h"
#include "TargetRegistry.h"
#include "ResourceRegistry.h"

std::string trim(const std::string& s);
void toLower(std::string& s);
bool isWritable(const std::string& path);
int writeLineToFile(const std::string& fileName, const std::string& value);
bool readLineFromFile(const std::string& fileName, std::string& line);
void fetchMachineName(std::string& machineName);
std::string cpuMaskToHex(uint64_t mask);

#endif
