/* 
 * Starter sample for path objects:
 * Create some basic examples of path object construction and use
 *
 * Required data: font
 */
#include <stdlib.h>
#include <string.h>

#include "pdflib.h"


int
main(void)
{

    /* This is where the data files are. Adjust as necessary. */
    const char* outfile = "starter_path.pdf";
    const char* searchpath = "../data";

    PDF * p;
    int path, tf, n;
    const char *result;
    double x, y;
#define OPTLISTSIZE 256
    char optlist[OPTLISTSIZE];
    const char * text=
"Lorem ipsum dolor sit amet, consectetur adipisicing elit, \
sed do eiusmod tempor incididunt ut labore et dolore magna \
aliqua. Ut enim ad minim veniam, quis nostrud exercitation \
ullamco laboris nisi ut aliquip ex ea commodo consequat. \
Duis aute irure dolor in reprehenderit in voluptate velit esse cillum \
dolore eu fugiat nulla pariatur. Excepteur sint occaecat \
cupidatat non proident, sunt in culpa qui officia deserunt mollit anim \
id est laborum. ";

    /* create a new PDFlib object */
    if ((p = PDF_new()) == (PDF *) 0) {
        printf("Couldn't create PDFlib object (out of memory)\n");
        return(2);
    }

    PDF_TRY(p) {
        /* This means we must check return values of load_font() etc. */
        PDF_set_option(p, "errorpolicy=return");

        snprintf(optlist, OPTLISTSIZE, "SearchPath={{%s}}", searchpath);
        PDF_set_option(p, optlist);

        if (PDF_begin_document(p, outfile, 0, "") == -1) {
            printf("Error: %s\n", PDF_get_errmsg(p));
            PDF_delete(p);
            return(2);
        }

        PDF_set_info(p, "Creator", "PDFlib starter sample");
        PDF_set_info(p, "Title", "starter_path");

        /* Start an A4 page */
        PDF_begin_page_ext(p, 0, 0, "width=a4.width height=a4.height");

        /* Construct a path object for an arrow shape */

        path = -1;

        /* The tip of the arrow gets rounded corners */
        path = PDF_add_path_point(p, path, 200.0,  25.0, "move", "round=10");
        path = PDF_add_path_point(p, path, 200.0,  75.0, "line", "");
        /* assign a name to the arrow's tip */
        path = PDF_add_path_point(p, path, 300.0,   0.0, "line", "name=tip");
        path = PDF_add_path_point(p, path, 200.0, -75.0, "line", "");
        path = PDF_add_path_point(p, path, 200.0, -25.0, "line", "");

        /* Start a new subpath for the straight base of the arrow */
        path = PDF_add_path_point(p, path, 200.0, -25.0, "move", "");
        path = PDF_add_path_point(p, path,   0.0, -25.0, "line", "");

        /* The center of the base can serve as a named attachment point */
        path = PDF_add_path_point(p, path,   0.0,   0.0, "line", "name=base");
        path = PDF_add_path_point(p, path,   0.0,  25.0, "line", "");
        path = PDF_add_path_point(p, path, 200.0,  25.0, "line", "");

        x = 100.0;
        y = 850.0;


        /* ----------------------------------------
         * Place arrow in its original direction
         * ----------------------------------------
         */
        y -= 100.0;
        PDF_draw_path(p, path, x, y,
            "stroke linewidth=3 fill fillcolor=Turquoise "
            "linecap=projecting attachmentpoint=base ");


        /* ----------------------------------------
         * Scale down arrow and align it to north east
         * ----------------------------------------
         */
        y -= 200.0;
        PDF_draw_path(p, path, x, y,
            "stroke linewidth=3 fill fillcolor=Turquoise "
            "linecap=projecting attachmentpoint=base scale=0.5 align={1 1}");


        /* ----------------------------------------
         * Scale to 50%, use the arrow tip as attachment point,
         * and align the arrow to the left
         * ----------------------------------------
         */
        y -= 100.0;
        PDF_draw_path(p, path, x, y,
            "stroke linewidth=3 fill fillcolor=Turquoise "
            "linecap=projecting attachmentpoint=tip scale=0.5 align={-1 0}");


        /* ----------------------------------------
         * Place text on the path; round all corners to
         * allow smoother text at the corners
         * ----------------------------------------
         */
        y -= 100.0;
        snprintf(optlist, OPTLISTSIZE, 
            "textpath={path=%d round=10} position={center bottom} "
            "fontname=NotoSerif-Regular encoding=winansi fontsize=8", path);
        PDF_fit_textline(p, text, 0, x, y, optlist);


        /* ----------------------------------------
         * Use the path as clipping path for a Textflow
         * ----------------------------------------
        */
        y -= 300.0;

        /* Feed the text to the Textflow object */
        tf = PDF_add_textflow(p, -1, text, 0,
            "fontname=NotoSerif-Regular fontsize=10 encoding=winansi "
            "alignment=justify");
        /* Use text twice to fill the arrow */
        tf = PDF_add_textflow(p, tf, text, 0,
            "fontname=NotoSerif-Regular fontsize=10 encoding=winansi "
            "alignment=justify");
        if (tf == -1) {
            printf("Error: %s\n", PDF_get_errmsg(p));
            PDF_delete(p);
            return(2);
        }

        /* Attach the path's reference point to the middle left (0%, 50%)
         * of the fitbox, and wrap the text inside the path (inversefill)
         */
        snprintf(optlist, OPTLISTSIZE,
            "wrap={inversefill "
                "paths={{path=%d refpoint={0%% 50%%} scale=1.5 }}}",
            path);
        result = PDF_fit_textflow(p, tf, x, y, x+450, y+225, optlist);

        if (strcmp(result, "_stop"))
        {
            /* In this example we don't care about overflow text */
        }
        PDF_delete_textflow(p, tf);


        /* ----------------------------------------
         * Query information about the path object
         * ----------------------------------------
        */
        n = (int) PDF_info_path(p, path, "numpoints", "");


        PDF_delete_path(p, path);
        PDF_end_page_ext(p, "");
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
