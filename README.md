# API Hook

- Features: Wrap DLL & log API call details, supported multi-thread
- Platform: Windows (passed build on VS2015, x86 debug configuration)
- Ver: 0.2
- Updated: 9/9/2017
- Created: 3/13/2001
- Author: loblab

## Use case

If you have SDK with binary lib (without source code), header file (API definitions),
you can create a fake lib to call the real lib, and log the call details.

I ever used this approach to capture & analyse SCSI & USB device communications.
It is simpler than capturing from device driver layer.

Example: [How to hack UT181A protocol (Chinese)](http://www.freebuf.com/articles/terminal/145017.html)
![Screenshot](https://raw.githubusercontent.com/loblab/apihook/master/apihook.png)

## Usage

1. Rename your original DLL (say myapi.dll) to myapi_.dll
2. define TARGET_DLL as "myapi_.dll" (in apihook.cpp)
3. Output this project as myapi.dll to replace the original DLL
4. Implement the APIs

If you don't care an API, simply pass.

```cpp
HID_TO_UART_API HID_UART_STATUS WINAPI
HidUart_GetNumDevices(DWORD* numDevices, _In_ const WORD vid, _In_ const WORD pid)
{   
    AutoLogCall alc(m_logger, "HidUart_GetNumDevices");
    HID_UART_STATUS rc = fn_HidUart_GetNumDevices(numDevices, vid, pid);
    return rc;
}
```

Log the details if you are interested in some input or output of a call.

```cpp
HID_TO_UART_API HID_UART_STATUS WINAPI
    HidUart_Write(HID_UART_DEVICE device, BYTE* buffer, DWORD numBytesToWrite, DWORD* numBytesWritten)
{
    AutoLogCall alc(m_logger, "HidUart_Write");
    HID_UART_STATUS rc = fn_HidUart_Write(device, buffer, numBytesToWrite, numBytesWritten);
    m_logger->LogNumber("numBytesWritten", *numBytesWritten);
    m_logger->LogBlock("data", buffer, numBytesToWrite);
    return rc;
}
```

