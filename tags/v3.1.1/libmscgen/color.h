#if !defined(COLOR_H)
#define COLOR_H

#include <map>
#include <string>
#include "error.h"

class MscColorType
{
public:
    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned char a;
    bool          valid;
    MscColorType() : r(0), g(0), b(0), a(255), valid(false) {}
    MscColorType(unsigned char R, unsigned char G, unsigned char B, unsigned char A=255) :
        r(R), g(G), b(B), a(A), valid(true) {}
    explicit MscColorType(const std::string&);
    explicit MscColorType(void*);
    operator void*() const;
    bool operator==(const MscColorType &x) const
        {return (r==x.r && g==x.g && b==x.b && a==x.a && valid && x.valid) ||
                (!valid && !x.valid);}
    std::string Print(void) const;
//    MscColorType &operator +=(const MscColorType &c);
    MscColorType operator +(const MscColorType &c) const;
    MscColorType Lighter(double p) const
        {return MscColorType(r+p*(255-r), g+p*(255-g), b+p*(255-b), a);}
    MscColorType Darker(double p) const
        {return MscColorType(r*(1-p), g*(1-p), b*(1-p), a);}
    MscColorType MoreTransparent(double p) const
        {return MscColorType(r, g, b, a*(1-p));}
    MscColorType MoreOpaque(double p) const
        {return MscColorType(r, g, b, a+p*(255-a));}
	MscColorType FlattenAlpha(void) const
	    {return MscColorType(int(255-r)*(255-a)/255+r, int(255-g)*(255-a)/255+g, int(255-b)*(255-a)/255+b);}
};

class ColorSet : public std::map<std::string, MscColorType>
{
public:
    bool AddColor(std::string, std::string, MscError &error, file_line_range linenum);
    MscColorType GetColor(const std::string &s) const;
};


#endif //COLOR_H
