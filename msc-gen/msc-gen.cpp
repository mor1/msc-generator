/*
    This file is part of Msc-generator.
	Copyright 2008,2009,2010 Zoltan Turanyi
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
	char buff[1024]; 
	GetModuleFileName(NULL, buff, 1024);
	std::string dir(buff);
	int pos = dir.find_last_of('\\');
	const string designlib_filename = dir.substr(0,pos).append("\\").append(fileName);
	string ret; 

	WIN32_FIND_DATA find_data;
	HANDLE h= FindFirstFile(designlib_filename.c_str(), &find_data);
	bool bFound = h != INVALID_HANDLE_VALUE;
	while (bFound) {
		ifstream in(find_data.cFileName);
		filebuf *pbuf = in.rdbuf();
		// get file size using buffer's members
		long size=pbuf->pubseekoff (0,ios::end,ios::in);
		pbuf->pubseekpos (0,ios::in);
		char *buffer=new char[size+1];
		// get file data  
		pbuf->sgetn(buffer,size);
		buffer[size]=0;
		ret += buffer;
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

    return do_main(args, designs.c_str());
	return 0;
}

