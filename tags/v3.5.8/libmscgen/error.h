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

/** @file error.h The declaration of classes for error registration.
 * @ingroup libmscgen_files */

#if !defined(ERROR_H)
#define ERROR_H

#include<string>
#include<vector>

#ifndef _ASSERT
#define  _ASSERT(A)
#endif

class Attribute;

/** Contains a position in the input files.
 * 
 * We may parse multiple files in succession, thus we number them, so we can
 * refer to a location in any of them.*/
class file_line
{
public:
    int file;      ///<The number of the file. Names are stored in MscError::Files. -1 if invalid location.
    unsigned line; ///<The line of the location
    unsigned col;  ///<The character inside the line. First char is index zero.
    file_line() {MakeInvalid();}
    file_line(unsigned a, unsigned b, unsigned c=0) : file(a), line(b), col(c) {}
    void MakeInvalid() {file = -1;}
    bool IsInvalid() const {return file<0;}
    bool operator == (const file_line&a) const {return file==a.file && line==a.line && col==a.col;}
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
    file_line NextChar() const {return file_line(file, line, col+1);}
    std::string Print();
};

/** A structure to describe a range within a file.*/
struct file_line_range {
    file_line start; ///< The first character of the range
    file_line end;   ///< The last character of the range. Equals `start` for single character ranges.
    file_line_range() {MakeInvalid();}
    file_line_range(file_line s, file_line e) : start(s), end(e) {}
    file_line_range &IncStartCol(unsigned i=1) {start.col+=i; return *this;}
    void MakeInvalid() {start.MakeInvalid(); end.MakeInvalid();}
    bool IsInvalid() const {return start.IsInvalid() || end.IsInvalid();}
    bool operator ==(const file_line_range &o) const {return start==o.start && end==o.end;}
    bool operator <(const file_line_range &o) const {return start==o.start ? end<o.end : start<o.start;}
};

/** Returns which of two ranges are shorter.*/
struct file_line_range_length_compare
{
    bool operator() (const file_line_range &a, const file_line_range &b) const {
        if (b.end.file - b.start.file == a.end.file - a.start.file) {
            if (b.end.line - b.start.line == a.end.line - a.start.line) {
                if (b.end.col - b.start.col == a.end.col - a.start.col)
                    return a.start<b.start;
                else return b.end.col - b.start.col > a.end.col - a.start.col;
            } else return b.end.line - b.start.line > a.end.line - a.start.line;
        } else return b.end.file - b.start.file > a.end.file - a.start.file;
    }
};

/** Describes an Error, a Warning or auxiliary information for one of them.*/
struct ErrorElement
{
    file_line relevant_line; ///<The location to which the information applies. This is shown.
    file_line ordering_line; ///<The location used at ordering the errors. Different for auxiliary info.
    bool isError;            ///<Ture if this is an error 
    bool isOnlyOnce;         ///<True if messages with this text shall only be displayed once.
    std::string text;        ///<The full text of the error/warning including location, etc.
    std::string message;     ///<The original text of the error/warning.
    bool operator < (const ErrorElement &other) const {return ordering_line < other.ordering_line;}
};

/** Helper class to contain and display files and errors/warnings.*/
class MscError {
protected:
    bool hadFatal; ///<Indicates if FatalError() was called. Such errors result in no chart being produced. Used to adjust command-line messages.
    std::vector<ErrorElement> Errors;             ///<Contains the error messages and their related auxiliary info
    std::vector<ErrorElement> ErrorsAndWarnings;  ///<Contains both error and warning messages and their related auxiliary info
    const std::vector<ErrorElement> &get_store(bool oWarnings) const {return oWarnings?ErrorsAndWarnings:Errors;} ///<returns the smesage store to use.
    void _sort(std::vector<ErrorElement> &store); ///<Sorts the messages based on ordering_line. Removes duplicates if isOnlyOnce is true.

    void Add(file_line linenum, file_line linenum_ord, const std::string &s, const std::string &once, bool is_err);
    void Add(const Attribute &a, bool atValue, const std::string &s, const std::string &once, bool is_err);

public:
    std::vector<std::string> Files; ///<The list of files we parsed in
    MscError() : hadFatal(false) {}
    unsigned AddFile(const std::string &filename);  ///<Add another file to the list of files.
    /** Adds a warning.
     * @param [in] linenum The location of the warning.
     * @param [in] s The message to display
     * @param [in] once Auxiliary info, a type of hint on what to do. Added as a separate element, but only once.*/
    void Warning(file_line linenum, const std::string &s, const std::string &once="")
        {Add(linenum, linenum, s, once, false);}
    /** Adds a warning auxiliary info.
     * @param [in] linenum The location of the warning to show.
     * @param [in] linenum_ord The location to use at ordering.
     * @param [in] s The message to display
     * @param [in] once Auxiliary info, a type of hint on what to do. Added as a separate element, but only once.*/
    void Warning(file_line linenum, file_line linenum_ord, const std::string &s, const std::string &once="")
        {Add(linenum, linenum_ord, "("+s+")", once, false);}
    /** Adds a warning for an attribute.
     * @param [in] a The faulty attribute.
     * @param [in] atValue True if the problem is with the value of the attr, false if with its name.
     * @param [in] s The message to display
     * @param [in] once Auxiliary info, a type of hint on what to do. Added as a separate element, but only once.*/
    void Warning(const Attribute &a, bool atValue, const std::string &s, const std::string &once="")
        {Add(a, atValue, s, once, false);}
    /** Adds an error.
     * @param [in] linenum The location of the error.
     * @param [in] s The message to display
     * @param [in] once Auxiliary info, a type of hint on what to do. Added as a separate element, but only once.*/
    void Error(file_line linenum, const std::string &s, const std::string &once="")
        {Add(linenum, linenum, s, once, true);}
    /** Adds a error auxiliary info.
     * @param [in] linenum The location of the error to show.
     * @param [in] linenum_ord The location to use at ordering.
     * @param [in] s The message to display
     * @param [in] once Auxiliary info, a type of hint on what to do. Added as a separate element, but only once.*/
    void Error(file_line linenum, file_line linenum_ord, const std::string &s, const std::string &once="")
        {Add(linenum, linenum_ord, "("+s+")", once, true);}
    /** Adds a error for an attribute.
     * @param [in] a The faulty attribute.
     * @param [in] atValue True if the problem is with the value of the attr, false if with its name.
     * @param [in] s The message to display
     * @param [in] once Auxiliary info, a type of hint on what to do. Added as a separate element, but only once.*/
    void Error(const Attribute &a, bool atValue, const std::string &s, const std::string &once="")
        {Add(a, atValue, s, once, true);}
    /** Adds a fatal an error that prevented chart generation.
     * @param [in] linenum The location of the error.
     * @param [in] s The message to display
     * @param [in] once Auxiliary info, a type of hint on what to do. Added as a separate element, but only once.*/
    void FatalError(file_line linenum, const std::string &s, const std::string &once="")
        {Add(linenum, linenum, s, once, true); hadFatal=true;}

    /** Print all the errors (and warnings) onto a string*/
    std::string Print(bool oWarnings) const;
    /** True if there were errors added*/
    bool hasErrors() const {return Errors.size()>0;}
    /** True if there were fatal errors added*/
    bool hasFatal() const {return hadFatal;}
    /** Sort errors and warnings by location */
    void Sort() {_sort(Errors); _sort(ErrorsAndWarnings);}
    /** Returns the number of errors (includes warnings if oWarnings is true).*/
    unsigned GetErrorNum(bool oWarnings) const {return (unsigned)get_store(oWarnings).size();}
    /** Get the location of an error */
    file_line GetErrorLoc(unsigned num, bool oWarnings) const {return get_store(oWarnings)[num].relevant_line;}
    /** Get the text of an error, including line numbers and all. */
    const char *GetErrorText(unsigned num, bool oWarnings) const {return get_store(oWarnings)[num].text.c_str();}
    ErrorElement FormulateElement(file_line linenum, file_line linenum_ord, bool is_err, bool is_once, const std::string &msg) const ;
    /** Clearing all collected errors and warnings, but keeps the files.*/
    void Clear() {Errors.clear(); ErrorsAndWarnings.clear(); hadFatal=false;}
};

#endif
