// ContourTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "contour_test.h"
#include <cairo-win32.h>

int _tmain(int /*argc*/, _TCHAR* /*argv*/[])
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
    
    //test a cairo image surface bug
    //cairo_surface_t *surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, 100, 100);
    //cairo_t *cr = cairo_create(surface);
    //cairo_rectangle(cr, 10, 10, 80, 80);
    //cairo_new_sub_path(cr);
    //cairo_move_to(cr, 20, 20);
    //cairo_line_to(cr, 20, 50);
    //cairo_line_to(cr, 50, 50);
    //cairo_line_to(cr, 50, 20);
    //cairo_line_to(cr, 20, 20);
    //cairo_new_sub_path(cr);
    //cairo_move_to(cr, 31.5, 31.5);
    //cairo_line_to(cr, 32.5, 31.5);
    //cairo_line_to(cr, 32.5, 32.5);
    //cairo_line_to(cr, 31.5, 32.5);
    //cairo_line_to(cr, 31.5, 31.5);
    //cairo_clip(cr);
    //cairo_rectangle(cr, 5.5, 30.5, 90, 5);
    //cairo_set_line_width(cr, 1.);
    //cairo_stroke(cr);
    //cairo_destroy(cr);
    //cairo_surface_destroy (surface);

    //test contour
    contour::contour_test();
} 