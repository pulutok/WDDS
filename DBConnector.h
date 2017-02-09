//
// Created by skyclad on 1/31/17.
//
#pragma once

#ifndef WDDS_DBCONNECTOR_H
#define WDDS_DBCONNECTOR_H

#include <iostream>
#include <cstring>
#include <mysql/mysql.h>
#include <string>

class DBConnector {
private:
    MYSQL *m_pConnection;
    MYSQL_STMT *m_pStmt;
    MYSQL_BIND *m_pBind;
    bool m_initConn;
    bool m_initStmt;
public:
    DBConnector();
    ~DBConnector();

    bool Initialize(std::string dbServerIp, std::string dbUser, std::string dbPwd, std::string dbName, unsigned int port);
    bool IsInitialized();
    void Disconnection();

    bool SetStmt();
    bool PrepareStmt(std::string& query);
    bool CreateBind(int size);
    bool SetBind(int arrayOffset, enum_field_types fieldType, void *buffer, unsigned int bufferLength, my_bool *is_null, unsigned long *gottenLength);
    bool execute();
};


#endif //WDDS_DBCONNECTOR_H
