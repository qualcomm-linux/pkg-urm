// Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
// SPDX-License-Identifier: BSD-3-Clause

#include "UrmSettings.h"
#include "Config.h"

int32_t UrmSettings::serverOnlineStatus = false;
MetaConfigs UrmSettings::metaConfigs{};
TargetConfigs UrmSettings::targetConfigs{};

const std::string UrmSettings::mTargetConfDir = URM_TARGET_DIR;

const std::string UrmSettings::mCommonResourcesPath =
                                    URM_COMMON_DIR "ResourcesConfig.yaml";
const std::string UrmSettings::mCustomResourcesPath =
                                    URM_CUSTOM_DIR "ResourcesConfig.yaml";
const std::string UrmSettings::mDevIndexedResourcesPath =
                                    URM_TARGET_DIR "ResourcesConfig.yaml";

const std::string UrmSettings::mCommonSignalsPath =
                                    URM_COMMON_DIR "SignalsConfig.yaml";
const std::string UrmSettings::mCustomSignalsPath =
                                    URM_CUSTOM_DIR "SignalsConfig.yaml";
const std::string UrmSettings::mDevIndexedSignalsPath =
                                    URM_TARGET_DIR "SignalsConfig.yaml";

const std::string UrmSettings::mCommonInitPath =
                                    URM_COMMON_DIR "InitConfig.yaml";
const std::string UrmSettings::mCustomInitPath =
                                    URM_CUSTOM_DIR "InitConfig.yaml";
const std::string UrmSettings::mDevIndexedInitPath =
                                    URM_TARGET_DIR "InitConfig.yaml";

const std::string UrmSettings::mCommonPropertiesPath =
                                    URM_COMMON_DIR "PropertiesConfig.yaml";
const std::string UrmSettings::mCustomPropertiesPath =
                                    URM_CUSTOM_DIR "PropertiesConfig.yaml";
const std::string UrmSettings::mDevIndexedPropertiesPath =
                                    URM_TARGET_DIR "PropertiesConfig.yaml";

const std::string UrmSettings::mCustomTargetPath =
                                    URM_CUSTOM_DIR "TargetConfig.yaml";
const std::string UrmSettings::mDevIndexedTargetPath =
                                    URM_TARGET_DIR "TargetConfig.yaml";

const std::string UrmSettings::mCustomExtFeaturesPath =
                                    URM_CUSTOM_DIR "ExtFeaturesConfig.yaml";
const std::string UrmSettings::mDevIndexedExtFeatPath =
                                    URM_TARGET_DIR "ExtFeaturesConfig.yaml";

const std::string UrmSettings::mCustomAppConfigPath =
                                    URM_CUSTOM_DIR "PerApp.yaml";
const std::string UrmSettings::mDevIndexedAppPath =
                                    URM_TARGET_DIR "PerApp.yaml";

const std::string UrmSettings::mDeviceNamePath =
                                    "/sys/devices/soc0/machine";
const std::string UrmSettings::mBaseCGroupPath =
                                    "/sys/fs/cgroup/";
const std::string UrmSettings::focusedCgroup =
                                    "urm.slice/focused.apps";

const std::string UrmSettings::mPersistenceFile =
                                    URM_RUNSTATE_DIR "urm_saved_values.txt";

int32_t UrmSettings::isServerOnline() {
    return serverOnlineStatus;
}

void UrmSettings::setServerOnlineStatus(int32_t isOnline) {
    serverOnlineStatus = isOnline;
}
