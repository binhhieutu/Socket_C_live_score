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
#include"databaseCLient.h"

static SOCKET sArray[100];
static int iCount;
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
     
    while( (new_socket = accept(s , (struct sockaddr *)&client, &c)) != INVALID_SOCKET )
    {
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
		CWinThread *cTh = AfxBeginThread(
		DataThreadFunc,
		(LPVOID)new_socket);
		++iCount;
		//m_Thread_handle[++iCount] = cTh->m_hThread;
		//cpTh[iCount] = cTh;
		sArray[iCount] = new_socket;
		//message = "Hello Client , I have received your connection.\n";
        //send(new_socket , message , strlen(message) , 0);

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

			if (temp == "LGIN") {
				/*bool flag = check_user_exists();*/
				if (0) {
					//if (check_password) {
					// 
					/*char* sever_rep = Stringtochar("Login Successfully !!");
					send(pYourSocket, sever_rep, strlen(sever_rep), 0);
					delete[] sever_rep;*/
					//	if (temp == "LIST")
					//	{
					//		//vector match(select_match());

					//	}
					//	else
					//		if (temp == "DETL") {
					//			/*select_Match_ID(string ID);
					//			for (int i = 0; i < vector....)
					//				senddatatoclient();*/
					//		}
					//}
				}
				else {
					char* sever_rep = Stringtochar("User Doesn't exists\r\nPlease try again ");
					send(pYourSocket, sever_rep, strlen(sever_rep), 0);
					delete[] sever_rep;
				}
			}
			else
				if (temp == "REGS") {
				//check_user_exists();
				// inset_table_user();
				}
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



