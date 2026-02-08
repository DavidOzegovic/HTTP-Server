#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")
#define DEFAULT_BUFLEN 512

int main()
{
    char recvbuf[DEFAULT_BUFLEN];
    int recvbuflen = DEFAULT_BUFLEN;
    WSADATA wsaData;
    struct sockaddr_in service;
    char httpRespond[100] = "GET /index.html HTTP/1.1";
    int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if(iResult != 0){
        printf("WSAStartup failed: %d\n", iResult);
        return 1;
    }

    SOCKET socketListener = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(socketListener == INVALID_SOCKET){
        printf("Socket isnt created program ends", WSAGetLastError());
        WSACleanup();
        return 1;
    }
    printf("Socket is created successfully\n");

    service.sin_family = AF_INET;
    service.sin_addr.s_addr = inet_addr("127.0.0.1");
    service.sin_port = htons(8080);
    if(bind(socketListener, (struct sockaddr*)&service, sizeof(service)) == SOCKET_ERROR){
        printf("Binding Socket failed\n", WSAGetLastError());
        closesocket(socketListener);
        WSACleanup();
        return 1;
    }
    printf("Binding returned successfully\n");

    if(listen(socketListener, SOMAXCONN) == SOCKET_ERROR){
        printf("listen function failed with error: %d\n", WSAGetLastError());
    }
    printf("Listening on socket.....\n");
    SOCKET acceptSocket = accept(socketListener, NULL, NULL);
    if(acceptSocket == INVALID_SOCKET){
        printf("Accept failed with error: %d\n", WSAGetLastError());
        closesocket(socketListener);
        WSACleanup();
        return 1;
    }
    printf("Client connected\n");
    
    iResult = recv(acceptSocket, recvbuf, recvbuflen, 0);
    recvbuf[iResult] = '\0';
    char *result = strchr(recvbuf, '\r');  
    if (result) *result = '\0';          

    if (strcmp(recvbuf, httpRespond) == 0) {
    printf("200\n");
    const char* httpPositiveRespond = "HTTP/1.1 200 OK\r\n" "Content-Type: text/html\r\n" "Content-Length: 20\r\n" "Connection: close\r\n" "\r\n" "<h1>Hello World</h1>";
    iResult = send(acceptSocket, httpPositiveRespond, (int)strlen(httpPositiveRespond), 0 );
        if(iResult == SOCKET_ERROR){
            printf("httpRequst isnt send error: %d\n", WSAGetLastError());
            closesocket(acceptSocket);
            WSACleanup();
            return 1;
        }
    }else{
        const char* httpNegativeRespond = "HTTP/1.1 404 Not Found\r\nContent-Length: 3\r\n\r\n404";
        iResult = send(acceptSocket, httpNegativeRespond, (int)strlen(httpNegativeRespond), 0 );
            if(iResult == SOCKET_ERROR){
            printf("httpRequst isnt send error: %d\n", WSAGetLastError());
            closesocket(acceptSocket);
            WSACleanup();
            return 1;
        }

    iResult = closesocket(acceptSocket);
    if (iResult == SOCKET_ERROR) {
        printf("close failed with error: %d\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }
    printf("Press Enter to exit...\n");
    getchar();

    closesocket(socketListener);
    WSACleanup();
    return 0;
    }
}