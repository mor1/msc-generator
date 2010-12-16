#if !defined(ERROR_H)
#define ERROR_H

#include<string>
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
    bool operator > (const file_line &a) const {
        if (file==a.file) {
            if (line==a.line)
                return col>a.col;
            else
                return line>a.line;
        }
        else return file>a.file;
    }
    bool operator <= (const file_line &a) const {
        return !operator>(a);
    }
    bool operator >= (const file_line &a) const {
        return !operator<(a);
    }
    std::string Print();
};

struct file_line_range {
    file_line start;
    file_line end;
    file_line_range() {}
    file_line_range(file_line s, file_line e) : start(s), end(e) {}
    file_line_range &IncStartCol(unsigned i=1) {start.col+=i; return *this;}
};

struct ErrorElement
{
    file_line relevant_line;
    file_line ordering_line;
    bool isError;
    bool isOnlyOnce;
    std::string text;
    std::string message;
    bool operator < (const ErrorElement &other) const {return ordering_line < other.ordering_line;}
};

class MscError {
protected:
    std::vector<ErrorElement> Errors;
    std::vector<ErrorElement> ErrorsAndWarnings;
    const std::vector<ErrorElement> &get_store(bool oWarnings) const {return oWarnings?ErrorsAndWarnings:Errors;}
    void _sort(std::vector<ErrorElement> &store);

    ErrorElement FormulateElement(file_line linenum, bool is_err, bool is_once, const std::string &msg) const ;

    void Add(file_line linenum, const std::string &s, const std::string &once, bool is_err);
    void Add(const Attribute &a, bool atValue, const std::string &s, const std::string &once, bool is_err);

public:
    std::vector<std::string> Files;
    unsigned AddFile(const std::string &filename);
    void Warning(file_line linenum, const std::string &s, const std::string &once="")
        {Add(linenum, s, once, false);}
    void Warning(const Attribute &a, bool atValue, const std::string &s, const std::string &once="")
        {Add(a, atValue, s, once, false);}
    void Error(file_line linenum, const std::string &s, const std::string &once="")
        {Add(linenum, s, once, true);}
    void Error(const Attribute &a, bool atValue, const std::string &s, const std::string &once="")
        {Add(a, atValue, s, once, true);}

    std::string Print(bool oWarnings) const;
    bool hasErrors() const {return Errors.size()>0;}
    void Sort() {_sort(Errors); _sort(ErrorsAndWarnings);}
    unsigned GetErrorNum(bool oWarnings) const {return get_store(oWarnings).size();}
    file_line GetErrorLoc(unsigned num, bool oWarnings) const {return get_store(oWarnings)[num].relevant_line;}
    const char *GetErrorText(unsigned num, bool oWarnings) const {return get_store(oWarnings)[num].text.c_str();}
};

#endif
