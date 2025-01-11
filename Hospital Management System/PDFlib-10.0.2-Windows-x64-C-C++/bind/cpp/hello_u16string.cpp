//
// PDFlib client: hello example in C++ using the std::u16string type
//

#include <iostream>

#define PDFLIB_CPP_U16STRING
#include "pdflib.hpp"

using namespace std;
using namespace pdflib;

/* Figure out whether we're running on an EBCDIC-based machine. */
#if 'A' == 0xC1
#define NATIVE_ENCODING u"ebcdicutf8"
#else
#define NATIVE_ENCODING u"utf8"
#endif

namespace
{

// Helper functions to convert from and to std::u16string
u16string get_u16string(const string& native_string);
string get_native_string(const u16string& utf16_string);

} // end anonymous namespace

int
main(void)
{
    try {
        PDFlib p;
        /* This is where the data files are. Adjust as necessary. */
        const u16string searchpath = u"../data";

        /*
         * Here we demonstrate how to convert a native std::string
         * to a std::u16string
         */
        const u16string fontopt =
            get_u16string("fontname=NotoSerif-Regular fontsize=24");

        //  This means we must check return values of load_font() etc.
        p.set_option(u"errorpolicy=return");

        // Set the search path for fonts files
        p.set_option(u"searchpath={{" + searchpath + u"}}");

        if (p.begin_document(u"hello_u16string.pdf", u"") == -1) {
            cerr << "Error: " << get_native_string(p.get_errmsg()) << endl;
            return 2;
        }

        p.set_info(u"Creator", u"hello_u16string.cpp");
        p.set_info(u"Author", u"Thomas Merz");
        p.set_info(u"Title", u"Hello World (C++ using std::u16string)!");

        p.begin_page_ext(0, 0, u"width=a4.width height=a4.height");

        p.fit_textline(u"Hello world!", 50, 700, fontopt);
        p.fit_textline(u"(says C++ using std::u16string)",  50, 676, fontopt);
        p.end_page_ext(u"");

        p.end_document(u"");
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

u16string get_u16string(const string& native_string)
{
    string const u16_bytes =
        conversion_helper.convert_to_unicode(NATIVE_ENCODING, native_string,
                                                    u"outputformat=utf16");

    return u16string(
            reinterpret_cast<u16string::value_type const *>(u16_bytes.c_str()),
            u16_bytes.length() / sizeof(u16string::value_type));
}

string get_native_string(const u16string& utf16_string)
{
    string const u16_bytes(
            reinterpret_cast<string::value_type const *>(utf16_string.c_str()),
            utf16_string.length() * sizeof(u16string::value_type));

    return conversion_helper.convert_to_unicode(u"utf16", u16_bytes,
                                            u"outputformat=" NATIVE_ENCODING);
}

} // end anonymous namespace
