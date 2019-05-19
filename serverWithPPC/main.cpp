#include <winsock2.h>
#include <windows.h>
#include <stdio.h>
#include <iostream>
#include <string.h>
#include <fstream>
#include <time.h>
#pragma comment(lib,"ws2_32.lib")
using namespace std;


ifstream cRFile("WEB2PPC.txt"); //只可读
ofstream cWFile("PPC2WEB.txt"); //只可写




DWORD WINAPI serverRead(LPVOID lpParamter)
{
    SOCKET sClient = *((SOCKET*)lpParamter); //sClient
    whlie(1)
    {
        char revData[255];
        int ret = recv(sClient, revData, 255, 0);
        if(ret>0)
        {
            if(strcmp(revData,"wi")==0){  //infrared  0
                cWFile << "0\n";
            }
            else if(strcmp(revData,"ws")==0){ //smoke  1
                cWFile << "1\n";
            }
        }

        Sleep(500);

    }



    return 0L;

}


int main(int argc, char* argv[])
{
	//初始化WSA
	WORD sockVersion = MAKEWORD(2, 2);
	WSADATA wsaData;
	if (WSAStartup(sockVersion, &wsaData) != 0)
	{
		return 0;
	}

	//创建套接字
	SOCKET slisten = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (slisten == INVALID_SOCKET)
	{
		printf("socket error !");
		return 0;
	}

	//绑定IP和端口
	struct sockaddr_in sin;
	sin.sin_family = AF_INET;
	sin.sin_port = htons(8888);
	sin.sin_addr.S_un.S_addr = INADDR_ANY;
	if (bind(slisten, (LPSOCKADDR)&sin, sizeof(sin)) == SOCKET_ERROR)
	{
		printf("bind error !");
		return 0;
	}

	//开始监听
	if (listen(slisten, 5) == SOCKET_ERROR)
	{
		printf("listen error !");
		return 0;
	}

	//循环接收数据
	SOCKET sClient;
	struct sockaddr_in remoteAddr;
	int nAddrlen = sizeof(remoteAddr);
	//char revData[255];
	printf("等待连接...\n");
	sClient = accept(slisten, (SOCKADDR *)&remoteAddr, &nAddrlen);
    printf("接受到一个连接：%s \r\n", inet_ntoa(remoteAddr.sin_addr));
    if (sClient == INVALID_SOCKET)
    {
         printf("accept error !");
         return 0;
    }
    HANDLE readThread = CreateThread(NULL,1000,serverRead,(LPVOID*)&sClient,0,NULL);
    CloseHandle(readThread);
    while(1)  //sendServer
    {
         if(){  //读到文件修改超过0.2秒,发送指令

            char sendData[255];  //文件中的指令

            send(sClient, sendData, strlen(sendData), 0);
         }
         Sleep(1000);
      }
    }
    closesocket(sClient);
	closesocket(slisten);
	WSACleanup();
	return 0;
}


