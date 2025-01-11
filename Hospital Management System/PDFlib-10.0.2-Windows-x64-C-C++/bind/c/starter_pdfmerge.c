/*
 * PDF merge starter:
 * Merge pages from multiple PDF documents (including annotations)
 *
 * Required software: PDFlib+PDI or PDFlib Personalization Server (PPS)
 * Required data: PDF documents
 */

#include <stdio.h>
#include <stdlib.h>

#include "pdflib.h"


int
main(void)
{

    /* This is where the data files are. Adjust as necessary. */
    const char * searchpath = "../data";

    /* By default annotations are also imported. In some cases this
     * requires the Noto fonts for creating annotation appearance streams.
     * We therefore set the searchpath to also point to the font directory.
     */
    const char * fontpath = "../../resource/font";

    PDF * p;
    const char * pdffiles[] =
    {
        "markup_annotations.pdf",
        "PLOP-datasheet.pdf",
        "pCOS-datasheet.pdf"
    };
    unsigned int i;

    /* create a new PDFlib object */
    if ((p = PDF_new()) == (PDF *) 0) {
        printf("Couldn't create PDFlib object (out of memory)\n");
        return(2);
    }

    PDF_TRY(p) {
#define OPTLISTSIZE 256
        char optlist[OPTLISTSIZE];

        snprintf(optlist, OPTLISTSIZE, "SearchPath={{%s}}", searchpath);
        PDF_set_option(p, optlist);

        snprintf(optlist, OPTLISTSIZE, "SearchPath={{%s}}", fontpath);
        PDF_set_option(p, optlist);

        /* This means we must check return values of load_font() etc. */
        PDF_set_option(p, "errorpolicy=return");

        if (PDF_begin_document(p, "starter_pdfmerge.pdf", 0, "") == -1) {
            printf("Error: %s\n", PDF_get_errmsg(p));
            PDF_delete(p);
            return(2);
        }

        PDF_set_info(p, "Creator", "PDFlib starter sample");
        PDF_set_info(p, "Title", "starter_pdfmerge");

        for (i=0; i < (int)(sizeof pdffiles / sizeof (const char *)); i++)
        {
            int indoc, endpage=0, pageno, page;

            /* Open the input PDF */
            indoc = PDF_open_pdi_document(p, pdffiles[i], 0, "");
            if (indoc == -1)
            {
                fprintf(stderr, "Error: %s\n", PDF_get_errmsg(p));
                continue;
            }

            endpage = (int) PDF_pcos_get_number(p, indoc, "length:pages");

            /* Loop over all pages of the input document */
            for (pageno = 1; pageno <= endpage; pageno++)
            {
                page = PDF_open_pdi_page(p, indoc, pageno, "");

                if (page == -1)
                {
                    fprintf(stderr, "Error: %s\n", PDF_get_errmsg(p));
                    continue;
                }
                /* Dummy page size; will be adjusted later */
                PDF_begin_page_ext(p, 10, 10, "");

                /* Create a bookmark with the file name */
                if (pageno == 1)
                    PDF_create_bookmark(p, pdffiles[i], 0, "");

                /* Place the imported page on the output page, and
                 * adjust the page size. If the page contains annotations
                 * these are also imported.
                 */
                PDF_fit_pdi_page(p, page, 0, 0, "adjustpage");
                PDF_close_pdi_page(p, page);

                PDF_end_page_ext(p, "");
            }
            PDF_close_pdi_document(p, indoc);
        }

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
