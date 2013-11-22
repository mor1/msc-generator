/*
    This file is part of Msc-generator.
	Copyright 2008,2009,2010,2011,2012,2013 Zoltan Turanyi
	Distributed under GNU Affero General Public License.

    Msc-generator is free software: you can redistribute it and/or modify
    it under the terms of the GNU Affero General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Msc-generator is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with Msc-generator.  If not, see <http://www.gnu.org/licenses/>.
*/

// msc-gen.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <assert.h>
#include "commandline.h"
#include "cairo.h"
#include "cairo-win32.h"

using namespace std;

string ReadDesigns(const char *fileName) 
{
	if (!fileName || !fileName[0]) fileName = "designlib.signalling";
	char buff[1024]; 
	GetModuleFileName(NULL, buff, 1024);
	std::string dir(buff);
	int pos = dir.find_last_of('\\');
	dir = dir.substr(0,pos).append("\\");
	string ret; 

	WIN32_FIND_DATA find_data;
	HANDLE h= FindFirstFile((dir+fileName).c_str(), &find_data);
	bool bFound = h != INVALID_HANDLE_VALUE;
	while (bFound) {
		FILE *in = fopen((dir+find_data.cFileName).c_str(), "rb");
		char *buffer = ReadFile(in);
		ret += buffer;
		free(buffer);
		bFound = FindNextFile(h, &find_data);
	}
	FindClose(h);
	return ret;
}

std::list<std::pair<std::string,std::string>> ReadShapes(const char *fileName)
{
    std::list<std::pair<std::string, std::string>> ret;
    if (!fileName || !fileName[0]) fileName = "*.shape";
    char buff[1024];
    GetModuleFileName(NULL, buff, 1024);
    std::string dir(buff);
    int pos = dir.find_last_of('\\');
    dir = dir.substr(0, pos).append("\\");

    WIN32_FIND_DATA find_data;
    HANDLE h = FindFirstFile((dir+fileName).c_str(), &find_data);
    bool bFound = h != INVALID_HANDLE_VALUE;
    while (bFound) {
        FILE *in = fopen((dir+find_data.cFileName).c_str(), "r");
        char *buffer = ReadFile(in);
        ret.emplace_back(dir+find_data.cFileName, buffer);
        free(buffer);
        bFound = FindNextFile(h, &find_data);
    }
    FindClose(h);
    //Now look for in the roaming folder
    h = FindFirstFile((string("%appdata%\\Msc-generator\\")+fileName).c_str(), &find_data);
    bFound = h != INVALID_HANDLE_VALUE;
    while (bFound) {
        FILE *in = fopen((dir+find_data.cFileName).c_str(), "r");
        char *buffer = ReadFile(in);
        ret.emplace_back(dir+find_data.cFileName, buffer);
        free(buffer);
        bFound = FindNextFile(h, &find_data);
    }
    FindClose(h);
    return ret;
}


bool progressbar(double percent, void *p) 
{
    HANDLE hOut = HANDLE(p);
    CONSOLE_SCREEN_BUFFER_INFO bInfo;
    COORD crStart, crCurr;
    GetConsoleScreenBufferInfo(hOut, &bInfo);
    crCurr = bInfo.dwCursorPosition; //the old position
    crStart.X = 0;
    crStart.Y = crCurr.Y;
    SetConsoleCursorPosition(hOut, crStart);
    printf("%3d%%", unsigned(floor(percent+0.5)));
    SetConsoleCursorPosition(hOut, crCurr);
    return false;
}

#define REG_SUBKEY_SETTINGS "Software\\Zoltan Turanyi\\Msc-generator\\Settings"
#define REG_KEY_LOAD_DATA "LoadData"

int _tmain(int argc, _TCHAR* argv[])
{
    std::list<std::string> args;
    for(int i=1; i<argc; i++)
        args.push_back(std::string(argv[i]));

	string designs = ReadDesigns("designlib.signalling");
    if (designs.length()==0) 
        designs = ReadDesigns("original_designlib.signalling");

    HANDLE hOut = GetStdHandle(STD_ERROR_HANDLE);

    DWORD len=0;
    char *buffer=NULL;
    std::string load_data;
    LONG res = RegGetValue(HKEY_CURRENT_USER, REG_SUBKEY_SETTINGS,
                           REG_KEY_LOAD_DATA, RRF_RT_REG_SZ, NULL, NULL, &len);
    if (res==ERROR_MORE_DATA || res==ERROR_SUCCESS) {
        buffer = (char*)malloc(len+1);
        res = RegGetValue(HKEY_CURRENT_USER, REG_SUBKEY_SETTINGS,
                          REG_KEY_LOAD_DATA, RRF_RT_REG_SZ, NULL, buffer, &len);
        if (res==ERROR_SUCCESS) {
            buffer[len] = 0;
            load_data = buffer;
        }
    }
    int ret = do_main(args, designs.c_str(), ReadShapes("*.shape"),
                      "\\f(courier new)\\mn(12)", 
                      progressbar, hOut, &load_data);
    RegSetKeyValue(HKEY_CURRENT_USER, REG_SUBKEY_SETTINGS,
                   REG_KEY_LOAD_DATA, REG_SZ, load_data.c_str(), load_data.length()+1);
    return ret;
}

