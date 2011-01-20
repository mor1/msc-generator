#if !defined(NUMBERING_H)
#define NUMBERING_H
#include <string>
#include <sstream>
#include <vector>
#include "error.h"

class Msc;

class NumberingStyleFragment {
public:
    typedef enum {ARABIC, ROMAN_UPPER, ROMAN_LOWER, ABC_UPPER, ABC_LOWER} ENumberingStyle;
    std::string     pre;
    std::string     post;
    ENumberingStyle type;

    explicit NumberingStyleFragment(ENumberingStyle t=ARABIC, std::string pr="", std::string po=""): type(t), pre(pr), post(po) {}
    static bool Parse(Msc *, file_line linenum, const char *text, std::vector<NumberingStyleFragment> &result);
    std::string Print(int n) const;
    int Input(const std::string &number, int &value);
    static bool FindReplaceNumberFormatToken(std::string &text, file_line l, int pos=0);
};

class Numbering {
    friend class NumberingStyle;
protected:
    std::vector<int> values;
public:
    //This flag is set after adding a number to box or bigarrow labels if they have content.
    //If we add levels with this flag set, we decrement the counter before adding the levels
    //so if the box title got number 7, its first content will get 7.1 and not 8.1
    bool decrementOnAddingLevels;
    Numbering() : decrementOnAddingLevels(false) {values.push_back(1);}
    int &Last(void) {return values[values.size()-1];}
    const int &Last(void) const {return values[values.size()-1];}
    void SetSize(int n);
    Numbering& operator ++() {++Last(); decrementOnAddingLevels=false; return *this;}
    Numbering& operator --() {--Last(); return *this;}
};

class NumberingStyle {
protected:
    std::vector<NumberingStyleFragment> elements;
    unsigned                            startAt;
public:
    std::string pre;
    std::string post;

    NumberingStyle(): startAt(0) {elements.push_back(NumberingStyleFragment());}
    NumberingStyleFragment &Last(void) {return elements[elements.size()-1];}
    const NumberingStyleFragment &Last(void) const {return elements[elements.size()-1];}
    void Reset() {pre.clear(); post.clear(); startAt=0; elements.clear(); elements.push_back(NumberingStyleFragment());}
    int Size(void) const {return startAt + elements.size();}
    void CopyShifted(const NumberingStyle &ns, int start=0);
    int Apply(const std::vector<NumberingStyleFragment> &nsfs);
    void Push(const NumberingStyleFragment &nf) {elements.push_back(nf);}
    void Push(const std::vector<NumberingStyleFragment> &nsfs) {elements.insert(elements.end(), nsfs.begin(), nsfs.end());}
    void Pop() {if (elements.size()>1) elements.pop_back();}
    std::string Print(const Numbering &n) const;
};

#endif //NUMBERING_H
