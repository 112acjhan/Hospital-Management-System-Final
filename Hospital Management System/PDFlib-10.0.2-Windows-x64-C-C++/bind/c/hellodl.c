/*
 * PDFlib client: hello example in C with dynamic DLL loading
 */

#include <stdio.h>
#include <stdlib.h>

#include "pdflibdl.h"

int
main(void)
{
    PDF *p;
    const PDFlib_api *PDFlib;
    
    /* This is where the data files are. Adjust as necessary. */
    const char* searchpath = "../data";

    /* load the PDFlib dynamic library and create a new PDFlib object*/
    if ((PDFlib = PDF_new_dl(&p)) == (PDFlib_api *) NULL)
    {
        printf("Couldn't create PDFlib object (DLL not found?)\n");
        return(2);
    }

    PDF_TRY_DL(PDFlib, p) {
#define OPTLISTSIZE 256
        char optlist[OPTLISTSIZE];
        const char* fontopt = 
            "fontname=NotoSerif-Regular encoding=winansi fontsize=24";

        /* errorpolicy=exception means that the program will stop
         * if one of the API functions runs into a problem.
         */
        PDFlib->PDF_set_option(p, "errorpolicy=exception");

        snprintf(optlist, OPTLISTSIZE, "SearchPath={{%s}}", searchpath);
        /* Set the search path for font files */
        PDFlib->PDF_set_option(p, optlist);

        if (PDFlib->PDF_begin_document(p, "hellodl.pdf", 0, "") == -1) {
            printf("Error: %s\n", PDFlib->PDF_get_errmsg(p));
            return(2);
        }

        PDFlib->PDF_set_info(p, "Creator", "hello.c");
        PDFlib->PDF_set_info(p, "Author", "Thomas Merz");
        PDFlib->PDF_set_info(p, "Title", "Hello World (C DLL)!");

        PDFlib->PDF_begin_page_ext(p, 0, 0, "width=a4.width height=a4.height");

        PDFlib->PDF_fit_textline(p, "Hello world!", 0, 50, 700, fontopt);
        PDFlib->PDF_fit_textline(p,"(says C DLL)", 0, 50, 676, fontopt);

        PDFlib->PDF_end_page_ext(p, "");

        PDFlib->PDF_end_document(p, "");
    }

    PDF_CATCH_DL(PDFlib, p) {
        printf("PDFlib exception occurred in hellodl sample:\n");
        printf("[%d] %s: %s\n",
	PDFlib->PDF_get_errnum(p), PDFlib->PDF_get_apiname(p),
	PDFlib->PDF_get_errmsg(p));
	PDF_delete_dl(PDFlib, p);
        return(2);
    }

    /* delete the PDFlib object and unload the library */
    PDF_delete_dl(PDFlib, p);

    return 0;
}
