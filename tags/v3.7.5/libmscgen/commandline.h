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
            std::list<std::pair<std::string, std::string>> &shape_files,
            std::string csh_textformat,
            MscProgress::ProgressCallback cb = NULL, void *param=NULL,
            std::string *load_data=NULL);

#endif /* COMMANDLINE_H */
