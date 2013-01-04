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
        sprintf(buff, "v%u.%u.%u", unsigned(a), unsigned(b), unsigned(c));
    else
        sprintf(buff, "v%u.%u", unsigned(a), unsigned(b));
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
"Usage: msc-gen [-T <type>] [-o <file>] [<infile>] [-Wno] [--pedantic]\n"
"               [-p[=<page size>] [-m{lrud}=<margin>]]\n"
"               [-x=<width>] [-y=<height>] [-s=<scale>]\n"
"               [--<chart_option>=<value> ...] [--<chart_design>]\n"
"       msc-gen -l\n"
"\n"
"Where:\n"
" -T <type>   Specifies the output file type, which maybe one of 'png', 'eps',\n"
"             'pdf', 'svg' or 'emf' (if supported on your system).\n"
"             Default is 'png'.\n"
" -o <file>   Write output to the named file.  If omitted, the input filename\n"
"             will be appended by an extension suitable for the output format.\n"
"             If neither input nor output file is given, msc-gen_out.* will be\n"
"             used.\n"
" <infile>    The file from which to read input.  If omitted or specified as\n"
"             '-', input will be read from stdin.\n"
" -p=[<page size]\n"
"             Full-page output. (PDF only now.) In this case the chart is \n"
"             drawn on fixed-size pages (following pagination) with one pixel\n"
"             equalling to 1/72 inches. If a chart page is larger than a physcal\n"
"             page it is simply cropped with a warning. Setting the scale with\n"
"             the -s option enables zooming. Page size can be set to ISO sizes\n"
"             from A0 to A6, and to US sizes, such as letter, legal, ledger and\n"
"             tabloid. Append a 'p' or an 'l' for portrait and landscape,\n"
"             respectively (except for 'tabloid' and 'ledger', which are\n"
"             by definition portrait and landscape, resp.). E.g., use 'A4p',\n"
"             'A2l' or 'letter_l'. Deafult is 'A4p'.\n"
" -m{lrud}=<margin>\n"
"             Useful only for full-page output, specifies the margin.\n"
"             A separate option is needed to specify the left, right, upwards\n"
"             and downwards margins, denoted by the second letter of the option.\n"
"             Margins are to be specified in inches (number only) or in\n"
"             centimeters, if appended with 'cm' (no spaces). The default\n"
"             margin is half inches everywhere.\n"
" -va=<center|up|down>\n"
" -ha=<center|left|right>\n"
"             Set the vertical and horizontal alignment within a page for full-\n"
"             page output.\n"
" -Wno        No warnings displayed.\n"
" --pedantic  When used, all entities must be declared before being used.\n"
" -x=<width>  Specifies chart width (in pixels). Only for PNG output.\n"
" -y=<height> Specifies chart height (in pixels). If only one of -x or -y\n"
"             is specified, the aspect ratio is kept. Only for PNG output.\n"
" -s=<scale>  Can be used to scale chart size up or down. Default is 1.0.\n"
"             Cannot be used together with any of -x or -y.\n"
"             Only for PNG or full-page output (-p).\n" 
"             For full-page output, you can set <scale> to 'width' which\n"
"             results in the chart width being set to the page width, or\n"
"             'auto', which scales such that all pages fits. For full-page\n"
"             output, you can specify multiple -s options, which makes\n"
"             msc-gen to try them in the order specified until one is\n"
"             found for which no pages need to be cropped. If none is\n"
"             such, the last one will be used and a warning will be given.\n"
" --<chart_option>=<value>\n"
"             These options will be evaluated before the input file. Any value\n"
"             here will be overwritten by a conflicting option in the file.\n"
" --<chart_design>\n"
"             Any chart design can be specified here, taking precedence over the\n"
"             design specified in the input file.\n"
" -l          Display program licence and exit.\n"
" -h          Display this help and exit.\n"
"\n"
"Msc-generator version %s, Copyright (C) 2008-2013 Zoltan Turanyi,\n"
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
"Copyright 2008,2009,2010,2011,2012,2013 Zoltan Turanyi\n"
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
    std::vector<double>   oScale; //-2=auto, -1=width, other values = given scale
    MscCanvas::EPageSize  oPageSize = MscCanvas::NO_PAGE;
    int                   oVA = -2, oHA =-2; //-1=left/top, 0=center, +1=right/bottom (-2==not set)
    double                margins[] = {36, 36, 36, 36}; // half inches everywhere
    string ss;

    Msc msc;
    msc.copyrightText = "\\md(0)\\mu(2)\\mr(0)\\mn(10)\\f(arial)\\pr\\c(150,150,150)"
                        "http://msc-generator.sourceforge.net ";
    msc.copyrightText.append(VersionText(LIBMSCGEN_MAJOR, LIBMSCGEN_MINOR, LIBMSCGEN_SUPERMINOR));

    msc.Error.AddFile("[options]");
    const file_line opt_pos(msc.current_file,0,0);
    const file_line_range opt_pos_range(opt_pos, opt_pos);
    bool show_usage = false;

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
            double os;
            if (i->at(2) != '=' || 
                (sscanf(i->c_str()+3, "%lf", &os)!=1 && tolower(i->at(3)) != 'a' && tolower(i->at(3)) != 'w'))
                msc.Error.Error(opt_pos, "Missing scale after '-s='. Using native size.");
            else if (tolower(i->at(3)) == 'a')
                oScale.push_back(-2); //auto
            else if (tolower(i->at(3)) == 'w')
                oScale.push_back(-1); //width
            else if (os<=0.001 || os>100) 
                msc.Error.Error(opt_pos, "Invalid scale, it should be between [0.001..100]. Ignoring it.");
            else 
                oScale.push_back(os); 
        } else if (i->at(0) == '-' && i->at(1) == 'p') {
            if (i->length()==2) 
                oPageSize = MscCanvas::A4P;
            else 
                oPageSize = i->at(2)=='=' ? MscCanvas::ConvertPageSize(i->c_str()+3) : MscCanvas::NO_PAGE;
            if (oPageSize == MscCanvas::NO_PAGE) {
                msc.Error.Error(opt_pos, "Invalid page size. Should be one of the ISO A-series, such as 'A4p' or 'A3l', or 'letter', 'legal', 'ledger' or 'tabloid'. Using 'A4p' as default.");
                oPageSize = MscCanvas::A4P;
            }
        } else if (i->at(0) == '-' && (i->at(1) == 'v' || i->at(1) == 'h') && 
                   i->at(2) == 'a') {
            if (i->length()<5 || i->at(3) != '=')
                msc.Error.Error(opt_pos, "I need a value for " + i->substr(0,3) + ". Ignoring this.");
            else {
                static const char h[] = "lcr", v[] = "ucd";
                static const char h_err[] = "left/center/right", v_err[] = "up/center/down";
                const char * const at = strchr(i->at(1)=='h' ? h : v, tolower(i->at(4)));
                if (at==NULL)
                    msc.Error.Error(opt_pos, "Bad value for "+ i->substr(0,3) + ". Use one of " + 
                                   (i->at(1)=='h' ? h_err : v_err) + ". Ignoring this.");
                else
                    (i->at(1)=='h' ? oHA : oVA) = int(at - (i->at(1)=='h' ? h : v)) - 1;
            }
        } else if (i->at(0) == '-' && i->at(1) == 'm') {
            static const char *dirs = "lrud";
            const char *at = strchr(dirs, tolower(i->at(2)));
            if (at==NULL) 
                msc.Error.Error(opt_pos, "Invalid margin selector. Use one of '-ml', '-mr', '-mu' or '-md' for left, right, upper or lower margins, respectively.  Ignoring this.");
            else if (i->at(3) != '=')
                msc.Error.Error(opt_pos, "After the marging option, I need a number with no spaces, like '-mu=1.2cm'. Ignoring this.");
            else {
                double mul = 72; //pixels per unit. Default is inch.
                char *buff = (char*)malloc(i->length());
                buff[0] = 0;
                double res;
                if (sscanf(i->c_str()+4, "%lf%s", &res, buff)==0)
                    msc.Error.Error(opt_pos, "After the marging option, I need a number with no spaces, like '-mu=1.2cm'. Ignoring this.");            
                else {
                    if (!strcmp("cm", buff)) mul=28.3464567; //cm to points, see http://www.asknumbers.com/CentimetersToPointsConversion.aspx
                    else if (buff[0] && strcmp("in", buff)) 
                        msc.Error.Error(opt_pos, "After margin size value, specify the measurement unit: 'in' or 'cm'. Assuming inches.");            
                    free(buff);
                    margins[at-dirs] = res*mul;
                }
            }
        } else if (*i == "-o") {
            if (i==--args.end()) {
                msc.Error.FatalError(opt_pos, "Missing output filename after '-o'.");
                show_usage = true;
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
                ArcBase *ret;
                switch (msc.SetDesign(true, a.name, true, &ret)) { 
                case 1:
                case 2:
                case 3:
                    msc.ignore_designs = true;
                    break;
                case 0:
                    msc.Error.Error(opt_pos,
                                    "Unknown chart design: '" + a.name + "'. Using 'plain'.",
                                    " Available designs are: " + msc.GetDesigns(true) +".");
                    break;
                default:
                    _ASSERT(0);
                    break;
                }
                msc.Arcs.Append(ret);
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

    //Determine option compatibility
    if (oPageSize!=MscCanvas::NO_PAGE && oOutType != MscCanvas::PDF) {
        msc.Error.Error(opt_pos, "-p can only be used with PDF output. Ignoring it.");
        oPageSize = MscCanvas::NO_PAGE;
    }
    bool has_auto_or_width = false;
    for (unsigned s=0; s<oScale.size() && !has_auto_or_width; s++)
        has_auto_or_width = oScale[s]<=0;
    if (has_auto_or_width && oPageSize == MscCanvas::NO_PAGE) {
        msc.Error.Error(opt_pos, "-s=auto and -s=width can only be used with full-page output. Using default scale of 1.0.");
        oScale.resize(1);
        oScale[0] = 1;
    }
    if (oScale.size()>1 && oPageSize == MscCanvas::NO_PAGE) {
        msc.Error.Error(opt_pos, "Multiple -s options can only be applied to full-page output. Using default scale of 1.0.");
        oScale.resize(1);
        oScale[0] = 1;
    }
    if (oPageSize!=MscCanvas::NO_PAGE && oHA!=-2) {
        msc.Error.Error(opt_pos, "-ha can only be used with full-page outputoutput. Ignoring it.");
        oHA = -1;
    }
    if (oPageSize!=MscCanvas::NO_PAGE && oVA!=-2) {
        msc.Error.Error(opt_pos, "-va can only be used with full-page output. Ignoring it.");
        oVA = -1;
    }

    if (msc.Error.hasFatal()) goto fatal;

    /* Parse input, either from a file, or stdin */
    char *input;
    if (oInputFile == "" || oInputFile == "-") {
        input = ReadFile(stdin);
    } else {
        FILE *in = fopen(oInputFile.c_str(), "r");

        if(in) {
            input = ReadFile(in);
            fclose(in);
        } else
            msc.Error.FatalError(opt_pos, "Failed to open input file '" + oInputFile +"'.");
    }
    if (msc.Error.hasFatal()) goto fatal;

    if (oCshize) {
        //Replace chart text with the cshized version of it
        MscInitializeCshAppearanceList();
        Csh csh;
        if (strlen(input)>std::numeric_limits<string::size_type>::max()-10) {
            string err = "The input '";
            err << oInputFile << "' is too long (" << strlen(input) << " bytes).";
            msc.Error.FatalError(opt_pos, err);
            err = "Error: Msc-generator cannot handle files longer than ";
            err << std::numeric_limits<string::size_type>::max()-10 << ".";
            msc.Error.Error(opt_pos, opt_pos, err);
        } else {
            csh.ParseText(input, (unsigned)strlen(input), -1, 1);
            string tmp = Cshize(input, (unsigned)strlen(input), csh.CshList, 1, csh_textformat.c_str());
            FILE *out = fopen(oOutputFile.c_str(), "w");
            if (out) {
                fwrite(tmp.c_str(), 1, tmp.length(), out);
                fclose(out);
            } else
                msc.Error.FatalError(opt_pos, "Failed to open input file '" + oOutputFile +"'.");
        }
    } else {
        //parse input text;
        msc.ParseText(input, oInputFile.c_str());
        msc.CompleteParse(oOutType, true);

        //Determine scaling
        std::vector<XY> scale(std::max(1U, oScale.size()), XY(1., 1.));
        if (oX>0 || oY>0) {
            if (oScale.size())
                msc.Error.Error(opt_pos, "Conflicting scaing options. Use either -s or one/both of -x/-y. Using no scaling.");
            else if (oX>0 && oY>0) {
                scale[0].x = double(oX)/double(msc.GetTotal().x.Spans());
                scale[0].y = double(oY)/double(msc.GetTotal().y.Spans());
            } else if (oX>0)
                scale[0].x = scale[0].y = double(oX)/double(msc.GetTotal().x.Spans());
            else if (oY>0)
                scale[0].x = scale[0].y = double(oY)/double(msc.GetTotal().y.Spans());
        } else if (oScale.size()==1)  //one specified
            scale[0].x = scale[0].y = oScale[0];
        else if (oScale.size()>1) { //multiple specified
            if (oPageSize == MscCanvas::NO_PAGE) {
                for (unsigned u = 0; u<oScale.size(); u++)
                    if (oScale[u]>0) {
                        oScale[0] = oScale[u];
                        break;
                    }
                oScale.resize(1);
                string s = "Multiple -s options are valid for full-page output (-p). Using scale of ";
                s << oScale[0] << ".";
                msc.Error.Error(opt_pos, s);
            } else
                for (unsigned u = 0; u<oScale.size(); u++)
                    scale[u].x = scale[u].y = oScale[u];
        }

        //Now cycle through pages and write them to individual files or a full-page  one
        msc.DrawToOutput(oOutType, scale, oOutputFile, false, oPageSize, margins, oHA, oVA, true);
    }
    free(input);
    if (msc.Error.hasFatal()) goto fatal;

    std::cerr << msc.Error.Print(oWarning);
    if (show_usage) usage();
    if (msc.Error.hasErrors())
        std::cerr << "There were errors, but a chart has been produced." << std::endl;
    else
        std::cerr << "Success." << std::endl;
    return EXIT_SUCCESS;

fatal:
    //Serious problems
    std::cerr << msc.Error.Print(oWarning);
    if (show_usage) usage();
    std::cerr << "Bailing out." << std::endl;
    return EXIT_FAILURE;
}

