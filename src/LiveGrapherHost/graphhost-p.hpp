// Copyright (c) FRC Team 3512, Spartatroniks 2012-2017. All Rights Reserved.

#pragma once

#include <string>

#include "graphhost.h"

class GraphHost {
public:
    explicit GraphHost(int port);
    ~GraphHost();
    int graphData(float x, float y, std::string dataset);

private:
    struct graphhost_t* inst;
};
