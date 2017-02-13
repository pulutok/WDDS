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
    boost::property_tree::ptree pt;
    boost::property_tree::ini_parser::read_ini(config, pt);

    m_DBServer = pt.get<std::string>("Database.DB_SERVER");
    m_DBPort   = pt.get<int>("Database.DB_PORT");
    m_DBUser   = pt.get<std::string>("Database.DB_USER");
    m_DBPass   = pt.get<std::string>("Database.DB_PASS");
    m_DBName   = pt.get<std::string>("Database.DB_NAME");

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
    DBConnector conn;
    std::string query = "INSERT INTO log (mac_addr, channel, strength, timestamp) VALUES (?, ?, ?, ?)";
    while(!m_endFlag)
    {
        std::this_thread::sleep_for(std::chrono::seconds(10));    // Sleep This thread 10 seconds
        WDDS_LOG parsed_packet;

        // Todo : Connect to DB
        if ( conn.Initialize(m_DBServer, m_DBUser, m_DBPass, m_DBName, m_DBPort) )
        {
            if (conn.SetStmt()
               && conn.CreateBind(4)
               && conn.PrepareStmt(query))
            {
                m_parsedQueueMutex.lock();
                // Critical section start
                while (!m_parsedOutputQueue->empty())
                {
                    parsed_packet = m_parsedOutputQueue->front();
                    // Todo : Logging data to DB
                    conn.SetBind(0, MYSQL_TYPE_STRING, (char *)(&parsed_packet.src_mac), parsed_packet.src_mac.length(), 0, 0);
                    conn.SetBind(1, MYSQL_TYPE_LONG, (char *)(&parsed_packet.channel), sizeof(unsigned long), 0, 0);
                    conn.SetBind(2, MYSQL_TYPE_FLOAT, (char *)(&parsed_packet.strength), sizeof(float), 0, 0);
                    conn.SetBind(3, MYSQL_TYPE_LONG, (char *)(&parsed_packet.channel), sizeof(unsigned long), 0, 0);

                    if ( !conn.execute() )
                    {
                        std::cerr << "[-] Error occurred in execute";
                    }
                    m_parsedOutputQueue->pop();
                }
                // Critical section end
                m_parsedQueueMutex.unlock();
            }
            conn.Disconnection();
        };

    }
}

void WDDS::start()
{
    // Todo : Create Scanner/Parser/Logger Thread & when input, set m_endFlag true
}