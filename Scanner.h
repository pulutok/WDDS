//
// Created by skyclad on 1/24/17.
//

#pragma once

#include <iostream>
#include <memory>
#include <string>
#include <boost/function.hpp>

#include <pcap/pcap.h>
#include "CustomDefines.h"


#ifndef WDDS_SCANNER_H
#define WDDS_SCANNER_H

class Scanner {
private:
    typedef boost::function<void(pcap_pkthdr *, u_char *)> PacketCallbackHandler;

    char *m_errbuf;
    pcap_t *m_pcap_handle;
    std::string m_device;
public:
    void scanWithCallback(PacketCallbackHandler handler, int timeout, int count);
    Scanner(const char *dev);
    ~Scanner();
};

#endif //WDDS_SCANNER_H