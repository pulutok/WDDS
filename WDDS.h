//
// Created by skyclad on 1/31/17.
//
#pragma once

#include "Scanner.h"
#include <queue>

#ifndef WDDS_WDDS_H
#define WDDS_WDDS_H

class WDDS {
private:
    u_int m_channel;
    std::string m_device;
    std::queue<char *> *m_input_queue;
    std::queue<char *> *m_output_queue;
    void swap_queue();
    void packet_handler(pcap_pkthdr *, char *data);
public:
    WDDS(const char *, u_int);
    ~WDDS();
    void start();
};

#endif //WDDS_WDDS_H
