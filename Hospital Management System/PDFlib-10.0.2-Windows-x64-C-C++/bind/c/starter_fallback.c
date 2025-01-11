/* 
 * Starter sample for fallback fonts
 *
 * Required data: suitable fonts
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
    const char* outfile = "starter_fallback.pdf";

#define OPTLISTSIZE 256  
    char optlist[OPTLISTSIZE];
    PDF *p;
    int row, col, table, test;
    double llx= 30, lly=30, urx=800, ury=570;
    const char *result;

    const char *header[] = {
        "Use case",
        "Option list for the 'fallbackfonts' option",
        "Base font",
        "With fallback font"
    };
    int MAXCOL = sizeof(header)/sizeof(header[0]);

    typedef struct {
        const char *usecase;
        const char *fontname;
        const char *fallbackoptions;
        const char *text;
    } testcase;

    static const testcase testcases[] = {
    {
      "Add enlarged pictogram",
      "NotoSerif-Regular",
      /* U+261E = WHITE RIGHT POINTING INDEX  */
      "{fontname=ZapfDingbats forcechars=U+261E fontsize=150% "
      "textrise=-15%}",
       "hand symbol: &#x261E;"
    },
    {
      "Add enlarged symbol glyph",
      "NotoSerif-Regular",
      "{fontname=Symbol forcechars=U+2663 fontsize=125%}",
       "club symbol: &#x2663;"
    },
    { /* Hebrew characters missing in the font will be pulled from Hebrew font*/
      "Add Hebrew characters to Latin font",
      "NotoSerif-Regular",
      "{fontname=NotoSerifHebrew-Regular}",
       "Hebrew: &#x05D0;"
    },
    };
    int n_testcases = sizeof(testcases)/sizeof(testcases[0]);

    /* create a new PDFlib object */
    if ((p = PDF_new()) == (PDF *) 0) {
        printf("Couldn't create PDFlib object (out of memory)\n");
        return(2);
    }

    PDF_TRY(p) {
        snprintf(optlist, OPTLISTSIZE, "SearchPath={{%s}}", searchpath);
        PDF_set_option(p, optlist);

        PDF_set_option(p, "textformat=utf8 charref=true glyphcheck=replace");

        /* This means that formatting and other errors will raise an
         * exception. This simplifies our sample code, but is not
         * recommended for production code.
         */
        PDF_set_option(p, "errorpolicy=exception");

        /* Set an output path according to the name of the topic */
        if (PDF_begin_document(p, outfile, 0, "") == -1) {
            printf("Error: %s\n", PDF_get_errmsg(p));
            PDF_delete(p);
            return(2);
        }

        PDF_set_info(p, "Creator", "PDFlib starter sample");
        PDF_set_info(p, "Title", "starter_fallback");

        /* Start Page */
        PDF_begin_page_ext(p, 0, 0, "width=a4.height height=a4.width");

        table = -1;

        /* Table header */
        for (row=1, col=1; col <= MAXCOL; col++)
        {
            snprintf(optlist, OPTLISTSIZE,
           "fittextline={fontname=NotoSerif-Regular fontsize=10} "
           "margin=4"
           );
            table = PDF_add_table_cell(p, table, col, row, header[col-1], 0,
                    optlist);
        }

        /* Create fallback samples, one use case per row */
        for (row=2, test=0; test < n_testcases; row++, test++)
        {
            col=1;

            /* Column 1: description of the use case */
            snprintf(optlist, OPTLISTSIZE,
            "fittextline={fontname=NotoSerif-Regular "
            "fontsize=10} margin=4");
            table = PDF_add_table_cell(p, table, col++, row,
                testcases[test].usecase, 0, optlist);

            /* Column 2: reproduce option list literally */
            snprintf(optlist, OPTLISTSIZE,
            "fittextline={fontname=NotoSerif-Regular "
            "fontsize=10} margin=4");
            table = PDF_add_table_cell(p, table, col++, row,
                testcases[test].fallbackoptions, 0, optlist);

            /* Column 3: text with base font */
            snprintf(optlist, OPTLISTSIZE,
                 "fittextline={fontname=%s fontsize=10 "
                 "replacementchar=? } margin=4",
                 testcases[test].fontname);
            table = PDF_add_table_cell(p, table, col++, row,
                    testcases[test].text, 0, optlist);

            /* Column 4: text with base font and fallback fonts */
            snprintf(optlist, OPTLISTSIZE,
                 "fittextline={fontname=%s "
                 "fontsize=10 fallbackfonts={%s}} margin=4",
                 testcases[test].fontname,
                 testcases[test].fallbackoptions);
            table = PDF_add_table_cell(p, table, col++, row,
               testcases[test].text, 0, optlist);
        }

        /* Place the table */
        snprintf(optlist, OPTLISTSIZE, "header=1 fill={{area=rowodd "
            "fillcolor={gray 0.9}}} stroke={{line=other}} ");
        result = PDF_fit_table(p, table, llx, lly, urx, ury, optlist);

        if (!strcmp(result, "_error"))
        {
            printf("Couldn't place table: %s\n", PDF_get_errmsg(p));
            PDF_delete(p);
            return(2);
        }

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
