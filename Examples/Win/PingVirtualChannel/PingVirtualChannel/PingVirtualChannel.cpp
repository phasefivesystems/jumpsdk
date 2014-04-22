// PingVirtualChannel.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <Windows.h>
#include <WtsApi32.h>

void Log(const char* format, ...)
{
    va_list args;
    va_start (args, format);
    
    vprintf(format, args);
    printf("\r\n");
    
    va_end(args);
}

#pragma pack(push, 1)
struct PingPayload
{
	UINT32 SequenceNo;
	UINT32 TimeStamp;
};
#pragma pop

int _tmain(int argc, _TCHAR* argv[])
{
	HANDLE hChannel = NULL;
	PingPayload payload = { 0 };
	ULONG bytesRead=0, bytesWritten=0;
	UINT32 currentSequenceNo = 0;

	hChannel = WTSVirtualChannelOpen(WTS_CURRENT_SERVER_HANDLE, -1, "JDECHO");
	if (hChannel == NULL)
	{
		Log("Unable to open virtual channel: %i\r\n", GetLastError());
		Log("Are you running inside a remote desktop session?");

		goto End;
	}

	while (true)
	{
		// Send the ping packet across the virtual channel
		payload.SequenceNo = currentSequenceNo++;
		payload.TimeStamp = GetTickCount();

		if (!WTSVirtualChannelWrite(hChannel, (PCHAR)&payload, sizeof(payload), &bytesWritten))
		{
			Log("WTSVirtualChannelWrite failed: %i\t\n", GetLastError());
			goto End;
		}

		if (bytesWritten != sizeof(payload))
		{
			Log("Error: Partial writes are not handled");
			goto End;
		}

		// Wait for the echo channel to send the same packet back
		if (!WTSVirtualChannelRead(hChannel, INFINITE, (PCHAR)&payload, sizeof(payload), &bytesRead))
		{
			Log("WTSVirtualChannelRead failed: %i", GetLastError());
			goto End;
		}

		if (bytesRead != sizeof(payload))
		{
			Log("Error partial reads are not handled");
			goto End;
		}

		// Print statistics
		Log("Got ping reply: %i, %ims", (int)payload.SequenceNo, (int)(GetTickCount()-payload.TimeStamp));

		// Wait for a second before pinging again
		Sleep(1000);
	}

End:

	if (hChannel != NULL)
	{
		WTSVirtualChannelClose(hChannel);
	}
}

