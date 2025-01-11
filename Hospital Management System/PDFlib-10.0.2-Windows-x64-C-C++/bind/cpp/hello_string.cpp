//
// PDFlib client: hello example in C++ using the plain std::string type
// that is assumed to contain UTF-8-encoded strings
//

#include <iostream>

#define PDFLIB_CPP_STRING
#include "pdflib.hpp"

using namespace std;
using namespace pdflib;

int
main(void)
{
    try {
        PDFlib p;
        ostringstream buf;
        /* This is where the data files are. Adjust as necessary. */
        const string searchpath = "../data";
        const string fontopt =
            "fontname=NotoSerif-Regular fontsize=24";

        //  This means we must check return values of load_font() etc.
        p.set_option("errorpolicy=return");

        // Set the search path for fonts files
        buf.str("");
        buf << "searchpath={{" << searchpath << "}}";
        p.set_option(buf.str());

        if (p.begin_document("hello_string.pdf", "") == -1) {
            cerr << "Error: " << p.get_errmsg() << endl;
            return 2;
        }

        p.set_info("Creator", "hello_string.cpp");
        p.set_info("Author", "Thomas Merz");
        p.set_info("Title", "Hello World (C++ using std::string)!");

        p.begin_page_ext(0, 0, "width=a4.width height=a4.height");

        p.fit_textline("Hello world!", 50, 700, fontopt);
        p.fit_textline("(says C++ using std::string)",  50, 676, fontopt);
        p.end_page_ext("");

        p.end_document("");
    }
    catch (PDFlib::Exception &ex) {
        cerr << "PDFlib exception occurred in hello sample: " << endl
              << "[" << ex.get_errnum() << "] " << ex.get_apiname()
              << ": " << ex.get_errmsg() << endl;
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
