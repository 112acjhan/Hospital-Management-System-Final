//
// PDFlib client: hello example in C++
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
        wostringstream buf;
        /* This is where the data files are. Adjust as necessary. */
        const wstring searchpath = L"../data";
        const wstring fontopt = 
            L"fontname=NotoSerif-Regular fontsize=24";

        //  This means we must check return values of load_font() etc.
        p.set_option(L"errorpolicy=return");
        
        // Set the search path for fonts files	
        buf.str(L"");
        buf << L"searchpath={{" << searchpath << L"}}";
        p.set_option(buf.str());

        if (p.begin_document(L"hello.pdf", L"") == -1) {
            wcerr << "Error: " << p.get_errmsg() << endl;
            return 2;
        }

        p.set_info(L"Creator", L"hello.cpp");
        p.set_info(L"Author", L"Thomas Merz");
        p.set_info(L"Title", L"Hello World (C++ using std::wstring)!");

        p.begin_page_ext(0, 0, L"width=a4.width height=a4.height");

        p.fit_textline(L"Hello world!", 50, 700, fontopt);
        p.fit_textline(L"(says C++ using std::wstring)",  50, 676, fontopt);
        p.end_page_ext(L"");

        p.end_document(L"");
    }
    catch (PDFlib::Exception &ex) {
        wcerr << L"PDFlib exception occurred in hello sample: " << endl
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
