/* 
 * Starter sample for text shaping features
 * Demonstrate text shaping for Arabic, Hebrew, and Devanagari scripts
 * Right-to-left text is reordered according to the Bidi algorithm.
 *
 * Required data: suitable fonts for the scripts
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
    const wstring outfile = L"starter_shaping.pdf";

    wostringstream optlist;
    PDFlib p;
    int row, col, table, tf;
    const double llx=25, lly=25, urx=825, ury=550;
    wstring result;

    const wstring header[] = {
        L"Language", L"Raw input", L"Reordered and shaped output"
    };
    const int MAXCOL = sizeof(header) / sizeof(header[0]);

    struct shaping {
        const wstring fontopt;  /* ontname and other font options */
        const wstring optlist;  /* text options */
        const int  textflow;    /* can't use Textflow for Bidi text */
        const wstring language; /* language name */
        const wstring text;     /* sample text */
    };

    const struct shaping shapingsamples[] = {
    /* Dummys to compensate for header row and +/-1 indexing in C */
    { L"", L"", 0, L"", L"" /* dummy1 */ },
    { L"", L"", 0, L"", L"" /* dummy2 */ },

    /* -------------------------- Arabic -------------------------- */
    { L"fontname=NotoNaskhArabic-Regular fallbackfonts={ {fontname=NotoSerif-Regular} }", L"shaping script=arab", 0, L"Arabic",
    L"&#x0627;&#x0644;&#x0639;&#x064E;&#x0631;&#x064E;&#x0628;&#x0650;"
    L"&#x064A;&#x0629;" },

    { L"fontname=NotoNaskhArabic-Regular fallbackfonts={ {fontname=NotoSerif-Regular} }", L"shaping script=arab", 0, L"Arabic",
    L"&#x0645;&#x0631;&#x062D;&#x0628;&#x0627;! (Hello)"
    },

    { L"fontname=NotoNaskhArabic-Regular fallbackfonts={ {fontname=NotoSerif-Regular} }", L"shaping script=arab", 0, L"Arabic",
    L"&#xFEFF;&#x0627;&#x0644;&#x0645;&#x0627;&#x062F;&#x0629;&#x0020;"
    L"&#x0031;&#x0020;&#x064A;&#x0648;&#x0644;&#x062F;&#x0020;&#x062C;"
    L"&#x0645;&#x064A;&#x0639;&#x0020;&#x0627;&#x0644;&#x0646;&#x0627;"
    L"&#x0633;&#x0020;&#x0623;&#x062D;&#x0631;&#x0627;&#x0631;&#x064B;"
    L"&#x0627;&#x0020;&#x0645;&#x062A;&#x0633;&#x0627;&#x0648;&#x064A;"
    L"&#x0646;&#x0020;&#x0641;&#x064A;&#x0020;&#x0627;&#x0644;&#x0643;"
    L"&#x0631;&#x0627;&#x0645;&#x0629;&#x0020;&#x0648;&#x0627;&#x0644;"
    L"&#x062D;&#x0642;&#x0648;&#x0642;&#x002E;&#x0020;"
    },

    { L"fontname=NotoNaskhArabic-Regular fallbackfonts={ {fontname=NotoSerif-Regular} }", L"shaping script=arab", 0, L"Arabic",
    L"&#x0648;&#x0642;&#x062F;&#x0020;&#x0648;&#x0647;&#x0628;&#x0648;"
    L"&#x0627;&#x0020;&#x0639;&#x0642;&#x0644;&#x0627;&#x064B;&#x0020;"
    L"&#x0648;&#x0636;&#x0645;&#x064A;&#x0631;&#x064B;&#x0627;&#x0020;"
    L"&#x0648;&#x0639;&#x0644;&#x064A;&#x0647;&#x0645;&#x0020;&#x0623;"
    L"&#x0646;&#x0020;&#x064A;&#x0639;&#x0627;&#x0645;&#x0644;&#x0020;"
    L"&#x0628;&#x0639;&#x0636;&#x0647;&#x0645;&#x0020;&#x0628;&#x0639;"
    L"&#x0636;&#x064B;&#x0627;&#x0020;&#x0628;&#x0631;&#x0648;&#x062D;"
    L"&#x0020;&#x0627;&#x0644;&#x0625;&#x062E;&#x0627;&#x0621;&#x002E;"
    },

    /* -------------------------- Hebrew -------------------------- */
    { L"fontname=NotoSerifHebrew-Regular fallbackfonts={ {fontname=NotoSerif-Regular} }", L"shaping script=hebr", 0, L"Hebrew",
      L"&#x05E2;&#x05B4;&#x05D1;&#x05B0;&#x05E8;&#x05B4;&#x05D9;&#x05EA;"
    },

    { L"fontname=NotoSerifHebrew-Regular fallbackfonts={ {fontname=NotoSerif-Regular} }", L"shaping script=hebr", 0, L"Hebrew",
    L"&#x05E1;&#x05E2;&#x05D9;&#x05E3;&#x0020;&#x05D0;&#x002E;&#x0020;"
    L"&#x05DB;&#x05DC;&#x0020;&#x05D1;&#x05E0;&#x05D9;&#x0020;&#x05D0;"
    L"&#x05D3;&#x05DD;&#x0020;&#x05E0;&#x05D5;&#x05DC;&#x05D3;&#x05D5;"
    L"&#x0020;&#x05D1;&#x05E0;&#x05D9;&#x0020;&#x05D7;&#x05D5;&#x05E8;"
    L"&#x05D9;&#x05DF;&#x0020;&#x05D5;&#x05E9;&#x05D5;&#x05D5;&#x05D9;"
    L"&#x05DD;&#x0020;&#x05D1;&#x05E2;&#x05E8;&#x05DB;&#x05DD;&#x0020;"
    L"&#x05D5;&#x05D1;&#x05D6;&#x05DB;&#x05D5;&#x05D9;&#x05D5;&#x05EA;"
    L"&#x05D9;&#x05D4;&#x05DD;&#x002E;&#x0020;"
    },

    { L"fontname=NotoSerifHebrew-Regular fallbackfonts={ {fontname=NotoSerif-Regular} }", L"shaping script=hebr", 0, L"Hebrew",
    L"&#x05DB;&#x05D5;&#x05DC;&#x05DD;&#x0020;&#x05D7;&#x05D5;&#x05E0;"
    L"&#x05E0;&#x05D5;&#x0020;&#x05D1;&#x05EA;&#x05D1;&#x05D5;&#x05E0;"
    L"&#x05D4;&#x0020;&#x05D5;&#x05D1;&#x05DE;&#x05E6;&#x05E4;&#x05D5;"
    L"&#x05DF;&#x002C;&#x0020;"
    },

    { L"fontname=NotoSerifHebrew-Regular fallbackfonts={ {fontname=NotoSerif-Regular} }", L"shaping script=hebr", 0, L"Hebrew",
    L"&#x05DC;&#x05E4;&#x05D9;&#x05DB;&#x05DA;&#x0020;&#x05D7;&#x05D5;"
    L"&#x05D1;&#x05D4;&#x0020;&#x05E2;&#x05DC;&#x05D9;&#x05D4;&#x05DD;"
    L"&#x0020;&#x05DC;&#x05E0;&#x05D4;&#x05D5;&#x05D2;&#x0020;&#x05D0;"
    L"&#x05D9;&#x05E9;&#x0020;&#x05D1;&#x05E8;&#x05E2;&#x05D4;&#x05D5;"
    L"&#x0020;&#x05D1;&#x05E8;&#x05D5;&#x05D7;&#x0020;&#x05E9;&#x05DC;"
    L"&#x0020;&#x05D0;&#x05D7;&#x05D5;&#x05D4;&#x002E;"
    },

    /* -------------------------- Hindi -------------------------- */
    { L"fontname=NotoSerifDevanagari-Regular fallbackfonts={ {fontname=NotoSerif-Regular} }", L"shaping script=deva", 1, L"Hindi",
      L"&#x0939;&#x093F;&#x0928;&#x094D;&#x0926;&#x0940;"
    },

    { L"fontname=NotoSerifDevanagari-Regular fallbackfonts={ {fontname=NotoSerif-Regular} }", L"shaping script=deva advancedlinebreak", 1, L"Hindi",
    L"&#x0905;&#x0928;&#x0941;&#x091A;&#x094D;&#x091B;&#x0947;&#x0926;"
    L"&#x0020;&#x0967;&#x002E;&#x0020;&#x0938;&#x092D;&#x0940;&#x0020;"
    L"&#x092E;&#x0928;&#x0941;&#x0937;&#x094D;&#x092F;&#x094B;&#x0902;"
    L"&#x0020;&#x0915;&#x094B;&#x0020;&#x0917;&#x094C;&#x0930;&#x0935;"
    L"&#x0020;&#x0914;&#x0930;&#x0020;&#x0905;&#x0927;&#x093F;&#x0915;"
    L"&#x093E;&#x0930;&#x094B;&#x0902;&#x0020;&#x0915;&#x0947;&#x0020;"
    L"&#x092E;&#x093E;&#x092E;&#x0932;&#x0947;&#x0020;&#x092E;&#x0947;"
    L"&#x0902;&#x0020;&#x091C;&#x0928;&#x094D;&#x092E;&#x091C;&#x093E;"
    L"&#x0924;&#x0020;&#x0938;&#x094D;&#x0935;&#x0924;&#x0928;&#x094D;"
    L"&#x0924;&#x094D;&#x0930;&#x0924;&#x093E;&#x0020;&#x0914;&#x0930;"
    L"&#x0020;&#x0938;&#x092E;&#x093E;&#x0928;&#x0924;&#x093E;&#x0020;"
    L"&#x092A;&#x094D;&#x0930;&#x093E;&#x092A;&#x094D;&#x0924;&#x0020;"
    L"&#x0939;&#x0948;&#x0020;&#x0964;&#x0020;&#x0909;&#x0928;&#x094D;"
    L"&#x0939;&#x0947;&#x0902;&#x0020;&#x092C;&#x0941;&#x0926;&#x094D;"
    L"&#x0918;&#x093F;&#x0020;&#x0914;&#x0930;&#x0020;&#x0905;&#x0928;"
    L"&#x094D;&#x0924;&#x0930;&#x093E;&#x0924;&#x094D;&#x092E;&#x093E;"
    L"&#x0020;&#x0915;&#x0940;&#x0020;&#x0926;&#x0947;&#x0928;&#x0020;"
    L"&#x092A;&#x094D;&#x0930;&#x093E;&#x092A;&#x094D;&#x0924;&#x0020;"
    L"&#x0939;&#x0948;&#x0020;&#x0914;&#x0930;&#x0020;&#x092A;&#x0930;"
    L"&#x0938;&#x094D;&#x092A;&#x0930;&#x0020;&#x0909;&#x0928;&#x094D;"
    L"&#x0939;&#x0947;&#x0902;&#x0020;&#x092D;&#x093E;&#x0908;&#x091A;"
    L"&#x093E;&#x0930;&#x0947;&#x0020;&#x0915;&#x0947;&#x0020;&#x092D;"
    L"&#x093E;&#x0935;&#x0020;&#x0938;&#x0947;&#x0020;&#x092C;&#x0930;"
    L"&#x094D;&#x0924;&#x093E;&#x0935;&#x0020;&#x0915;&#x0930;&#x0928;"
    L"&#x093E;&#x0020;&#x091A;&#x093E;&#x0939;&#x093F;&#x090F;&#x0020;"
    L"&#x0964;"
    },

    /* -------------------------- Sanskrit -------------------------- */
    { L"fontname=NotoSerifDevanagari-Regular fallbackfonts={ {fontname=NotoSerif-Regular} }", L"shaping script=deva", 1, L"Sanskrit",
    L"&#x0938;&#x0902;&#x0938;&#x094D;&#x0915;&#x0943;&#x0924;&#x092E;"
    L"&#x094D;"
    },

    };
    const int MAXROW = sizeof(shapingsamples) / sizeof(shapingsamples[0]);

    try {
        /* This means that formatting and other errors will raise an
         * exception. This simplifies our sample code, but is not
         * recommended for production code.
         */
        
        //  This means we must check return values of load_font() etc.
        p.set_option(L"errorpolicy=exception");
        
        // Set the search path for fonts and PDF files
        optlist.str(L"");
        optlist << L"searchpath={{" << searchpath << L"}} charref";
        p.set_option(optlist.str());

        /* Set an output path according to the name of the topic */
        if (p.begin_document(outfile, L"") == -1) {
            wcerr << L"Error: " << p.get_errmsg() << endl;
            return 2;
        }

        p.set_info(L"Creator", L"PDFlib starter sample");
        p.set_info(L"Title", L"starter_shaping");

        table = -1;

        /* Create table header */
        for (row=1, col=1; col <= MAXCOL; col++)
        {
            optlist.str(L"");
            optlist
                << L"fittextline={fontname=NotoSerif-Regular "
                   L"fontsize=14} colwidth=" << (col==1 ? L"10%" : L"45%");
            table = p.add_table_cell(table, col, row, header[col-1],
                    optlist.str());
        }

        /* Create shaping samples */
        for (row=2; row < MAXROW; row++)
        {
            col=1;

            /* Column 1: language name */
            optlist.str(L"");
            optlist << L"fittextline={fontname=NotoSerif-Regular "
                       L"fontsize=12}";
            table = p.add_table_cell(table, col++, row,
                                shapingsamples[row].language, optlist.str());

            /* Column 2: raw text */
            optlist.str(L"");
            optlist << shapingsamples[row].fontopt
                << L" fontsize=12 leading=150% alignment=left";
            tf = p.create_textflow(shapingsamples[row].text, optlist.str());
            optlist.str(L"");
            optlist << L"margin=4 fittextflow={verticalalign=top} textflow="
                << tf;
            table = p.add_table_cell(table, col++, row, L"", optlist.str());

            /* Column 3: shaped and reordered text (Textline or Textflow) */
            if (shapingsamples[row].textflow) {
                optlist.str(L"");
                optlist << shapingsamples[row].fontopt
                    << L" " << shapingsamples[row].optlist
                    << L" fontsize=12 leading=150% alignment=left";
                tf = p.create_textflow(shapingsamples[row].text, optlist.str());
                optlist.str(L"");
                optlist << L"margin=4 fittextflow={verticalalign=top} "
                    << L"textflow=" << tf;
                table = p.add_table_cell(table, col++, row, L"", optlist.str());
            }
            else {
                optlist.str(L"");
                optlist << L"fittextline={"
                    << shapingsamples[row].fontopt << L" " 
                    << shapingsamples[row].optlist
                    << L" fontsize=12 }";
                table = p.add_table_cell(table, col++, row,
                         shapingsamples[row].text, optlist.str());
            }
        }

        /* ---------- Place the table on one or more pages ---------- */
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
