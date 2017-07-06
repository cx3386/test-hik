
#include "hikvision.h"
#include <cstdlib>
#include <cstring>
#include <iostream>
#include "Windows.h"
#include "HCNetSDK.h"
#include "plaympeg4.h"

#include <time.h>



using namespace std;
using namespace cv;

LONG nPort = -1;

volatile int gbHandling = 0;//image interval



//����ص� ��ƵΪYUV����(YV12)����ƵΪPCM����
void CALLBACK DecCBFun(long nPort, char * pBuf, long nSize, FRAME_INFO * pFrameInfo, long nReserved1, long nReserved2)
{
	if (gbHandling) {
		gbHandling--;
		return;
	}
	long lFrameType = pFrameInfo->nType;
	if (lFrameType == T_YV12) {

		Mat pImg(pFrameInfo->nHeight, pFrameInfo->nWidth, CV_8UC1);	//pImg = Gray

		Mat src(pFrameInfo->nHeight + pFrameInfo->nHeight / 2, pFrameInfo->nWidth, CV_8UC1, pBuf);
		cvtColor(src, pImg, CV_YUV2GRAY_YV12);
		//imshow("IPCamera", pImg);
		//waitKey(1);
		mutex.lock();
        //if (isProcessed == false)
            //imageProcessed.wait(&mutex);
		pRawImage = pImg;
        //isProcessed == false;
        imageNeedProcess.wakeOne();
		mutex.unlock();
		//  Sleep(-1);
		
        //waitKey(1);//waitkey is used for event process in opencv gui.

	}

	gbHandling = 7;//every 8 frame

}


///ʵʱ���ص�
void CALLBACK fRealDataCallBack(LONG lRealHandle, DWORD dwDataType, BYTE *pBuffer, DWORD dwBufSize, void *pUser)
{
	switch (dwDataType) {
	case NET_DVR_SYSHEAD: //ϵͳͷ

		if (!PlayM4_GetPort(&nPort)) { //��ȡ���ſ�δʹ�õ�ͨ����
			break;
		}
		//m_iPort = lPort; //��һ�λص�����ϵͳͷ������ȡ�Ĳ��ſ�port�Ÿ�ֵ��ȫ��port���´λص�����ʱ��ʹ�ô�port�Ų���
		if (dwBufSize > 0) {
			if (!PlayM4_SetStreamOpenMode(nPort, STREAME_REALTIME)) { //����ʵʱ������ģʽ
				break;
			}

			if (!PlayM4_OpenStream(nPort, pBuffer, dwBufSize, 10 * 1024 * 1024)) { //�����ӿ�
				break;
			}

			if (!PlayM4_Play(nPort, NULL)) { //���ſ�ʼ
				break;
			}
			if (!PlayM4_SetDecCallBack(nPort, DecCBFun)) {
				break;
			}
		}
		break;
	case NET_DVR_STREAMDATA:   //��������
		if (dwBufSize > 0 && nPort != -1) {
			if (!PlayM4_InputData(nPort, pBuffer, dwBufSize)) {
				qDebug() << "error" << PlayM4_GetLastError(nPort) << endl;
				break;
			}
		}
		break;
	default: //��������
		if (dwBufSize > 0 && nPort != -1) {
			if (!PlayM4_InputData(nPort, pBuffer, dwBufSize)) {
				break;
			}
		}
		break;
	}
	Sleep(1);
}


void CALLBACK g_ExceptionCallBack(DWORD dwType, LONG lUserID, LONG lHandle, void *pUser)
{
	char tempbuf[256] = { 0 };
	switch (dwType) {
	case EXCEPTION_RECONNECT:    //Ԥ��ʱ����
		qDebug("----------reconnect--------%d\n", time(NULL));
		break;
	default:
		break;
	}
}

BOOL HikVision::hikRealPlay(HWND h)
{
	//---------------------------------------

	// ��ʼ��
	NET_DVR_Init();
	//��������ʱ��������ʱ��
	NET_DVR_SetConnectTime(2000, 1);
	NET_DVR_SetReconnect(10000, true);
	//LONG lUserID;

	//---------------------------------------
	// ע���豸
	NET_DVR_DEVICEINFO_V30 struDeviceInfo;
	lUserID = NET_DVR_Login_V30("192.168.2.84", 8000, "admin", "www.cx3386.com", &struDeviceInfo);
	if (lUserID < 0) {
		qDebug("Login error, %d\n", NET_DVR_GetLastError());
		NET_DVR_Cleanup();
		return FALSE;
	}

	//---------------------------------------
	//�����쳣��Ϣ�ص�����
	NET_DVR_SetExceptionCallBack_V30(0, NULL, g_ExceptionCallBack, NULL);

	//---------------------------------------
	//����Ԥ�������ûص�������
	//LONG lRealPlayHandle;
	//cvNamedWindow("Mywindow", 0);
	//cvNamedWindow("IPCamera", 0);

	//HWND  h = (HWND)cvGetWindowHandle("Mywindow");

	if (h == 0) {
		qDebug() << "realplay init window failed!" << endl;
	}


	NET_DVR_PREVIEWINFO struPlayInfo = { 0 };
	struPlayInfo.hPlayWnd = h;         //��ҪSDK����ʱ�����Ϊ��Чֵ����ȡ��������ʱ����Ϊ��
	struPlayInfo.lChannel = 1;           //Ԥ��ͨ����
	struPlayInfo.dwStreamType = 0;       //0-��������1-��������2-����3��3-����4���Դ�����
	struPlayInfo.dwLinkMode = 0;         //0- TCP��ʽ��1- UDP��ʽ��2- �ಥ��ʽ��3- RTP��ʽ��4-RTP/RTSP��5-RSTP/HTTP
	struPlayInfo.bBlocked = 0;			//0-������ȡ��, 1-����ȡ��, �������SDK�ڲ�connectʧ�ܽ�����5s�ĳ�ʱ���ܹ�����,���ʺ�����ѯȡ������.

	lRealPlayHandle = NET_DVR_RealPlay_V40(lUserID, &struPlayInfo, fRealDataCallBack, NULL);

	//NET_DVR_SaveRealData(lRealPlayHandle, sFileName);
	if (lRealPlayHandle < 0) {
		qDebug("NET_DVR_RealPlay_V40 error\n");
		qDebug("%d\n", NET_DVR_GetLastError());
		NET_DVR_Logout(lUserID);
		NET_DVR_Cleanup();
		return FALSE;
	}
	//waitKey();

	//Sleep(-1);


	return TRUE;
}

BOOL HikVision::hikStopRealPlay()
{
	//---------------------------------------
	//�ر�Ԥ��
	if (!NET_DVR_StopRealPlay(lRealPlayHandle)) {
		return FALSE;
	}
	//ע���û�

	if (!NET_DVR_Logout(lUserID)) {
		return FALSE;
	}
	//�ͷ�SDK��Դ

	return (NET_DVR_Cleanup());
}


