//
// Created by skyclad on 1/24/17.
//

#include "Scanner.h"

// Initializer
Scanner::Scanner(const char *dev) : m_device(dev)
{
}

// Destructor
Scanner::~Scanner()
{
}

void Scanner::changeChannel(int channelTo) {
    char chan[64] = {0};
    sprintf(chan, "iwconfig %s channel %d", m_device.c_str(), channelTo);
    system(chan);
    std::clog << "[*] Channel Changed To : " << chan << std::endl;
}

void Scanner::scanWithCallback(WDDS *wdds, bool (WDDS::*PacketCallbackHandler)(Tins::PDU&)) {
    std::clog << "[*] Scanning Started" << std::endl;
    Tins::Sniffer sniffer(m_device, Tins::Sniffer::promisc_type::NON_PROMISC, "", true);
    sniffer.sniff_loop(Tins::make_sniffer_handler(wdds, PacketCallbackHandler));
}