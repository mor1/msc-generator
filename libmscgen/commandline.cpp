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

/** @file commandline.cpp All that is needed to perform a command-line action.
 * @ingroup libmscgen */

#include <iostream>
#include <assert.h>
#include <limits>
#include "cairo.h"
#include "commandline.h"
#include "msc.h"

/** Returns "vX.Y.Z" based on a, b and c.*/
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


/** Reads a text file from `in`.*/
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

/** Print program version and return.
*/
static void version()
{
    printf(
"Msc-generator %s (using cairo %s)\n"
"\n"
"Copyright (C) 2008-2014 Zoltan Turanyi\n"
"Msc-generator comes with ABSOLUTELY NO WARRANTY.\n"
"This is free software, and you are welcome to redistribute it under certain\n"
"conditions; type `msc-gen -l' for details.\n",
VersionText(LIBMSCGEN_MAJOR, LIBMSCGEN_MINOR, LIBMSCGEN_SUPERMINOR),
cairo_version_string());
}
/** Print program usage and return.
 */
static void usage()
{
    printf(
"Msc-generator draws signalling charts from textual descriptions."
"\n"
"Usage: msc-gen [OPTIONS] [infile]"
"   or: msc-gen -l\n"
"   or: msc-gen --help\n"
"   or: msc-gen --version\n"
"\n"
"Options:\n"
" -T <type>   Specifies the output file type, which maybe one of 'png', 'eps',\n"
"             'pdf', 'svg' or 'emf' (if supported on your system).\n"
"             Default is 'png'. The token 'ismap' produces an NCSA formatted\n"
"             ismap file. The token 'lmap' is accepted and results\n"
"             a text file listing (the first line of) all labels and their\n"
"             page number and coordinates.\n"
" -o <file>   Write output to the named file.  If omitted, the input filename\n"
"             will be appended by an extension suitable for the output format.\n"
"             If neither input nor output file is given, msc-gen_out.* will be\n"
"             used. Specifying '-' will result in the out be written to\n"
"             standard output."
" <infile>    The file from which to read input.  If omitted or specified as\n"
"             '-', input will be read from stdin.\n"
" -i <infile> To retain compatibility with mscgen, this is an alternate way to\n"
"             specify the input file.\n"
" --force-mscgen\n"
"             Forces the chart to be interpreted in mscgen mode. Note that many\n"
"             Msc-generator attributes, commands and keywords are still\n"
"             recognized. This setting makes conflicting syntax be intrepreted\n"
"             as mscgen would do. Without this switch Msc-generator uses the\n"
"             mscgen mode only if the chart starts with the text 'msc {'.\n"
" --prevent-mscgen\n"
"             Prevents the chart to be interpreted in mscgen mode. Note that some\n"
"             mscgen attributes and symbols are still recognized. This setting\n"
"             makes conflicting syntax be intrepreted as Msc-generator would do.\n"
"             Without this switch Msc-generator uses the mscgen mode if the chart\n"
"             starts with the text 'msc {'.\n"
" -p[=<page size]\n"
"             Full-page output. (PDF only now.) In this case the chart is\n"
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
" -a[h]       Automatic pagination. Used only with full-page output. If\n"
"             specified, scale cannot be 'auto'. Specifying -ah will\n"
"             insert a heading after automatically inserted page breaks.\n"
" -Wno        No warnings displayed.\n"
" -Wno-mscgen Disables warnings for deprecated constructs kept only for\n"
"             backwards compatibility with mscgen. Has no effect with\n"
"             --force-mscgen, in that case no such warnings are emitted.\n"
" -Pno        No progress indicator displayed.\n"
" --pedantic  When used, all entities must be declared before being used.\n"
" -x=<width>  Specifies chart width (in pixels). Only for bitmap output.\n"
" -y=<height> Specifies chart height (in pixels). If only one of -x or -y\n"
"             is specified, the aspect ratio is kept. Only for bitmap output.\n"
" -s=<scale>  Can be used to scale chart size up or down. Default is 1.0.\n"
"             Cannot be used together with any of -x or -y.\n"
"             Only for bitmap or full-page output (-p).\n"
"             For full-page output, you can set <scale> to 'width' which\n"
"             results in the chart width being set to the page width, or\n"
"             'auto', which scales such that all pages fits. For full-page\n"
"             output, you can specify multiple -s options, which makes\n"
"             msc-gen to try them in the order specified until one is\n"
"             found for which no pages need to be cropped. If none is\n"
"             such, the last one will be used and a warning will be given.\n"
" -F <font>   Use specified font. This must be a font name available in the\n"
"             local system, and overrides the MSCGEN_FONT environment variable\n"
"             if that is also set. (On Linux\\Unix try fontconfig.)\n"
" --no-designs\n"
"             If you specify this no design files will be loaded (except the\n"
"             ones you specify with -D). This is useful to increase performance\n"
"             when you do not use them anyway."
" -D <design_file>\n"
"             Load file containing additional chart design definitions.\n"
"             You can have multiple of this option to load several design\n"
"             files, after the default ones. These files are loaded even if\n"
"             --no-designs is specified.\n"
" --<chart_option>=<value>\n"
"             These options will be evaluated before the input file. Any value\n"
"             here will be overwritten by a conflicting option in the file.\n"
" --<chart_design>\n"
"             Any chart design can be specified here, taking precedence over the\n"
"             design specified in the input file.\n"
" -l          Display program licence and exit.\n"
" -h          Display this help and exit.\n"
" --help      Display this help and exit.\n"
" --version   Display version information and exit.\n"
"\n");
}




/** Print program licence and return.
 */
static void licence()
{
    printf(
"Msc-generator, a message sequence chart renderer.\n"
"This file is part of Msc-generator.\n"
"Copyright 2008,2009,2010,2011,2012,2013,2014 Zoltan Turanyi\n"
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

/** Displays sizes of key objects, for debugging.*/
void sizes() 
{
    std::cout << "Element:" << sizeof(Element) <<std::endl;
    std::cout << "ArcBase:" << sizeof(ArcBase) <<std::endl;
    std::cout << "ArcLabelled:" << sizeof(ArcLabelled) <<std::endl;
    std::cout << "ArcDirArrow:" << sizeof(ArcDirArrow) <<std::endl;
    std::cout << "ArcSelfArrow:" << sizeof(ArcSelfArrow) <<std::endl;
    std::cout << "ArcBigArrow:" << sizeof(ArcBigArrow) <<std::endl;
    std::cout << "ArcBox:" << sizeof(ArcBox) <<std::endl;
    std::cout << "ArcBoxSeries:" << sizeof(ArcBoxSeries) <<std::endl;
    std::cout << "ArcPipe:" << sizeof(ArcPipe) <<std::endl;
    std::cout << "ArcPipeSeries:" << sizeof(ArcPipeSeries) <<std::endl;
    std::cout << "ArcDivider:" << sizeof(ArcDivider) <<std::endl;
    std::cout << "CommandEntity:" << sizeof(CommandEntity) <<std::endl;
    std::cout << "EntityApp:" << sizeof(EntityApp) <<std::endl;
    std::cout << "CommandNote:" << sizeof(CommandNote) <<std::endl;
    std::cout << "CommandNewpage:" << sizeof(CommandNewpage) <<std::endl;
    std::cout << "ArcParallel:" << sizeof(ArcParallel) <<std::endl;
    std::cout << "Style:" << sizeof(MscStyle) <<std::endl;
    std::cout << "ColorType:" << sizeof(ColorType) <<std::endl;
    std::cout << "Contour:" << sizeof(Contour) <<std::endl;
    std::cout << "SimpleContour:" << sizeof(contour::SimpleContour) <<std::endl;
    std::cout << "Block:" << sizeof(Block) <<std::endl;
}

/** The main routine to perform a command-line action.
 * Reads and writes input and output files. Errors are emitted to std::cerr.
 * @param [in] args The list of command-line arguments.
 * @param [in] design_files A list of <name,content> pairs of design library files,
 *                          which shall be parsed before the input file. Any error
 *                          occuring in them will be also reported.
 * @param [in] csh_textformat The text format to use for csh-ized processing.
 * @param [in] cb The callback to call regularly during processing to show progress.
 * @param [in] param The parameter to pass to the progress callback.
 * @param load_data Information on how much each processing step relatively takes.
 *                  Used to make progress display as much linear as possible.
 *                  It is also updated (refined) during processing, so the caller 
 *                  can write it out.*/
int do_main(const std::list<std::string> &args, 
            const std::list<std::pair<std::string, std::string>> &design_files, 
            string csh_textformat,
            MscProgress::ProgressCallback cb, void *param,
            std::string *load_data)
{
    Canvas::EOutputType     oOutType=Canvas::PNG;
    string                  oOutputFile;
    string                  oInputFile;
    bool                    oWarning = true;
    bool                    oProgress = true;
    bool                    oCshize = false;
    bool                    oLmap = false;
    int                     oX = -1;
    int                     oY = -1;
    bool                    oUseWidthAttr = true;
    std::vector<double>     oScale; //-2=auto, -1=width, other values = given scale
    PageSizeInfo::EPageSize oPageSize = PageSizeInfo::NO_PAGE;
    int                     oVA = -2, oHA =-2; //-1=left/top, 0=center, +1=right/bottom (-2==not set)
    double                  margins[] = {36, 36, 36, 36}; // half inches everywhere
    bool                    oA = false;
    bool                    oAH = false;
    bool                    oLoadDesigns = true;
    string                  oFont;
    string ss;

    const char * const font = getenv("MSCGEN_FONT");
    if (font) oFont = font;

    if (args.size()==1) {
        if (*args.begin() == "-l") {
            licence();
            return EXIT_SUCCESS;
        } else if (*args.begin() == "-h" || *args.begin() == "--help") {
            usage();
            return EXIT_SUCCESS;
        } else if (*args.begin() == "--version") {
            version();
            return EXIT_SUCCESS;
        }
    }
    //Check if we need a progress indicator or if designs are disabled
    for (auto &arg : args) 
        if (arg == "-Pno") 
            oProgress = false;
        else if (arg == "--nodesigns")
            oLoadDesigns = false;

    Msc msc;
    msc.prepare_for_tracking = false;
    msc.Progress.callback = oProgress ? cb : NULL;
    msc.Progress.data = param;
    if (load_data)
        msc.Progress.ReadLoadData(load_data->c_str());
    msc.copyrightText = "\\md(0)\\mu(2)\\mr(0)\\mn(10)\\pr\\c(150,150,150)"
                        "http://msc-generator.sourceforge.net ";
    msc.copyrightText.append(VersionText(LIBMSCGEN_MAJOR, LIBMSCGEN_MINOR, LIBMSCGEN_SUPERMINOR));

    msc.Error.AddFile("[options]");
    const FileLineCol opt_pos(msc.current_file,0,0);
    const FileLineColRange opt_pos_range(opt_pos, opt_pos);
    bool show_usage = false;

    msc.Progress.StartSection(MscProgress::PARSE);
    //Load deisgn definitions
    if (oLoadDesigns)
        for (const auto &p : design_files) 
            msc.ParseText(p.second.c_str(), p.first.c_str());

    for (std::list<std::string>::const_iterator i=args.begin(); i!=args.end(); i++) {
        if (i->length()>=2 && i->at(0) == '-' && i->at(1) == 'x') {
            if (i->at(2) != '=' || sscanf(i->c_str()+3, "%d", &oX)!=1)
                msc.Error.Error(opt_pos, "Missing size after '-x='. Using native size.");
            else if (oX<10 || oX>200000) {
                msc.Error.Error(opt_pos, "Invalid x size, it should be between [10..200000]. Using native size.");
                oX = -1;
            } else
                oUseWidthAttr = false;
        } else if (i->length()>=3 && i->at(0) == '-' && i->at(1) == 'y') {
            if (i->at(2) != '=' || sscanf(i->c_str()+3, "%d", &oY)!=1)
                msc.Error.Error(opt_pos, "Missing size after '-y='. Using native size.");
            else if (oY<10 || oY>200000) {
                msc.Error.Error(opt_pos, "Invalid y size, it should be between [10..200000]. Using native size.");
                oY = -1;
            } else 
                oUseWidthAttr = false;
        } else if (i->length()>=4 && i->at(0) == '-' && i->at(1) == 's') {
            double os;
            if (i->at(2) != '=' ||
                (sscanf(i->c_str()+3, "%lf", &os)!=1 && tolower(i->at(3)) != 'a' && tolower(i->at(3)) != 'w'))
                msc.Error.Error(opt_pos, "Missing scale after '-s='. Using native size.");
            else if (tolower(i->at(3)) == 'a') {
                oScale.push_back(-2); //auto
                oUseWidthAttr = false;
            } else if (tolower(i->at(3)) == 'w') {
                oScale.push_back(-1); //width
                oUseWidthAttr = false;
            } else if (os<=0.001 || os>100) {
                msc.Error.Error(opt_pos, "Invalid scale, it should be between [0.001..100]. Ignoring it.");
            } else {
                oScale.push_back(os);
                oUseWidthAttr = false;
            }
        } else if (i->length()>=2 && i->at(0) == '-' && i->at(1) == 'p') {
            if (i->length()==2)
                oPageSize = PageSizeInfo::A4P;
            else
                oPageSize = i->at(2)=='=' ? PageSizeInfo::ConvertPageSize(i->c_str()+3) : PageSizeInfo::NO_PAGE;
            if (oPageSize == PageSizeInfo::NO_PAGE) {
                msc.Error.Error(opt_pos, "Invalid page size. Should be one of the ISO A-series, such as 'A4p' or 'A3l', or 'letter', 'legal', 'ledger' or 'tabloid'. Using 'A4p' as default.");
                oPageSize = PageSizeInfo::A4P;
            } else 
                oUseWidthAttr = false;
        } else if (i->length()>=3 && i->at(0) == '-' && (i->at(1) == 'v' || i->at(1) == 'h') &&
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
        } else if (i->length()>=2 && i->at(0) == '-' && i->at(1) == 'm') {
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
                    if (!strcmp("cm", buff)) mul = PT_PER_CM;
                    else if (buff[0] && strcmp("in", buff))
                        msc.Error.Error(opt_pos, "After margin size value, specify the measurement unit: 'in' or 'cm'. Assuming inches.");
                    free(buff);
                    margins[at-dirs] = res*mul;
                }
            }
        } else if (*i == "--force-mscgen") {
            msc.mscgen_compat = EMscgenCompat::FORCE_MSCGEN;
        } else if (*i == "--prevent-mscgen") {
            msc.mscgen_compat = EMscgenCompat::NO_COMPAT;
        } else if (*i == "-Wno-mscgen") {
            msc.Error.warn_mscgen = false;
        } else if (*i == "-a") {
            oA = true; oAH = false;
        } else if (*i == "-ah") {
            oA = true; oAH=true;
        } else if (*i == "-o") {
            if (i==--args.end()) {
                msc.Error.FatalError(opt_pos, "Missing output filename after '-o'.");
                show_usage = true;
            } else
                oOutputFile = *(++i);
        } else if (*i == "-F") {
            if (++i==args.end()) {
                msc.Error.Error(opt_pos,
                    "Missing font name after '-F'.",
                    "Using " + (oFont.length() ? "'"+oFont+"' instead." : "default font."));
                show_usage = true;
            } else {
                //above we have already moved i to the parameter of -F
                string font_name = *i;
                if (Canvas::HasFontFace(font_name)) {
                    oFont = *i;
                } else if (font_name.length()) {
                    oFont = font_name;
                    msc.Error.Warning(opt_pos,
                        "Font '"+*i+"' is not available.",
                        "Using '"+oFont+"' instead.");
                } else {
                    msc.Error.Error(opt_pos,
                        "Font '"+*i+"' is not available. Ignoring option.",
                        "Using " + (oFont.length() ? "previously set '"+oFont+"' instead." : "default font."));
                }
            }
        } else if (*i == "-D") {
            if (i==--args.end()) {
                msc.Error.Error(opt_pos,
                    "Missing design file name after '-F'.");
                show_usage = true;
            } else {
                i++;
                if (oLoadDesigns) {
                    FILE *in = fopen(i->c_str(), "r");
                    if (in) {
                        char *buff = ReadFile(in);
                        if (buff) {
                            //parse immediately, so that later command line options 
                            //see the definitions here
                            msc.ParseText(buff, i->c_str());
                            free(buff);
                        }
                        fclose(in);
                    } else
                        msc.Error.FatalError(opt_pos, "Failed to open design file '" + oInputFile +"'.");
                }
            }
        } else if (i->length()>=2 && i->at(0)=='-' && i->at(1)=='T') { //begins with "-T"
            string format;
            if (i->length()>2)
                format = i->substr(2);
            else if (i==--args.end()) {
                msc.Error.Error(opt_pos,
                                "Missing output type after '-T'.",
                                "Assuming 'png'.");
                oOutType = Canvas::PNG;
                show_usage = true;
            } else {
                i++;
                format = *i;
            }
            if (format.length()) {
                if (format == "csh") {
                    oCshize = true;
                    oLmap = false;
                } else
#ifdef CAIRO_HAS_PNG_FUNCTIONS
                if (format == "png") {
                    oOutType = Canvas::PNG;
                    oCshize = oLmap = false;
                } else
#endif
#ifdef CAIRO_HAS_PS_SURFACE
                 if (format == "eps") {
                     oOutType = Canvas::EPS;
                     oCshize = oLmap = false;
                 } else
#endif
#ifdef CAIRO_HAS_PDF_SURFACE
                 if (format == "pdf") {
                     oOutType = Canvas::PDF;
                     oCshize = oLmap = false;
                 } else
#endif
#ifdef CAIRO_HAS_SVG_SURFACE
                 if (format == "svg") {
                     oOutType = Canvas::SVG;
                     oCshize = oLmap = false;
                 } else
#endif
#ifdef CAIRO_HAS_WIN32_SURFACE
                 if (format == "emf") {
                     oOutType = Canvas::EMF;
                     oCshize = oLmap = false;
                 } else if (format == "wmf")  {//undocumented
                     oOutType = Canvas::WMF;
                     oCshize = oLmap = false;
                 } else
#endif
                 if (format == "ismap") { //undocumented
                     oOutType = Canvas::ISMAP;
                     oCshize = oLmap = false;
                 } else if (format == "lmap") {
                     oLmap = true;
                     oCshize = false;
                     oOutType = Canvas::PDF; //use this format for laying out (-p also works with it)
                 } else {
                     msc.Error.Error(opt_pos,
                                     "Unknown output format '" + format + "'."
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
                     " 'lmap' or 'csh'. Using 'png'.");
                     oOutType = Canvas::PNG;
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
        } else if (*i == "-Pno") {
            oProgress = false;
        } else if (*i == "--nocopyright") {
            msc.copyrightText.clear();
        } else if (*i == "--nodesigns") {
            oLoadDesigns= false;
        } else if (*i == "--pedantic") {
            msc.pedantic = true;
        } else if (i->substr(0,13) == "--csh_format=") {
            csh_textformat += i->substr(13);
        } else if (i->length()>=2 && (*i)[0]=='-' && (*i)[1]=='-') { //starts with "--"
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
                                    " Available designs are: " + msc.GetDesignNames(true) +".");
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
        } else if (*i == "-i") {
            if (i==--args.end()) {
                if (oInputFile=="")
                    msc.Error.FatalError(opt_pos, "Missing input filename after '-i'.");
                else 
                    msc.Error.FatalError(opt_pos, "You have specified an input file, "
                                                  "ignoring '-i' at the end of command line.");
                show_usage = true;
            } else
                if (oInputFile=="")
                    oInputFile = *(++i);
                else {
                    msc.Error.Error(opt_pos, "Already specified the input file as: '"  
                                             + oInputFile + "'. Ignoring this option.");
                    show_usage = true;
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
    //Add font
    if (oFont.length())
        msc.AddAttribute(Attribute("text.font.face", oFont.c_str(), opt_pos_range,
        opt_pos_range));


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
        else if (oLmap)
            oOutputFile.append(".map");
        else switch (oOutType) {
        case Canvas::PNG:
            oOutputFile.append(".png"); break;
        case Canvas::EPS:
            oOutputFile.append(".eps"); break;
        case Canvas::PDF:
            oOutputFile.append(".pdf"); break;
        case Canvas::SVG:
            oOutputFile.append(".svg"); break;
        case Canvas::EMF:
            oOutputFile.append(".emf"); break;
        case Canvas::ISMAP:
            oOutputFile.append(".map"); break;
        default:
            assert(0);
        }
    } else if (oOutputFile=="-") {
        //this means standard output
        oOutputFile.clear();
    }

    //Determine option compatibility
    if (oPageSize!=PageSizeInfo::NO_PAGE && oOutType != Canvas::PDF) {
        msc.Error.Error(opt_pos, "-p can only be used with PDF output. Ignoring it.");
        oPageSize = PageSizeInfo::NO_PAGE;
    }
    bool has_auto_or_width = false;
    for (unsigned s=0; s<oScale.size() && !has_auto_or_width; s++)
        has_auto_or_width = oScale[s]<=0;
    if (has_auto_or_width && oPageSize == PageSizeInfo::NO_PAGE) {
        msc.Error.Error(opt_pos, "-s=auto and -s=width can only be used with full-page output. Using default scale of 1.0.");
        oScale.assign(1, 1.0);
    }
    if (oScale.size()>1 && oPageSize == PageSizeInfo::NO_PAGE) {
        msc.Error.Error(opt_pos, "Multiple -s options can only be applied to full-page output. Using default scale of 1.0.");
        oScale.assign(1, 1.0);
    }
    if (oPageSize==PageSizeInfo::NO_PAGE && oHA!=-2) {
        msc.Error.Error(opt_pos, "-ha can only be used with full-page output (-p). Ignoring it.");
        oHA = -1;
    }
    if (oHA == -2) oHA = -1;
    if (oPageSize==PageSizeInfo::NO_PAGE && oVA!=-2) {
        msc.Error.Error(opt_pos, "-va can only be used with full-page output (-p). Ignoring it.");
        oVA = -1;
    }
    if (oVA == -2) oVA = -1;
    if (oPageSize==PageSizeInfo::NO_PAGE && oA) {
        msc.Error.Error(opt_pos, "-a can only be used with full-page output (-p). Ignoring it.");
        oA = false;
    }
    if (oA) {
        bool had_auto = false;
        for (int s=0; s<(int)oScale.size() && !had_auto; s++)
            if (oScale[s]==-2) {
                had_auto = true;
                oScale.erase(oScale.begin()+s);
                s--;
            }
        if (had_auto && oScale.size())
            msc.Error.Error(opt_pos, "Scale to fit page ('auto') cannot be used with automatic pagination (-a). Ignoring it.");
        else if (had_auto && oScale.size()==0) {
            msc.Error.Error(opt_pos, "Scale to fit page ('auto') cannot be used with automatic pagination (-a). Replacing with scale to fit page width ('width').");
            oScale.push_back(-1);
        }
        if (oScale.size()>1) {
            msc.Error.Error(opt_pos, "Multiple -s options cannot be applied to automatic pagination. Using default scale of 1.0.");
            oScale.assign(1, 1.0);
        }
    }

    char *input = NULL;
    if (msc.Error.hasFatal()) goto fatal;

    /* Parse input, either from a file, or stdin */
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
        Csh csh(ArcBase::defaultDesign, &msc.Shapes, NULL);
        if (strlen(input)>std::numeric_limits<string::size_type>::max()-10) {
            string err = "The input '";
            err << oInputFile << "' is too long (" << strlen(input) << " bytes).";
            msc.Error.FatalError(opt_pos, err);
            err = "Error: Msc-generator cannot handle files longer than ";
            err << std::numeric_limits<string::size_type>::max()-10 << ".";
            msc.Error.Error(opt_pos, opt_pos, err);
        } else {
            //convert crlf to '\n'
            char *from = input, *to = input;
            for (; *from; from++)
                if (*from=='\r') continue; 
                else if (from==to) to++; 
                else *to++ = *from;
            *to = 0;
            const unsigned len = to-input;
            csh.ParseText(input, len, -1, 1);
            string tmp = Cshize(input, len, csh.CshList, 1, csh_textformat.c_str());
            FILE *out = oOutputFile.length() ? fopen(oOutputFile.c_str(), "w") : stdout;
            if (out) {
                fwrite(tmp.c_str(), 1, tmp.length(), out);
                if (oOutputFile.length())
                    fclose(out);
            } else
                msc.Error.FatalError(opt_pos, "Failed to open input file '" + oOutputFile +"'.");
        }
    } else {
        //Here either we have a valid graphics output format (or ISMAP) or we have oLmap==true
        //parse input text;
        msc.ParseText(input, oInputFile.c_str());
        XY pageSize(0,0);
        if (oA) {
            pageSize = PageSizeInfo::GetPhysicalPageSize(oPageSize);
            pageSize.x -= margins[0] + margins[1];
            pageSize.y -= margins[2] + margins[3];
            if (oScale.size() && oScale[0]>0)
                pageSize /= oScale[0];
            if (pageSize.x<10 || pageSize.y<10)
                oA=false;
        }
        msc.CompleteParse(oOutType, true, oA, oAH, pageSize, oScale.size() ? oScale[0]==-1 : false);

        //Determine scaling
        std::vector<XY> scale(std::max(std::vector<XY>::size_type(1), oScale.size()), XY(1., 1.));
        if (oUseWidthAttr)
            oX = msc.GetWidthAttr(); //nothing happens if equals -1
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
            if (oPageSize == PageSizeInfo::NO_PAGE) {
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

        const bool to_stdout = oOutputFile.length()==0;
        if (to_stdout) {
            //We cannot write to standard output a multi-page file, unless -p.
            if (msc.pageBreakData.size()>1 && oPageSize==PageSizeInfo::NO_PAGE && !oLmap) {
                msc.Error.Error(opt_pos, "Cannot write multiple files (one per page) to the standard output.");
                goto fatal;
            }
            oOutputFile = tmpnam(NULL);
        }
        const XY scale_to_use = Canvas::DetermineScaling(msc.GetTotal(), scale,
            PageSizeInfo::GetPhysicalPageSize(oPageSize), margins,
            msc.GetCopyrightTextHeight(), &msc.pageBreakData);
        if (oLmap) {
            //Register all the labels
            msc.RegisterLabelArcList(msc.Arcs);
            FILE *fout = fopen(oOutputFile.c_str(), "wt");
            msc.labelData.sort([](const LabelInfo &a, const LabelInfo &b) {return a.coord.y.from<b.coord.y.from; });
            for (const auto &l : msc.labelData) {
                //check page number
                unsigned p = 0;
                while (msc.pageBreakData.size() > p && 
                       msc.pageBreakData[p].y<=l.coord.y.from)
                    p++;
                //p is now the page number indexed from 1. 
                Block b(l.coord);
                //Shift the coordinates to compensate with where the page starts 
                b.Shift(XY(-msc.GetTotal().x.from, -msc.pageBreakData[p-1].y+msc.pageBreakData[p-1].autoHeadingSize));
                //scale with the requested user scaling & round to integers
                b.Scale(scale_to_use).RoundWider();
                //Get the first line
                const string first_line = l.text.substr(0, l.text.find_first_of('\n'));
                //emit line 
                //T P X1 Y1 X2 Y2 S
                //type page bb first line
                fprintf(fout, "%c %d %g %g %g %g %s\n", 
                              LabelInfo::labelTypeChar[l.type], p, 
                              b.x.from, b.y.from, b.x.till, b.y.till,
                              first_line.c_str());
            }
            fclose(fout);
        } else {
            //Now cycle through pages and write them to individual files or a full-page one
            msc.DrawToFile(oOutType, scale_to_use, oOutputFile, false, false, 
                           PageSizeInfo::GetPhysicalPageSize(oPageSize),
                           margins, oHA, oVA, true);
            msc.Progress.Done();
            if (load_data)
                *load_data = msc.Progress.WriteLoadData();
        }
        //copy temporary file to stdout, if so requested
        if (to_stdout) {
            FILE *in = fopen(oOutputFile.c_str(), "r");
            const size_t size = 4096;
            char buff[size];
            while (in && !feof(in)) {
                const size_t r = fread(buff, 1, size, in);
                fwrite(buff, 1, r, stdout);
            }
            fclose(in);
            remove(oOutputFile.c_str());
        }
    }
    if (input)
        free(input);
    if (msc.Error.hasFatal()) goto fatal;

    std::cerr << msc.Error.Print(oWarning);
    if (show_usage) usage();
    if (msc.Error.hasErrors()) {
        std::cerr << "There were errors, but ";
        if (oLmap || oOutType == Canvas::ISMAP)
            std::cerr << "a map";
        else if (oCshize)
            std::cerr << "the colorized chart";
        else 
            std::cerr << "a chart";
        std::cerr << " has been produced." << std::endl;
    } else
        std::cerr << "Success." << std::endl;
    return EXIT_SUCCESS;

fatal:
    //Serious problems
    std::cerr << msc.Error.Print(oWarning);
    if (show_usage) usage();
    std::cerr << "Bailing out." << std::endl;
    return EXIT_FAILURE;
}

