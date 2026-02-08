    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include <winsock2.h>
    #include <ws2tcpip.h>
    #pragma comment(lib, "Ws2_32.lib")
    #define DEFAULT_BUFLEN 512

    int main()
    {
    
        WSADATA wsaData;
        struct sockaddr_in service;
        int recvbuflen = DEFAULT_BUFLEN;
        char* sendbuf = "Client: sending data test: ";
        char recvbuf[DEFAULT_BUFLEN] = "";

        int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
        if(iResult != 0){
            printf("WSAStartup failed: %d\n", iResult);
            return 1;
        }

        SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if(clientSocket == INVALID_SOCKET){
            printf("Socket isnt created program ends with error: %d\n", WSAGetLastError());
            WSACleanup();
            return 1;
        }
        printf("Socket is created successfully\n");
        service.sin_family = AF_INET;
        service.sin_addr.s_addr = inet_addr("127.0.0.1");
        service.sin_port = htons(8080);

        iResult = connect(clientSocket, (SOCKADDR *) & service, sizeof (service));
        if(iResult == SOCKET_ERROR){
            printf("Connect function didnt start: %d\n", WSAGetLastError());
            closesocket(clientSocket);
            WSACleanup();
            return 1;
        }
        const char* httpRequest = "GET /index.html HTTP/1.1\r\nHost: localhost\r\nConnection: close\r\n\r\n";
        iResult = send(clientSocket, httpRequest, (int)strlen(httpRequest), 0 );
        if(iResult == SOCKET_ERROR){
            printf("httpRequst isnt send error: %d\n", WSAGetLastError());
            closesocket(clientSocket);
            WSACleanup();
            return 1;
        }
        iResult = recv(clientSocket, recvbuf, recvbuflen, 0);
        recvbuf[iResult] = '\0';
        char* body = strstr(recvbuf, "\r\n\r\n");
        if (body){
        body += 4; 
        }

        if (body && strncmp(body, "200", 3) == 0)
        {
        printf("Server responded with 200 OK\n");
        closesocket(clientSocket);
        WSACleanup();
        }else if (body && strcmp(body, "404") == 0) 
        {
        printf("Server responded with 404 Not Found\n");
        closesocket(clientSocket);
        WSACleanup();
        }else{
        printf("Unknown response from server: %s\n", body ? body : recvbuf);
        closesocket(clientSocket);
        WSACleanup();
        }
        
        printf("Press Enter to exit...\n");
        getchar();

        WSACleanup();
        return 0;
    }