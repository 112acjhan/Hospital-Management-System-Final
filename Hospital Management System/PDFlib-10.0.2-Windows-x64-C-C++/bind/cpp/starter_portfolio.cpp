/* 
 * PDF portfolio starter:
 * Package multiple PDF and other documents into a PDF portfolio.
 * The documents in the portfolio will be assigned predefined
 * viewing. The documents in the Portfolio will be assigned predefined
 * and custom metadata fields; for the custom fields a schema description
 * is created.
 *
 * Required data: PDF and other input documents
 */

#include <iostream>

#include "pdflib.hpp"

using namespace std;
using namespace pdflib;

int
main(void)
{
    /* This is where the data files are. Adjust as necessary. */
    const wstring searchpath = L"../data";
    wostringstream optlist;
    int i, font, folder;
    PDFlib p;

    typedef struct
    {
        const wstring filename;
        const wstring description;
        const wstring status;
        const int id;
    } portfolio;

    /* The documents for the Portfolio along with description and metadata */
    static const portfolio pf[] =
    {
    {
        L"nesrin.jpg",
        L"Zabrisky point",
        L"archived",
        300
    },
    {
        L"markup_annotations.pdf",
        L"PDF sample with markup annotations",
        L"published",
        101
    },
    {
        L"PLOP-datasheet.pdf",
        L"PDF Linearization, Optimization, Protection",
        L"published",
        103
    },
    {
        L"pCOS-datasheet.pdf",
        L"PDF Information Retrieval Tool",
        L"published",
        104
    }
    };
    const int n_files = sizeof(pf) / sizeof(pf[0]);

    try {
        //  This means we must check return values of load_font() etc.
        p.set_option(L"errorpolicy=return");

        // Set the search path for fonts and PDF files
        optlist.str(L"");
        optlist << L"searchpath={{" << searchpath << L"}}";
        p.set_option(optlist.str());


        if (p.begin_document(L"starter_portfolio.pdf",
                                    L"") == -1)
        {
            wcerr << L"Error: " << p.get_errmsg() << endl;
            return 2;
        }

        p.set_info(L"Creator", L"PDFlib starter sample");
        p.set_info(L"Title", L"starter_portfolio");

        /* Insert two files in the root folder along with their description
         * and the following custom fields:
         * status   string describing the document status
         * id       numerical identifier, prefixed with "PHX"
         */
        for (i = 0; i < 2; i++)
        {
            optlist.str(L"");
            optlist <<
                L"description={" << pf[i].description << L"} "
                L"fieldlist={ "
                    L"{key=status value=" << pf[i].status << L"} "
                    L"{key=id value=" << pf[i].id << L" prefix=PHX type=text} "
                L"}";

            /* -1 means root folder */
            p.add_portfolio_file(-1, pf[i].filename, optlist.str());
        }

        /* Create the "datasheets" folder in the root folder */
        folder = p.add_portfolio_folder(-1, L"datasheets", 
                L"description={Folder with datasheets}");

        /* Insert documents in the "datasheets" folder along with
         * description and custom fields
         */
        for (i = 2; i < n_files; i++)
        {
            optlist.str(L"");
            optlist <<
                L"description={" << pf[i].description << L"} "
                L"fieldlist={ "
                L"{key=status value=" << pf[i].status << L"} "
                L"{key=id value=" << pf[i].id << L" prefix=PHX type=text} "
                L"}";

            /* Add the file to the "datasheets" folder */
            p.add_portfolio_file(folder, pf[i].filename, optlist.str());
        }

    /* Create a single-page document as cover sheet */
    p.begin_page_ext(0, 0, L"width=a4.width height=a4.height");

    font = p.load_font(L"NotoSerif-Regular", L"unicode", L"");
    if (font == -1)
    {
        wcerr << L"Error: " << p.get_errmsg() << endl;
        return 2;
    }

    p.setfont(font, 24);
    p.fit_textline(L"Welcome to the PDFlib Portfolio sample!", 50, 700,
                    L"");

        p.end_page_ext(L"");

        /* Set options for Portfolio display */
        optlist.str(L"");
        optlist << L"portfolio={initialview=detail ";

        /* Add schema definition for Portfolio metadata */
    optlist <<
        L"schema={ "
        /* Some predefined fields are included here to make them visible. */
        L"{order=1 label=Name key=_filename visible editable} "
        L"{order=2 label=Description key=_description visible} "
        L"{order=3 label=Size key=_size visible} "
        L"{order=4 label={Last edited} key=_moddate visible} "

        /* User-defined fields */
        L"{order=5 label=Status key=status type=text editable} "
        L"{order=6 label=ID key=id type=text editable} ";

    optlist << L"}}";

        p.end_document(optlist.str());
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
