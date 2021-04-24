/*
Coded by Robel Sharma
Date: 20-08-2013
If you use in any product please
make sure to write my credits

*/
#pragma once

#include "string.h"
#include "string"
#include<winsock2.h>
using namespace std;
class CClientDlg;

class ClientCon
{
public:
	ClientCon(CClientDlg *dlg);
	~ClientCon(void);
	void StartConnect(string sAddress, int iPort, string sUsername, string password);
	void SendData(string sMessage);
	void Send_user_password_login(string user,string pass);
	void Send_user_password_REGISTER(string user, string pass);
	WSADATA wsa;
    SOCKET s;
	CClientDlg *m_pClient;
	string m_pUser;
	bool m_login = false;

};

