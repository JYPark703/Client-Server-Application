/*	Server Code
	I used the header as WinSock2.h for Windows socket programming.
	And TCP connection and chat between server and client are implemented
*/
#include <stdio.h>
#include <WinSock2.h>
#include <time.h>
#include<string.h>
#include <stdlib.h>


#pragma comment(lib, "ws2_32")

#define PORT	4578 //Port to listen on
#define PACKET_SIZE 1024

DWORD WINAPI ProcessInputSend(LPVOID arg);

char line[PACKET_SIZE], chatdata[PACKET_SIZE + 1];
char *escape = "/quit";
BOOL bIsQuit;
SOCKET hClient;


int main() {
	WSADATA wsaData;
	HANDLE hThread;
	DWORD ThreadId;
	int size;

	bIsQuit = FALSE;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
	time_t tick;

	//Create Socket
	SOCKET hListen;
	hListen = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

	//Create a structure and assign a value to hold the components of the socket
	SOCKADDR_IN tListenAddr = {};
	tListenAddr.sin_family = AF_INET;
	tListenAddr.sin_port = htons(PORT);
	tListenAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	//bind
	bind(hListen, (SOCKADDR*)&tListenAddr, sizeof(tListenAddr));

	//listen
	listen(hListen, SOMAXCONN);

	//Create client side socket, crate structure to hlod information, assign value, and approve when client requests
	SOCKADDR_IN tClntAddr = {};
	int iClntSize = sizeof(tClntAddr);
	hClient = accept(hListen, (SOCKADDR*)&tClntAddr, &iClntSize);

	//Output informaion received from client
	char cBuffer[PACKET_SIZE] = {};
	recv(hClient, cBuffer, PACKET_SIZE, 0);
	tick = time(NULL);
	printf("Recv Msg: %s		%s\n", cBuffer, ctime(&tick));

	//Send information to client
	char cMsg[] = "Server Send";
	send(hClient, cMsg, strlen(cMsg), 0);

	//Chat with the client
	hThread = CreateThread(NULL, 0, ProcessInputSend, 0, 0, &ThreadId);
	if (hThread == NULL) {
		printf("fail make thread\n");
	}
	else {
		CloseHandle(hThread);
	}

	while (!bIsQuit) {
		ZeroMemory(chatdata, sizeof(chatdata));

		if ((size = recv(hClient, chatdata, PACKET_SIZE, 0)) == INVALID_SOCKET) {
			printf("recv ERROR\n");
			exit(0);
		}
		else {
			chatdata[size] = '\0';
			tick = time(NULL);
			printf("%s	%s\n", chatdata, ctime(&tick));
		}
	}

	closesocket(hClient);
	closesocket(hListen);

	WSACleanup();
	return 0;
}


DWORD WINAPI ProcessInputSend(LPVOID arg)
{
	while (TRUE) {
		if (fgets(chatdata, PACKET_SIZE, stdin)) {		//Read stream
			chatdata[strlen(chatdata) - 1] = '\0';
			sprintf(line, "\n[Server] : %s", chatdata);		//Output the first contents of the buffer
			if (send(hClient, line, strlen(line), 0) < 0) {
				printf("Write fail\n");
			}

			if (strstr(line, escape) != 0) {		//Exit command
				printf("Bye\n");
				closesocket(hClient);
				bIsQuit = TRUE;
				exit(0);
			}
		}
	}
	return 0;
}