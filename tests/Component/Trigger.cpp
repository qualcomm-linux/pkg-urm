// Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
// SPDX-License-Identifier: BSD-3-Clause

#include <getopt.h>

#include "URMTests.h"
#include "Extensions.h"
#include "Config.h"

#define TEST_CLASS "COMPONENT"

URM_REGISTER_CONFIG(RESOURCE_CONFIG, URM_TEST_DATA_DIR "configs/ResourcesConfig.yaml")
URM_REGISTER_CONFIG(PROPERTIES_CONFIG, URM_TEST_DATA_DIR "configs/PropertiesConfig.yaml")
URM_REGISTER_CONFIG(SIGNALS_CONFIG, URM_TEST_DATA_DIR "configs/SignalsConfig.yaml")
URM_REGISTER_CONFIG(TARGET_CONFIG, URM_TEST_DATA_DIR "configs/TargetConfig.yaml")
URM_REGISTER_CONFIG(INIT_CONFIG, URM_TEST_DATA_DIR "configs/InitConfig.yaml")
URM_REGISTER_CONFIG(APP_CONFIG, URM_TEST_DATA_DIR "configs/PerApp.yaml")


int32_t main(int32_t argc, char* argv[]) {
    const char* shortPrompts = "hp:";
    const struct option longPrompts[] = {
        {"help", no_argument, nullptr, 'h'},
        {"npath", required_argument, nullptr, 'p'},
        {nullptr, no_argument, nullptr, 0}
    };

    std::string nodesPath = "";

    int32_t c;
    while((c = getopt_long(argc, argv, shortPrompts, longPrompts, nullptr)) != -1) {
        switch(c) {
            case 'h':
                std::cout<<"This suite tests individual URM components."<<std::endl;
                std::cout<<"Usage: <path_to_binary> [--npath <path_to_custom_test_nodes>]"<<std::endl;
                std::cout<<"Example: /usr/bin/UrmComponentTests"<<std::endl;
                std::cout<<"Or: /usr/bin/UrmComponentTests --npath \"/run/urm/tests/nodes\""<<std::endl;
                return 0;
            case 'p':
                nodesPath = optarg;
                break;
            default:
                break;
        }
    }

    if(nodesPath.length() > 0) {
        TestAggregator::setBaseTestNodePath(nodesPath);
    }

    return TestAggregator::runAll(TEST_CLASS);
}
