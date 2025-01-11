/* 
 * PDFlib Virtual File system (PVF):
 * Create a PVF file which holds an image or PDF, and import the data from the
 * PVF file
 *
 * This avoids disk access and is especially useful when the same image or PDF
 * is imported multiply. For examples, images which sit in a database don't
 * have to be written and re-read from disk, but can be passed to PDFlib
 * directly in memory. A similar technique can be used for loading other data
 * such as fonts, ICC profiles, etc.
 */
#include <iostream>
#include <cstdlib>
#include <cstring>

#include "pdflib.hpp"

using namespace std;
using namespace pdflib;

int
main(void)
{
    try {
        /* This is where the data files are. Adjust as necessary. */
        const wstring searchpath = L"../data";
        const wstring outfile = L"starter_pvf.pdf";

        PDFlib p;
        const char * svgdata =
"<?xml version='1.0'?> \
<svg viewBox='0 0 100 100' version='1.1' xmlns='http://www.w3.org/2000/svg'> \
<circle cx='50' cy='50' r='40' fill='orange'/> \
</svg>";

        wostringstream optlist;

        //  This means we must check return values of load_font() etc.
        p.set_option(L"errorpolicy=return");

        // Set the search path for fonts and PDF files
        optlist.str(L"");
        optlist << L"searchpath={{" << searchpath << L"}}";
        p.set_option(optlist.str());


        /* Set an output path according to the name of the topic */
        if (p.begin_document(outfile, L"") == -1) {
            wcerr << L"Error: " << p.get_errmsg() << endl;
            return 2;
        }

        p.set_info(L"Creator", L"PDFlib starter sample");
        p.set_info(L"Title", L"starter_pvf");

        p.create_pvf(L"/pvf/svg", svgdata, strlen(svgdata), L"");

        /* Load the graphics from the PVF */
        int svg = p.load_graphics(L"svg", L"/pvf/svg", L"");
        if (svg == -1) {
            wcerr << L"Error: " << p.get_errmsg() << endl;
           return 2;
        }

        /* Fit the graphics on page 1 */
        p.begin_page_ext(0,0, L"width=a4.width height=a4.height");

        p.fit_graphics(svg, 350, 750, L"");

        p.end_page_ext(L"");

        /* Fit the svg on page 2 */
        p.begin_page_ext(0,0, L"width=a4.width height=a4.height");

        p.fit_graphics(svg, 350, 50, L"");

        p.end_page_ext(L"");

        /* Delete the virtual file to free the allocated memory */
        p.delete_pvf(L"/pvf/svg");
        p.end_document(L"");
    }
    catch (PDFlib::Exception &ex) {
        wcerr << L"PDFlib exception occurred in invoice sample: " << endl
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
