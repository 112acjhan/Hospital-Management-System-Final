//
// PDFlib client: creation of a business card in Java
// PDFlib client: fill Blocks with PPS to create a business card
// Required software: PDFlib Personalization Server (PPS)
//

#include <iostream>
#include "pdflib.hpp"

using namespace std;
using namespace pdflib;

int
main(void)
{
    try {
        PDFlib p;
        int         i, blockcontainer, page, pagecount, pageno;
        const wstring infile = L"boilerplate.pdf";
        /* This is where font/image/PDF input files live. Adjust as necessary.
         * Note that this directory must also contain the font files.
         */
        const wstring searchpath = L"../data";

        /* By default annotations are also imported. In some cases this
         * requires the Noto fonts for creating annotation appearance streams.
         */
    
        const wstring fontpath = L"../../resource/font";

        wostringstream optlist;
        struct blockdata {
            blockdata(wstring n, wstring v): name(n), value(v) {}
            wstring name;
            wstring value;
        };

        const blockdata data[] = {
            blockdata(L"name",                   L"Victor Kraxi"),
            blockdata(L"business.title",         L"Chief Paper Officer"),
            blockdata(L"business.address.line1", L"17, Aviation Road"),
            blockdata(L"business.address.city",  L"Paperfield"),
            blockdata(L"business.telephone.voice",L"phone +1 234 567-89"),
            blockdata(L"business.telephone.fax", L"fax +1 234 567-98"),
            blockdata(L"business.email",         L"victor@kraxi.com"),
            blockdata(L"business.homepage",      L"www.kraxi.com"),
        };

        const int BLOCKCOUNT = sizeof(data) / sizeof(blockdata);

        // This means we must check return values of load_font() etc.
        // Set the search path for fonts and PDF files
        optlist.str(L"");
        optlist << L"errorpolicy=return searchpath={{" << searchpath << L"}}";
        p.set_option(optlist.str());

        optlist.str(L"");
        optlist << L"searchpath={{" << fontpath << L"}}";
        p.set_option(optlist.str());

        if (p.begin_document(L"businesscard.pdf", L"") == -1) {
            wcerr << L"Error: " << p.get_errmsg() << endl;
            return(2);
        }

        p.set_info(L"Creator", L"businesscard");
        p.set_info(L"Author", L"Thomas Merz");
        p.set_info(L"Title", L"PDFlib Block processing sample");

        blockcontainer = p.open_pdi_document(infile, L"");
        if (blockcontainer == -1) {
            wcerr << L"Error: " << p.get_errmsg() << endl; return(2);
        }

        pagecount = (int) p.pcos_get_number(blockcontainer, L"length:pages");

        for (pageno = 1; pageno <= pagecount; pageno++){

            page = p.open_pdi_page(blockcontainer, pageno, L"");
            if (page == -1) {
                wcerr << L"Error: " << p.get_errmsg() << endl; return(2);
            }

            p.begin_page_ext(20, 20, L"");              // dummy page size

            // This will adjust the page size to the Block container's size.
            p.fit_pdi_page(page, 0, 0, L"adjustpage");

            // Fill all text Blocks with dynamic data 
            for (i = 0; i < (int) BLOCKCOUNT; i++) {
                if (p.fill_textblock(page, data[i].name, data[i].value,
                    L"") == -1) {
                        wcerr << L"Error: " << p.get_errmsg() << endl;
                }
            }

            p.end_page_ext(L"");
            p.close_pdi_page(page);
        }

        p.end_document(L"");
        p.close_pdi_document(blockcontainer);
    }
    catch (PDFlib::Exception &ex) {
        wcerr << L"PDFlib exception occurred in businesscard sample: " << endl;
        wcerr << L"[" << ex.get_errnum() << L"] " << ex.get_apiname()
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
