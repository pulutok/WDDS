//
// Created by skyclad on 1/31/17.
//

#include "DBConnector.h"

// Initializer
DBConnector::DBConnector() :
        m_initConn(false),
        m_initStmt(false)
{

}

// Destructor
DBConnector::~DBConnector()
{
    Disconnection();
}

void DBConnector::Disconnection()
{
    if(m_initStmt) {
        m_initStmt = false;
        mysql_stmt_close(m_pStmt);
        m_pStmt = nullptr;
    }
    if(m_initConn) {
        m_initConn = false;
        mysql_close(m_pConnection);
        delete m_pConnection;
        m_pConnection = nullptr;
    }
}

bool DBConnector::Initialize(std::string dbServerIp, std::string dbUser, std::string dbPwd, std::string dbName,
                             int port)
{
    bool result = false;

    if ( dbServerIp.length() && dbUser.length() && dbPwd.length() )
    {
        m_pConnection = new MYSQL;
        mysql_init(m_pConnection);
        if(mysql_real_connect(m_pConnection, dbServerIp.c_str(), dbUser.c_str(), dbPwd.c_str(), dbName.c_str(), (unsigned)port, nullptr, 0))
        {
            m_initConn = true;
            result = true;
        } else {
            printf("[-] ERROR in DBConnector::Initialize()\n\t(%d) %s\n", mysql_errno(m_pConnection), mysql_error(m_pConnection));
        }
    }
    return result;
}

bool DBConnector::IsInitialized()
{
    return m_initConn;
}

bool DBConnector::SetStmt()
{
    bool result = false;
    if(m_initConn)
    {
        m_pStmt = mysql_stmt_init(m_pConnection);
        if(m_pStmt)
        {
            m_initStmt = true;
            result = true;
        }
    }
    return result;
}

bool DBConnector::PrepareStmt(std::string &query)
{
    bool result = true;
    if(mysql_stmt_prepare(m_pStmt, query.c_str(), query.length()))
    {
        printf("[-] ERROR in DBConnector::PrepareStmt()\n\t(%d) %s\n", mysql_stmt_errno(m_pStmt), mysql_stmt_error(m_pStmt));
        result = false;
    }
    return result;
}

bool DBConnector::CreateBind(int size)
{
    bool result = false;
    m_pBind = new MYSQL_BIND[size];
    if(m_pBind)
    {
        memset(m_pBind, 0, sizeof(MYSQL_BIND) * size);
        result = true;
    }
    return result;
}

bool DBConnector::SetBind(int arrayOffset, enum_field_types fieldType, void *buffer, unsigned long bufferLength, my_bool *is_null,
                          unsigned long *gottenLength)
{
    bool result = true;
    try
    {
        m_pBind[arrayOffset].buffer_type    = fieldType;
        m_pBind[arrayOffset].buffer         = buffer;
        m_pBind[arrayOffset].buffer_length  = bufferLength;
        m_pBind[arrayOffset].is_null        = is_null;
        m_pBind[arrayOffset].length         = gottenLength;
    }
    catch( ... )
    {
        result = false;
    }
    return result;
}

bool DBConnector::execute()
{
    bool result = false;
    if(mysql_stmt_bind_param(m_pStmt, m_pBind) == 0
       && mysql_stmt_execute(m_pStmt) == 0)
    {
        result = true;
    }
    else
    {
        printf("[-] ERROR in DBConnector::execute()\n\t(%d) %s\n", mysql_stmt_errno(m_pStmt), mysql_stmt_error(m_pStmt));
    }
    return result;
}
