//
// Created by skyclad on 1/31/17.
//

#include "DBConnector.h"

// Initializer
DBConnector::DBConnector() :
        m_initialized(false)
{

}

// Destructor
DBConnector::~DBConnector()
{
    disconnection();
}

void DBConnector::disconnection()
{
    if(m_initialized) {
        m_initialized = false;
        mysql_close(m_connection);
        delete m_connection;
        m_connection = nullptr;
    }
}

bool DBConnector::initialize(std::string db_server_ip, std::string db_user, std::string db_pwd, std::string db_name,
                             unsigned int port)
{
    bool result = false;

    if ( db_server_ip.length() && db_user.length() && db_pwd.length() )
    {
        m_connection = new MYSQL;
        mysql_init(m_connection);
        if(mysql_real_connect(m_connection, db_server_ip.c_str(), db_user.c_str(), db_pwd.c_str(), db_name.c_str(), port, nullptr, 0))
        {
            m_initialized = true;
            result = true;
        } else {
            printf("[-] MYSQL ERROR (%d) : %s\n", mysql_errno(m_connection), mysql_error(m_connection));
        }
    }
    return result;
}

bool DBConnector::is_initialized()
{
    return m_initialized;
}