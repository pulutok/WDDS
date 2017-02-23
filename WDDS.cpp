//
// Created by skyclad on 1/31/17.
//

#include "WDDS.h"

// Initializer
WDDS::WDDS(const char *device, u_int channel, std::string configFile) :
          m_parsedInputQueue(new std::queue<WDDS_LOG>),
          m_parsedOutputQueue(new std::queue<WDDS_LOG>),
          m_device(device),
          m_channel(channel),
          m_endFlag(false)
{
    // Todo : .ini file parsing and save configure
    boost::property_tree::ptree pt;
    boost::property_tree::ini_parser::read_ini(configFile, pt);

    m_DBServer = pt.get<std::string>("Database.DB_SERVER");
    m_DBPort   = pt.get<int>("Database.DB_PORT");
    m_DBUser   = pt.get<std::string>("Database.DB_USER");
    m_DBPass   = pt.get<std::string>("Database.DB_PASS");
    m_DBName   = pt.get<std::string>("Database.DB_NAME");

};

// Destructor
WDDS::~WDDS()
{
    delete m_parsedInputQueue;
    delete m_parsedOutputQueue;
};


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

void WDDS::Scanning()
{
    // Todo : Scanning with Scanner
    Scanner scanner(m_device.c_str());
    try{
        scanner.changeChannel(m_channel);
        scanner.scanWithCallback(this, &WDDS::PacketHandler);
    }
    catch( ... )
    {
        std::cerr << "[-] Unexpected Error in Scanner" << std::endl;
        m_endFlag = true;
    }
};

bool WDDS::PacketHandler(Tins::PDU& pdu)
{
    Parsing(pdu);
    return true;
}

void WDDS::Parsing(Tins::PDU& pdu)
{
    // Todo : Parsing & Input Parsed data to Queue
    Tins::Dot11ProbeRequest *probeRequest;
    if ( (probeRequest = pdu.find_pdu<Tins::Dot11ProbeRequest>()) )
    {
        Tins::RadioTap radioTap = pdu.rfind_pdu<Tins::RadioTap>();

        WDDS_LOG wdds_log;

        wdds_log.strength  = radioTap.dbm_signal();
        wdds_log.src_mac   = probeRequest->addr2().to_string(); // Source Address
        wdds_log.channel   = ( ( radioTap.channel_freq() - 2412 ) / 5 ) + 1;
        wdds_log.timestamp = static_cast<u_int>(std::time(0));

        m_parsedQueueMutex.lock();
        // Critical section start
        m_parsedInputQueue->push(wdds_log);
        // Critical section end
        m_parsedQueueMutex.unlock();
    }
}

void WDDS::Logging()
{
    DBConnector conn;
    std::string query = "INSERT INTO log (mac_addr, channel, strength, timestamp) VALUES (?, ?, ?, ?)";
    while(!m_endFlag)
    {
        std::this_thread::sleep_for(std::chrono::seconds(10));    // Sleep This thread 10 seconds
        SwapParsedQueue();  // Swap Input/Output Queue
        // Todo : Connect to DB
        if (  m_parsedOutputQueue->size() > 0 && conn.Initialize(m_DBServer, m_DBUser, m_DBPass, m_DBName, m_DBPort) )
        {
            std::clog << "[*] Found Logs : " << m_parsedOutputQueue->size() << std::endl;
            if (conn.SetStmt()
               && conn.CreateBind(4)
               && conn.PrepareStmt(query))
            {
                while (!m_parsedOutputQueue->empty()) {
                    WDDS_LOG parsed_packet = m_parsedOutputQueue->front();
                    // Todo : Logging data to DB
                    conn.SetBind(0, MYSQL_TYPE_STRING, (char *)(parsed_packet.src_mac.c_str()),
                                 parsed_packet.src_mac.length(), 0, 0);
                    conn.SetBind(1, MYSQL_TYPE_LONG, (char *) (&parsed_packet.channel), sizeof(int32_t), 0, 0);
                    conn.SetBind(2, MYSQL_TYPE_LONG, (char *) (&parsed_packet.strength), sizeof(int32_t), 0, 0);
                    conn.SetBind(3, MYSQL_TYPE_LONG, (char *) (&parsed_packet.timestamp), sizeof(unsigned long), 0, 0);

                    if (!conn.execute()) {
                        std::cerr << "[-] Error occurred in execute";
                    }
                    m_parsedOutputQueue->pop();
                }
            }
            conn.Disconnection();
        };

    }
}

void WDDS::start()
{
    std::string input;
    // Todo : Create Scanner/Logger Thread & when input, set m_endFlag true
    std::thread scanThread(&WDDS::Scanning, this);
    std::thread logThread(&WDDS::Logging, this);

    scanThread.join();
    logThread.join();
}