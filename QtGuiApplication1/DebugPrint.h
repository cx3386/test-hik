#pragma once

#include<Windows.h> 
#include <tchar.h> 

//���������Ϣ��������������ڵĺ궨�� 
//ʹ��win API��DEBUG�汾��ִ�У�RELEASE�汾�򲻻� 
//������ʹ��DebugView��WinDbg�ȹ��߲鿴��� 


#define DP0(fmt) {TCHAR sOut[256];_stprintf_s(sOut,_T(fmt));OutputDebugString(sOut);} 
#define DP1(fmt,var) {TCHAR sOut[256];_stprintf_s(sOut,_T(fmt),var);OutputDebugString(sOut);} 
#define DP2(fmt,var1,var2) {TCHAR sOut[256];_stprintf_s(sOut,_T(fmt),var1,var2);OutputDebugString(sOut);} 
#define DP3(fmt,var1,var2,var3) {TCHAR sOut[256];_stprintf_s(sOut,_T(fmt),var1,var2,var3);OutputDebugString(sOut);} 

#define DP0(fmt) ; 
#define DP1(fmt, var) ; 
#define DP2(fmt,var1,var2) ; 
#define DP3(fmt,var1,var2,var3) ; 