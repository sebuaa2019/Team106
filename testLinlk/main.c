#include <winsock2.h>
#include <windows.h>
#include <stdio.h>
#pragma comment(lib,"ws2_32.lib")

int main(int argc, char* argv[])
{
	//��ʼ��WSA
	WORD sockVersion = MAKEWORD(2, 2);
	WSADATA wsaData;
	if (WSAStartup(sockVersion, &wsaData) != 0)
	{
		return 0;
	}

	//�����׽���
	SOCKET slisten = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (slisten == INVALID_SOCKET)
	{
		printf("socket error !");
		return 0;
	}

	//��IP�Ͷ˿�
	struct sockaddr_in sin;
	sin.sin_family = AF_INET;
	sin.sin_port = htons(8888);
	sin.sin_addr.S_un.S_addr = INADDR_ANY;
	if (bind(slisten, (LPSOCKADDR)&sin, sizeof(sin)) == SOCKET_ERROR)
	{
		printf("bind error !");
		return 0;
	}

	//��ʼ����
	if (listen(slisten, 5) == SOCKET_ERROR)
	{
		printf("listen error !");
		return 0;
	}

	//ѭ����������
	SOCKET sClient;
	struct sockaddr_in remoteAddr;
	int nAddrlen = sizeof(remoteAddr);
	//char revData[255];
	printf("�ȴ�����...\n");
	sClient = accept(slisten, (SOCKADDR *)&remoteAddr, &nAddrlen);
    printf("���ܵ�һ�����ӣ�%s \r\n", inet_ntoa(remoteAddr.sin_addr));
    if (sClient == INVALID_SOCKET)
    {
         printf("accept error !");
         return 0;
    }
    while(1)
    {
       char revData[255];
    //printf("�ȴ�����...\n");
    //sClient = accept(slisten, (SOCKADDR *)&remoteAddr, &nAddrlen);
      int ret = recv(sClient, revData, 255, 0);
		//printf(ret);
      if (ret > 0)
      {
          revData[ret] = 0x00;
          printf("%s\n",revData);

     //��������
         char *sendData = "hello,PPC603\n";
         send(sClient, sendData, strlen(sendData), 0);
      }
    }
    closesocket(sClient);
	closesocket(slisten);
	WSACleanup();
	return 0;
}