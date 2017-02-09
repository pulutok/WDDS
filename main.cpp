// Coding Convention : Cemel case

#include <iostream>
#include "Scanner.h"
#include <pcap/pcap.h>
#include "DBConnector.h"
#include <ctime>

void handler(pcap_pkthdr *pkthdr, u_char *data) {
    std::clog << "===============================================" << std::endl;
    std::clog << "Timestamp : " << pkthdr->ts.tv_sec << std::endl;
    std::clog << "caplen    : " << pkthdr->caplen << std::endl;
    std::clog << "len       : " << pkthdr->len << std::endl;
}

int main(int argc, char *argv[]) {
    DBConnector connector;
    if ( !connector.Initialize("localhost", "root", "root", "wdds", 3306) )
        return -1;

    std::string query = "INSERT INTO log (mac_addr, channel, timestamp, strength) VALUES (?, ?, ?, ?)";

    std::string test_mac = "AA:BB:CC:DD:EE:FF";
    int test_channel = 1;
    std::time_t result = std::time(nullptr);
    int test_timestamp = static_cast<int>(mktime(std::localtime(&result)));
    float test_strength = 60.5;

    if ( connector.SetStmt()
         && connector.CreateBind(4)
         && connector.PrepareStmt(query) )
    {
        connector.SetBind(0, MYSQL_TYPE_STRING, const_cast<char *>(test_mac.c_str()), (unsigned int)test_mac.length(), 0, 0);
        connector.SetBind(1, MYSQL_TYPE_LONG, (char *) &test_channel, sizeof(long), 0, 0);
        connector.SetBind(2, MYSQL_TYPE_LONG, (char *) &test_timestamp, sizeof(long), 0, 0);
        connector.SetBind(3, MYSQL_TYPE_FLOAT, (char *) &test_strength, sizeof(float), 0, 0);
        if (!connector.execute()) {
            connector.Disconnection();
            return -1;
        }
        else
        {
            std::clog << "[+] Success" << std::endl;
            connector.Disconnection();
        }
    }

    /*if(argc != 2) {
        std::cerr << "[*] Usage : " << argv[0] << " device" << std::endl;
        return -1;
    }
    Scanner s(argv[1]);
    s.scanWithCallback(handler, 0, 0);
    return 0;
     */
    return 0;
}