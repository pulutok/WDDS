//
// Created by skyclad on 1/31/17.
//
#pragma once

#ifndef WDDS_DBCONNECTOR_H
#define WDDS_DBCONNECTOR_H

#include <mysql/mysql.h>
#include <string>

class DBConnector {
private:
    MYSQL *m_connection;
    bool m_initialized;

public:
    DBConnector();
    ~DBConnector();

    bool initialize(std::string db_server_ip, std::string db_user, std::string db_pwd, std::string db_name, int port);
    void disconnection();

    bool is_initialized();
};


#endif //WDDS_DBCONNECTOR_H
