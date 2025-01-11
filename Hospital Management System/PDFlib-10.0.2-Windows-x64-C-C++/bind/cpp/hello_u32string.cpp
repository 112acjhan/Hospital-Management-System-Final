//
// PDFlib client: hello example in C++ using the std::u32string type
//

#include <iostream>

#define PDFLIB_CPP_U32STRING
#include "pdflib.hpp"

using namespace std;
using namespace pdflib;

/* Figure out whether we're running on an EBCDIC-based machine. */
#if 'A' == 0xC1
#define NATIVE_ENCODING U"ebcdicutf8"
#else
#define NATIVE_ENCODING U"utf8"
#endif

namespace
{

// Helper functions to convert from and to std::u32string
u32string get_u32string(const string& utf8_string);
string get_native_string(const u32string& utf32_string);

} // end anonymous namespace

int
main(void)
{
    try {
        PDFlib p;
        /* This is where the data files are. Adjust as necessary. */
        const u32string searchpath = U"../data";

        /*
         * Here we demonstrate how to convert a native std::string
         * to a std::u32string
         */
        const u32string fontopt =
            get_u32string("fontname=NotoSerif-Regular fontsize=24");

        //  This means we must check return values of load_font() etc.
        p.set_option(U"errorpolicy=return");

        // Set the search path for fonts files
        p.set_option(U"searchpath={{" + searchpath + U"}}");

        if (p.begin_document(U"hello_u32string.pdf", U"") == -1) {
            cerr << "Error: " << get_native_string(p.get_errmsg()) << endl;
            return 2;
        }

        p.set_info(U"Creator", U"hello_u32string.cpp");
        p.set_info(U"Author", U"Thomas Merz");
        p.set_info(U"Title", U"Hello World (C++ using std::u32string)!");

        p.begin_page_ext(0, 0, U"width=a4.width height=a4.height");

        p.fit_textline(U"Hello world!", 50, 700, fontopt);
        p.fit_textline(U"(says C++ using std::u32string)",  50, 676, fontopt);
        p.end_page_ext(U"");

        p.end_document(U"");
    }
    catch (PDFlib::Exception &ex) {
        cerr << "PDFlib exception occurred in hello sample: " << endl
              << "[" << ex.get_errnum() << "] " << get_native_string(ex.get_apiname())
              << ": " << get_native_string(ex.get_errmsg()) << endl;
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

namespace
{

// This PDFlib object is used solely for conversion purposes between the encoding
// of the PDFlib API parameters and the native encoding.
PDFlib conversion_helper;

u32string get_u32string(const string& native_string)
{
    string const u32_bytes =
        conversion_helper.convert_to_unicode(NATIVE_ENCODING, native_string,
                                                    U"outputformat=utf32");

    return u32string(
            reinterpret_cast<u32string::value_type const *>(u32_bytes.c_str()),
            u32_bytes.length() / sizeof(u32string::value_type));
}

string get_native_string(const u32string& utf32_string)
{
    string const u32_bytes(
            reinterpret_cast<string::value_type const *>(utf32_string.c_str()),
            utf32_string.length() * sizeof(u32string::value_type));

    return conversion_helper.convert_to_unicode(U"utf32", u32_bytes,
                                            U"outputformat=" NATIVE_ENCODING);
}

} // end anonymous namespace
