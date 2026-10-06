// Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
// SPDX-License-Identifier: BSD-3-Clause

#include "Extensions.h"
#include "Config.h"

__attribute__((constructor))
void registerWithResourceTuner() {
    URM_REGISTER_CONFIG(RESOURCE_CONFIG, URM_TEST_DATA_DIR "configs/ResourcesConfig.yaml")
    URM_REGISTER_CONFIG(PROPERTIES_CONFIG, URM_TEST_DATA_DIR "configs/PropertiesConfig.yaml")
    URM_REGISTER_CONFIG(SIGNALS_CONFIG, URM_TEST_DATA_DIR "configs/SignalsConfig.yaml")
    URM_REGISTER_CONFIG(TARGET_CONFIG, URM_TEST_DATA_DIR "configs/TargetConfig.yaml")
    URM_REGISTER_CONFIG(INIT_CONFIG, URM_TEST_DATA_DIR "configs/InitConfig.yaml")
    URM_REGISTER_CONFIG(APP_CONFIG, URM_TEST_DATA_DIR "configs/PerApp.yaml")
}
