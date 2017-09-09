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

#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <sstream>
#include <fstream>
#include <map>

class Logger
{
public:
	Logger();
	~Logger();
	bool Open(const char* datadir);
	void Close();
	void LogString(const char* title, const char* value);
	void LogNumber(const char* title, long value, int radix = 10);
	void LogNumber(const char* title, long value, const char* fmt);
	void LogFloat(const char* title, double value, const char* fmt="%f");
	void LogBlock(const char* title, const void* pData, int len);
	bool SaveBlock(const char* filename, const void* pData, int len);
	void LogHead(const char* tag);
	void LogTail();

private:
	long m_cCall; // m_cCall the calls
	std::ofstream m_fs;
	char m_dir[_MAX_PATH];
	ULONGLONG m_tick0; //start time
	void LogTime();

private:
	std::map<unsigned int, std::ostringstream&> m_streams;
	std::ostringstream& GetThreadStream();
};

class AutoLogCall
{
public:
	AutoLogCall(Logger* logger, const char* func)
	{
		m_logger = logger;
		m_logger->LogHead(func);
	}

	~AutoLogCall()
	{
		m_logger->LogTail();
	}

private:
	Logger* m_logger;
};
