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
    explicit MscColorType(unsigned);
    unsigned ConvertToUnsigned() const {return ((unsigned(r))<<24) + ((unsigned(g))<<16) + ((unsigned(b))<<8) + (unsigned(a));}
    bool operator==(const MscColorType &x) const
        {return (r==x.r && g==x.g && b==x.b && a==x.a && valid && x.valid) ||
                (!valid && !x.valid);}
    std::string Print(void) const;
//    MscColorType &operator +=(const MscColorType &c);
    MscColorType operator +(const MscColorType &c) const;
    MscColorType Lighter(double p) const
        {return MscColorType((unsigned char)(r+p*(255-r)), (unsigned char)(g+p*(255-g)), (unsigned char)(b+p*(255-b)), (unsigned char)a);}
    MscColorType Darker(double p) const
        {return MscColorType((unsigned char)(r*(1-p)), (unsigned char)(g*(1-p)), (unsigned char)(b*(1-p)), (unsigned char)a);}
    MscColorType MoreTransparent(double p) const
        {return MscColorType(r, g, b, (unsigned char)(a*(1-p)));}
    MscColorType MoreOpaque(double p) const
        {return MscColorType(r, g, b, (unsigned char)(a+p*(255-a)));}
	MscColorType FlattenAlpha(void) const
	    {return MscColorType(unsigned(255-r)*(255-a)/255+r, unsigned(255-g)*(255-a)/255+g, unsigned(255-b)*(255-a)/255+b);}
};

class ColorSet : public std::map<std::string, MscColorType>
{
public:
    bool AddColor(std::string, std::string, MscError &error, file_line_range linenum);
    MscColorType GetColor(const std::string &s) const;
};


#endif //COLOR_H
