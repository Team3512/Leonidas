// Copyright (c) 2012-2017 FRC Team 3512. All Rights Reserved.

#include "graphhost-p.hpp"

#include <string>

#include "graphhost.h"

GraphHost::GraphHost(int port) {
    inst = GraphHost_create(port);

    return;
}

GraphHost::~GraphHost() {
    GraphHost_destroy(inst);

    return;
}

int GraphHost::graphData(float x, float y, std::string dataset) {
    return GraphHost_graphData(x, y, dataset.c_str(), inst);
}
