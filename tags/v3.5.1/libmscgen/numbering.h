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
    ENumberingStyle type;
    std::string     pre;
    std::string     post;

    explicit NumberingStyleFragment(ENumberingStyle t=ARABIC, std::string pr="", std::string po=""): type(t), pre(pr), post(po) {}
    static bool Parse(Msc *, file_line linenum, const char *text, std::vector<NumberingStyleFragment> &result);
    std::string Print(int n) const;
    unsigned Input(const std::string &number, int &value);
    static bool FindReplaceNumberFormatToken(std::string &text, file_line l, std::string::size_type pos=0);
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
    void SetSize(unsigned n);
    Numbering& operator ++() {++Last(); decrementOnAddingLevels=false; return *this;}
    Numbering& operator --() {--Last(); return *this;}
};

class NumberingStyle {
protected:
    std::pair<bool, std::vector<NumberingStyleFragment>> elements;
    unsigned                            startAt;
public:
    std::pair<bool, std::string> pre;
    std::pair<bool, std::string> post;

    NumberingStyle(): startAt(0) {elements.first = pre.first = post.first = true; elements.second.push_back(NumberingStyleFragment());}
    NumberingStyleFragment &Last(void) {_ASSERT(IsComplete()); return elements.second[elements.second.size()-1];}
    const NumberingStyleFragment &Last(void) const {_ASSERT(IsComplete()); return elements.second[elements.second.size()-1];}
    bool IsComplete() const {return elements.first && pre.first && post.first;}
    NumberingStyle &operator +=(const NumberingStyle&o); 
    void Reset() {elements.first = pre.first = post.first = true; pre.second.clear(); post.second.clear(); startAt=0; elements.second.clear(); elements.second.push_back(NumberingStyleFragment());}
    void Empty() {elements.first = pre.first = post.first = false;} 
    size_t Size(void) const {_ASSERT(IsComplete()); return startAt + elements.second.size();}
    void CopyShifted(const NumberingStyle &ns, unsigned start=0);
    int Apply(const std::vector<NumberingStyleFragment> &nsfs);
    void Push(const NumberingStyleFragment &nf) {_ASSERT(IsComplete()); elements.second.push_back(nf);}
    void Push(const std::vector<NumberingStyleFragment> &nsfs) {elements.first = true; elements.second.insert(elements.second.end(), nsfs.begin(), nsfs.end());}
    void Pop() {_ASSERT(IsComplete()); if (elements.second.size()>1) elements.second.pop_back();}
    std::string Print(const Numbering &n) const;
};

#endif //NUMBERING_H
