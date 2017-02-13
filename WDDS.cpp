//
// Created by skyclad on 1/31/17.
//

#include "WDDS.h"

// Initializer
WDDS::WDDS(const char *device, u_int channel, std::string config)
        : m_rawInputQueue(new std::queue<char *>),
          m_rawOutputQueue(new std::queue<char *>),
          m_parsedInputQueue(new std::queue<WDDS_LOG>),
          m_parsedOutputQueue(new std::queue<WDDS_LOG>),
          m_device(device),
          m_channel(channel),
          m_endFlag(false)
{
    // Todo : .ini file parsing and save configure
};

// Destructor
WDDS::~WDDS()
{
    delete m_rawInputQueue;
    delete m_rawOutputQueue;
    delete m_parsedInputQueue;
    delete m_parsedOutputQueue;
};

void WDDS::SwapRawQueue()
{
    m_rawQueueMutex.lock();
    // Critical section start
    std::queue<char *> *tmp;
    tmp = m_rawInputQueue;
    m_rawInputQueue = m_rawOutputQueue;
    m_rawOutputQueue = tmp;
    // Critical section end
    m_rawQueueMutex.unlock();
}

void WDDS::SwapParsedQueue()
{
    m_parsedQueueMutex.lock();
    // Critical section start
    std::queue<WDDS_LOG> *tmp;
    tmp = m_parsedInputQueue;
    m_parsedInputQueue = m_parsedOutputQueue;
    m_parsedOutputQueue = tmp;
    // Critical section end
    m_parsedQueueMutex.unlock();
}

void WDDS::PacketHandler(pcap_pkthdr *pkthdr, char *data)
{
    char *input_data = new char( sizeof(pkthdr) + pkthdr->caplen );
    memcpy(input_data, pkthdr, sizeof(pkthdr));
    memcpy(input_data + sizeof(pkthdr), data, pkthdr->caplen);
    m_rawQueueMutex.lock();
    // Critical section start
    m_rawInputQueue->push(input_data);
    // Critical section end
    m_rawQueueMutex.unlock();
}

void WDDS::Parsing()
{
    while(!m_endFlag)
    {
        std::this_thread::sleep_for(std::chrono::seconds(60));     // Sleep This thread 60 seconds
        SwapRawQueue();
        char *raw_packet;
        while (!m_rawOutputQueue->empty()) {

            raw_packet = m_rawOutputQueue->front();
            // Todo : Parsing & Input Parsed data to Queue
            //
            //
            m_rawOutputQueue->pop();
        }
    }
}

void WDDS::Logging()
{
    while(!m_endFlag)
    {
        std::this_thread::sleep_for(std::chrono::seconds(10));    // Sleep This thread 10 seconds
        WDDS_LOG parsed_packet;
        while (!m_parsedOutputQueue->empty()) {
            // Todo : Connect to DB
            parsed_packet = m_parsedOutputQueue->front();
            // Todo : Logging data to DB
            //
            //
            m_parsedOutputQueue->pop();
        }
    }
}

void WDDS::start()
{
    // Todo : Create Scanner/Parser/Logger Thread & when input, set m_endFlag true
}