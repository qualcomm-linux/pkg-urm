// Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
// SPDX-License-Identifier: BSD-3-Clause

#ifndef RESOURCE_TUNER_SOCKET_CLIENT_H
#define RESOURCE_TUNER_SOCKET_CLIENT_H

#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <cstring>

#include "Request.h"
#include "Signal.h"
#include "SafeOps.h"
#include "Utils.h"
#include "ClientEndpoint.h"
#include "ErrCodes.h"
#include "Config.h"

#define RESTUNE_SOCKET_PATH URM_SOCKET_PATH

class SocketClient : public ClientEndpoint {
private:
    int32_t sockFd;

public:
    SocketClient();
    ~SocketClient();

    virtual int32_t initiateConnection();
    virtual int32_t sendMsg(char* buf, size_t bufSize);
    virtual int32_t readMsg(char* buf, size_t bufSize);
    virtual int32_t closeConnection();
};

#endif
