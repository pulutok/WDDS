#include <iostream>
#include "Scanner.h"
#include <pcap/pcap.h>

void handler(pcap_pkthdr *pkthdr, u_char *data) {
    std::clog << "===============================================" << std::endl;
    std::clog << "Timestamp : " << pkthdr->ts.tv_sec << std::endl;
    std::clog << "caplen    : " << pkthdr->caplen << std::endl;
    std::clog << "len       : " << pkthdr->len << std::endl;
}

int main(int argc, char *argv[]) {
    if(argc != 2) {
        std::cerr << "[*] Usage : " << argv[0] << " device" << std::endl;
        return -1;
    }
    Scanner s(argv[1]);
    s.scanWithCallback(handler, 0, 0);
    return 0;
}