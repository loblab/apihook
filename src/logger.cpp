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

#define XML_HEAD "<?xml version='1.0' encoding='UTF-8'?>"
#define XSL_HEAD "<?xml-stylesheet type='text/xsl' href='http://loblab.web.com/apihook/view.xsl'?>"

using namespace std;

Logger::Logger()
{
}


Logger::~Logger()
{
}

ostringstream& Logger::GetThreadStream()
{
	DWORD tid = GetCurrentThreadId();
	try
	{
		return m_streams.at(tid);
	}
	catch (const out_of_range& oor)
	{
		ostringstream* oss = new ostringstream();
		m_streams.insert({ tid, *oss });
		return m_streams.at(tid);
	}
}

bool Logger::Open(const char* datadir)
{
	char logfile[_MAX_PATH];

	strcpy(m_dir, datadir);
	sprintf(logfile, "%s\\%s", m_dir, "log.xml");

	m_cCall = 0;
	m_fs.open(logfile, ofstream::out | ofstream::trunc);
	m_fs << XML_HEAD << endl;
	//m_fs << XSL_HEAD << endl;

	m_fs << "<calls ";
	LogTime();
	m_fs << ">" << endl;

	m_tick0 = GetTickCount64();
	return true;
}

void Logger::Close()
{
	m_fs << "</calls>" << endl;
	m_fs.close();
}

void Logger::LogTime()
{
	// log current time
	char buf[24];
	SYSTEMTIME t;
	GetLocalTime(&t);
	sprintf(buf, "%02d:%02d:%02d", t.wHour, t.wMinute, t.wSecond);
	m_fs << "time='" << buf << "' ";
}

void Logger::LogString(const char* title, const char* value)
{
	ostringstream& oss = GetThreadStream();
	oss << title << "='" << value << "' ";
}

void Logger::LogNumber(const char* title, long value, int radix)
{
	char buf[33];
	ltoa(value, buf, radix);
	LogString(title, buf);
}

void Logger::LogNumber(const char* title, long value, const char* fmt)
{
	char buf[33];
	sprintf(buf, fmt, value);
	LogString(title, buf);
}

void Logger::LogFloat(const char* title, double value, const char* fmt)
{
	char buf[33];
	sprintf(buf, fmt, value);
	LogString(title, buf);
}

void Logger::LogBlock(const char* title, const void* pData, int len)
{
	if (0 == len)
		return;

	char* buf = new char[len * 3 + 10];
	char* p = buf;
	const unsigned char* data = (const unsigned char*)pData;
	sprintf(p, "%3d - ", len);
	p += strlen(p);
	for (int i = 0; i<len; i++)
	{
		sprintf(p, "%02X ", *data);
		p += 3;
		data++;
	}
	*(--p) = '\0'; //remove the last space
	LogString(title, buf);
	delete[] buf;
}

bool Logger::SaveBlock(const char* filename, const void* pData, int len)
{
	if (0 == len)
		return false;

	char filepath[_MAX_PATH];
	sprintf(filepath, "%s\\%s", m_dir, filename);

	ofstream ofs(filepath, ofstream::out | ofstream::trunc | ofstream::binary);
	ofs.write((const char*)pData, len);
	ofs.close();
	return true;
}

void Logger::LogHead(const char* tag)
{
	ostringstream& oss = GetThreadStream();
	oss << "<" << tag << " ";
	m_cCall++;
	LogNumber("id", m_cCall, "%04d");

	// log current relative time
	ULONGLONG tick = GetTickCount64();
	LogFloat("tick", (double)(tick - m_tick0)/1000.0, "%.3f");
	// log current thread id
	DWORD tid = GetCurrentThreadId();
	LogNumber("thread", tid, 16);
}

void Logger::LogTail()
{
	ostringstream& oss = GetThreadStream();
	oss << "/>" << endl;
	m_fs << oss.str();
	oss.str("");
	//oss.clear();
}
