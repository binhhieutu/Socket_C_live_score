/*
Coded by Robel Sharma
Date: 20-08-2013
If you use in any product please
make sure to write my credits

*/

#include "StdAfx.h"
#include "ServerManager.h"
#include "Server.h"
#include "ServerDlg.h"
#include <Windows.h>
#include<winsock2.h>
#include<WS2tcpip.h>
#include"databaseClient.h"
#include"databaseMatch.h"
static SOCKET sArray[1000];
static int iCount=0;
char* Stringtochar(string str)
{

	//std::string str = "string";
	char* cstr = new char[str.length() + 1];
	strcpy(cstr, str.c_str());
	// do stuff
	return cstr;
}
ServerManager::ServerManager(CServerDlg* dialog)
{
	m_pDialog = dialog;
}


ServerManager::~ServerManager()
{
	
	closesocket(s);
    WSACleanup();
}

void ServerManager::ClearServer()
{
	if (iCount <= 0)
		return;
	closesocket(s);
    WSACleanup();

	/*
	for(int i=1;i<=iCount;++i)
	{
		DWORD dwCode;  
        GetExitCodeThread(cpTh[i]->m_hThread, &dwCode);  
        delete cpTh[i];
		//CloseHandle(m_Thread_handle[i]);
	}*/
}

void ServerManager::StartListening(int iPort)
{
	
    CString num;
	m_pDialog->GetDlgItemText(IDC_NUM_CLIENT, num);
	int tmp = _wtoi(num.GetString());
	if (tmp != 0)
		this->NumberofClient = tmp;

	iCount=0;
	printf("\nInitialising Winsock...");
    if (WSAStartup(MAKEWORD(2,2),&wsa) != 0)
    {
        printf("Failed. Error Code : %d",WSAGetLastError());
        return;
    }
     
    printf("Initialised.\n");
     
    //Create a socket
    if((s = socket(AF_INET , SOCK_STREAM , 0 )) == INVALID_SOCKET)
    {
        printf("Could not create socket : %d" , WSAGetLastError());
		m_pDialog->ShowServerInfo("Could not create socket");
    }
	//selectData(url);
	selectData2(url2);
	//selectData(url);
    printf("Socket created.\n");
     
    //Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons( iPort );
     
    //Bind
    if( bind(s ,(struct sockaddr *)&server , sizeof(server)) == SOCKET_ERROR)
    {
        printf("Bind failed with error code : %d" , WSAGetLastError());
		m_pDialog->ShowServerInfo("Bind failed with error code");
        exit(EXIT_FAILURE);
		
    }
     
    puts("Bind done");
 
    //Listen to incoming connections
    listen(s , 100);
	 char *message;
	 puts("Waiting for incoming connections...");
     m_pDialog->ShowServerInfo("Waiting for incoming connections...\r\n");
     c = sizeof(struct sockaddr_in);
     
	 while ((new_socket = accept(s, (struct sockaddr*)&client, &c)) != INVALID_SOCKET)
	 {
		 if (iCount >= this->NumberofClient) {
			 m_pDialog->ShowServerInfo("So many connection, new connection won't be allowed !\r\n");
			 continue; 
		 }
        puts("Connection accepted");
       // m_pDialog->ShowServerInfo("Connection accepted\n");
        //Reply to the client
		socklen_t len;
		struct sockaddr_storage addr;
		char ipstr[INET6_ADDRSTRLEN];
		int port;

		len = sizeof addr;
		getpeername(new_socket, (struct sockaddr*)&addr, &len);

		// deal with IPv4:
		if (addr.ss_family == AF_INET) {
			struct sockaddr_in *s = (struct sockaddr_in *)&addr;
			port = ntohs(s->sin_port);
			inet_ntop(AF_INET, &s->sin_addr, ipstr, sizeof ipstr);
		}

		printf("Peer IP address: %s\n", ipstr);
		m_pDialog->ShowServerInfo("Connected Peer IP address: "+string(ipstr)+"\r\n");
		string tmp = "Slot :" + to_string(iCount+1) + "\\" + to_string(this->NumberofClient) + "\r\n";
		m_pDialog->ShowServerInfo(tmp);
		CWinThread *cTh = AfxBeginThread(
		DataThreadFunc,
		(LPVOID)new_socket);
		++iCount;
		//m_Thread_handle[++iCount] = cTh->m_hThread;
		//cpTh[iCount] = cTh;
		sArray[iCount] = new_socket;
		message = "Connect successfully";
        send(new_socket , message , strlen(message) , 0);

		//SetStaticVariable(iTempCount, new_socket);
    }
     
    if (new_socket == INVALID_SOCKET)
    {
        printf("accept failed with error code : %d" , WSAGetLastError());
        return;
    }
}




UINT __cdecl ServerManager::DataThreadFunc(LPVOID pParam)
{	
	SOCKET pYourSocket = reinterpret_cast<SOCKET>(pParam);
    //UINT retCode = pYourClass->ThreadFunc();
	//SendReceiveData(pYourClass);


	char *message;
//	message = "Welcome to Matrix chat room.\r\n";
//  send(pYourSocket , message , strlen(message) , 0);
	char server_reply[2000];
    int recv_size;

	while((recv_size = recv(pYourSocket , server_reply , 2000 , 0)) != SOCKET_ERROR)
	{
		server_reply[recv_size] = '\0';
		string temp;
		for (int i = 0; i < 4; i++)
			temp += server_reply[i];
		if (temp == "LGIN"|| temp == "REGS" )
		{	
			string user = "",pass = "";
			for (int i = 4; server_reply[i] != '.'; i++)
				user += server_reply[i];

			for (int i = user.size()+5; server_reply[i] != '\0'; i++)
				pass += server_reply[i]; 
			bool flag = check_user_exists(user);
			if (temp == "LGIN" ){
				if (flag) {
					if (check_password(pass)) {
					char* sever_rep = Stringtochar("Login Successfully !!");
					send(pYourSocket, sever_rep, strlen(sever_rep), 0);
					delete[] sever_rep;
					//	}
					//	else
					//		if (temp == "DETL") {
					//			/*select_Match_ID(string ID);
					//			for (int i = 0; i < vector....)
					//				senddatatoclient();*/
					//		}
					}
					else
					{
						char* sever_rep = Stringtochar("Wrong password !!!\r\nPlease try again ");
						send(pYourSocket, sever_rep, strlen(sever_rep), 0);
						delete[] sever_rep;
					}
				}
				else {
					char* sever_rep = Stringtochar("User Doesn't exists\r\nPlease try again ");
					send(pYourSocket, sever_rep, strlen(sever_rep), 0);
					delete[] sever_rep;
				}
			}
			else
				if (temp == "REGS") {
					if (!flag)
					{
						insertData_Client(url,user,pass);
						char* sever_rep = Stringtochar("Register Successfully !!");
						send(pYourSocket, sever_rep, strlen(sever_rep), 0);
						delete[] sever_rep;
					}
					else
					{
						char* sever_rep = Stringtochar("User already exists\r\nPlease try again ");
						send(pYourSocket, sever_rep, strlen(sever_rep), 0);
						delete[] sever_rep;
					}
				// inset_table_user();
				}
		}
		else
		if (temp == "LTAL")
		{
			char* sever_rep = Stringtochar("ID  TIME          Team 1     Score    Team 2 \r\n");
			send(pYourSocket, sever_rep, strlen(sever_rep), 0);
			delete[] sever_rep;
			for (int i = 0; i < Matchs.size(); i++) {
				//string infoMatch = Matchs[i].id + " " + Matchs[i].time + " " + Matchs[i].teamA + " " + Matchs[i].scoreA + " " + Matchs[i].teamA + " " + Matchs[i].scoreA + " " + Matchs[i].scoreB + " " + Matchs[i].teamB+"\r\n";
				string infoMatch = Matchs[i].tostring();
				char* S = new char[infoMatch.length() + 1];
				strcpy(S, infoMatch.c_str());
				send(pYourSocket, S, strlen(S), 0);
				delete[]S;
			}
		}
		else
		if (temp == "LTID")
		{
			string ID = "";
			for (int i =4; server_reply[i] != '\0'; i++)
				ID+= server_reply[i];
			
			ID = info(ID);
			char* K = new char[ID.length() + 1];
			strcpy(K, ID.c_str());
			send(pYourSocket, K, strlen(K), 0);
			delete[]K;

		}


			//if( send(pYourSocket, server_reply, recv_size , 0) < 0)
			//{
			//	puts("Send failed");
			//	
			//	//return -1;
			//}
	}
    return 0;
}

UINT ServerManager::SendReceiveData(SOCKET cSocket)
{
	//std::vector<char> cstr(mess.c_str(), mess.c_str() + mess.size() + 1);
	//send(cSocket, server_reply, recv_size, 0)
	return 0;
}

void ServerManager::SetStaticVariable(int iC, SOCKET cS)
{
	iCount = iC;
	sArray[iCount] = cS;
}



