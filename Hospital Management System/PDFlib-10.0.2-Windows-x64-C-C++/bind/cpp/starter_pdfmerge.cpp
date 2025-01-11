/* 
 * PDF merge starter:
 * Merge pages from multiple PDF documents (including annotations)
 *
 * Required software: PDFlib+PDI or PDFlib Personalization Server (PPS)
 * Required data: PDF documents
 */


#include <iostream>

#include "pdflib.hpp"

using namespace std;
using namespace pdflib;

int
main(void)
{
    try {
        /* This is where the data files are. Adjust as necessary. */
        const wstring searchpath = L"../data";

        /* By default annotations are also imported. In some cases this
         * requires the Noto fonts for creating annotation appearance streams.
         */
        const wstring fontpath = L"../../resource/font";

        PDFlib p;
        const wstring pdffiles[] =
        {
                L"markup_annotations.pdf",
                L"PLOP-datasheet.pdf",
                L"pCOS-datasheet.pdf"
        };
        const int filecount = sizeof(pdffiles) / sizeof(pdffiles[0]);
        int i;

        wostringstream optlist;

        //  This means we must check return values of load_font() etc.
        p.set_option(L"errorpolicy=return");

        // Set the search path for fonts and PDF files
        optlist.str(L"");
        optlist << L"searchpath={{" << searchpath << L"}}";
        p.set_option(optlist.str());

        optlist.str(L"");
        optlist << L"searchpath={{" << fontpath << L"}}";
        p.set_option(optlist.str());

        if (p.begin_document(L"starter_pdfmerge.pdf", L"") == -1) {
            wcerr << L"Error: " << p.get_errmsg() << endl;
            return 2;
        }
        p.set_info(L"Creator", L"PDFlib starter sample");
        p.set_info(L"Title", L"starter_pdfmerge");

        for (i = 0; i < filecount; i++) {
            int indoc, endpage, pageno, page; 

            /* Open the input PDF */
            indoc = p.open_pdi_document(pdffiles[i], L"");
            if (indoc == -1) {
                wcerr << L"Error: " << p.get_errmsg() << endl;
                return 2;
            }

            endpage = static_cast<int>(p.pcos_get_number(indoc, L"length:pages"));


            /* Loop over all pages of the input document */
            for (pageno = 1; pageno <= endpage; pageno++)
            {
                page = p.open_pdi_page(indoc, pageno, L"");
                if (page == -1) {
                    wcerr << L"Error: " << p.get_errmsg() << endl;
                    continue;
                }
                /* Dummy page size; will be adjusted later */
                p.begin_page_ext(10, 10, L"");

                /* Create a bookmark with the file name */
                if (pageno == 1)
                    p.create_bookmark(pdffiles[i], L"");

                /* Place the imported page on the output page, and
                 * adjust the page size. If the page contains annotations
                 * these are also imported.
                 */
                p.fit_pdi_page(page, 0, 0, L"adjustpage");
                p.close_pdi_page(page);

                p.end_page_ext(L"");
            }
            p.close_pdi_document(indoc);
        }

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
