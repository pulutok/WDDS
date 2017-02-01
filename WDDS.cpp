//
// Created by skyclad on 1/31/17.
//

#include "WDDS.h"

// Initializer
WDDS::WDDS(const char *device, u_int channel)
        : m_input_queue(new std::queue<char *>),
          m_output_queue(new std::queue<char *>),
          m_device(device),
          m_channel(channel)
{

};

// Destructor
WDDS::~WDDS()
{
    delete m_input_queue;
    delete m_output_queue;
};

void WDDS::swap_queue()
{
    std::queue<char *> *tmp;
    tmp = m_input_queue;
    m_input_queue = m_output_queue;
    m_output_queue = tmp;
}

void WDDS::packet_handler(pcap_pkthdr *pkthdr, char *data)
{
    char *input_data = new char( sizeof(pkthdr) + pkthdr->caplen );
    memcpy(input_data, pkthdr, sizeof(pkthdr));
    memcpy(input_data + sizeof(pkthdr), data, pkthdr->caplen);
    m_input_queue->push(input_data);
}