/* 
 * PDFlib client: hello example in C
 */

#include <stdio.h>
#include <stdlib.h>

#include "pdflib.h"

int
main(void)
{
    PDF *p;
    /* This is where the data files are. Adjust as necessary. */
    const char* searchpath = "../data";

    /* create a new PDFlib object */
    if ((p = PDF_new()) == (PDF *) 0)
    {
        printf("Couldn't create PDFlib object (out of memory)\n");
        return(2);
    }

    PDF_TRY(p) {
#define OPTLISTSIZE 256 
        char optlist[OPTLISTSIZE];
        const char* fontopt = 
            "fontname=NotoSerif-Regular encoding=winansi fontsize=24";
        /* This means we must check return values of load_font() etc. */
        PDF_set_option(p, "errorpolicy=return");

        snprintf(optlist, OPTLISTSIZE, "SearchPath={{%s}}", searchpath);
        /* Set the search path for font files */
        PDF_set_option(p, optlist);

        if (PDF_begin_document(p, "hello.pdf", 0, "") == -1) {
            printf("Error: %s\n", PDF_get_errmsg(p));
            return(2);
        }

        PDF_set_info(p, "Creator", "hello.c");
        PDF_set_info(p, "Author", "Thomas Merz");
        PDF_set_info(p, "Title", "Hello World (C)!");

        PDF_begin_page_ext(p, 0, 0, "width=a4.width height=a4.height");

        PDF_fit_textline(p, "Hello world!", 0, 50, 700, fontopt);
        PDF_fit_textline(p, "(says C)",  0, 50, 676, fontopt);

        PDF_end_page_ext(p, "");

        PDF_end_document(p, "");
    }

    PDF_CATCH(p) {
        printf("PDFlib exception occurred in hello sample:\n");
        printf("[%d] %s: %s\n",
        PDF_get_errnum(p), PDF_get_apiname(p), PDF_get_errmsg(p));
        PDF_delete(p);
        return(2);
    }

    PDF_delete(p);

    return 0;
}
