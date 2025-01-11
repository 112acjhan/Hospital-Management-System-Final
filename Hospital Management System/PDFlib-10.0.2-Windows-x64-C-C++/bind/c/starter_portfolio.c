/*
 * PDF portfolio starter:
 * Package multiple PDF and other documents into a PDF portfolio.
 * The documents in the portfolio will be assigned predefined
 * and custom metadata fields; for the custom fields a schema description
 * is created.
 *
 * Required data: PDF and other input documents
 */

#include <string.h>

#include "pdflib.h"


int
main(void)
{

    /* This is where the data files are. Adjust as necessary. */
    const char *searchpath = "../data";
#define OPTLISTSIZE 1024
    char optlist[OPTLISTSIZE], *target;
    int i, font, folder;
    PDF * p;

    typedef struct
    {
    const char *filename;
    const char *description;
    const char *status;
    int id;
    } portfolio;

    /* The documents for the Portfolio along with description and metadata */
    static const portfolio pf[] =
    {
    {
        "nesrin.jpg",
        "Zabrisky point",
        "archived",
        300
    },
    {
        "markup_annotations.pdf",
        "PDF sample with markup annotations",
        "published",
        101
    },
    {
        "PLOP-datasheet.pdf",
        "PDF Linearization, Optimization, Protection",
        "published",
        103
    },
    {
        "pCOS-datasheet.pdf",
        "PDF Information Retrieval Tool",
        "published",
        104
    }
    };
    const int n_files = sizeof(pf) / sizeof(pf[0]);

    /* create a new PDFlib object */
    if ((p = PDF_new()) == (PDF *) 0)
    {
        printf("Couldn't create PDFlib object (out of memory)\n");
        return(2);
    }

    PDF_TRY(p)
    {
        snprintf(optlist, OPTLISTSIZE, "SearchPath={{%s}}", searchpath);
        PDF_set_option(p, optlist);

        /* This means we must check return values of load_font() etc. */
        PDF_set_option(p, "errorpolicy=return textformat=bytes");

        if (PDF_begin_document(p, "starter_portfolio.pdf", 0,
        "") == -1)
        {
            printf("Error: %s\n", PDF_get_errmsg(p));
            PDF_delete(p);
            return(2);
        }

        PDF_set_info(p, "Creator", "PDFlib starter sample");
        PDF_set_info(p, "Title", "starter_portfolio");

        /* Insert two files in the root folder along with their description
         * and the following custom fields:
         * status   string describing the document status
         * id       numerical identifier, prefixed with "PHX"
         */
        for (i = 0; i < 2; i++)
        {
            snprintf(optlist, OPTLISTSIZE,
                "description={%s} "
                "fieldlist={ "
                        "{key=status value=%s} "
                        "{key=id value=%d prefix=PHX type=text} "
                "}",
                pf[i].description, pf[i].status, pf[i].id);

            /* -1 means root folder */
            PDF_add_portfolio_file(p, -1, pf[i].filename, 0, optlist);
        }

        /* Create the "datasheets" folder in the root folder */
        folder = PDF_add_portfolio_folder(p, -1, "datasheets", 0,
                "description={Folder with datasheets}");

        /* Insert documents in the "datasheets" folder along with
         * description and custom fields
         */
        for (i = 2; i < n_files; i++)
        {
            snprintf(optlist, OPTLISTSIZE,
            "description={%s} "
            "fieldlist={ "
                "{key=status value=%s} "
                "{key=id value=%d prefix=PHX type=text} "
            "}",
            pf[i].description, pf[i].status, pf[i].id);

            /* Add the file to the "datasheets" folder */
            PDF_add_portfolio_file(p, folder, pf[i].filename, 0, optlist);
        }

        /* Create a single-page document as cover sheet */
        PDF_begin_page_ext(p, 0, 0, "width=a4.width height=a4.height");

        font = PDF_load_font(p, "NotoSerif-Regular", 0, "unicode", "");
        if (font == -1)
        {
            printf("Error: %s\n", PDF_get_errmsg(p));
            PDF_delete(p);
            return(2);
        }

        PDF_setfont(p, font, 24);
        PDF_fit_textline(p, "Welcome to the PDFlib Portfolio sample!", 0,
                50, 700, "");

        PDF_end_page_ext(p, "");

        /* Set options for Portfolio display */
        target = optlist;
        int restsize = OPTLISTSIZE;
        int len = snprintf(target, (size_t)restsize, "portfolio={initialview=detail ");
        if (len >= restsize){
            printf("option list buffer too small\n");
            PDF_delete(p);
            return(2);
        }
        target += len;
        restsize -= len;

        /* Add schema definition for Portfolio metadata */
        len = snprintf(target, (size_t)restsize,
            "schema={ "
            /* Some predefined fields are included here to make them visible. */
            "{order=1 label=Name key=_filename visible editable} "
            "{order=2 label=Description key=_description visible} "
            "{order=3 label=Size key=_size visible} "
            "{order=4 label={Last edited} key=_moddate visible} "

            /* User-defined fields */
            "{order=5 label=Status key=status type=text editable} "
            "{order=6 label=ID key=id type=text editable} ");
        if (len >= restsize){
            printf("option list buffer too small\n");
            PDF_delete(p);
            return(2);
        }
        target += len;
        restsize -= len;
        len = snprintf(target, (size_t)restsize, "}}");

        if (len >= restsize){
            printf("option list buffer too small\n");
            PDF_delete(p);
            return(2);
        }
        target += len;
        PDF_end_document(p, optlist);
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
