//
// Created by skyclad on 1/24/17.
//

#pragma once

#include <iostream>
#include <string>
#include <tins/tins.h>
#include "CustomDefines.h"
#include "WDDS.h"

#ifndef WDDS_SCANNER_H
#define WDDS_SCANNER_H

class WDDS;

class Scanner {
private:
    std::string m_device;

public:
    void scanWithCallback(WDDS *wdds, bool (WDDS::*)(Tins::PDU&));
    void changeChannel(int channelTo);
    Scanner(const char *dev);
    ~Scanner();
};

#endif //WDDS_SCANNER_H