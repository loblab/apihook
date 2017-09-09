// Copyright 2017 loblab
// 
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// 
//     http://www.apache.org/licenses/LICENSE-2.0
// 
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <windows.h>
#include "SLABHIDtoUART.h"
#include "logger.h"

typedef HID_TO_UART_API HID_UART_STATUS
(WINAPI *FN_HidUart_GetNumDevices)(DWORD* numDevices, _In_ const WORD vid, _In_ const WORD pid);

typedef HID_TO_UART_API HID_UART_STATUS
(WINAPI *FN_HidUart_GetString)(DWORD deviceNum, WORD vid, WORD pid, char* deviceString, DWORD options);

typedef HID_TO_UART_API HID_UART_STATUS
(WINAPI *FN_HidUart_GetOpenedString)(HID_UART_DEVICE device, char* deviceString, DWORD options);

typedef HID_TO_UART_API HID_UART_STATUS
(WINAPI *FN_HidUart_GetIndexedString)(DWORD deviceNum, WORD vid, WORD pid, DWORD stringIndex, char* deviceString);

typedef HID_TO_UART_API HID_UART_STATUS
(WINAPI *FN_HidUart_GetOpenedIndexedString)(HID_UART_DEVICE device, DWORD stringIndex, char* deviceString);

typedef HID_TO_UART_API HID_UART_STATUS
(WINAPI *FN_HidUart_GetAttributes)(DWORD deviceNum, WORD vid, WORD pid, WORD* deviceVid, WORD* devicePid, WORD* deviceReleaseNumber);
 
typedef HID_TO_UART_API HID_UART_STATUS
(WINAPI *FN_HidUart_GetOpenedAttributes)(HID_UART_DEVICE device, WORD* deviceVid, WORD* devicePid, WORD* deviceReleaseNumber);

typedef HID_TO_UART_API HID_UART_STATUS
(WINAPI *FN_HidUart_Open)(HID_UART_DEVICE* device, DWORD deviceNum, WORD vid, WORD pid);

typedef HID_TO_UART_API HID_UART_STATUS
(WINAPI *FN_HidUart_Close)(HID_UART_DEVICE device);

typedef HID_TO_UART_API HID_UART_STATUS
(WINAPI *FN_HidUart_IsOpened)(HID_UART_DEVICE device, BOOL* opened);

typedef HID_TO_UART_API HID_UART_STATUS
(WINAPI *FN_HidUart_SetUartEnable)(HID_UART_DEVICE device, BOOL enable);

typedef HID_TO_UART_API HID_UART_STATUS
(WINAPI *FN_HidUart_GetUartEnable)(HID_UART_DEVICE device, BOOL* enable);

typedef HID_TO_UART_API HID_UART_STATUS
(WINAPI *FN_HidUart_Read)(HID_UART_DEVICE device, BYTE* buffer, DWORD numBytesToRead, DWORD* numBytesRead);

typedef HID_TO_UART_API HID_UART_STATUS
(WINAPI *FN_HidUart_Write)(HID_UART_DEVICE device, BYTE* buffer, DWORD numBytesToWrite, DWORD* numBytesWritten);

typedef HID_TO_UART_API HID_UART_STATUS
(WINAPI *FN_HidUart_FlushBuffers)(HID_UART_DEVICE device, BOOL flushTransmit, BOOL flushReceive);

typedef HID_TO_UART_API HID_UART_STATUS
(WINAPI *FN_HidUart_CancelIo)(HID_UART_DEVICE device);

typedef HID_TO_UART_API HID_UART_STATUS
(WINAPI *FN_HidUart_SetTimeouts)(HID_UART_DEVICE device, DWORD readTimeout, DWORD writeTimeout);

typedef HID_TO_UART_API HID_UART_STATUS
(WINAPI *FN_HidUart_GetTimeouts)(HID_UART_DEVICE device, DWORD* readTimeout, DWORD* writeTimeout);

typedef HID_TO_UART_API HID_UART_STATUS
(WINAPI *FN_HidUart_GetUartStatus)(HID_UART_DEVICE device, WORD* transmitFifoSize, WORD* receiveFifoSize, BYTE* errorStatus, BYTE* lineBreakStatus);

typedef HID_TO_UART_API HID_UART_STATUS
(WINAPI *FN_HidUart_SetUartConfig)(_In_ HID_UART_DEVICE device, _In_ const DWORD baudRate, _In_ const BYTE dataBits, _In_ const BYTE parity, _In_ const BYTE stopBits, _In_ const BYTE flowControl);

typedef HID_TO_UART_API HID_UART_STATUS
(WINAPI *FN_HidUart_GetUartConfig)(HID_UART_DEVICE device, DWORD* baudRate, BYTE* dataBits, BYTE* parity, BYTE* stopBits, BYTE* flowControl);

typedef HID_TO_UART_API HID_UART_STATUS
(WINAPI *FN_HidUart_StartBreak)(HID_UART_DEVICE device, BYTE duration);

typedef HID_TO_UART_API HID_UART_STATUS
(WINAPI *FN_HidUart_StopBreak)(HID_UART_DEVICE device);

typedef HID_TO_UART_API HID_UART_STATUS
(WINAPI *FN_HidUart_Reset)(HID_UART_DEVICE device);

typedef HID_TO_UART_API HID_UART_STATUS
(WINAPI *FN_HidUart_ReadLatch)(_In_ HID_UART_DEVICE device, WORD* latchValue);

typedef HID_TO_UART_API HID_UART_STATUS
(WINAPI *FN_HidUart_WriteLatch)(_In_ HID_UART_DEVICE device, _In_ const WORD latchValue, _In_ const WORD latchMask);

typedef HID_TO_UART_API HID_UART_STATUS
(WINAPI *FN_HidUart_GetPartNumber)(HID_UART_DEVICE device, BYTE* partNumber, BYTE* version);

typedef HID_TO_UART_API HID_UART_STATUS
(WINAPI *FN_HidUart_GetLibraryVersion)(_Out_writes_bytes_(1) BYTE* major, _Out_writes_bytes_(1) BYTE* minor, _Out_writes_bytes_(sizeof(BOOL)) BOOL* release);

typedef HID_TO_UART_API HID_UART_STATUS
(WINAPI *FN_HidUart_GetHidLibraryVersion)(_Out_writes_bytes_(1) BYTE* major, _Out_writes_bytes_(1) BYTE* minor, _Out_writes_bytes_(sizeof(BOOL)) BOOL* release);

typedef HID_TO_UART_API HID_UART_STATUS
(WINAPI *FN_HidUart_GetHidGuid)(_Out_ void* guid);

typedef HID_TO_UART_API HID_UART_STATUS
(WINAPI *FN_HidUart_SetLock)(HID_UART_DEVICE device, WORD lock);

typedef HID_TO_UART_API HID_UART_STATUS
(WINAPI *FN_HidUart_GetLock)(HID_UART_DEVICE device, WORD* lock);

typedef HID_TO_UART_API HID_UART_STATUS
(WINAPI *FN_HidUart_SetUsbConfig)(_In_ HID_UART_DEVICE device, _In_ const WORD vid, _In_ const WORD pid, _In_ const BYTE power, _In_ const BYTE powerMode, _In_ const WORD releaseVersion, _In_ const BYTE flushBuffers, _In_ const BYTE mask);

typedef HID_TO_UART_API HID_UART_STATUS
(WINAPI *FN_HidUart_GetUsbConfig)(HID_UART_DEVICE device, WORD* vid, WORD* pid, BYTE* power, BYTE* powerMode, WORD* releaseVersion, BYTE* flushBuffers);

typedef HID_TO_UART_API HID_UART_STATUS
(WINAPI *FN_HidUart_SetManufacturingString)(HID_UART_DEVICE device, char* manufacturingString, BYTE strlen);

typedef HID_TO_UART_API HID_UART_STATUS
(WINAPI *FN_HidUart_GetManufacturingString)(HID_UART_DEVICE device, char* manufacturingString, BYTE* strlen);

typedef HID_TO_UART_API HID_UART_STATUS
(WINAPI *FN_HidUart_SetProductString)(HID_UART_DEVICE device, char* productString, BYTE strlen);

typedef HID_TO_UART_API HID_UART_STATUS
(WINAPI *FN_HidUart_GetProductString)(HID_UART_DEVICE device, char* productString, BYTE* strlen);

typedef HID_TO_UART_API HID_UART_STATUS
(WINAPI *FN_HidUart_SetSerialString)(HID_UART_DEVICE device, char* serialString, BYTE strlen);

typedef HID_TO_UART_API HID_UART_STATUS
(WINAPI *FN_HidUart_GetSerialString)(HID_UART_DEVICE device, char* serialString, BYTE* strlen);

#define FN_VAR(_type, _var) \
	_type _var;

#define FN_INIT(_type, _var, _name) \
	_var = (_type)GetProcAddress(hDll, _name); \
	if (NULL == _var) \
		return FALSE;

//perl - ne 'print "FN_VAR(FN_$1, fn_$1)\n" if /(HidUart_\w+)/;' api.h
FN_VAR(FN_HidUart_GetNumDevices, fn_HidUart_GetNumDevices)
FN_VAR(FN_HidUart_GetString, fn_HidUart_GetString)
FN_VAR(FN_HidUart_GetOpenedString, fn_HidUart_GetOpenedString)
FN_VAR(FN_HidUart_GetIndexedString, fn_HidUart_GetIndexedString)
FN_VAR(FN_HidUart_GetOpenedIndexedString, fn_HidUart_GetOpenedIndexedString)
FN_VAR(FN_HidUart_GetAttributes, fn_HidUart_GetAttributes)
FN_VAR(FN_HidUart_GetOpenedAttributes, fn_HidUart_GetOpenedAttributes)
FN_VAR(FN_HidUart_Open, fn_HidUart_Open)
FN_VAR(FN_HidUart_Close, fn_HidUart_Close)
FN_VAR(FN_HidUart_IsOpened, fn_HidUart_IsOpened)
FN_VAR(FN_HidUart_SetUartEnable, fn_HidUart_SetUartEnable)
FN_VAR(FN_HidUart_GetUartEnable, fn_HidUart_GetUartEnable)
FN_VAR(FN_HidUart_Read, fn_HidUart_Read)
FN_VAR(FN_HidUart_Write, fn_HidUart_Write)
FN_VAR(FN_HidUart_FlushBuffers, fn_HidUart_FlushBuffers)
FN_VAR(FN_HidUart_CancelIo, fn_HidUart_CancelIo)
FN_VAR(FN_HidUart_SetTimeouts, fn_HidUart_SetTimeouts)
FN_VAR(FN_HidUart_GetTimeouts, fn_HidUart_GetTimeouts)
FN_VAR(FN_HidUart_GetUartStatus, fn_HidUart_GetUartStatus)
FN_VAR(FN_HidUart_SetUartConfig, fn_HidUart_SetUartConfig)
FN_VAR(FN_HidUart_GetUartConfig, fn_HidUart_GetUartConfig)
FN_VAR(FN_HidUart_StartBreak, fn_HidUart_StartBreak)
FN_VAR(FN_HidUart_StopBreak, fn_HidUart_StopBreak)
FN_VAR(FN_HidUart_Reset, fn_HidUart_Reset)
FN_VAR(FN_HidUart_ReadLatch, fn_HidUart_ReadLatch)
FN_VAR(FN_HidUart_WriteLatch, fn_HidUart_WriteLatch)
FN_VAR(FN_HidUart_GetPartNumber, fn_HidUart_GetPartNumber)
FN_VAR(FN_HidUart_GetLibraryVersion, fn_HidUart_GetLibraryVersion)
FN_VAR(FN_HidUart_GetHidLibraryVersion, fn_HidUart_GetHidLibraryVersion)
FN_VAR(FN_HidUart_GetHidGuid, fn_HidUart_GetHidGuid)
FN_VAR(FN_HidUart_SetLock, fn_HidUart_SetLock)
FN_VAR(FN_HidUart_GetLock, fn_HidUart_GetLock)
FN_VAR(FN_HidUart_SetUsbConfig, fn_HidUart_SetUsbConfig)
FN_VAR(FN_HidUart_GetUsbConfig, fn_HidUart_GetUsbConfig)
FN_VAR(FN_HidUart_SetManufacturingString, fn_HidUart_SetManufacturingString)
FN_VAR(FN_HidUart_GetManufacturingString, fn_HidUart_GetManufacturingString)
FN_VAR(FN_HidUart_SetProductString, fn_HidUart_SetProductString)
FN_VAR(FN_HidUart_GetProductString, fn_HidUart_GetProductString)
FN_VAR(FN_HidUart_SetSerialString, fn_HidUart_SetSerialString)
FN_VAR(FN_HidUart_GetSerialString, fn_HidUart_GetSerialString)

static Logger* m_logger;

BOOL Hooker_Init(HMODULE hDll, Logger* logger)
{
	m_logger = logger;

	// Load the DLL and get the export functions
	//perl -ne 'print "FN_INIT(FN_$1, fn_$1, \"$1\")\n" if /(HidUart_\w+)/;' api.h
	FN_INIT(FN_HidUart_GetNumDevices, fn_HidUart_GetNumDevices, "HidUart_GetNumDevices")
	FN_INIT(FN_HidUart_GetString, fn_HidUart_GetString, "HidUart_GetString")
	FN_INIT(FN_HidUart_GetOpenedString, fn_HidUart_GetOpenedString, "HidUart_GetOpenedString")
	FN_INIT(FN_HidUart_GetIndexedString, fn_HidUart_GetIndexedString, "HidUart_GetIndexedString")
	FN_INIT(FN_HidUart_GetOpenedIndexedString, fn_HidUart_GetOpenedIndexedString, "HidUart_GetOpenedIndexedString")
	FN_INIT(FN_HidUart_GetAttributes, fn_HidUart_GetAttributes, "HidUart_GetAttributes")
	FN_INIT(FN_HidUart_GetOpenedAttributes, fn_HidUart_GetOpenedAttributes, "HidUart_GetOpenedAttributes")
	FN_INIT(FN_HidUart_Open, fn_HidUart_Open, "HidUart_Open")
	FN_INIT(FN_HidUart_Close, fn_HidUart_Close, "HidUart_Close")
	FN_INIT(FN_HidUart_IsOpened, fn_HidUart_IsOpened, "HidUart_IsOpened")
	FN_INIT(FN_HidUart_SetUartEnable, fn_HidUart_SetUartEnable, "HidUart_SetUartEnable")
	FN_INIT(FN_HidUart_GetUartEnable, fn_HidUart_GetUartEnable, "HidUart_GetUartEnable")
	FN_INIT(FN_HidUart_Read, fn_HidUart_Read, "HidUart_Read")
	FN_INIT(FN_HidUart_Write, fn_HidUart_Write, "HidUart_Write")
	FN_INIT(FN_HidUart_FlushBuffers, fn_HidUart_FlushBuffers, "HidUart_FlushBuffers")
	FN_INIT(FN_HidUart_CancelIo, fn_HidUart_CancelIo, "HidUart_CancelIo")
	FN_INIT(FN_HidUart_SetTimeouts, fn_HidUart_SetTimeouts, "HidUart_SetTimeouts")
	FN_INIT(FN_HidUart_GetTimeouts, fn_HidUart_GetTimeouts, "HidUart_GetTimeouts")
	FN_INIT(FN_HidUart_GetUartStatus, fn_HidUart_GetUartStatus, "HidUart_GetUartStatus")
	FN_INIT(FN_HidUart_SetUartConfig, fn_HidUart_SetUartConfig, "HidUart_SetUartConfig")
	FN_INIT(FN_HidUart_GetUartConfig, fn_HidUart_GetUartConfig, "HidUart_GetUartConfig")
	FN_INIT(FN_HidUart_StartBreak, fn_HidUart_StartBreak, "HidUart_StartBreak")
	FN_INIT(FN_HidUart_StopBreak, fn_HidUart_StopBreak, "HidUart_StopBreak")
	FN_INIT(FN_HidUart_Reset, fn_HidUart_Reset, "HidUart_Reset")
	FN_INIT(FN_HidUart_ReadLatch, fn_HidUart_ReadLatch, "HidUart_ReadLatch")
	FN_INIT(FN_HidUart_WriteLatch, fn_HidUart_WriteLatch, "HidUart_WriteLatch")
	FN_INIT(FN_HidUart_GetPartNumber, fn_HidUart_GetPartNumber, "HidUart_GetPartNumber")
	FN_INIT(FN_HidUart_GetLibraryVersion, fn_HidUart_GetLibraryVersion, "HidUart_GetLibraryVersion")
	FN_INIT(FN_HidUart_GetHidLibraryVersion, fn_HidUart_GetHidLibraryVersion, "HidUart_GetHidLibraryVersion")
	FN_INIT(FN_HidUart_GetHidGuid, fn_HidUart_GetHidGuid, "HidUart_GetHidGuid")
	FN_INIT(FN_HidUart_SetLock, fn_HidUart_SetLock, "HidUart_SetLock")
	FN_INIT(FN_HidUart_GetLock, fn_HidUart_GetLock, "HidUart_GetLock")
	FN_INIT(FN_HidUart_SetUsbConfig, fn_HidUart_SetUsbConfig, "HidUart_SetUsbConfig")
	FN_INIT(FN_HidUart_GetUsbConfig, fn_HidUart_GetUsbConfig, "HidUart_GetUsbConfig")
	FN_INIT(FN_HidUart_SetManufacturingString, fn_HidUart_SetManufacturingString, "HidUart_SetManufacturingString")
	FN_INIT(FN_HidUart_GetManufacturingString, fn_HidUart_GetManufacturingString, "HidUart_GetManufacturingString")
	FN_INIT(FN_HidUart_SetProductString, fn_HidUart_SetProductString, "HidUart_SetProductString")
	FN_INIT(FN_HidUart_GetProductString, fn_HidUart_GetProductString, "HidUart_GetProductString")
	FN_INIT(FN_HidUart_SetSerialString, fn_HidUart_SetSerialString, "HidUart_SetSerialString")
	FN_INIT(FN_HidUart_GetSerialString, fn_HidUart_GetSerialString, "HidUart_GetSerialString")

	return TRUE;
}

HID_TO_UART_API HID_UART_STATUS WINAPI
HidUart_GetNumDevices(DWORD* numDevices, _In_ const WORD vid, _In_ const WORD pid)
{
	AutoLogCall alc(m_logger, "HidUart_GetNumDevices");
	HID_UART_STATUS rc = fn_HidUart_GetNumDevices(numDevices, vid, pid);
	return rc;
}

HID_TO_UART_API HID_UART_STATUS WINAPI
	HidUart_GetString(DWORD deviceNum, WORD vid, WORD pid, char* deviceString, DWORD options)
{
	AutoLogCall alc(m_logger, "HidUart_GetString");
	HID_UART_STATUS rc = fn_HidUart_GetString(deviceNum, vid, pid, deviceString, options);
	m_logger->LogNumber("deviceNum", deviceNum);
	m_logger->LogNumber("vid", vid, "%04X");
	m_logger->LogNumber("pid", pid, "%04X");
	//m_logger->LogString("deviceString", deviceString);
	return rc;
}

HID_TO_UART_API HID_UART_STATUS WINAPI
	HidUart_GetOpenedString(HID_UART_DEVICE device, char* deviceString, DWORD options)
{
	AutoLogCall alc(m_logger, "HidUart_GetOpenedString");
	HID_UART_STATUS rc = fn_HidUart_GetOpenedString(device, deviceString, options);
	m_logger->LogNumber("options", options);
	//m_logger->LogString("deviceString", deviceString);
	return rc;
}

HID_TO_UART_API HID_UART_STATUS WINAPI
	HidUart_GetIndexedString(DWORD deviceNum, WORD vid, WORD pid, DWORD stringIndex, char* deviceString)
{
	AutoLogCall alc(m_logger, "HidUart_GetIndexedString");
	HID_UART_STATUS rc = fn_HidUart_GetIndexedString(deviceNum, vid, pid, stringIndex, deviceString);
	return rc;
}

HID_TO_UART_API HID_UART_STATUS WINAPI
	HidUart_GetOpenedIndexedString(HID_UART_DEVICE device, DWORD stringIndex, char* deviceString)
{
	AutoLogCall alc(m_logger, "HidUart_GetOpenedIndexedString");
	HID_UART_STATUS rc = fn_HidUart_GetOpenedIndexedString(device, stringIndex, deviceString);
	return rc;
}

HID_TO_UART_API HID_UART_STATUS WINAPI
	HidUart_GetAttributes(DWORD deviceNum, WORD vid, WORD pid, WORD* deviceVid, WORD* devicePid, WORD* deviceReleaseNumber)
{
	AutoLogCall alc(m_logger, "HidUart_GetAttributes");
	HID_UART_STATUS rc = fn_HidUart_GetAttributes(deviceNum, vid, pid, deviceVid, devicePid, deviceReleaseNumber);
	return rc;
}

HID_TO_UART_API HID_UART_STATUS WINAPI
	HidUart_GetOpenedAttributes(HID_UART_DEVICE device, WORD* deviceVid, WORD* devicePid, WORD* deviceReleaseNumber)
{
	AutoLogCall alc(m_logger, "HidUart_GetOpenedAttributes");
	HID_UART_STATUS rc = fn_HidUart_GetOpenedAttributes(device, deviceVid, devicePid, deviceReleaseNumber);
	return rc;
}

HID_TO_UART_API HID_UART_STATUS WINAPI
	HidUart_Open(HID_UART_DEVICE* device, DWORD deviceNum, WORD vid, WORD pid)
{
	AutoLogCall alc(m_logger, "HidUart_Open");
	HID_UART_STATUS rc = fn_HidUart_Open(device, deviceNum, vid, pid);
	m_logger->LogNumber("deviceNum", deviceNum);
	m_logger->LogNumber("vid", vid, "%04X");
	m_logger->LogNumber("pid", pid, "%04X");
	return rc;
}

HID_TO_UART_API HID_UART_STATUS WINAPI
	HidUart_Close(HID_UART_DEVICE device)
{
	AutoLogCall alc(m_logger, "HidUart_Close");
	HID_UART_STATUS rc = fn_HidUart_Close(device);
	return rc;
}

HID_TO_UART_API HID_UART_STATUS WINAPI
	HidUart_IsOpened(HID_UART_DEVICE device, BOOL* opened)
{
	AutoLogCall alc(m_logger, "HidUart_IsOpened");
	HID_UART_STATUS rc = fn_HidUart_IsOpened(device, opened);
	return rc;
}

HID_TO_UART_API HID_UART_STATUS WINAPI
	HidUart_SetUartEnable(HID_UART_DEVICE device, BOOL enable)
{
	AutoLogCall alc(m_logger, "HidUart_SetUartEnable");
	HID_UART_STATUS rc = fn_HidUart_SetUartEnable(device, enable);
	return rc;
}

HID_TO_UART_API HID_UART_STATUS WINAPI
	HidUart_GetUartEnable(HID_UART_DEVICE device, BOOL* enable)
{
	AutoLogCall alc(m_logger, "HidUart_GetUartEnable");
	HID_UART_STATUS rc = fn_HidUart_GetUartEnable(device, enable);
	return rc;
}

HID_TO_UART_API HID_UART_STATUS WINAPI
	HidUart_Read(HID_UART_DEVICE device, BYTE* buffer, DWORD numBytesToRead, DWORD* numBytesRead)
{
	AutoLogCall alc(m_logger, "HidUart_Read");
	HID_UART_STATUS rc = fn_HidUart_Read(device, buffer, numBytesToRead, numBytesRead);
	m_logger->LogNumber("numBytesToRead", numBytesToRead);
	m_logger->LogBlock("data", buffer, *numBytesRead);
	return rc;
}

HID_TO_UART_API HID_UART_STATUS WINAPI
	HidUart_Write(HID_UART_DEVICE device, BYTE* buffer, DWORD numBytesToWrite, DWORD* numBytesWritten)
{
	AutoLogCall alc(m_logger, "HidUart_Write");
	HID_UART_STATUS rc = fn_HidUart_Write(device, buffer, numBytesToWrite, numBytesWritten);
	m_logger->LogNumber("numBytesWritten", *numBytesWritten);
	m_logger->LogBlock("data", buffer, numBytesToWrite);
	return rc;
}

HID_TO_UART_API HID_UART_STATUS WINAPI
	HidUart_FlushBuffers(HID_UART_DEVICE device, BOOL flushTransmit, BOOL flushReceive)
{
	AutoLogCall alc(m_logger, "HidUart_FlushBuffers");
	HID_UART_STATUS rc = fn_HidUart_FlushBuffers(device, flushTransmit, flushReceive);
	return rc;
}

HID_TO_UART_API HID_UART_STATUS WINAPI
	HidUart_CancelIo(HID_UART_DEVICE device)
{
	AutoLogCall alc(m_logger, "HidUart_CancelIo");
	HID_UART_STATUS rc = fn_HidUart_CancelIo(device);
	return rc;
}

HID_TO_UART_API HID_UART_STATUS WINAPI
	HidUart_SetTimeouts(HID_UART_DEVICE device, DWORD readTimeout, DWORD writeTimeout)
{
	AutoLogCall alc(m_logger, "HidUart_SetTimeouts");
	HID_UART_STATUS rc = fn_HidUart_SetTimeouts(device, readTimeout, writeTimeout);
	m_logger->LogNumber("readTimeout", readTimeout);
	m_logger->LogNumber("writeTimeout", writeTimeout);
	return rc;
}

HID_TO_UART_API HID_UART_STATUS WINAPI
	HidUart_GetTimeouts(HID_UART_DEVICE device, DWORD* readTimeout, DWORD* writeTimeout)
{
	AutoLogCall alc(m_logger, "HidUart_GetTimeouts");
	HID_UART_STATUS rc = fn_HidUart_GetTimeouts(device, readTimeout, writeTimeout);
	m_logger->LogNumber("readTimeout", *readTimeout);
	m_logger->LogNumber("writeTimeout", *writeTimeout);
	return rc;
}

HID_TO_UART_API HID_UART_STATUS WINAPI
	HidUart_GetUartStatus(HID_UART_DEVICE device, WORD* transmitFifoSize, WORD* receiveFifoSize, BYTE* errorStatus, BYTE* lineBreakStatus)
{
	AutoLogCall alc(m_logger, "HidUart_GetUartStatus");
	HID_UART_STATUS rc = fn_HidUart_GetUartStatus(device, transmitFifoSize, receiveFifoSize, errorStatus, lineBreakStatus);
	return rc;
}

HID_TO_UART_API HID_UART_STATUS WINAPI
	HidUart_SetUartConfig(_In_ HID_UART_DEVICE device, _In_ const DWORD baudRate, _In_ const BYTE dataBits, _In_ const BYTE parity, _In_ const BYTE stopBits, _In_ const BYTE flowControl)
{
	AutoLogCall alc(m_logger, "HidUart_SetUartConfig");
	HID_UART_STATUS rc = fn_HidUart_SetUartConfig(device, baudRate, dataBits, parity, stopBits, flowControl);
	m_logger->LogNumber("baudRate", baudRate);
	m_logger->LogNumber("dataBits", dataBits);
	m_logger->LogNumber("parity", parity);
	m_logger->LogNumber("stopBits", stopBits);
	m_logger->LogNumber("flowControl", flowControl);
	return rc;
}

HID_TO_UART_API HID_UART_STATUS WINAPI
	HidUart_GetUartConfig(HID_UART_DEVICE device, DWORD* baudRate, BYTE* dataBits, BYTE* parity, BYTE* stopBits, BYTE* flowControl)
{
	AutoLogCall alc(m_logger, "HidUart_GetUartConfig");
	HID_UART_STATUS rc = fn_HidUart_GetUartConfig(device, baudRate, dataBits, parity, stopBits, flowControl);
	m_logger->LogNumber("baudRate", *baudRate);
	m_logger->LogNumber("dataBits", *dataBits);
	m_logger->LogNumber("parity", *parity);
	m_logger->LogNumber("stopBits", *stopBits);
	m_logger->LogNumber("flowControl", *flowControl);
	return rc;
}

HID_TO_UART_API HID_UART_STATUS WINAPI
	HidUart_StartBreak(HID_UART_DEVICE device, BYTE duration)
{
	AutoLogCall alc(m_logger, "HidUart_StartBreak");
	HID_UART_STATUS rc = fn_HidUart_StartBreak(device, duration);
	return rc;
}

HID_TO_UART_API HID_UART_STATUS WINAPI
	HidUart_StopBreak(HID_UART_DEVICE device)
{
	AutoLogCall alc(m_logger, "HidUart_StopBreak");
	HID_UART_STATUS rc = fn_HidUart_StopBreak(device);
	return rc;
}

HID_TO_UART_API HID_UART_STATUS WINAPI
	HidUart_Reset(HID_UART_DEVICE device)
{
	AutoLogCall alc(m_logger, "HidUart_Reset");
	HID_UART_STATUS rc = fn_HidUart_Reset(device);
	return rc;
}

HID_TO_UART_API HID_UART_STATUS WINAPI
	HidUart_ReadLatch(_In_ HID_UART_DEVICE device, WORD* latchValue)
{
	AutoLogCall alc(m_logger, "HidUart_ReadLatch");
	HID_UART_STATUS rc = fn_HidUart_ReadLatch(device, latchValue);
	return rc;
}

HID_TO_UART_API HID_UART_STATUS WINAPI
	HidUart_WriteLatch(_In_ HID_UART_DEVICE device, _In_ const WORD latchValue, _In_ const WORD latchMask)
{
	AutoLogCall alc(m_logger, "HidUart_WriteLatch");
	HID_UART_STATUS rc = fn_HidUart_WriteLatch(device, latchValue, latchMask);
	return rc;
}

HID_TO_UART_API HID_UART_STATUS WINAPI
	HidUart_GetPartNumber(HID_UART_DEVICE device, BYTE* partNumber, BYTE* version)
{
	AutoLogCall alc(m_logger, "HidUart_GetPartNumber");
	HID_UART_STATUS rc = fn_HidUart_GetPartNumber(device, partNumber, version);
	m_logger->LogNumber("partNumber", *partNumber);
	m_logger->LogNumber("version", *version);
	return rc;
}

HID_TO_UART_API HID_UART_STATUS WINAPI
	HidUart_GetLibraryVersion(_Out_writes_bytes_(1) BYTE* major, _Out_writes_bytes_(1) BYTE* minor, _Out_writes_bytes_(sizeof(BOOL)) BOOL* release)
{
	AutoLogCall alc(m_logger, "HidUart_GetLibraryVersion");
	HID_UART_STATUS rc = fn_HidUart_GetLibraryVersion(major, minor, release);
	return rc;
}

HID_TO_UART_API HID_UART_STATUS WINAPI
	HidUart_GetHidLibraryVersion(_Out_writes_bytes_(1) BYTE* major, _Out_writes_bytes_(1) BYTE* minor, _Out_writes_bytes_(sizeof(BOOL)) BOOL* release)
{
	AutoLogCall alc(m_logger, "HidUart_GetHidLibraryVersion");
	HID_UART_STATUS rc = fn_HidUart_GetHidLibraryVersion(major, minor, release);
	return rc;
}

HID_TO_UART_API HID_UART_STATUS WINAPI
	HidUart_GetHidGuid(_Out_ void* guid)
{
	AutoLogCall alc(m_logger, "HidUart_GetHidGuid");
	HID_UART_STATUS rc = fn_HidUart_GetHidGuid(guid);
	return rc;
}

HID_TO_UART_API HID_UART_STATUS WINAPI
	HidUart_SetLock(HID_UART_DEVICE device, WORD lock)
{
	AutoLogCall alc(m_logger, "HidUart_SetLock");
	HID_UART_STATUS rc = fn_HidUart_SetLock(device, lock);
	return rc;
}
HID_TO_UART_API HID_UART_STATUS WINAPI
	HidUart_GetLock(HID_UART_DEVICE device, WORD* lock)
{
	AutoLogCall alc(m_logger, "HidUart_GetLock");
	HID_UART_STATUS rc = fn_HidUart_GetLock(device, lock);
	return rc;
}

HID_TO_UART_API HID_UART_STATUS WINAPI
	HidUart_SetUsbConfig(_In_ HID_UART_DEVICE device, _In_ const WORD vid, _In_ const WORD pid, 
		_In_ const BYTE power, _In_ const BYTE powerMode, _In_ const WORD releaseVersion, 
		_In_ const BYTE flushBuffers, _In_ const BYTE mask)
{
	AutoLogCall alc(m_logger, "HidUart_SetUsbConfig");
	HID_UART_STATUS rc = fn_HidUart_SetUsbConfig(device, vid, pid, power, powerMode, releaseVersion, flushBuffers, mask);
	return rc;
}

HID_TO_UART_API HID_UART_STATUS WINAPI
	HidUart_GetUsbConfig(HID_UART_DEVICE device, WORD* vid, WORD* pid, 
		BYTE* power, BYTE* powerMode, WORD* releaseVersion, BYTE* flushBuffers)
{
	AutoLogCall alc(m_logger, "HidUart_GetUsbConfig");
	HID_UART_STATUS rc = fn_HidUart_GetUsbConfig(device, vid, pid, power, powerMode, releaseVersion, flushBuffers);
	return rc;
}

HID_TO_UART_API HID_UART_STATUS WINAPI
	HidUart_SetManufacturingString(HID_UART_DEVICE device, char* manufacturingString, BYTE strlen)
{
	AutoLogCall alc(m_logger, "HidUart_SetManufacturingString");
	HID_UART_STATUS rc = fn_HidUart_SetManufacturingString(device, manufacturingString, strlen);
	return rc;
}

HID_TO_UART_API HID_UART_STATUS WINAPI
	HidUart_GetManufacturingString(HID_UART_DEVICE device, char* manufacturingString, BYTE* strlen)
{
	AutoLogCall alc(m_logger, "HidUart_GetManufacturingString");
	HID_UART_STATUS rc = fn_HidUart_GetManufacturingString(device, manufacturingString, strlen);
	return rc;
}

HID_TO_UART_API HID_UART_STATUS WINAPI
	HidUart_SetProductString(HID_UART_DEVICE device, char* productString, BYTE strlen)
{
	AutoLogCall alc(m_logger, "HidUart_SetProductString");
	HID_UART_STATUS rc = fn_HidUart_SetProductString(device, productString, strlen);
	return rc;
}

HID_TO_UART_API HID_UART_STATUS WINAPI
	HidUart_GetProductString(HID_UART_DEVICE device, char* productString, BYTE* strlen)
{
	AutoLogCall alc(m_logger, "HidUart_GetProductString");
	HID_UART_STATUS rc = fn_HidUart_GetProductString(device, productString, strlen);
	return rc;
}

HID_TO_UART_API HID_UART_STATUS WINAPI
	HidUart_SetSerialString(HID_UART_DEVICE device, char* serialString, BYTE strlen)
{
	AutoLogCall alc(m_logger, "HidUart_SetSerialString");
	HID_UART_STATUS rc = fn_HidUart_SetSerialString(device, serialString, strlen);
	return rc;
}

HID_TO_UART_API HID_UART_STATUS WINAPI
	HidUart_GetSerialString(HID_UART_DEVICE device, char* serialString, BYTE* strlen)
{
	AutoLogCall alc(m_logger, "HidUart_GetSerialString");
	HID_UART_STATUS rc = fn_HidUart_GetSerialString(device, serialString, strlen);
	return rc;
}

