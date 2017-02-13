//
// Created by skyclad on 1/31/17.
//
#pragma once

#include "Scanner.h"
#include "DBConnector.h"
#include "CustomDefines.h"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <chrono>
#include <string>
#include <thread>
#include <mutex>
#include <queue>

#ifndef WDDS_WDDS_H
#define WDDS_WDDS_H

class WDDS {
private:
    bool m_endFlag;
    u_int m_channel;
    std::string m_device;

    std::queue<char *> *m_rawInputQueue;
    std::queue<char *> *m_rawOutputQueue;
    std::mutex m_rawQueueMutex;

    std::queue<WDDS_LOG> *m_parsedInputQueue;
    std::queue<WDDS_LOG> *m_parsedOutputQueue;
    std::mutex m_parsedQueueMutex;

    std::string m_DBServer;
    int m_DBPort;
    std::string m_DBUser;
    std::string m_DBPass;
    std::string m_DBName;

    void SwapRawQueue();
    void SwapParsedQueue();
    void PacketHandler(pcap_pkthdr *pkthdr, char *data);
    void Scanning();
    void Parsing();
    void Logging();
public:
    WDDS(const char *device, u_int channel, std::string config);
    ~WDDS();
    void start();
};

#endif //WDDS_WDDS_H
