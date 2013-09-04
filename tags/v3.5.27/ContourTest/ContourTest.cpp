// ContourTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "contour_test.h"
#include <cairo-win32.h>

int _tmain(int argc, _TCHAR* argv[])
{
    //Test if cairo can do fonts on EMF files
    //HDC hdc = CreateEnhMetaFile(NULL, _T("aaa.emf"), NULL, _T("Msc-generator\0\0"));
    //cairo_surface_t *surface = cairo_win32_printing_surface_create(hdc);
    //cairo_t *cr = cairo_create(surface);
    //cairo_move_to(cr, 100, 100);
    //cairo_show_text(cr, "AAAAA");
    //cairo_destroy(cr);
    //cairo_surface_show_page(surface);
    //cairo_surface_destroy (surface);
    //CloseEnhMetaFile(hdc); 
    
    //test contour
    contour::contour_test();
} 