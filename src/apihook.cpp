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
#include "logger.h"

#define TARGET_DLL "SLABHIDtoUART_.dll"
#define LOG_FOLDER "D:\\temp\\apihook" //create this folder first!

// global variables
static HMODULE m_hDll;
static Logger m_logger;
extern BOOL Hooker_Init(HMODULE hDll, Logger* logger);

BOOL Init()
{
	if (!m_logger.Open(LOG_FOLDER))
		return FALSE;

	m_hDll = LoadLibrary(TARGET_DLL);
	if (NULL == m_hDll)
		return FALSE;

	if (!Hooker_Init(m_hDll, &m_logger))
		return FALSE;
	return TRUE;
}

BOOL Release()
{
	if (m_hDll)
		FreeLibrary(m_hDll);
	m_logger.Close();
	return TRUE;
}

BOOL WINAPI DllMain(HANDLE hinstDLL, DWORD dwReason, LPVOID lpvReserved)
{
	if (DLL_PROCESS_ATTACH == dwReason)
		return Init();
	if (DLL_PROCESS_DETACH == dwReason)
		return Release();
	return TRUE;
}
