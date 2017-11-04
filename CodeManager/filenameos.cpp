#include "stdafx.h"
#include "filenameos.h"
#include "winioctl.h"

using namespace std;


BOOL GetVolumeInfo(IN TCHAR chFlag, OUT PVOLUME_INFO pVolumeInfo)
{
	TCHAR szVol[] = { '\\', '\\', '.', '\\', chFlag, ':', 0 };
	HANDLE hDrv = CreateFile(szVol, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
	if (INVALID_HANDLE_VALUE == hDrv)
	{
		return FALSE;
	}
	VOLUME_DISK_EXTENTS vde;
	DWORD dwBytes;
	BOOL bOK = DeviceIoControl(hDrv, IOCTL_VOLUME_GET_VOLUME_DISK_EXTENTS, NULL, 0, &vde, sizeof(vde), &dwBytes, NULL);
	if (!bOK)
	{
		CloseHandle(hDrv);
		return FALSE;
	}
	pVolumeInfo->chFlag = chFlag;                            //�����̷�
	pVolumeInfo->dwDiskNumber = vde.Extents->DiskNumber;    //��������Ӳ�̱��,����0
	return TRUE;
}

//template<class TypeName>
//TypeName GetFileDirectory(TypeName strFilePath)
//{
//	char path_buffer[MAX_PATH];
//	char drive[MAX_PATH];
//	char dir[MAX_PATH];
//	char fname[MAX_PATH];
//	char ext[MAX_PATH];
//
//	_splitpath(strFilePath, drive, dir, fname, ext);
//	TypeName strPath = drive;
//	strPath += dir;
//	return strPath;
//}

//�����ļ����ļ�
void TraversalFile(std::string strFilePath, std::vector<CString>& arrFilePath)
{
	CFileFind finder;
	std::string strPath = GetFileDirectory(strFilePath);
	strPath += "*.*";

	//���ַ���stringת����CString
	//char* p = const_cast<char*>(strPath.c_str());
	//CString strCPath = (LPCTSTR)p;
	CString strCPath = (LPCTSTR)strPath.c_str();
	BOOL bWorking = finder.FindFile(strCPath);
	while (bWorking)
	{
		//����ǵ�Ļ���ֱ������
		bWorking = finder.FindNextFile();
		if(finder.IsDots())
			continue;
		if (finder.IsDirectory())
		{
			CString str = finder.GetFilePath();
			std::string strSPath = str;
			strSPath += "\\";
			TraversalFile(strSPath, arrFilePath);
		}
		CString str1 = finder.GetFilePath();
		arrFilePath.push_back(str1);
	}
	return;
}

string LTrim(const string &str, const char * szBlank)
{
	if (szBlank == NULL || strlen(szBlank) == 0)
		return str;
	int nLen = (int)str.length();
	int nPos = 0;
	while (nPos < nLen)
	{
		//if cant find blank
		//strchr = Find a character in a string, using the current locale or a specified LC_CTYPE conversion state category.
		if (strchr(szBlank, str[nPos]) != NULL)
			nPos++;
		else
			break;
	}
	if (nPos == nLen)
		return "";

	//substr��C++���Ժ�������Ҫ�����Ǹ������ַ�����Ҫ���ָ��λ�ÿ�ʼ��������ָ���ĳ��ȡ�
	return str.substr(nPos, nLen - nPos);
}

//��ȡָ���ַ���
char* _cdecl GetPointStr(const char* str, int pos, char* step)
{
	if (str == NULL || pos < 0 || step == NULL)
		return "";
	int i = 0, nNum = 0;
	for (i = 0; i < strlen(str); ++i)
	{
		if (NULL != strchr(step, str[i]))
			nNum++;
	}

	if (nNum == 0 || nNum < pos)
		return "";
	char values[MAX_PATH];
	memset(values, 0, sizeof(char)*MAX_PATH);
	int step_time = 0;
	int first_char_pos = 0;
	for (int i = 0; i < strlen(str); ++i)
	{
		if (NULL != strchr(step, str[i]))
			first_char_pos = i + 1;
		if (step_time == pos)
		{
			if (i < first_char_pos)
				break;
			strncpy(values, str + first_char_pos, i - first_char_pos);
			break;
		}
		step_time++;
	}
	if (i == strlen(str) - 1)
		strncpy(values, str + first_char_pos, i + 1);
	return values;
}

string RTrim(const string &str, const char * szBlank)
{
	if (szBlank == NULL || strlen(szBlank) == 0)
		return str;
	int nPos = (int)str.length();
	while (nPos > 0)
	{
		//�����ַ����� �״γ���һ���ַ���λ�ã�����������ַ��������ַ������ٶ�ʵ������
		//char string[17];
		//char *ptr, c = 'r';
		//strcpy(string, "Thisisastring");
		//ptr = strchr(string, c);
		//if (ptr)
		//	printf("The character %cis at position:%s\n", c, ptr);
		//else
		//	printf("The character was not found\n");
		//return0;
		if (strchr(szBlank, str[nPos - 1]) != NULL)
			nPos--;
		else
			break;
	}
	return str.substr(0, nPos);
}

//��ȥ�ַ����еĿո�
string Trim(const string &str, const char* szBlank /*= NULL*/)
{
	return LTrim(RTrim(str, szBlank), szBlank);
}

void strSplit(const string & strInput, vector<std::string> & arrStr, char chSep/* = ','*/, bool bRemovePair/* = false*/)
{
	//����������һ���ַ�����������Ҫ���ַ��������ch�ַ������ҽ�ÿ��ch�ַ�ǰ����ַ��洢��arrStr 
	//1.ת��const char*
	//��ȡ�����ַ�����
	const char* pCh = strInput.c_str();
	//
	std::string strWord;
	//
	bool bBegin = true;
	//��ȡһ������
	while (*pCh != '\0' && *pCh != '\n')
	{
		//������ָ���ַ�
		if (*pCh == chSep)
		{
			strWord = Trim(strWord);
			if (bRemovePair && strWord[0] == ',')
			{
				std::string strMid = strWord.substr(1, strWord.size() - 2);
				if (strMid.find(',') != -1 || strMid.find('\'') != -1)
					strWord = strMid;
			}
			arrStr.push_back(strWord);
			strWord = "";
			pCh++;
			bBegin = true;
			continue;
		}
	}
}



std::string GetFileDirectory(std::string strFilePath)
{
	char path_buffer[MAX_PATH];
	char drive[MAX_PATH];
	char dir[MAX_PATH];
	char fname[MAX_PATH];
	char ext[MAX_PATH];

	_splitpath(strFilePath.c_str(), drive, dir, fname, ext);
	string strPath = drive;
	strPath += dir;
	return strPath;
}

//for (TCHAR chFlag = 'A'; chFlag <= 'Z'; ++chFlag)
//{
//	PVOLUME_INFO pi = { 0 };
//	GetVolumeInfo(chFlag, &pi);
//}