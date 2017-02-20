//
// Created by skyclad on 1/24/17.
//

#pragma once

#include <iostream>
#include <memory>
#include <string>
#include <tins/tins.h>
#include "CustomDefines.h"

#ifndef WDDS_SCANNER_H
#define WDDS_SCANNER_H

class Scanner {
private:
    std::string m_device;

public:
    void scanWithCallback(bool (*)(Tins::PDU&));
    void changeChannel(int channelTo);
    Scanner(const char *dev);
    ~Scanner();
};

#endif //WDDS_SCANNER_H