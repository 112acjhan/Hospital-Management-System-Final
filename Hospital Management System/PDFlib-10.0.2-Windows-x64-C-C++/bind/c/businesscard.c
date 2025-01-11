/* 
 * PDFlib client: fill Blocks with PPS to create a business card
 * Required software: PDFlib Personalization Server (PPS)
 */

#include <stdio.h>
#include <stdlib.h>

#include "pdflib.h"

int
main(void)
{
    PDF 	*p;
    int 	i, blockcontainer, page, pagecount, pageno;

    /*
     * This is where font/image/PDF input files live. Adjust as necessary.
     * Note that this directory must also contain the font files.
     */

    const char* searchpath = "../data";

    const char 	*infile = "boilerplate.pdf";

    /* By default annotations are also imported. In some cases this
     * requires the Noto fonts for creating annotation appearance streams.
     * We therefore set the searchpath to also point to the font directory.
     */

    const char * fontpath = "../../resource/font";

    typedef struct { const char *name; const char *value; } blockdata;

    const blockdata dataset[] = {
    { "name",			"Victor Kraxi" },
    { "business.title",		"Chief Paper Officer" },
    { "business.address.line1",	"17, Aviation Road" },
    { "business.address.city",	"Paperfield" },
    { "business.telephone.voice","phone +1 234 567-89" },
    { "business.telephone.fax",	"fax +1 234 567-98" },
    { "business.email",		"victor@kraxi.com" },
    { "business.homepage",	"www.kraxi.com" },
    };

#define BLOCKCOUNT (sizeof(dataset)/sizeof(dataset[0]))

    /* create a new PDFlib object */
    if ((p = PDF_new()) == (PDF *) 0)
    {
        printf("Couldn't create PDFlib object (out of memory)\n");
        return(2);
    }

    PDF_TRY(p) {
#define OPTLISTSIZE 256 
        char optlist[OPTLISTSIZE];

        /* This means we must check return values of load_font() etc. */
        /* Set the search path for fonts and PDF files */
        snprintf(optlist, OPTLISTSIZE, "errorpolicy=return SearchPath={{%s}}", searchpath);
        PDF_set_option(p, optlist);

        snprintf(optlist, OPTLISTSIZE, "SearchPath={{%s}}", fontpath);
        PDF_set_option(p, optlist);

        PDF_set_option(p, "textformat=bytes");

        if (PDF_begin_document(p, "businesscard.pdf", 0, "") == -1) {
            printf("Error: %s\n", PDF_get_errmsg(p));
            return(2);
        }

        PDF_set_info(p, "Creator", "businesscard");
        PDF_set_info(p, "Author", "Thomas Merz");
        PDF_set_info(p, "Title", "PDFlib Block processing sample");

        blockcontainer = PDF_open_pdi_document(p, infile, 0, "");
        if (blockcontainer == -1) {
            printf("Error: %s\n", PDF_get_errmsg(p));
            return(2);
        }

        pagecount = (int) PDF_pcos_get_number(p, blockcontainer, "length:pages");
        for (pageno = 1; pageno <= pagecount; pageno++ ){


            page = PDF_open_pdi_page(p, blockcontainer, pageno, "");
            if (page == -1) {
                printf("Error: %s\n", PDF_get_errmsg(p));
                return(2);
            }

            PDF_begin_page_ext(p, 20, 20, "");		/* dummy page size */

            /* This will adjust the page size to the block container's size. */
            PDF_fit_pdi_page(p, page, 0, 0, "adjustpage");

            /* Fill all text blocks with dynamic data */
            for (i = 0; i < (int) BLOCKCOUNT; i++) {
                if (PDF_fill_textblock(p, page, dataset[i].name, dataset[i].value, 0,
                    "") == -1) {
                printf("Warning: %s\n", PDF_get_errmsg(p));
                }
            }

            PDF_end_page_ext(p, "");
            PDF_close_pdi_page(p, page);
        }
        PDF_end_document(p, "");
        PDF_close_pdi_document(p, blockcontainer);
    }

    PDF_CATCH(p) {
        printf("PDFlib exception occurred in businesscard sample:\n");
        printf("[%d] %s: %s\n",
        PDF_get_errnum(p), PDF_get_apiname(p), PDF_get_errmsg(p));
        PDF_delete(p);
        return(2);
    }

    PDF_delete(p);

    return 0;
}
