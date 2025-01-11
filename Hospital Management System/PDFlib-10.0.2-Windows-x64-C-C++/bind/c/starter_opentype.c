/*
 * Starter sample for OpenType font features
 *
 * Demonstrate various typographic OpenType features after checking
 * whether a particular feature is supported in a font.
 *
 * Required data: suitable font with OpenType feature tables
 *
 * This sample uses a default font which includes various features.
 * Depending on the implementation of the features you
 * may have to replace the sample text below.
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
    const char* outfile = "starter_opentype.pdf";

#define OPTLISTSIZE 256
    char optlist[OPTLISTSIZE];
    PDF *p;
    int row, col, table, test, font;
    const double llx= 50, lly=50, urx=800, ury=550;
    const char *result;

    /* This font will be used unless another one is specified in the table */
    const char *defaulttestfont = "NotoSerif-Regular";

    const char *header[] = {
        "OpenType feature",
        "Option list",
        "Font name",
        "Raw input (feature disabled)",
        "Feature enabled"
    };
    const int MAXCOL = sizeof(header) / sizeof(header[0]);

    typedef struct {
        const char *description;
        /* the default test font above will be used if this string is empty */
        const char *fontname;
        const char *feature;
        const char *text;
    } testcase;

    static const testcase testcases[] = {
    {
      "ligatures",
      "",
      "liga",
       "ff fi fl ffi ffl"
    },
    {
      "discretionary ligatures",
      "",
      "dlig",
       "ch tz"
    },
    {
      "small capitals",
      "",
      "smcp",
       "PostScript"
    },
    {
      "ordinals",
      "",
      "ordn",
       "1o 2a 3o"
    },
    {
      "fractions",
      "",
      "frac",
       "1/2 1/4 3/4"
    },
    {
      "slashed zero",
      "",
      "zero",
       "0"
    },
    {
      "historical forms",
      "",
      "hist",
       "s"
    },
    {
      "proportional figures",
      "",
      "pnum",
       "0123456789"
    },
    {
      "old-style figures",
      "",
      "onum",
       "0123456789"
    },
    {
      "lining figures",
      "",
      "lnum",
       "0123456789"
    },
    {
      "superscript",
      "",
      "sups",
       "123"
    },
    {
      "subscript",
      "",
      "subs",
       "H2SO4"
    },
    };
    const int n_testcases = sizeof(testcases) / sizeof(testcases[0]);

    /* create a new PDFlib object */
    if ((p = PDF_new()) == (PDF *) 0) {
        printf("Couldn't create PDFlib object (out of memory)\n");
        return(2);
    }

    PDF_TRY(p) {
        snprintf(optlist, OPTLISTSIZE, "SearchPath={{%s}}", searchpath);
        PDF_set_option(p, optlist);
        PDF_set_option(p, "textformat=utf8 charref=true");

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
        PDF_set_info(p, "Title", "starter_opentype");

        table = -1;

        /* Table header */
        for (row=1, col=1; col <= MAXCOL; col++)
        {
            snprintf(optlist, OPTLISTSIZE,
           "fittextline={fontname=NotoSerif-Regular fontsize=12} "
           "margin=4"
           );
            table = PDF_add_table_cell(p, table, col, row, header[col-1], 0,
                    optlist);
        }

        /* Create a table with feature samples, one feature per table row */
        for (row=2, test=0; test < n_testcases; row++, test++)
        {
            const char *testfont;
#define BUFSIZE 256  
            char buf[BUFSIZE];

            /* Use the entry in the test table if available, and the
             * default test font otherwise. This way we can easily check
             * a font for all features, as well as insert suitable fonts
             * for individual features.
             */
            if (testcases[test].fontname && *testcases[test].fontname)
                testfont = testcases[test].fontname;
            else
                testfont = defaulttestfont;

            col=1;

            /* Common option list for columns 1-3 */
            snprintf(optlist, OPTLISTSIZE,
            "fittextline={fontname=NotoSerif-Regular fontsize=12} "
            "margin=4");

            /* Column 1: feature description */
            table = PDF_add_table_cell(p, table, col++, row,
                testcases[test].description, 0, optlist);

            /* Column 2: option list */
            snprintf(buf, BUFSIZE, "features={%s}", testcases[test].feature);
            table = PDF_add_table_cell(p, table, col++, row, buf, 0, optlist);

            /* Column 3: font name */
            table = PDF_add_table_cell(p, table, col++, row, testfont, 0,
                    optlist);

            /* Column 4: raw input text with  feature disabled */
            snprintf(optlist, OPTLISTSIZE,
                 "fittextline={fontname={%s} fontsize=12 "
                 "} margin=4", testfont);
            table = PDF_add_table_cell(p, table, col++, row,
                    testcases[test].text, 0, optlist);

            /* Column 5: text with enabled feature, or warning if the
             * feature is not available in the font
             */
            font = PDF_load_font(p, testfont, 0,
                    "unicode", "");

            /* Check whether font contains the required feature table */
            snprintf(optlist, OPTLISTSIZE, "name=%s", testcases[test].feature);
            if (PDF_info_font(p, font, "feature", optlist) == 1)
            {
                /* feature is available: apply it to the text */
                snprintf(optlist, OPTLISTSIZE,
                     "fittextline={fontname={%s} fontsize=12 "
                     "features={%s}} margin=4",
                     testfont, testcases[test].feature);
                table = PDF_add_table_cell(p, table, col++, row,
                   testcases[test].text, 0, optlist);
            }
            else
            {
                /* feature is not available: emit a warning */
                snprintf(optlist, OPTLISTSIZE,
                     "fittextline={fontname=NotoSerif-Regular "
                     "fontsize=12 fillcolor=red} margin=4");
                table = PDF_add_table_cell(p, table, col++, row,
                        "(feature not available in this font)", 0, optlist);
            }

        }
        /*
         * Loop until all of the table is placed; create new pages as long
         * as more table instances need to be placed.
         */
        do {
            PDF_begin_page_ext(p, 0, 0, "width=a4.height height=a4.width");

            /* Shade every other row; draw lines for all table cells. */
            snprintf(optlist, OPTLISTSIZE, "header=1 fill={{area=rowodd "
                "fillcolor={gray 0.9}}} stroke={{line=other}} ");

            /* Place the table instance */
            result = PDF_fit_table(p, table, llx, lly, urx, ury, optlist);

            if (!strcmp(result, "_error"))
            {
                printf("Couldn't place table: %s\n", PDF_get_errmsg(p));
                PDF_delete(p);
                return(2);
            }

            PDF_end_page_ext(p, "");

        } while (!strcmp(result, "_boxfull"));

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
