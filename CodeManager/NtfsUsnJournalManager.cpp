#include "stdafx.h"
#include <iostream>
#include <Windows.h>
#include <fstream>
#include <winioctl.h>

using namespace std;

char* volName = "e:\\";		//	����������
HANDLE hVol;				//	���ڴ洢�����̾��
USN_JOURNAL_DATA UsnInfo;	//	���ڴ洢USN��־�Ļ�����Ϣ
#define BUF_LEN 4096
