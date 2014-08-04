/*
This file is part of Msc-generator.
Copyright 2008,2009,2010,2011,2012,2013,2014 Zoltan Turanyi
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

/** @file commandline.h All that is needed to perform a command-line action.
* @ingroup libmscgen */

#if !defined(COMMANDLINE_H)
#define COMMANDLINE_H

#include <list>
#include <string>
#include "version.h"
#include "progress.h"

const char *VersionText(char a=LIBMSCGEN_MAJOR,
                        char b=LIBMSCGEN_MINOR,
                        char c=LIBMSCGEN_SUPERMINOR);

char *ReadFile(FILE *in);
int do_main(const std::list<std::string> &args, 
            const std::list<std::pair<std::string, std::string>> &design_files, 
            std::string csh_textformat,
            MscProgress::ProgressCallback cb = NULL, void *param=NULL,
            std::string *load_data=NULL);

#endif /* COMMANDLINE_H */
