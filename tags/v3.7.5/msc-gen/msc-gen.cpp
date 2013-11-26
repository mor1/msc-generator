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

/** Get the path to a folder.
* 1==the folder of the executable
* 2==appdata/mscgenerator fodler */
std::string GetFolder(unsigned folder)
{
    char buff[MAX_PATH];
    std::string dir;
    if (folder == 1) {
        GetModuleFileName(NULL, buff, MAX_PATH);
        dir = buff;
        unsigned pos = dir.find_last_of('\\');
        _ASSERT(pos!=std::string::npos);
        dir.erase(pos+1);
    } else if (folder==2) {
        if (!SUCCEEDED(SHGetFolderPath(NULL, CSIDL_APPDATA, NULL, 0, buff)))
            return dir;
        dir = buff;
        dir.append("\\Msc-generator\\");
    }
    return dir;
}

std::list<std::pair<std::string, std::string>> ReadDesigns()
{
    string dir = GetFolder(1);
    std::list<std::pair<std::string, std::string>> ret;

	WIN32_FIND_DATA find_data;
	HANDLE h= FindFirstFile((dir+"designlib.signalling").c_str(), &find_data);
    if (h == INVALID_HANDLE_VALUE) {
        FindClose(h);
        h = FindFirstFile((dir+"original_designlib.signalling").c_str(), &find_data);
    }
	bool bFound = h != INVALID_HANDLE_VALUE;
	while (bFound) {
		FILE *in = fopen((dir+find_data.cFileName).c_str(), "r");
		char *buffer = ReadFile(in);
        if (buffer) {
            ret.emplace_back(dir+find_data.cFileName, buffer);
            free(buffer);
        }
		bFound = FindNextFile(h, &find_data);
	}
	FindClose(h);

    dir = GetFolder(2);
    h = FindFirstFile((dir+"*.signalling").c_str(), &find_data);
    bFound = h != INVALID_HANDLE_VALUE;
    while (bFound) {
        FILE *in = fopen((dir+find_data.cFileName).c_str(), "r");
        char *buffer = ReadFile(in);
        if (buffer) {
            ret.emplace_back(dir+find_data.cFileName, buffer);
            free(buffer);
        }
        bFound = FindNextFile(h, &find_data);
    }
    FindClose(h);
    return ret;
}

std::list<std::pair<std::string, std::string>> ReadShapes()
{
    string dir = GetFolder(1);
    std::list<std::pair<std::string, std::string>> ret;

    WIN32_FIND_DATA find_data;
    HANDLE h = FindFirstFile((dir+"default.shape").c_str(), &find_data);
    bool bFound = h != INVALID_HANDLE_VALUE;
    while (bFound) {
        FILE *in = fopen((dir+find_data.cFileName).c_str(), "r");
        char *buffer = ReadFile(in);
        if (buffer) {
            ret.emplace_back(dir+find_data.cFileName, buffer);
            free(buffer);
        }
        bFound = FindNextFile(h, &find_data);
    }
    FindClose(h);

    dir = GetFolder(2);
    h = FindFirstFile((dir+"*.shape").c_str(), &find_data);
    bFound = h != INVALID_HANDLE_VALUE;
    while (bFound) {
        FILE *in = fopen((dir+find_data.cFileName).c_str(), "r");
        char *buffer = ReadFile(in);
        if (buffer) {
            ret.emplace_back(dir+find_data.cFileName, buffer);
            free(buffer);
        }
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
    std::list<std::pair<std::string, std::string>> designs, shapes;
    bool oLoadShapes = true;
    bool oLoadDesigns = true;
    std::list<std::string> args;
    for (int i = 1; i<argc; i++) {
        args.push_back(std::string(argv[i]));
        if (args.back()=="--nodesigns")
            oLoadDesigns = false;
        else if (args.back()=="--noshapes")
            oLoadShapes = false;
    }

    if (oLoadShapes)
        shapes = ReadShapes();
    if (oLoadDesigns)
        designs = ReadDesigns();

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

    int ret = do_main(args, designs, shapes, "\\f(courier new)\\mn(12)", 
                      progressbar, hOut, &load_data);
    RegSetKeyValue(HKEY_CURRENT_USER, REG_SUBKEY_SETTINGS,
                   REG_KEY_LOAD_DATA, REG_SZ, load_data.c_str(), load_data.length()+1);
    return ret;
}

