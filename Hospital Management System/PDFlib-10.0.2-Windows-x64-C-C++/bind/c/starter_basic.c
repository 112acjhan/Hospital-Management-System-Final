/*
 * Basic starter:
 * Create some simple text, vector graphics and image output
 *
 * Required data: font, image file
 */

#include <stdio.h>
#include <stdlib.h>

#include "pdflib.h"


int
main(void)
{

    /* This is where the data files are. Adjust as necessary. */
    const char* searchpath = "../data";
    const char* imagefile = "nesrin.jpg";

    PDF * p;
    int image;

    /* create a new PDFlib object */
    if ((p = PDF_new()) == (PDF *) 0) {
        printf("Couldn't create PDFlib object (out of memory)\n");
        return(2);
    }

    PDF_TRY(p) {
#define OPTLISTSIZE 256
        char optlist[OPTLISTSIZE];

        /* This means we must check return values of load_font() etc. */
        PDF_set_option(p, "errorpolicy=return");

        snprintf(optlist, OPTLISTSIZE, "SearchPath={{%s}}", searchpath);
        PDF_set_option(p, optlist);

        if (PDF_begin_document(p, "starter_basic.pdf", 0, "") == -1) {
            printf("Error: %s\n", PDF_get_errmsg(p));
            PDF_delete(p);
            return(2);
        }

        PDF_set_info(p, "Creator", "PDFlib starter sample");
        PDF_set_info(p, "Title", "starter_basic");

        // Supply all strings in UTF-8 format
        PDF_set_option(p, "stringformat=utf8");

        // Load the image before the first page and use it on all pages
        image = PDF_load_image(p, "auto", imagefile, 0, "");

        if (image == -1) {
            printf("Error: %s\n", PDF_get_errmsg(p));
            PDF_delete(p);
            return(2);
        }

        /* Page 1 */
        PDF_begin_page_ext(p, 0,0, "width=a4.width height=a4.height");
        /* Use NotoSerif-Regular font with text format UTF-8 for placing the text
         * and demonstrate various options how to pass the text to PDFlib
         */
        snprintf(optlist, OPTLISTSIZE,
                "fontname={NotoSerif-Regular} fontsize=24");

        /* Plain ASCII text */
        PDF_fit_textline(p, "en: Hello!", 0, 50, 700, optlist);

        /* Hexadecimal character codes */
        PDF_fit_textline(p, "\x67\x72\x3A\x20\xCE\x93\xCE\xB5\xCE\xB9\xCE"
                            "\xAC\x21", 0, 50, 650, optlist);

        PDF_fit_textline(p, "ru: \xD0\x9F\xD1\x80\xD0\xB8\xD0\xB2\xD0\xB5"
                            "\xD1\x82!", 0, 50, 600, optlist);

        /* PDFlib character references */
        snprintf(optlist, OPTLISTSIZE,
                "fontname={NotoSerif-Regular} fontsize=24 charref=true");
        PDF_fit_textline(p, "es: &#xA1;Hola!", 0, 50, 550, optlist);


        PDF_fit_image(p, image, (float) 0.0, (float) 0.0, "scale=0.25");

        PDF_end_page_ext(p, "");

        /* Page 2 */
        PDF_begin_page_ext(p, 0,0, "width=a4.width height=a4.height");

        /* Demonstrate different ways of setting the fill color */

        /* Red rectangle */
        PDF_set_graphics_option(p, "fillcolor=red");
        PDF_rect(p, 200, 200, 250, 150);
        PDF_fill(p);

        /* Blue circle */
        PDF_set_graphics_option(p, "fillcolor={rgb 0 0 1}");
        PDF_arc(p, 400, 600, 100, 0, 360);
        PDF_fill(p);

        /* Thick gray line */
        PDF_set_graphics_option(p, "fillcolor={gray 0.5}");
        PDF_setlinewidth(p, 10);
        PDF_moveto(p, 100, 500);
        PDF_lineto(p, 300, 700);
        PDF_stroke(p);

        /* Using the same image handle means the data will be copied
         * to the PDF only once, which saves space.
         */
        PDF_fit_image(p, image, 150.0, 25.0, "scale=0.25");
        PDF_end_page_ext(p, "");

        /* Page 3 */
        PDF_begin_page_ext(p, 0,0, "width=a4.width height=a4.height");

        /* Fit the image to a box of predefined size (without distortion) */
        snprintf(optlist, OPTLISTSIZE,
            "boxsize={400 400} position={center} fitmethod=meet");

        PDF_fit_image(p, image, 100, 200, optlist);

        PDF_end_page_ext(p, "");

        PDF_close_image(p, image);
        PDF_end_document(p, "");
    }

    PDF_CATCH(p) {
        printf("PDFlib exception occurred:\n");
        printf("[%d] %s: %s\n",
            PDF_get_errnum(p), PDF_get_apiname(p), PDF_get_errmsg(p));
        PDF_delete(p);
        return(2);
    }

    PDF_delete(p);

    return 0;
}
