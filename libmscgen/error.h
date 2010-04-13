#if !defined(ERROR_H)
#define ERROR_H

#include<string>
#include<map>
#include<vector>

class Attribute;

class file_line
{
public:
    unsigned file;
    unsigned line;
    unsigned col;
    file_line() : file(0), line(0), col(0) {}
    file_line(unsigned a, unsigned b, unsigned c=0) : file(a), line(b), col(c) {}
    bool operator < (const file_line&a) const {
        if (file==a.file) {
            if (line==a.line)
                return col<a.col;
            else
                return line<a.line;
        }
        else return file < a.file;
    }
};


class MscError {
protected:
    std::multimap<file_line, std::string> Errors;
    std::multimap<file_line, std::string> Warnings;

    void Add(file_line linenum, const std::string &s, const std::string &once,
              std::multimap<file_line, std::string>& store);
    void Add(const Attribute &a, const std::string &s, const std::string &once,
              std::multimap<file_line, std::string>& store);

public:
    std::vector<std::string> Files;
    bool strict;
    MscError();
    unsigned AddFile(const std::string &filename);
    void Warning(file_line linenum, const std::string &s, const std::string &once="")
        {Add(linenum, s, once, Warnings);}
    void Warning(const Attribute &a, const std::string &s, const std::string &once="")
        {Add(a, s, once, Warnings);}
    void Error(file_line linenum, const std::string &s, const std::string &once="")
        {Add(linenum, s, once, Errors);}
    void Error(const Attribute &a, const std::string &s, const std::string &once="")
        {Add(a, s, once, Errors);}

    std::string Print(bool oWarnings) const;
    bool hasErrors() const {return Errors.size()>0;}
	file_line GetFileLineOfErrorNo(unsigned num) const;
};

#endif
