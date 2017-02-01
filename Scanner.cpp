//
// Created by skyclad on 1/24/17.
//

#include "Scanner.h"

// Initializer
Scanner::Scanner(const char *dev) : m_device(dev), m_errbuf(new char[PCAP_ERRBUF_SIZE])
{
    m_pcap_handle = pcap_open_live(m_device.c_str(), MAX_BUF_LEN, 1, 10, m_errbuf);
    if(m_pcap_handle == NULL) {
        std::cerr << "[-] Couldn't open device\n\t=>" << m_errbuf << std::endl;
        throw;
    }
}

// Destructor
Scanner::~Scanner()
{
    delete [] m_errbuf;
}

void Scanner::scanWithCallback(PacketCallbackHandler handler, int timeout, int count) {
    std::clog << "[*] Scanning Started" << std::endl;
    u_int packet_count = 0;

    pcap_pkthdr *pkthdr;
    u_char *packet_data;
    bool endless_flag = false;

    if(count == 0)
        endless_flag = true;

    while( endless_flag || packet_count < count ) { // If count == 0, endless loop
        int res = pcap_next_ex(m_pcap_handle, &pkthdr, (const u_char **)&packet_data);
        if( res == 0 ) continue; // Nothing captured
        if( res < 0 ) {  // Error Occured
            std::cerr << "[-] Packet Capture Error : "  << pcap_geterr(m_pcap_handle) << std::endl;
            break;
        }
        ++packet_count;
        handler(pkthdr, packet_data);
    }
}