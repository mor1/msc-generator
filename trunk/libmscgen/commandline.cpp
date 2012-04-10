/*
    This file is part of Msc-generator.
    Copyright 2008,2009,2010,2011,2012 Zoltan Turanyi
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
#include <limits>
#include "commandline.h"
#include "msc.h"

const char *VersionText(char a, char b, char c)
{
    static char buff[20];
    if (c<0) c=0;
    if (b<0) b=0;
    if (a<0) a=0;
    if (c!=0)
        sprintf(buff, "v%d.%d.%d", unsigned(a), unsigned(b), unsigned(c));
    else
        sprintf(buff, "v%d.%d", unsigned(a), unsigned(b));
    return buff;
}


char *ReadFile(FILE *in)
{
    if (!in) return NULL;
    size_t alloc = 16384;
    size_t len = 0;
    char *buff = (char*)malloc(alloc);
    if (buff == NULL) return NULL;
    while (!feof(in)) {
        len += fread(buff+len, 1, alloc-1-len, in);
        char *buff2;
        if (len == alloc-1) {
            buff2 = (char*)realloc(buff, alloc+=16384);
            if (buff2 == NULL) {
                free(buff);
                return NULL;
            }
            buff = buff2;
        }
    }
    buff[len]=0;
    return buff;
}


/** Print program usage and return.
 */
static void usage()
{
    printf(
"Usage: msc-gen [-T <type>] [-o <file>] [<infile>] [-Wno]\n"
"               [--pedantic] [[-x=<width>] [-y=<height>] | [-s=<scale>]\n"
"               [--<chart_option>=<value> ...] [--<chart_design>]\n"
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
" --pedantic  When used, all entities must be declared before being used.\n"
" -x=<width>  Specifies chart width (in pixels).\n"
" -y=<height> Specifies chart height (in pixels). If only one of -x or -y\n"
"             is specified, the aspect ratio is kept.\n"
" -s=<scale>  Can be used to scale chart size up or down. Default is 1.0.\n"
"             Cannot be used together with any of -x or -y.\n"
" --<chart_option>=<value>\n"
"             These options will be evaluated before the input file. Any value\n"
"             here will be overwritten by a conflicting option in the file.\n"
" --<chart_design>\n"
"             Any chart design can be specified here, taking precedence over the\n"
"             design specified in the input file.\n"
" -l          Display program licence and exit.\n"
" -h          Display this help and exit.\n"
"\n"
"Msc-generator version %s, Copyright (C) 2008-9 Zoltan Turanyi,\n"
"Msc-generator comes with ABSOLUTELY NO WARRANTY.\n"
"This is free software, and you are welcome to redistribute it under certain\n"
"conditions; type `mscgen -l' for details.\n",
VersionText(LIBMSCGEN_MAJOR, LIBMSCGEN_MINOR, LIBMSCGEN_SUPERMINOR));
}

/** Print program licence and return.
 */
static void licence()
{
    printf(
"Msc-generator, a message sequence chart renderer.\n"
"This file is part of Msc-generator.\n"
"Copyright 2008,2009,2010,2011,2012 Zoltan Turanyi\n"
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

int do_main(const std::list<std::string> &args, const char *designs,
            string csh_textformat)
{
    MscCanvas::OutputType oOutType=MscCanvas::PNG;
    string                oOutputFile;
    string                oInputFile;
    bool                  oWarning = true;
    bool                  oCshize = false;
    int                   oX = -1;
    int                   oY = -1;
    double                oScale = 0;
    string ss;

    Msc msc;
    msc.copyrightText = "\\md(0)\\mu(2)\\mr(0)\\mn(10)\\f(arial)\\pr\\c(150,150,150)"
                        "http://msc-generator.sourceforge.net ";
    msc.copyrightText.append(VersionText(LIBMSCGEN_MAJOR, LIBMSCGEN_MINOR, LIBMSCGEN_SUPERMINOR));

    msc.Error.AddFile("[options]");
    const file_line opt_pos(msc.current_file,0,0);
    const file_line_range opt_pos_range(opt_pos, opt_pos);
    bool show_usage = false;
    bool fail_options = false;

    //Load deisgn definitions
    if (designs)
        msc.ParseText(designs, "[designlib]");

    for (std::list<std::string>::const_iterator i=args.begin(); i!=args.end(); i++) {
        if (i->at(0) == '-' && i->at(1) == 'x') {
            if (i->at(2) != '=' || sscanf(i->c_str()+3, "%d", &oX)!=1)
                msc.Error.Error(opt_pos, "Missing size after '-x='. Using native size.");
            else if (oX<10 || oX>200000) {
                msc.Error.Error(opt_pos, "Invalid x size, it should be between [10..200000]. Using native size.");
                oX = -1;
            }
        } else if (i->at(0) == '-' && i->at(1) == 'y') {
            if (i->at(2) != '=' || sscanf(i->c_str()+3, "%d", &oY)!=1)
                msc.Error.Error(opt_pos, "Missing size after '-y='. Using native size.");
            else if (oY<10 || oY>200000) {
                msc.Error.Error(opt_pos, "Invalid y size, it should be between [10..200000]. Using native size.");
                oY = -1;
            }
        } else if (i->at(0) == '-' && i->at(1) == 's') {
            if (i->at(2) != '=' || sscanf(i->c_str()+3, "%lf", &oScale)!=1)
                msc.Error.Error(opt_pos, "Missing scale after '-s='. Using native size.");
            else if (oScale<=0.001 || oScale>100) {
                msc.Error.Error(opt_pos, "Invalid scale, it should be between [0.001..100]. Ignoring it.");
                oScale = 0;
            }
        } else if (*i == "-o") {
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
                oOutType = MscCanvas::PNG;
                show_usage = true;
            } else {
                i++;
                if (*i == "csh")
                    oCshize = true;
                else
#ifdef CAIRO_HAS_PNG_FUNCTIONS
                if (*i == "png")
                    oOutType = MscCanvas::PNG;
                else
#endif
#ifdef CAIRO_HAS_PS_SURFACE
                 if (*i == "eps")
                     oOutType = MscCanvas::EPS;
                 else
#endif
#ifdef CAIRO_HAS_PDF_SURFACE
                 if (*i == "pdf")
                     oOutType = MscCanvas::PDF;
                 else
#endif
#ifdef CAIRO_HAS_SVG_SURFACE
                 if (*i == "svg")
                     oOutType = MscCanvas::SVG;
                 else
#endif
#ifdef CAIRO_HAS_WIN32_SURFACE
                 if (*i == "emf")
                     oOutType = MscCanvas::EMF;
                 else
                 if (*i == "wmf")  //undocumented
                     oOutType = MscCanvas::WMF;
                 else
#endif
                 {
                     msc.Error.Error(opt_pos,
                                     "Unknown output format '" + *i + "'."
                                     "Use one of"
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
                     " 'emf'"
#endif
                     " or 'csh'. Using 'png'.");
                     oOutType = MscCanvas::PNG;
                 }
            }
        } else if (*i == "-l") {
            licence();
            return EXIT_SUCCESS;
        } else if (*i == "-h") {
            usage();
            return EXIT_SUCCESS;
        } else if (*i == "-Wno") {
            oWarning = false;
        } else if (*i == "--nocopyright") {
            msc.copyrightText.clear();
        } else if (*i == "--pedantic") {
            msc.pedantic = true;
        } else if (i->substr(0,13) == "--csh_format=") {
            csh_textformat += i->substr(13);
        } else if ((*i)[0]=='-' && (*i)[1]=='-') { //starts with "--"
            string name(i->substr(2));
            if (name.find("=") == name.npos) {
                //no "=" in switch
                Attribute a(name.c_str(), true, opt_pos_range, opt_pos_range, "yes");
                a.error = true;  //supress errors in AddAttribute
                if (msc.AddAttribute(a)) continue;
                switch (msc.SetDesign(true, a.name, true)) {
                case 0:
                    msc.ignore_designs = true;
                    break;
                case 1:
                    msc.Error.Error(opt_pos,
                                    "Unknown chart design: '" + a.name + "'. Using 'plain'.",
                                    " Available designs are: " + msc.GetDesigns(true) +".");
                    break;
                case 2:
                    msc.Error.Warning(a, true, "Use of '+=' to set a full design.", "Use 'msc = " + a.value + "' to suppress this warning.");
                    break;
                case 3:
                    msc.Error.Warning(a, true, "Use of '=' to apply a partial design.", "Use 'msc += " + a.value + "' to suppress this warning.");
                    break;
                default:
                    _ASSERT(0);
                    break;
                }
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
                                               opt_pos_range));
            }
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
        if (oCshize)
            oOutputFile.append(".csh");
        else switch (oOutType) {
        case MscCanvas::PNG:
            oOutputFile.append(".png"); break;
        case MscCanvas::EPS:
            oOutputFile.append(".eps"); break;
        case MscCanvas::PDF:
            oOutputFile.append(".pdf"); break;
        case MscCanvas::SVG:
            oOutputFile.append(".svg"); break;
        case MscCanvas::EMF:
            oOutputFile.append(".emf"); break;
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
    //Replace chart text with the cshized version of it
    if (oCshize) {
        MscInitializeCshAppearanceList();
        Csh csh;
        if (strlen(input)>std::numeric_limits<string::size_type>::max()-10)  {
            std::cerr << "Error: Msc-generator cannot handle files longer than " << std::numeric_limits<string::size_type>::max()-10 << ".";
            std::cerr << "The input '" << oInputFile << "' is longer than this (" << strlen(input) << ").";
            std::cerr << "Bailing out." << std::endl;
            return EXIT_FAILURE;
        }
        csh.ParseText(input, (unsigned)strlen(input), -1, 1);
        string tmp = Cshize(input, (unsigned)strlen(input), csh.CshList, 1, csh_textformat.c_str());
        FILE *out = fopen(oOutputFile.c_str(), "w");
        if (!out) {
            std::cerr<< "Error: Failed to open output file '" << oOutputFile << "'.\n";
            std::cerr << "Bailing out." << std::endl;
            return EXIT_FAILURE;
        }
        fwrite(tmp.c_str(), 1, tmp.length(), out);
        fclose(out);
    } else {
        //parse input text;
        msc.ParseText(input, oInputFile.c_str());
        msc.CompleteParse(oOutType, true);

        //Determine scaling
        XY scale(1., 1.);
        if (oX>0 || oY>0) {
            if (oScale)
                msc.Error.Error(opt_pos, "Conflicting scaing options. Use either -s or one/both of -x/-y. Using no scaling.");
            else if (oX>0 && oY>0) {
                scale.x = double(oX)/double(msc.GetTotal().x);
                scale.y = double(oY)/double(msc.GetTotal().y);
            } else if (oX>0)
                scale.x = scale.y = double(oX)/double(msc.GetTotal().x);
            else if (oY>0)
                scale.x = scale.y = double(oY)/double(msc.GetTotal().y);
        } else if (oScale)
            scale.x = scale.y = oScale;

        std::cerr << msc.Error.Print(oWarning);
        //Now cycle through pages and write them to individual files
        msc.DrawToOutput(oOutType, scale, oOutputFile, false);
    }

    free(input);

    std::cerr << "Success." << std::endl;
    if (show_usage) usage();
    return EXIT_SUCCESS;
}
