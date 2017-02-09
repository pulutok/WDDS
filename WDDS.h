//
// Created by skyclad on 1/31/17.
//
#pragma once

#include "Scanner.h"
#include "DBConnector.h"
#include "customDefines.cpp"
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/thread/thread.hpp>
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

    void SwapRawQueue();
    void SwapParsedQueue();
    void PacketHandler(pcap_pkthdr *pkthdr, char *data);
    void Parsing();
    void Logging();
public:
    WDDS(const char *device, u_int channel);
    ~WDDS();
    void start();
};

#endif //WDDS_WDDS_H
