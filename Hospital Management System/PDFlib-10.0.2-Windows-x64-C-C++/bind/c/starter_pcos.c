/* 
 * pCOS starter:
 * Dump information from an existing PDF document
 *
 * Required software: PDFlib+PDI or PDFlib Personalization Server (PPS)
 * Required data: PDF input file
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "pdflib.h"

int
main(void)
{
    /* This is where the data files are. Adjust as necessary. */
    const char *searchpath = "../data";

    PDF *p;
    const char *pdfinput = "PLOP-datasheet.pdf";

    const char *docoptlist = "requiredmode=minimum";
    int count, xfa_present, plainmetadata, objtype;
    int i, doc;
    const char *pcosmode;

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

        /* This means we must check return values of load_font() etc. */
        PDF_set_option(p, "errorpolicy=return");

        /* We do not create any output document, so no call to
         * begin_document() is required.
         */

        /* Open the input document */
        if ((doc = PDF_open_pdi_document(p, pdfinput, 0, docoptlist)) == -1) {
            printf("Error: %s\n", PDF_get_errmsg(p));
            PDF_delete(p);
            return(2);
        }

        /* --------- general information (always available) */

        pcosmode = PDF_pcos_get_string(p, doc, "pcosmodename");

        printf("   File name: %s\n",
            PDF_pcos_get_string(p, doc,"filename"));

        printf(" PDF version: %s\n",
            PDF_pcos_get_string(p, doc, "pdfversionstring"));

        printf("  Encryption: %s\n",
            PDF_pcos_get_string(p, doc, "encrypt/description"));

        printf("   Master pw: %s\n",
            ((PDF_pcos_get_number(p, doc, "encrypt/master")!= 0) ? "yes":"no"));

        printf("     User pw: %s\n",
            ((PDF_pcos_get_number(p, doc, "encrypt/user")!= 0) ? "yes" : "no"));

        printf("Text copying: %s\n",
            ((PDF_pcos_get_number(p, doc, "encrypt/nocopy")!= 0) ? "no":"yes"));

        printf("  Linearized: %s\n\n",
            ((PDF_pcos_get_number(p, doc, "linearized") != 0) ? "yes" : "no"));

        if (!strcmp(pcosmode, "minimum"))
        {
            printf("Minimum mode: no more information available\n\n");
            PDF_delete(p);
            return(0);
        }

        /* --------- more details (requires at least user password) */
        printf("PDF/X status: %s\n", PDF_pcos_get_string(p, doc, "pdfx"));

        printf("PDF/A status: %s\n", PDF_pcos_get_string(p, doc, "pdfa"));

        xfa_present =
            (int) PDF_pcos_get_number(p, doc, "type:/Root/AcroForm/XFA")
                    != pcos_ot_null;
        printf("    XFA data: %s\n", xfa_present ? "yes" : "no");

        printf("  Tagged PDF: %s\n",
            (PDF_pcos_get_number(p, doc, "tagged") != 0) ? "yes" : "no");

        printf("No. of pages: %d\n",
             (int) PDF_pcos_get_number(p, doc, "length:pages"));

        printf(" Page 1 size: width=%.3f, height=%.3f\n",
             PDF_pcos_get_number(p, doc, "pages[0]/width"),
             PDF_pcos_get_number(p, doc, "pages[0]/height"));

        count = (int) PDF_pcos_get_number(p, doc, "length:fonts");
        printf("No. of fonts: %d\n",  count);

        for (i = 0; i < count; i++)
        {
            if (PDF_pcos_get_number(p, doc, "fonts[%d]/embedded", i) != 0)
                printf("embedded ");
            else
                printf("unembedded ");

            printf("%s font ",PDF_pcos_get_string(p, doc, "fonts[%d]/type", i));
            printf("%s\n", PDF_pcos_get_string(p, doc, "fonts[%d]/name", i));
        }

        printf("\n");

        plainmetadata =
                PDF_pcos_get_number(p, doc, "encrypt/plainmetadata") != 0;

        if (!strcmp(pcosmode, "restricted")  && !plainmetadata
                && PDF_pcos_get_number(p, doc, "encrypt/nocopy") != 0)
        {
            printf("Restricted mode: no more information available");
            PDF_delete(p);
            return(0);
        }

        /* ----- document info keys and XMP metadata (requires master pw) */

        count = (int) PDF_pcos_get_number(p, doc, "length:/Info");

        for (i = 0; i < count; i++)
        {
            objtype = (int) PDF_pcos_get_number(p, doc, "type:/Info[%d]",i);
            printf("%12s: ", PDF_pcos_get_string(p, doc, "/Info[%d].key", i));

            /* Info entries can be stored as string or name objects */
            if (objtype ==  pcos_ot_name || objtype == pcos_ot_string)
            {
                printf("'%s'\n", PDF_pcos_get_string(p, doc, "/Info[%d]", i));
            }
            else
            {
                printf("(%s object)\n",
                    PDF_pcos_get_string(p, doc, "type:/Info[%d]", i));
            }
        }

        printf("\nXMP metadata: ");

        objtype = (int) PDF_pcos_get_number(p, doc, "type:/Root/Metadata");
        if (objtype == pcos_ot_stream)
        {
            const unsigned char *contents;
            int len;

            contents = PDF_pcos_get_stream(p, doc, &len, "", "/Root/Metadata");
            printf("%d UTF-8 bytes \n\n", len);
        }
        else
        {
            printf("not present\n");
        }

        PDF_close_pdi_document(p, doc);
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
