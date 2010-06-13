#if !defined(COMMANDLINE_H)
#define COMMANDLINE_H

#include <list>
#include <string>
#include "version.h"

const char *VersionText(char a=LIBMSCGEN_MAJOR,
                        char b=LIBMSCGEN_MINOR,
                        char c=LIBMSCGEN_SUPERMINOR);

char *ReadFile(FILE *in);
int do_main(const std::list<std::string> &args, const char *designs,
            std::string csh_textformat);

#endif /* COMMANDLINE_H */
