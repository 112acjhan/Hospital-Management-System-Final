/* 
 * PDFlib Virtual File system (PVF):
 * Create a PVF file which holds an image or PDF, and import the data from the
 * PVF file
 *
 * This avoids disk access and is especially useful when the same image or PDF
 * is imported multiply. For examples, images which sit in a database don't
 * have to be written and re-read from disk, but can be passed to PDFlib
 * directly in memory. A similar technique can be used for loading other data
 * such as fonts, ICC profiles, etc.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pdflib.h"

int
main(void)
{

    /* This is where the data files are. Adjust as necessary. */
    const char* searchpath = "../data";
    const char* outfile = "starter_pvf.pdf";

    PDF * p;
    /* Some constant SVG data as an example for fetching input data from memory */
    const char * svgdata = 
"<?xml version='1.0'?> \
<svg viewBox='0 0 100 100' version='1.1' xmlns='http://www.w3.org/2000/svg'> \
  <circle cx='50' cy='50' r='40' fill='orange'/> \
</svg>";
    int svg;

    /* create a new PDFlib object */
    if ((p = PDF_new()) == (PDF *) 0) {
        printf("Couldn't create PDFlib object (out of memory)\n");
        return(2);
    }

    PDF_TRY(p) {
#define OPTLISTSIZE 256
        char optlist[OPTLISTSIZE];

        snprintf(optlist, OPTLISTSIZE, "SearchPath=%s", searchpath);
        PDF_set_option(p, optlist);

        /* This means we must check return values of load_font() etc. */
        PDF_set_option(p, "errorpolicy=return");

        /* Set an output path according to the name of the topic */
        if (PDF_begin_document(p, outfile, 0, "") == -1) {
            printf("Error: %s\n", PDF_get_errmsg(p));
            PDF_delete(p);
            return(2);
        }

        PDF_set_info(p, "Creator", "PDFlib starter sample");
        PDF_set_info(p, "Title", "starter_pvf");

        PDF_create_pvf(p, "/pvf/svg", 0, svgdata, strlen(svgdata), "");

        /* Load the graphics from the PVF */
        svg = PDF_load_graphics(p, "svg", "/pvf/svg", 0, "");
        if (svg == -1) {
            printf("Error: %s\n", PDF_get_errmsg(p));
            PDF_delete(p);
            return(2);
        }

        /* Fit the graphics on page 1 */
        PDF_begin_page_ext(p, 0,0, "width=a4.width height=a4.height");

        PDF_fit_graphics(p, svg, 350, 750, "");

        PDF_end_page_ext(p, "");

        /* Fit the graphics on page 2 */
        PDF_begin_page_ext(p, 0,0, "width=a4.width height=a4.height");

        PDF_fit_graphics(p, svg, 350, 50, "");

        PDF_end_page_ext(p, "");

        /* Delete the virtual file to free the allocated memory */
        PDF_delete_pvf(p, "/pvf/svg", 0);

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
