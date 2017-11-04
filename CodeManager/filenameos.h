#pragma once
#include <vector>
#include <iostream>
#include <string.h>

using namespace std;

typedef struct _VOLUME_INFO
{
	TCHAR    chFlag;
	DWORD    dwDiskNumber;
}VOLUME_INFO, *PVOLUME_INFO;

//��ȡӲ����Ϣ
BOOL GetVolumeInfo(IN TCHAR chFlag, OUT PVOLUME_INFO pVolumeInfo);

string Trim(const string &str, const char* szBlank = NULL);

//ȥ��ָ���ַ���
char* _cdecl GetPointStr(const char* str, int pos, char* step);

//����ָ���ļ����ļ�
void TraversalFile(std::string strFilePath, std::vector<CString>& arrFilePath);

//��ȡ�ļ��ļ���
//template<class TypeName>
//TypeName GetFileDirectory(TypeName strFilePath);

std::string GetFileDirectory(std::string strFilePath);

//�и��ַ���
void strSplit(const string & strInput, vector<std::string> & arrStr, char chSep = ',', bool bRemovePair = false);