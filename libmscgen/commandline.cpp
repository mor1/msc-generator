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

#include <iostream>
#include <assert.h>
#include "commandline.h"
#include "msc.h"
#include "version.h"

unsigned ReadFile(char **buff, FILE *in)
{
    unsigned alloc = 16384;
    unsigned len = 0;
    *buff = (char*)malloc(alloc);
    while (!feof(in)) {
        len += fread(*buff+len, 1, alloc-len, in);
        if (len == alloc)
            *buff = (char*)realloc(*buff, alloc+=16384);
    }
    return len;
}


/** Print program usage and return.
 */
static void usage()
{
    printf(
"Usage: mscgen [-p] [-T <type>] [--pedantic] [--strict] [-o <file>] [<infile>]\n"
"       mscgen -l\n"
"\n"
"Where:\n"
" -T <type>   Specifies the output file type, which maybe one of 'png', 'eps',\n"
"             'pdf', 'svg' or 'wmf' (if supported on your system). Default is 'png'.\n"
" -o <file>   Write output to the named file.  If omitted the input filename\n"
"             will be appended by .png or .ps. If neither input nor output \n"
"             file is given, mscgen_out.{png,eps,svg,pdf,wmf} will be used.\n"
" <infile>    The file from which to read input.  If omitted or specified as\n"
"             '-', input will be read from stdin.\n"
" -l          Display program licence and exit.\n"
" -p          Print parsed msc output (for parser debug).\n"
" --pedantic  When used all entities must be declared before being used.\n"
" --strict    Requires strict adherence to syntax rules, will bail out on errors\n"
" -Wno        No warnings displayed."
" -Werror     Warnings result in errors and no output."
"\n"
"Msc-generator version %s, Copyright (C) 2008-9 Zoltan Turanyi,\n"
"Msc-generator comes with ABSOLUTELY NO WARRANTY.  This is free software, and you are\n"
"welcome to redistribute it under certain conditions; type `mscgen -l' for\n"
"details.\n",
VERSION);
}

/** Print program licence and return.
 */
static void licence()
{
    printf(
"Msc-generator, a message sequence chart renderer.\n"
"Copyright (C) 2008-2009 Zoltan Turanyi\n"
"\n"
"This program is free software; you can redistribute it and/or modify\n"
"it under the terms of the GNU General Public License as published by\n"
"the Free Software Foundation; either version 2 of the License, or\n"
"(at your option) any later version.\n"
"\n"
"This program is distributed in the hope that it will be useful,\n"
"but WITHOUT ANY WARRANTY; without even the implied warranty of\n"
"MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n"
"GNU General Public License for more details.\n"
"\n"
"You should have received a copy of the GNU General Public License\n"
"along with this program; if not, write to the Free Software\n"
"Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA\n");
}

int do_main(const std::list<std::string> &args, const char *designs)
{
    MscDrawer::OutputType oOutType=MscDrawer::PNG;
    string                oOutputFile;
    string                oInputFile;
    bool                  oPrint = false;
    bool                  oWerror = false;
    bool                  oWarning = true;
    string ss;

    Msc msc;

    for (std::list<std::string>::const_iterator i=args.begin(); i!=args.end(); i++) {
        if (*i == "-o") {
            if (i==--args.end()) {
                std::cerr<<"Error: Missing output filename after '-o'.";
                usage();
                return EXIT_FAILURE;
            }
            oOutputFile = *(++i);
        } else if (*i == "-T") {
            if (i==--args.end()) {
                std::cerr<<"Error: Missing output type after '-T'.";
                usage();
                return EXIT_FAILURE;
            }
            i++;
#ifdef CAIRO_HAS_PNG_FUNCTIONS
            if (*i == "png")
                oOutType = MscDrawer::PNG;
            else
#endif
#ifdef CAIRO_HAS_PS_SURFACE
            if (*i == "eps")
                oOutType = MscDrawer::EPS;
            else
#endif
#ifdef CAIRO_HAS_PDF_SURFACE
	    if (*i == "pdf")
                oOutType = MscDrawer::PDF;
            else
#endif
#ifdef CAIRO_HAS_SVG_SURFACE
            if (*i == "svg")
                oOutType = MscDrawer::SVG;
            else
#endif
#ifdef CAIRO_HAS_WIN32_SURFACE
            if (*i == "wmf")
                oOutType = MscDrawer::WMF;
            else
#endif
             {
                std::cerr<<"Error: Unknown output format '" << *i << "'. Use one of "
#ifdef CAIRO_HAS_PNG_FUNCTIONS
                     " 'png'"
#endif
#ifdef CAIRO_HAS_PS_SURFACE
                     " 'eps'"
#endif
#ifdef CAIRO_HAS_PDF_SURFACE
                     " 'pdf'"
#endif
#ifdef CAIRO_HAS_SVG_SURFACE
                     " 'svg'"
#endif
#ifdef CAIRO_HAS_WIN32_SURFACE
                     " 'wmf'"
#endif
                     ".\n";
                return EXIT_FAILURE;
            }
        } else if ((*i)[0]=='-' && (*i)[1]=='-') { //starts with "--"
            string name(i->substr(2));
            if (name.find("=") == name.npos) {
                //no "=" in switch
                Attribute a(name.c_str(), true, msc.current_pos, msc.current_pos, "yes");
                a.error = true;  //supress errors in AddAttribute
                if (msc.AddAttribute(a)) continue;
                if (!msc.SetDesign(a.name, true))
                    msc.Error.Warning(msc.current_pos,
                                      "Unknown chart design: '" + a.name + "'. Using 'plain'.",
                                      " Available styles are: " + msc.GetDesigns() +".");
            } else {
                float num;
                string value(name.substr(name.find("=")+1));
                name.erase(name.find("="));
                if (value == "yes" || name == "Yes" || name == "YES")
                    msc.AddAttribute(Attribute(name.c_str(), true, msc.current_pos,
                                               msc.current_pos, value.c_str()));
                else if(value == "no" || name == "No" || name == "NO")
                    msc.AddAttribute(Attribute(name.c_str(), false, msc.current_pos,
                                               msc.current_pos, value.c_str()));
                else if (sscanf(value.c_str(), "%f", &num) == 1)
                    msc.AddAttribute(Attribute(name.c_str(), num, msc.current_pos,
                                               msc.current_pos, value.c_str()));
                else
                    msc.AddAttribute(Attribute(name.c_str(), value.c_str(), msc.current_pos,
                                               msc.current_pos, name.c_str()));
            }
        } else if (*i == "-l") {
            licence();
            return EXIT_SUCCESS;
        } else if (*i == "-Wno") {
            oWarning = false;
        } else if (*i == "-Werror") {
            oWerror = true;
        } else if (*i == "-l") {
            licence();
            return EXIT_SUCCESS;
        } else
            if (oInputFile=="")
                oInputFile=*i;
            else {
                std::cerr<<"Error: Unknown option '"<<*i<<"'. (Can only specify one input file?)";
                usage();
                return EXIT_FAILURE;
            }
    }

    /* Determine output filename */
    if (oOutputFile == "") {
        if (oInputFile == "" || oInputFile == "-")
            oOutputFile = "mscgen_out";
        else {
            oOutputFile = oInputFile;
            size_t dot=oInputFile.find_last_of('.');
            size_t dash=oInputFile.find_last_of("/\\");
            //Remove extension, if any and not only an extension
            if (dot!=oInputFile.npos && dot!=0 &&
                (dash==oInputFile.npos || dash<dot))
                oOutputFile.erase(dot);
            //oOutputFile.erase(oOutputFile.find_last_of("."));
	    switch (oOutType) {
            case MscDrawer::PNG:
    	        oOutputFile.append(".png"); break;
            case MscDrawer::EPS:
    	        oOutputFile.append(".eps"); break;
            case MscDrawer::PDF:
    	        oOutputFile.append(".pdf"); break;
            case MscDrawer::SVG:
    	        oOutputFile.append(".svg"); break;
            default:
                assert(0);
            }
        }
    }

    if (msc.Error.hasErrors()) {
        //Problem with switches
        std::cerr << msc.Error.Print(oWarning);
        return EXIT_FAILURE;
    }

    char *input;
    unsigned length;

    /* Parse input, either from a file, or stdin */
    if (oInputFile == "" || oInputFile == "-") {
        length = ReadFile(&input, stdin);
    } else {
        FILE *in = fopen(oInputFile.c_str(), "r");

        if(!in) {
            std::cerr<< "Error: Failed to open input file '" << oInputFile << "'";
            return EXIT_FAILURE;
        }
        length = ReadFile(&input, in);
        fclose(in);
    }

    //Load deisgn definitions
    msc.ParseText(designs, sizeof(designs)-1, "[designlib]");
    //parse input text;
    msc.ParseText(input, length, oInputFile.c_str());
    msc.CompleteParse(oOutType);
    std::cerr << msc.Error.Print(oWarning);
    //Now cycle through pages and write them to individual files
    msc.DrawToOutput(oOutType, oOutputFile);
    std::cerr << "Success." << std::endl;
    return EXIT_SUCCESS;
}
