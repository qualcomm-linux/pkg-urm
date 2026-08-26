// Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
// SPDX-License-Identifier: BSD-3-Clause-Clear

#include "PredefCallbacks.h"

static std::vector<std::pair<std::string, std::string>> gIrqAffBackup;

void irqAffinityApplierCallback(void* context) {
    if(context == nullptr) return;
    Resource* resource = static_cast<Resource*>(context);

    gIrqAffBackup.clear();
    uint64_t mask = 0;
    for(int32_t i = 0; i < resource->getValuesCount(); i++) {
        mask |= ((uint64_t)1 << (resource->getValueAt(i)));
    }

    std::string dirPath = "/proc/irq/";
    DIR* dir = opendir(dirPath.c_str());
    if(dir == nullptr) {
        return;
    }

    struct dirent* entry;
    while((entry = readdir(dir)) != nullptr) {
        std::string filePath = dirPath + std::string(entry->d_name) + "/";
        filePath.append("smp_affinity");

        if(AuxRoutines::fileExists(filePath)) {
            gIrqAffBackup.emplace_back(filePath, AuxRoutines::readFromFile(filePath));

            // Convert to hex
            std::ostringstream oss;
            oss<<std::hex<<std::nouppercase;
            if(mask == 0) {
                oss<<"0";
            } else {
                oss<<mask;
            }
            std::string hexMask = oss.str();
            TYPELOGV(NOTIFY_NODE_WRITE_S, filePath.c_str(), hexMask.c_str());
            AuxRoutines::writeToFile(filePath, hexMask);
        }
    }
    closedir(dir);
}

void irqAffinityTearCallback(void* context) {
    if(context == nullptr) return;

    for(const auto& kv : gIrqAffBackup) {
        const std::string& path = kv.first;
        const std::string& oldVal = kv.second;
        TYPELOGV(NOTIFY_NODE_RESET, path.c_str(), oldVal.c_str());
        AuxRoutines::writeToFile(path, oldVal);
    }
    gIrqAffBackup.clear();
}

static void setMsmPerfCpuFreq(void* context) {
    if(context == nullptr) return;
    Resource* resource = static_cast<Resource*>(context);

    ResConfInfo* rConf = ResourceRegistry::getInstance()->getResConf(resource->getResCode());
    if(rConf == nullptr) return;

    int32_t clusterID = resource->getClusterValue();
    std::string resourceNodePath = rConf->mResourcePath;

    // Value to write
    int32_t valueToBeWritten = resource->getValueAt(0);

    // Read the current node content, e.g. "0:0 1:0 2:0 3:0 4:0"
    std::string currentContent = AuxRoutines::readFromFile(resourceNodePath);

    // Trim trailing whitespace / newline
    currentContent.erase(currentContent.find_last_not_of(" \t\r\n") + 1);

    // Parse tokens of the form "cluster:value", update the matching cluster
    std::istringstream iss(currentContent);
    std::string token;
    std::string updatedContent;
    int8_t clusterFound = false;

    while(iss >> token) {
        std::size_t colonPos = token.find(':');
        if(colonPos != std::string::npos) {
            int32_t tokenCluster = std::stoi(token.substr(0, colonPos));
            if(tokenCluster == clusterID) {
                // Replace the value for the matching cluster
                token = std::to_string(clusterID) + ":" + std::to_string(valueToBeWritten);
                clusterFound = true;
            }
        }
        if(!updatedContent.empty()) {
            updatedContent += " ";
        }
        updatedContent += token;
    }

    if(!clusterFound) {
        return;
    }

    TYPELOGV(NOTIFY_NODE_WRITE_S, resourceNodePath.c_str(), updatedContent.c_str());
    AuxRoutines::writeToFile(resourceNodePath, updatedContent);
}

URM_REGISTER_RES_APPLIER_CB(0x008a000d, setMsmPerfCpuFreq);
URM_REGISTER_RES_APPLIER_CB(0x008a000e, setMsmPerfCpuFreq);
