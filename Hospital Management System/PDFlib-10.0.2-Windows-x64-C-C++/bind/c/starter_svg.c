/* 
 * Starter SVG:
 * Load SVG graphics and fit into a box
 *
 * Required data: SVG graphics
 */
#include <stdio.h>
#include <stdlib.h>

#include "pdflib.h"

int
main(void)
{
    /* This is where the data files are. Adjust as necessary. */
    const char* searchpath = "../data";
    const char* outfile = "starter_svg.pdf";
    const char *graphicsfile = "PDFlib-logo.svg";

    PDF * p;
    int graphics;
    int x = 100, y = 300;
    int boxwidth = 400, boxheight = 400;

    /* create a new PDFlib object */
    if ((p = PDF_new()) == (PDF *) 0) {
        printf("Couldn't create PDFlib object (out of memory)\n");
        return(2);
    }

    PDF_TRY(p)
    {
#define OPTLISTSIZE 256
        char optlist[OPTLISTSIZE];

        snprintf(optlist, OPTLISTSIZE, "SearchPath={{%s}}", searchpath);
        PDF_set_option(p, optlist);

        /* This means we must check return values of load_graphics() etc. */
        PDF_set_option(p, "errorpolicy=return");

        PDF_set_option(p, "textformat=bytes");

        if (PDF_begin_document(p, outfile, 0, "") == -1)
        {
            printf("Error: %s\n", PDF_get_errmsg(p));
            PDF_delete(p);
            return(2);
        }

        PDF_set_info(p, "Creator", "PDFlib starter sample");
        PDF_set_info(p, "Title", "starter_svg");

        /* Load the graphics */
        graphics = PDF_load_graphics(p, "auto", graphicsfile, 0, "");
        if (graphics == -1)
        {
            printf("Error: %s\n", PDF_get_errmsg(p));
            PDF_delete(p);
            return(2);
        }

        PDF_begin_page_ext(p, 0, 0, "width=a4.width height=a4.height");

        /* ------------------------------------------------------
         * Fit the graphics into a box with proportional resizing
         * ------------------------------------------------------
         */

        /* The "boxsize" option defines a box with a given width and height and
         * its lower left corner located at the reference point.
         * "position={center}" positions the graphics in the center of the
         * box.
         * "fitmethod=meet" resizes the graphics proportionally until its height
         * or width completely fits into the box.
         * The "showborder" option is used to illustrate the borders of the box.
         */
        snprintf(optlist, OPTLISTSIZE,
            "boxsize={%d %d} position={center} fitmethod=meet showborder",
            boxwidth, boxheight);

        /* Before actually fitting the graphics we check whether fitting is
         * possible.
         */
        if (PDF_info_graphics(p, graphics, "fittingpossible", optlist) == 1)
        {
            PDF_fit_graphics(p, graphics, x, y, optlist);
        }
        else
        {
            printf("Cannot place graphics: %s\n", PDF_get_errmsg(p));
        }

        PDF_end_page_ext(p, "");

        PDF_close_graphics(p, graphics);

        PDF_end_document(p, "");

    }
    PDF_CATCH(p)
    {
        printf("PDFlib exception occurred:\n");
        printf("[%d] %s: %s\n",
            PDF_get_errnum(p), PDF_get_apiname(p), PDF_get_errmsg(p));
        PDF_delete(p);
        return(2);
    }

    PDF_delete(p);
    return 0;
}
