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

char *ReadFile(FILE *in)
{
	if (!in) return NULL;
    unsigned alloc = 16384;
    unsigned len = 0;
    char *buff = (char*)malloc(alloc);
    while (!feof(in)) {
        len += fread(buff+len, 1, alloc-1-len, in);
        if (len == alloc-1)
            buff = (char*)realloc(buff, alloc+=16384);
    }
    buff[len]=0;
    return buff;
}


/** Print program usage and return.
 */
static void usage()
{
    printf(
"Usage: msc-gen [-T <type>] [-o <file>] [<infile>] [-Wno] [-Werror]\n"
"               [--pedantic] [--strict] [<chart_option>=<value> ...]\n"
"       msc-gen -l\n"
"\n"
"Where:\n"
" -T <type>   Specifies the output file type, which maybe one of 'png', 'eps',\n"
"             'pdf', 'svg' or 'wmf' (if supported on your system).\n" 
"             Default is 'png'.\n"
" -o <file>   Write output to the named file.  If omitted, the input filename\n"
"             will be appended by an extension suitable for the output format.\n"
"             If neither input nor output file is given, msc-gen_out.* will be\n"
"             used.\n"
" <infile>    The file from which to read input.  If omitted or specified as\n"
"             '-', input will be read from stdin.\n"
" -Wno        No warnings displayed.\n"
" -Werror     Warnings result in errors and no output.\n"
" --pedantic  When used, all entities must be declared before being used.\n"
" --strict    Requires strict adherence to syntax rules, will bail out on errors.\n"
" --<chart_design>\n"
"             Any chart design can be specified here, taking precedence over the\n"
"             design specified in the input file.\n"
" -l          Display program licence and exit.\n"
"\n"
"Msc-generator version %s, Copyright (C) 2008-9 Zoltan Turanyi,\n"
"Msc-generator comes with ABSOLUTELY NO WARRANTY.\n" 
"This is free software, and you are welcome to redistribute it under certain\n" 
"conditions; type `mscgen -l' for details.\n",
VERSION);
}

/** Print program licence and return.
 */
static void licence()
{
    printf(
"Msc-generator, a message sequence chart renderer.\n"
"This file is part of Msc-generator.\n"
"Copyright 2008,2009,2010 Zoltan Turanyi\n"
"Distributed under GNU Affero General Public License.\n"
"\n"
"Msc-generator is free software: you can redistribute it and/or modify\n"
"it under the terms of the GNU Affero General Public License as published by\n"
"the Free Software Foundation, either version 3 of the License, or\n"
"(at your option) any later version.\n"
"\n"
"Msc-generator is distributed in the hope that it will be useful,\n"
"but WITHOUT ANY WARRANTY; without even the implied warranty of\n"
"MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n"
"GNU Affero General Public License for more details.\n"
"\n"
"You should have received a copy of the GNU Affero General Public License\n"
"along with Msc-generator.  If not, see <http://www.gnu.org/licenses/>.\n");
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
    msc.copyrightText = "\\md(0)\\mu(2)\\mr(0)\\mn(10)\\f(arial)\\pr\\c(150,150,150)"
                        "http://msc-generator.sourceforge.net ";
	msc.copyrightText.append(VERSION);
	
	msc.Error.AddFile("[options]");
    const file_line opt_pos(msc.current_file,0,0);
    const file_line_range opt_pos_range(opt_pos, opt_pos);
    bool show_usage = false;
    bool fail_options = false;

	//Load deisgn definitions
	if (designs)
		msc.ParseText(designs, "[designlib]");

    for (std::list<std::string>::const_iterator i=args.begin(); i!=args.end(); i++) {
        if (*i == "-o") {
            if (i==--args.end()) {
                msc.Error.Error(opt_pos,
                                "Missing output filename after '-o'.");
                show_usage = true;
                fail_options = true;
            } else
                oOutputFile = *(++i);
        } else if (*i == "-T") {
            if (i==--args.end()) {
                msc.Error.Error(opt_pos,
                                "Missing output type after '-T'.",
                                "Assuming 'png'.");
                oOutType = MscDrawer::PNG;
                show_usage = true;
            } else {
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
                     msc.Error.Error(opt_pos,
                                     "Unknown output format '" + *i + "'."
                                     "Use one of "
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
                     ". Using 'png'.");
                     oOutType = MscDrawer::PNG;
                 }
            }
        } else if ((*i)[0]=='-' && (*i)[1]=='-') { //starts with "--"
            string name(i->substr(2));
            if (name.find("=") == name.npos) {
                //no "=" in switch
                Attribute a(name.c_str(), true, opt_pos_range, opt_pos_range, "yes");
                a.error = true;  //supress errors in AddAttribute
                if (msc.AddAttribute(a)) continue;
                if (!msc.SetDesign(a.name, true))
                    msc.Error.Error(opt_pos,
                                    "Unknown chart design: '" + a.name + "'. Using 'plain'.",
                                    " Available styles are: " + msc.GetDesigns() +".");
				else
					msc.ignore_designs = true;
            } else {
                float num;
                string value(name.substr(name.find("=")+1));
                name.erase(name.find("="));
                if (value == "yes" || name == "Yes" || name == "YES")
                    msc.AddAttribute(Attribute(name.c_str(), true, opt_pos_range,
                                               opt_pos_range, value.c_str()));
                else if(value == "no" || name == "No" || name == "NO")
                    msc.AddAttribute(Attribute(name.c_str(), false, opt_pos_range,
                                               opt_pos_range, value.c_str()));
                else if (sscanf(value.c_str(), "%f", &num) == 1)
                    msc.AddAttribute(Attribute(name.c_str(), num, opt_pos_range,
                                               opt_pos_range, value.c_str()));
                else
                    msc.AddAttribute(Attribute(name.c_str(), value.c_str(), opt_pos_range,
                                               opt_pos_range, name.c_str()));
            }
        } else if (*i == "-l") {
            licence();
            return EXIT_SUCCESS;
        } else if (*i == "-Wno") {
            oWarning = false;
        } else if (*i == "-Werror") {
            oWerror = true;
        } else
            if (oInputFile=="")
                oInputFile=*i;
            else {
                if ((*i)[0] == '-')
                    msc.Error.Error(opt_pos,
                                    "Unknown option '" + *i + "'. Ignoring it.");
                else
                    msc.Error.Error(opt_pos,
                                    "Unknown option '" + *i + "'. Ignoring it.",
                                    "Can only specify one input file.");
                show_usage = true;
            }
    }

    /* Determine output filename */
    if (oOutputFile == "") {
        if (oInputFile == "" || oInputFile == "-")
            oOutputFile = "mscgen_out";
        else
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

    if (fail_options) {
        //Problem with switches
        std::cerr << msc.Error.Print(oWarning);
        if (show_usage) usage();
        std::cerr << "Bailing out." << std::endl;
        return EXIT_FAILURE;
    }

    char *input;

    /* Parse input, either from a file, or stdin */
    if (oInputFile == "" || oInputFile == "-") {
        input = ReadFile(stdin);
    } else {
        FILE *in = fopen(oInputFile.c_str(), "r");

        if(!in) {
            std::cerr<< "Error: Failed to open input file '" << oInputFile << "'.\n";
            std::cerr << "Bailing out." << std::endl;
            return EXIT_FAILURE;
        }
        input = ReadFile(in);
        fclose(in);
    }

    //parse input text;
    msc.ParseText(input, oInputFile.c_str());
    free(input);
    msc.CompleteParse(oOutType, true);
    std::cerr << msc.Error.Print(oWarning);
    //Now cycle through pages and write them to individual files
    msc.DrawToOutput(oOutType, oOutputFile);

    std::cerr << "Success." << std::endl;
    if (show_usage) usage();
    return EXIT_SUCCESS;
}
