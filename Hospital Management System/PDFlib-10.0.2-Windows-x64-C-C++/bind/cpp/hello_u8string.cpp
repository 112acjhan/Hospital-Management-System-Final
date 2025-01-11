//
// PDFlib client: hello example in C++ using the std::u8string type.
// This requires a C++20 compiler.
//

#include <iostream>

#define PDFLIB_CPP_U8STRING
#include "pdflib.hpp"

using namespace std;
using namespace pdflib;

/* Figure out whether we're running on an EBCDIC-based machine. */
#if 'A' == 0xC1
#define NATIVE_ENCODING u8"ebcdicutf8"
#else
#define NATIVE_ENCODING u8"utf8"
#endif

namespace
{

// Helper functions to convert from and to std::u8string
u8string get_u8string(const string& native_string);
string get_native_string(const u8string& utf8_string);

} // end anonymous namespace

int
main(void)
{
    try {
        PDFlib p;
        /* This is where the data files are. Adjust as necessary. */
        const u8string searchpath = u8"../data";

        /*
         * Here we demonstrate how to convert a native std::string
         * to a std::u8string
         */
        const u8string fontopt =
            get_u8string("fontname=NotoSerif-Regular fontsize=24");

        //  This means we must check return values of load_font() etc.
        p.set_option(u8"errorpolicy=return");

        // Set the search path for fonts files
        p.set_option(u8"searchpath={{" + searchpath + u8"}}");

        if (p.begin_document(u8"hello_u8string.pdf", u8"") == -1) {
            cerr << "Error: " << get_native_string(p.get_errmsg()) << endl;
            return 2;
        }

        p.set_info(u8"Creator", u8"hello_u8string.cpp");
        p.set_info(u8"Author", u8"Thomas Merz");
        p.set_info(u8"Title", u8"Hello World (C++ using std::u8string)!");

        p.begin_page_ext(0, 0, u8"width=a4.width height=a4.height");

        p.fit_textline(u8"Hello world!", 50, 700, fontopt);
        p.fit_textline(u8"(says C++ using std::u8string)",  50, 676, fontopt);
        p.end_page_ext(u8"");

        p.end_document(u8"");
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

u8string get_u8string(const string& native_string)
{
    string const u8_bytes =
        conversion_helper.convert_to_unicode(NATIVE_ENCODING, native_string,
                                                    u8"outputformat=utf8");

    return u8string(
            reinterpret_cast<u8string::value_type const *>(u8_bytes.c_str()),
            u8_bytes.length() / sizeof(u8string::value_type));
}

string get_native_string(const u8string& utf8_string)
{
    string const u8_bytes(
            reinterpret_cast<string::value_type const *>(utf8_string.c_str()),
            utf8_string.length() * sizeof(u8string::value_type));

    return conversion_helper.convert_to_unicode(u8"utf8", u8_bytes,
                                            u8"outputformat=" NATIVE_ENCODING);
}

} // end anonymous namespace
