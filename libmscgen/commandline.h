#if !defined(COMMANDLINE_H)
#define COMMANDLINE_H

#include <list>
#include <string>

char *ReadFile(FILE *in);
int do_main(const std::list<std::string> &args, const char *designs,
            std::string csh_textformat);

#endif /* COMMANDLINE_H */
