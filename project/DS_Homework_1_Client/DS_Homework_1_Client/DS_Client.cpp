/*	Client Code
	I used the header as WinSock2.h for Windows socket programming.
	And TCP connection and chat between server and client are implemented
	*/
#include <stdio.h>
#include <WinSock2.h>
#include<process.h>
#include<time.h>
#include<string.h>
#include <stdlib.h>

#pragma comment(lib, "ws2_32")

#define PORT	4578 //Port number of server
#define PACKET_SIZE 1024
#define SERVER_IP "192.168.21.1" //IP address


DWORD WINAPI ProcessInputSend(LPVOID arg);

char line[PACKET_SIZE], chatdata[PACKET_SIZE + 1];
char *escape = "/quit";
BOOL bIsQuit;
SOCKET hSocket;

int main() {
	WSADATA wsaData;
	HANDLE hThread;
	DWORD ThreadId;
	int size;

	bIsQuit = FALSE;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
	time_t tick;

	//Create Socket
	hSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

	//Create a structure and assign a value to hold the components of the socket
	SOCKADDR_IN tAddr = {};
	tAddr.sin_family = AF_INET;
	tAddr.sin_port = htons(PORT);
	tAddr.sin_addr.s_addr = inet_addr(SERVER_IP);

	//connect
	connect(hSocket, (SOCKADDR*)&tAddr, sizeof(tAddr));

	//Send information to server
	char cMsg[] = "Client Send";
	send(hSocket, cMsg, strlen(cMsg), 0);

	//Output information received from server
	tick = time(NULL);
	char cBuffer[PACKET_SIZE] = {};
	recv(hSocket, cBuffer, PACKET_SIZE, 0);
	printf("Recv Msg : %s		%s\n", cBuffer, ctime(&tick));

	//Chat with the server
	hThread = CreateThread(NULL, 0, ProcessInputSend, 0, 0, &ThreadId);
	if (hThread == NULL) {
		printf("fail make thread\n");
	}
	else {
		CloseHandle(hThread);
	}

	while (!bIsQuit) {
		ZeroMemory(chatdata, sizeof(chatdata));

		if ((size = recv(hSocket, chatdata, PACKET_SIZE, 0)) == INVALID_SOCKET) {
			printf("recv ERROR\n");
			exit(0);
		}
		else {
			chatdata[size] = '\0';
			tick = time(NULL);
			printf("%s	%s\n", chatdata, ctime(&tick));
		}
	}

	closesocket(hSocket);
	WSACleanup();
	return 0;
}

DWORD WINAPI ProcessInputSend(LPVOID arg)
{
	while (TRUE) {
		if (fgets(chatdata, PACKET_SIZE, stdin)) {		//Read stream
			chatdata[strlen(chatdata) - 1] = '\0';
			sprintf(line, "\n[Client] : %s", chatdata);		//Output the first contents of the buffer
			if (send(hSocket, line, strlen(line), 0) < 0) {
				printf("Write fail\n");
			}

			if (strstr(line, escape) != 0) {		//Exit command
				printf("Bye\n");
				closesocket(hSocket);
				bIsQuit = TRUE;
				exit(0);
			}
		}
	}
	return 0;
}