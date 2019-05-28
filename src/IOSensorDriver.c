
#include"stdlib.h"

#include"string.h"
#include"stdio.h"
#include"ioLib.h"


#define MAX_SENSOR_NUMBER       15
#define MAX_SENSOR_TYPE		5

#define SENSOR_TYPE_DOOR	0
#define SENSOR_TYPE_MOVEMENT	1
#define SENSOR_TYPE_WATER	2
#define SENSOR_TYPE_FIRE	3
#define SENSOR_TYPE_SMOKE	4


#define PACKET_SEND_HEAD1	0x55
#define PACKET_SEND_HEAD2	0xaa
#define PACKET_RCV_HEAD1	0xaa
#define PACKET_RCV_HEAD2	0x55



typedef struct tagComCmdItem
{
	int nCnt;
	unsigned char buf[128];
	
}ComCmdItem, *lpComCmdItem;

typedef struct tagQueuePtr
{
	int nHead;
	int nTail;
	
}QueuePtr, *lpQueuePtr;

typedef struct tagSensorMap
{
	int type;
	int idx;
	
}SensorMap, *lpSensorMap;


SensorMap  m_nSensorMap[MAX_SENSOR_NUMBER];
int  m_nSensorValue[MAX_SENSOR_TYPE][MAX_SENSOR_NUMBER/MAX_SENSOR_TYPE];


unsigned   char m_cKeyInQueue[0x20];
QueuePtr   m_nKeyQueuePtr;

	
ComCmdItem m_myRcvCMDQueue[0x10];
QueuePtr   nRcvCmdQueuePtr;


ComCmdItem m_mySendCMDQueue[0x10];
QueuePtr   nSendCmdQueuePtr;
	
unsigned   char m_uTempRxBuf[0x100];
QueuePtr   nRxBufPtr;


unsigned   char m_uTempTxBuf[0x100];
QueuePtr   nTxBufPtr;	


int gFd;


int m_nArmFlag ;
int m_nDisarmFlag;




/****************************************************************************************/
/*											*/
/*			SerialPort Process Driver 					*/
/*											*/
/****************************************************************************************/

int SerialPortOpen()
{
	int fd;
	fd = open("/tyCo/1", O_RDWR, 0);
	if(fd < 0)
	{
		return fd;
	}
	ioctl(fd, FIOFLUSH, 0);
	ioctl(fd, FIOOPTIONS, OPT_RAW);
	ioctl(fd, FIOBAUDRATE, 57600);
	/*ioctl(ComFd, SIO_HW_OPTS_SET, CS8|STOPB|PARENB|PARODD|CLOCAL|CREAD);*/
	/*ioctl(gFd, SIO_HW_OPTS_SET, CS8|CLOCAL|CREAD);*/
	return fd;
}

int SerialPortSendTxBuf(int fd)
{
	int i;
	while(nTxBufPtr.nHead!=nTxBufPtr.nTail)
	{
		/*printf("%c",*(m_uTempTxBuf+nTxBufPtr.nHead));*/
		while(write(fd,m_uTempTxBuf+nTxBufPtr.nHead, 1)<=0){}
		nTxBufPtr.nHead++;
		nTxBufPtr.nHead &= 0xff;
		
	}

	return 1;
}

int SerialPortReadData(char* Buf, int len, int fd)
{
	int i, j;
	int n;

	j=0;
	while(1)
	{
		ioctl(fd, FIONREAD, &n);
		if(n>0)
		{
			for(i=0; i<n; i++)
			{
				read(fd, Buf+j, 1);
				j++;
				if(j>=len) return j;
			}
		}
			return j;
	}
	return -1;
}


/****************************************************************************************/
/*											*/
/*		IO Simulator Porcess function 						*/
/*											*/
/****************************************************************************************/

void SIOPollingTask()
{
	unsigned char buf[128];
	int nRcvLen;

	while(1)
	{
		nRcvLen = SerialPortReadData(buf,128,gFd);
		if(nRcvLen>0)
			ProcessRecvBuf(buf, nRcvLen);
		ProcessRcvCMD();

		while(ProcessSendCMD());
		SerialPortSendTxBuf(gFd);

		taskDelay(10);
	}
}

unsigned int GetOneCmd(int *nCount, unsigned char* pCmdBuf)
{
	int i;

	if(nRcvCmdQueuePtr.nHead == nRcvCmdQueuePtr.nTail)
	{
		*nCount = 0;
		return 0; 
	}

	*nCount = m_myRcvCMDQueue[nRcvCmdQueuePtr.nHead].nCnt;
	for(i=0; i<m_myRcvCMDQueue[nRcvCmdQueuePtr.nHead].nCnt;  i++)
		pCmdBuf[i] = m_myRcvCMDQueue[nRcvCmdQueuePtr.nHead].buf[i] ;


	nRcvCmdQueuePtr.nHead++;
	nRcvCmdQueuePtr.nHead &= 0xf;

	return 1;
}




void ProcessRecvBuf(unsigned char *pBuf, int nCount)
{

	int nBufLen,i,ptr,nPacketLen;

	for(i=0;i<nCount;i++)
	{
		m_uTempRxBuf[nRxBufPtr.nTail] = pBuf[i];
		nRxBufPtr.nTail++;
		nRxBufPtr.nTail &= 0xff;
	}


	if(nRxBufPtr.nHead <= nRxBufPtr.nTail)
		nBufLen = nRxBufPtr.nTail -  nRxBufPtr.nHead;
	else
		nBufLen = 256 + nRxBufPtr.nTail -  nRxBufPtr.nHead;

	if(nBufLen < 4)
		return;

	for(i=0;i<nBufLen-1;i++)
		if(	(m_uTempRxBuf[nRxBufPtr.nHead] == PACKET_RCV_HEAD1)  && 
			(m_uTempRxBuf[ (nRxBufPtr.nHead+1) & 0xff ] == PACKET_RCV_HEAD2))  /*FIND PACKET BEGIN*/
			break;
		else
		{
			nRxBufPtr.nHead++;
			nRxBufPtr.nHead &= 0xff;
		}

	if(i>= nBufLen-1)/*not find*/
		return;


	/* read packet to cmd buf*/
	if(nRxBufPtr.nHead <= nRxBufPtr.nTail)
		nBufLen = nRxBufPtr.nTail -  nRxBufPtr.nHead;
	else
		nBufLen = 256 + nRxBufPtr.nTail -  nRxBufPtr.nHead;

	if(nBufLen < 4)
		return;


	ptr = nRxBufPtr.nHead + 2;
	ptr &= 0xff;

	nPacketLen = m_uTempRxBuf[ptr] ;
	ptr++;
	ptr &= 0xff;

	if( nPacketLen+1 <= nBufLen)
	{
		m_myRcvCMDQueue[nRcvCmdQueuePtr.nTail].nCnt = nPacketLen ;

		for(i=0; i<nPacketLen; i++)
		{
			m_myRcvCMDQueue[nRcvCmdQueuePtr.nTail].buf[i] = m_uTempRxBuf[ptr];
			ptr++;
			ptr &= 0xff;
		}

		nRcvCmdQueuePtr.nTail++;
		nRcvCmdQueuePtr.nTail &= 0xf;

		nRxBufPtr.nHead += (nPacketLen + 4);
		nRxBufPtr.nHead &= 0xff;
	}
}

int AddToKeyQueue(char cKey)
{
	m_cKeyInQueue[m_nKeyQueuePtr.nTail] = cKey;
	m_nKeyQueuePtr.nTail++;
	m_nKeyQueuePtr.nTail &= 0x1f;

}

void AddArmEvent()
{
	m_nArmFlag = 1;
}
void AddDisarmEvent()
{
	m_nDisarmFlag = 1;
}


int ProcessRcvCMD()
{

	char szCMD[128];
	char szID[16];
	int  nCount;
	char cKey;
	int  para[24];
	int  i;
	int  nSensorID;
	while(GetOneCmd(&nCount,(unsigned char *)szCMD))
    	{
		/*printf("-%s %d\n",szCMD,nCount);*/
		
		szID[0] = szCMD[0];
		szID[1] = szCMD[1];
		szID[2] = szCMD[2];
		szID[3] = 0;


		if(strcmp(szID,"key")==0)
		{
			sscanf(szCMD+4,"%c",&cKey);
			AddToKeyQueue(cKey);
			return;
		}

		if(strcmp(szID,"ARM")==0)
		{

			AddArmEvent();
			return;
		}

		if(strcmp(szID,"DAR")==0)
		{
			AddDisarmEvent();
			return;
		}


		if((strcmp(szID,"SEN")==0))
		{
			sscanf(szCMD+3,"%d %d %d %d 
                                        %d %d %d %d 
                                        %d %d %d %d 
                                        %d %d %d %d 
                                        %d %d %d %d ",
                                        para+0,para+1,para+2,para+3,
					para+4,para+5,para+6,para+7,
					para+8,para+9,para+10,para+11,
					para+12,para+13,para+14,para+15,
					para+16,para+17,para+18,para+19);

			for(i=0;i<8;i++)
			{
				nSensorID = para[2*i] ;

				m_nSensorValue[m_nSensorMap[nSensorID].type][m_nSensorMap[nSensorID].idx] = para[2*i+1];
			}
			return;
		}

    	}

}

unsigned int PutOneCmd(char* pCmdBuf)
{
	int i,nCount;


	nCount = strlen(pCmdBuf);


	/*printf("*%s %d\n",pCmdBuf,nCount);*/


	m_mySendCMDQueue[nSendCmdQueuePtr.nTail].nCnt = nCount;
	for(i=0;i<nCount;i++)
		m_mySendCMDQueue[nSendCmdQueuePtr.nTail].buf[i] = pCmdBuf[i];

	nSendCmdQueuePtr.nTail++;
	nSendCmdQueuePtr.nTail &= 0xf;

	return 1;

}


/*  covert output cmd to tx buf */
int ProcessSendCMD()
{

	int i,ptr;

	if(nSendCmdQueuePtr.nHead == nSendCmdQueuePtr.nTail)
		return 0; 

	ptr = nTxBufPtr.nTail;

	m_uTempTxBuf[ptr] = PACKET_SEND_HEAD1;
	ptr++;
	ptr &= 0xff;

	m_uTempTxBuf[ptr] = PACKET_SEND_HEAD2;
	ptr++;
	ptr &= 0xff;

	m_uTempTxBuf[ptr] = 0;
	ptr++;
	ptr &= 0xff;

	m_uTempTxBuf[ptr] = (unsigned char )m_mySendCMDQueue[nSendCmdQueuePtr.nHead].nCnt;
	ptr++;
	ptr &= 0xff;
	

	for(i=0;i<m_mySendCMDQueue[nSendCmdQueuePtr.nHead].nCnt;i++)
	{
		m_uTempTxBuf[ptr] = m_mySendCMDQueue[nSendCmdQueuePtr.nHead].buf[i];
		ptr++;
		ptr &= 0xff;
	}

	m_uTempTxBuf[ptr] =0; /* for checksum */
	ptr++;
	ptr &= 0xff;

	nTxBufPtr.nTail += (m_mySendCMDQueue[nSendCmdQueuePtr.nHead].nCnt+5);
	nTxBufPtr.nTail &= 0xff;

	nSendCmdQueuePtr.nHead++;
	nSendCmdQueuePtr.nHead &=0xf ;

	return 1;

}




/****************************************************************************************/
/*											*/
/*			HomeAlarm IO Interface API					*/
/*											*/
/****************************************************************************************/

/*-----------------------------------*/
/*          init function            */
/*-----------------------------------*/

void HomeAlarmIODrvInit()
{
	int i;
	for(i=0;i<MAX_SENSOR_NUMBER;i++)
	{
		m_nSensorMap[i].type = i/3;
		m_nSensorMap[i].idx  = i%3;
		m_nSensorValue[m_nSensorMap[i].type][m_nSensorMap[i].idx]=0;
	}

	m_nArmFlag = 0;
	m_nDisarmFlag = 0;



	nRcvCmdQueuePtr.nHead = 0;
	nRcvCmdQueuePtr.nTail = 0;

	nSendCmdQueuePtr.nHead = 0;
	nSendCmdQueuePtr.nTail = 0;


	nRxBufPtr.nHead = 0;
	nRxBufPtr.nTail = 0;

	nTxBufPtr.nHead = 0;
	nTxBufPtr.nTail = 0;


	m_nKeyQueuePtr.nHead =0;
	m_nKeyQueuePtr.nTail =0;


	gFd = SerialPortOpen();
	if(gFd==-1)
	{
		printf("/tyCo/1 open error \n");
		while(1);
	}
	printf("/tyCo/1 opend \n");

	SetLight(0);
	SetLed(0,0);
	SetLed(1,0);

	taskSpawn("IOPolling",50,0,4096,SIOPollingTask);


}



/*-------------------------------------------------*/
/*        key and sensor input function            */
/*-------------------------------------------------*/

int OnArm()
{
	int rtn;
	rtn = m_nArmFlag;
	m_nArmFlag = 0;
	return rtn;
}


int OnDisarm()
{

	int rtn;
	rtn = m_nDisarmFlag;
	m_nDisarmFlag = 0;
	return rtn;
}



int OnKey(char  *cKey)
{
	if(m_nKeyQueuePtr.nHead == m_nKeyQueuePtr.nTail)
		return 0;

	*cKey = m_cKeyInQueue[m_nKeyQueuePtr.nHead];

	m_nKeyQueuePtr.nHead++;
	m_nKeyQueuePtr.nHead &=0x1f;

	return 1;
}



int OnMovement(int nIdx)
{
	return(m_nSensorValue[SENSOR_TYPE_MOVEMENT][nIdx]);
}


int Ondoor(int nIdx)
{
	return(m_nSensorValue[SENSOR_TYPE_DOOR][nIdx]);
}

int OnFire(int nIdx)
{
	return(m_nSensorValue[SENSOR_TYPE_FIRE][nIdx]);
	
}

int OnSmooke(int nIdx)
{

	return(m_nSensorValue[SENSOR_TYPE_SMOKE][nIdx]);

}


int OnWater(int nIdx)
{

	return(m_nSensorValue[SENSOR_TYPE_WATER][nIdx]);

}


int OnSensor(int  nID)
{
	return(m_nSensorValue[m_nSensorMap[nID].type][m_nSensorMap[nID].idx]);
}


/*----------------------------------------------------------*/
/*        led\siren\light\telphone  output function         */
/*----------------------------------------------------------*/
void SetSiren(int nState)
{
	char szBuf[64];
	sprintf(szBuf,"SIRN %d", nState);
	PutOneCmd(szBuf);
}


void SetLed(int nID,int nState)  /* id=0 greed led; id=1:red led */
{

	char szBuf[64];
	
	if(nID==1)
		sprintf(szBuf,"RLED %d", nState);
	else
		sprintf(szBuf,"GLED %d", nState);

	PutOneCmd(szBuf);

}

void SetLight(int nState){
	char szBuf[64];
	sprintf(szBuf,"LIGT %d", nState);
	PutOneCmd(szBuf);
}
void SetCall(char *szPhoneNum, int nType, int nZone)
{
	char szBuf[64];
	sprintf(szBuf,"CALL %s %d %d", szPhoneNum,nType,nZone);
	PutOneCmd(szBuf);

}


