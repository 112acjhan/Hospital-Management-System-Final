/*
 * PDF impose:
 * Import all pages from one more existing PDFs and place col x row pages
 * on each sheet of the output PDF (imposition). If there are annotations
 * on an imported page these are also imported and scaled or rotated as
 * required.
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
    PDF		*p;
    /* This is where font/image/PDF input files live. Adjust as necessary. */
    const char* searchpath = "../data";

    /* By default annotations are also imported. In some cases this
     * requires the Noto fonts for creating annotation appearance streams.
     * We therefore set the searchpath to also point to the font directory.
     */
    const char * fontpath = "../../resource/font";

    const char * pdffiles[] =
    {
        "markup_annotations.pdf",
        "PLOP-datasheet.pdf",
        "pCOS-datasheet.pdf"
    };

    const char * title = "PDF Impose";
    unsigned int i;
    int col = 0, row = 0;
#define OPTLISTSIZE 256
    char optlist[OPTLISTSIZE];
    double scale = 1;          /* scaling factor of a page */
    double rowheight;          /* row height for the page to be placed */
    double colwidth;           /* column width for the page to be placed */

    double sheetwidth = 595;   /* width of the sheet */
    double sheetheight = 842;  /* height of the sheet */
    int maxcol=2, maxrow=2;    /* maxcol x maxrow pages will be placed on one sheet */

    /* create a new PDFlib object */
    if ((p = PDF_new()) == (PDF *) 0)
    {
        printf("Couldn't create PDFlib object (out of memory)\n");
        return(2);
    }

    PDF_TRY(p)
    {
        int pageopen = 0;

        /* This means we must check return values of load_font() etc. */
        PDF_set_option(p, "errorpolicy=return");

        snprintf(optlist, OPTLISTSIZE, "SearchPath={{%s}}", searchpath);
        PDF_set_option(p, optlist);

        snprintf(optlist, OPTLISTSIZE, "SearchPath={{%s}}", fontpath);
        PDF_set_option(p, optlist);

        if (PDF_begin_document(p, "starter_pdfimpose.pdf", 0, "") == -1)
        {
            printf("Error: %s\n", PDF_get_errmsg(p));
            return (2);
        }

        PDF_set_info(p, "Creator", "PDFlib starter sample");
        PDF_set_info(p, "Title", title);

        /* ---------------------------------------------------------------------
         * Define the sheet width and height,  the number of rows and columns
         * and calculate the scaling factor and cell dimensions for the
         * multi-page imposition.
         * ---------------------------------------------------------------------
         */
        if (maxrow > maxcol)
            scale = 1.0 / maxrow;
        else
            scale = 1.0 / maxcol;

        rowheight = sheetheight * scale;
        colwidth = sheetwidth * scale;

        /* Loop over all input documents */
        for (i = 0; i < (sizeof pdffiles / sizeof(const char *)); i++)
        {
            int indoc, maxpage, pageno, page;

            /* Open the input PDF */
            indoc = PDF_open_pdi_document(p, pdffiles[i], 0, "");
            if (indoc == -1)
            {
                fprintf(stderr, "Error: %s\n", PDF_get_errmsg(p));
                continue;
            }

            maxpage = (int) PDF_pcos_get_number(p, indoc, "length:pages");

            for (pageno = 1; pageno <= maxpage; pageno++)
            {
                page = PDF_open_pdi_page(p, indoc, pageno, "");

                if (page == -1)
                {
                    printf("Error: %s\n", PDF_get_errmsg(p));
                    continue;
                }
                /* Start a new page */
                if (!pageopen)
                {
                    PDF_begin_page_ext(p, sheetwidth, sheetheight, "");
                    pageopen = 1;
                }
                /* The save/restore pair is required to get an independent
                 * clipping area for each mini page. Note that clipping
                 * is not required for the imported pages, but affects
                 * the rectangle around each page. Without clipping we
                 * would have to move the rectangle a bit inside the
                 * imported page to avoid drawing outside its area.
                 */
                PDF_save(p);

                /* Clipping path for the rectangle */
                PDF_rect(p, col * colwidth,
                        sheetheight - (row + 1) * rowheight, colwidth,
                        rowheight);
                PDF_clip(p);

                snprintf(optlist, OPTLISTSIZE, "boxsize={%f %f} fitmethod=meet", colwidth,
                        rowheight);

                /* Fit the imported page in the box */
                PDF_fit_pdi_page(p, page, col * colwidth,
                        sheetheight - (row + 1) * rowheight, optlist);
                PDF_close_pdi_page(p, page);

                /* Draw a frame around the mini page */
                snprintf(optlist, OPTLISTSIZE, "linewidth=%f", scale);
                PDF_set_graphics_option(p, optlist);
                PDF_rect(p, col * colwidth,
                        sheetheight - (row + 1) * rowheight, colwidth,
                        rowheight);
                PDF_stroke(p);

                PDF_restore(p);

                col++;

                /* Start a new row if the current row is full */
                if (col == maxcol)
                {
                    col = 0;
                    row++;
                }
                if (row == maxrow)
                {
                    row = 0;
                    PDF_end_page_ext(p, "");
                    pageopen = 0;
                }
            }
            PDF_close_pdi_document(p, indoc);
        }

        /* finish the last partial page */
        if (row != 0 || col != 0)
            PDF_end_page_ext(p, "");

        PDF_end_document(p, "");
    }

    PDF_CATCH(p)
    {
        printf("PDFlib exception occurred in starter_pdfimpose sample:\n");
        printf("[%d] %s: %s\n",
        PDF_get_errnum(p), PDF_get_apiname(p), PDF_get_errmsg(p));
        PDF_delete(p);
        return(2);
    }

    PDF_delete(p);

    return 0;
}
