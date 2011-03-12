/*
    This file is part of Msc-generator.
	Copyright 2008,2009,2010,2011 Zoltan Turanyi
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
#include <assert.h>
#include "commandline.h"

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


int _tmain(int argc, _TCHAR* argv[])
{
    std::list<std::string> args;
    for(int i=1; i<argc; i++)
        args.push_back(std::string(argv[i]));

	string designs = ReadDesigns("designlib.signalling");

    return do_main(args, designs.c_str(), "\\f(courier new)\\mn(12)");
	return 0;
}

