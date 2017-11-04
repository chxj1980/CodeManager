//#include <iostream>
//#include <Windows.h>
//#include <fstream>
//
////_CRT_SECURE_NO_WARNINGS
//using namespace std;
//
//char* volName = "e:\\";			// ����������
//HANDLE hVol;					// ���ڴ��������̾��
//USN_JOURNAL_DATA UsnInfo;		// ���ڴ���USN��־�Ļ�����Ϣ
//#define BUF_LEN 4096
//ofstream fout("i:\\log.txt");	// ���������ݼ�¼���ı�������鿴
//long counter = 0;
//
//int main()
//{
//	bool status;
//	bool isNTFS = false;
//	bool getHandleSuccess = false;
//	bool initUsnJournalSuccess = false;
//	char sysNameBuf[MAX_PATH] = { 0 };
//
//	status = GetVolumeInformationA(volName,	NULL,0,NULL,NULL,NULL,sysNameBuf,MAX_PATH);
//	if (0 != status)
//	{
//		printf("�ļ�ϵͳ��: %s\n", sysNameBuf);
//		if (0 == strcmp(sysNameBuf, "NTFS"))
//		{
//			isNTFS = true;
//		}
//		else
//		{
//			printf("�������̷�NTFS��ʽ\n");
//		}
//	}
//
//	if (isNTFS) 
//	{
//		char fileName[MAX_PATH];
//		fileName[0] = '\0';
//		strcpy(fileName, "\\\\.\\");				// ������ļ�������Ϊ\\.\C:����ʽ
//		strcat(fileName, volName);
//		string fileNameStr = (string)fileName;
//		fileNameStr.erase(fileNameStr.find_last_of(":") + 1);
//		printf("�����̵�ַ: %s\n", fileNameStr.data());
//		/*2.����Ϊ0*//*3.���������FILE_SHARE_WRITE*/	/*4.���ﲻ��Ҫ*//*5.�������OPEN_EXISTING, CREATE_ALWAYS���ܻᵼ�´���*//*6.FILE_ATTRIBUTE_NORMAL���ܻᵼ�´���*/
//		hVol = CreateFileA(fileNameStr.data(),GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE,NULL,OPEN_EXISTING,	FILE_ATTRIBUTE_READONLY,NULL);		
//		if (INVALID_HANDLE_VALUE != hVol)
//		{
//			getHandleSuccess = true;
//		}
//		else 
//		{
//			printf("��ȡ�����̾��ʧ�� ���� handle:%x error:%d\n", hVol, GetLastError());
//		}
//	}
//
//	if (getHandleSuccess)
//	{
//		DWORD br;
//		CREATE_USN_JOURNAL_DATA cujd;
//		cujd.MaximumSize = 0;
//		cujd.AllocationDelta = 0;
//		status = DeviceIoControl(hVol,FSCTL_CREATE_USN_JOURNAL,	&cujd,sizeof(cujd),	NULL,0,	&br,NULL);
//		if (0 != status)
//		{
//			initUsnJournalSuccess = true;
//		}
//		else
//		{
//			printf("��ʼ��USN��־�ļ�ʧ�� ���� status:%x error:%d\n", status, GetLastError());
//		}
//
//	}
//
//	if (initUsnJournalSuccess) 
//	{
//		bool getBasicInfoSuccess = false;
//		DWORD br;
//		status = DeviceIoControl(hVol,FSCTL_QUERY_USN_JOURNAL,NULL,	0,&UsnInfo,sizeof(USN_JOURNAL_DATA),&br,NULL);
//		if (0 != status)
//		{
//			getBasicInfoSuccess = true;
//		}
//		else 
//		{
//			printf("��ȡUSN��־������Ϣʧ�� ���� status:%x error:%d\n", status, GetLastError());
//		}
//
//		if (getBasicInfoSuccess) 
//		{
//			printf("UsnJournalID: %xI64\n", UsnInfo.UsnJournalID);
//			printf("lowUsn: %xI64\n", UsnInfo.FirstUsn);
//			printf("highUsn: %xI64\n", UsnInfo.NextUsn);
//
//			READ_USN_JOURNAL_DATA usnReadData;
//			usnReadData.UsnJournalID = UsnInfo.UsnJournalID;
//			//MFT_ENUM_DATA med;
//			//med.StartFileReferenceNumber = 0;
//			//med.LowUsn = 0;								//UsnInfo.FirstUsn; ���ﾭ���Է��֣������FirstUsn��ʱ����ȷ�����»�ȡ�������������ݣ�����ֱ��д0��.
//			//med.HighUsn = UsnInfo.NextUsn;
//
//			CHAR buffer[BUF_LEN];						// ���ڴ����¼�Ļ���,�����㹻�ش�
//			memset(buffer, 0, BUF_LEN);
//			DWORD usnDataSize;
//			PUSN_RECORD UsnRecord;
//
//			//while (0 != DeviceIoControl(hVol, /*IOCTL_DISK_GET_DRIVE_GEOMETRY*//*FSCTL_ENUM_USN_DATA*/FSCTL_READ_USN_JOURNAL, &med, sizeof(med), buffer, BUF_LEN, &usnDataSize, NULL))
//			while (0 != DeviceIoControl(hVol, /*IOCTL_DISK_GET_DRIVE_GEOMETRY*//*FSCTL_ENUM_USN_DATA*/FSCTL_READ_USN_JOURNAL, &usnReadData, sizeof(usnReadData), &buffer, BUF_LEN, &usnDataSize, NULL))
//			{
//				DWORD dwRetBytes = usnDataSize - sizeof(USN);
//				UsnRecord = (PUSN_RECORD)(((PCHAR)buffer) + sizeof(USN));		// �ҵ���һ��USN��¼
//				while (dwRetBytes>0) 
//				{
//					const int strLen = UsnRecord->FileNameLength;				// ��ӡ��ȡ������Ϣ
//					char fileName[MAX_PATH] = { 0 };
//					WideCharToMultiByte(CP_OEMCP, NULL, UsnRecord->FileName, strLen / 2, fileName, strLen, NULL, FALSE);
//					printf("FileName: %s\n", fileName);
//					printf("FileReferenceNumber: %xI64\n", UsnRecord->FileReferenceNumber);		// ��������file reference number����������ȡ�ļ���·����Ϣ
//					printf("ParentFileReferenceNumber: %xI64\n", UsnRecord->ParentFileReferenceNumber);
//					printf("\n");
//					fout << "FileName:" << fileName << endl;
//					fout << "frn:" << UsnRecord->FileReferenceNumber << endl;
//					fout << "pfrn:" << UsnRecord->ParentFileReferenceNumber << endl;
//					fout << endl;
//					counter++;
//					DWORD recordLen = UsnRecord->RecordLength;
//					dwRetBytes -= recordLen;
//					UsnRecord = (PUSN_RECORD)(((PCHAR)UsnRecord) + recordLen);
//				}
//				//med.StartFileReferenceNumber = *(USN *)&buffer;		//��ȡ��һҳ���ݣ�MTF����Ƿֶ�ҳ������İɣ�
//				// Update starting USN for next call
//				usnReadData.StartUsn = *(USN *)&buffer;
//			}
//
//			printf("��%d���ļ�\n", counter);
//			fout << "��" << counter << "���ļ�" << endl;
//			fout << flush;
//			fout.close();
//		}
//
//		DELETE_USN_JOURNAL_DATA dujd;
//		dujd.UsnJournalID = UsnInfo.UsnJournalID;
//		dujd.DeleteFlags = USN_DELETE_FLAG_DELETE;
//
		//status = DeviceIoControl(hVol,	FSCTL_DELETE_USN_JOURNAL, &dujd, sizeof(dujd), NULL, 0, &br, NULL);
		//if (0 != status)
		//	printf("�ɹ�ɾ��USN��־�ļ�!\n");
		//else
		//	printf("ɾ��USN��־�ļ�ʧ�� ���� status:%x error:%d\n", status, GetLastError());
//	}
//
//	if (getHandleSuccess)
//		CloseHandle(hVol);
//	system("pause");
//	return 0;
//}