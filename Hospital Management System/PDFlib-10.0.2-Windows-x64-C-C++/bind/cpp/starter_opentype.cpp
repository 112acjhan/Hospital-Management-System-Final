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

#include <iostream>
#include <string>

#include "pdflib.hpp"

using namespace std;
using namespace pdflib;

int
main(void)
{
    /* This is where the data files are. Adjust as necessary. */
    const wstring searchpath = L"../data";
    const wstring outfile = L"starter_opentype.pdf";

    wostringstream optlist;
    PDFlib p;
    int row, col, table, test, font;
    const double llx=50, lly=50, urx=800, ury=550;
    wstring result;

    /* This font will be used unless another one is specified in the table */
    const wstring defaulttestfont = L"NotoSerif-Regular";

    const wstring header[] = {
        L"OpenType feature",
        L"Option list",
        L"Font name",
        L"Raw input (feature disabled)",
        L"Feature enabled"
    };
    const int MAXCOL = sizeof(header) / sizeof(header[0]);

    typedef struct {
        const wstring description;
        /* the default test font above will be used if this string is empty */
        const wstring fontname;
        const wstring feature;
        const wstring text;
    } testcase;

    static const testcase testcases[] = {
    {
      L"ligatures",
      L"",
      L"liga",
       L"ff fi fl ffi ffl"
    },
    {
      L"discretionary ligatures",
      L"",
      L"dlig",
       L"ch tz"
    },
    {
      L"small capitals",
      L"",
      L"smcp",
       L"PostScript"
    },
    {
      L"ordinals",
      L"",
      L"ordn",
       L"1o 2a 3o"
    },
    {
      L"fractions",
      L"",
      L"frac",
       L"1/2 1/4 3/4"
    },
    {
      L"slashed zero",
      L"",
      L"zero",
       L"0"
    },
    {
      L"historical forms",
      L"",
      L"hist",
       L"s"
    },
    {
      L"proportional figures",
      L"",
      L"pnum",
       L"0123456789"
    },
    {
      L"old-style figures",
      L"",
      L"onum",
       L"0123456789"
    },
    {
      L"lining figures",
      L"",
      L"lnum",
       L"0123456789"
    },
    {
      L"superscript",
      L"",
      L"sups",
       L"123"
    },
    {
      L"subscript",
      L"",
      L"subs",
       L"H2SO4"
    },
    };
    const int n_testcases = sizeof(testcases) / sizeof(testcases[0]);

    try {
        /* This means that formatting and other errors will raise an
         * exception. This simplifies our sample code, but is not
         * recommended for production code.
         */
        p.set_option(L"errorpolicy=exception");

        // Set the search path for fonts and PDF files
        optlist.str(L"");
        optlist << L"searchpath={{" << searchpath << L"}} charref=true";
        p.set_option(optlist.str());


        /* Set an output path according to the name of the topic */
        if (p.begin_document(outfile, L"") == -1) {
            wcerr << L"Error: " << p.get_errmsg() << endl;
            return 2;
        }

        p.set_info(L"Creator", L"PDFlib starter sample");
        p.set_info(L"Title", L"starter_opentype");

        table = -1;

        /* Table header */
        for (row=1, col=1; col <= MAXCOL; col++)
        {
            optlist.str(L"");
            optlist << L"fittextline={fontname=NotoSerif-Regular "
                       L"fontsize=12} margin=4";
            table = p.add_table_cell(table, col, row, header[col-1],
                                        optlist.str());
        }

        /* Create a table with feature samples, one feature per table row */
        for (row=2, test=0; test < n_testcases; row++, test++)
        {
            wstring testfont;
            wostringstream buf;

            /* Use the entry in the test table if available, and the
             * default test font otherwise. This way we can easily check
             * a font for all features, as well as insert suitable fonts
             * for individual features.
             */
            if (testcases[test].fontname.length() > 0)
                testfont = testcases[test].fontname;
            else
                testfont = defaulttestfont;

            col = 1;

            /* Common option list for columns 1-3 */
            optlist.str(L"");
            optlist << L"fittextline={fontname=NotoSerif-Regular "
                       L"fontsize=12} margin=4";

            /* Column 1: feature description */
            table = p.add_table_cell(table, col++, row,
                testcases[test].description,optlist.str());

            /* Column 2: option list */
            buf << L"features={" << testcases[test].feature << L"}";
            table = p.add_table_cell(table, col++, row, buf.str(),
                                        optlist.str());

            /* Column 3: font name */
            table = p.add_table_cell(table, col++, row, testfont,
                                        optlist.str());

            /* Column 4: raw input text with  feature disabled */
            optlist.str(L"");
            optlist << L"fittextline={fontname={" << testfont << L"} "
                       L"fontsize=12} margin=4";
            table = p.add_table_cell(table, col++, row,
                    testcases[test].text, optlist.str());

            /* Column 5: text with enabled feature, or warning if the
             * feature is not available in the font
             */
            font = p.load_font(testfont, L"unicode", L"");

            /* Check whether font contains the required feature table */
            optlist.str(L"");
            optlist << L"name=" << testcases[test].feature;
            if (p.info_font(font, L"feature", optlist.str()) == 1)
            {
                /* feature is available: apply it to the text */                optlist.clear();
                optlist.str(L"");
                optlist << L"fittextline={fontname={" << testfont << L"} "
                    << L"fontsize=12 features={"
                    << testcases[test].feature << L"}} margin=4";
                table = p.add_table_cell(table, col++, row,
                   testcases[test].text, optlist.str());
            }
            else
            {
                /* feature is not available: emit a warning */
                optlist.str(L"");
                optlist << L"fittextline={fontname=NotoSerif-Regular "
                           L"fontsize=12 fillcolor=red} margin=4";
                table = p.add_table_cell(table, col++, row,
                        L"(feature not available in this font)", optlist.str());
            }
        }
        /*
         * Loop until all of the table is placed; create new pages
         * as long as more table instances need to be placed.
         */
        do {
            p.begin_page_ext(0, 0, L"width=a4.height height=a4.width");

            /* Shade every other row; draw lines for all table cells. */
            optlist.str(L"");
            optlist << L"header=1 fill={{area=rowodd "
                       L"fillcolor={gray 0.9}}} stroke={{line=other}}";

            /* Place the table instance */
            result = p.fit_table(table, llx, lly, urx, ury, optlist.str());

            if (result == L"_error") {
                wcerr << L"Couldn't place table: " << p.get_errmsg() << endl;
                return 2;
            }

            p.end_page_ext(L"");

        } while (result == L"_boxfull");

        p.end_document(L"");
    }
    catch (PDFlib::Exception &ex) {
        wcerr << L"PDFlib exception occurred:" << endl
              << L"[" << ex.get_errnum() << L"] " << ex.get_apiname()
              << L": " << ex.get_errmsg() << endl;
        return 99;
    }
    catch (exception &e) {
        cerr << "C++ exception occurred: " << e.what() << endl;
        return 99;
    }
    catch (...) {
        cerr << "Generic C++ exception occurred!" << endl;
        return 99;
    }

    return 0;
}
