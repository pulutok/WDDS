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

}

void DBConnector::disconnection()
{

}

bool DBConnector::initialize(std::string db_server_ip, std::string db_user, std::string db_pwd, std::string db_name,
                             int port)
{

}

bool DBConnector::is_initialized()
{
    return m_initialized;
}