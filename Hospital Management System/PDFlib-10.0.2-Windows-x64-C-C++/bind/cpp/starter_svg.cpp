// 
// Starter SVG:
// Load SVG graphics and fit into a box
//
// Required data: SVG graphics
//

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
        const wstring outfile = L"starter_svg.pdf";

        const wstring graphicsfile = L"PDFlib-logo.svg";
        int graphics;
        int x = 100, y = 300;
        int boxwidth = 400, boxheight = 400;
        wostringstream optlist;

        PDFlib p;

        optlist.str(L"");
        optlist << L"SearchPath={{" << searchpath << L"}}";
        p.set_option(optlist.str());

        /* This means we must check return values of load_graphics() etc. */
        p.set_option(L"errorpolicy=return");

        if (p.begin_document(outfile, L"") == -1) {
            wcerr << "Error: " << p.get_errmsg() << endl;
            return 2;
        }

        p.set_info(L"Creator", L"PDFlib starter sample");
        p.set_info(L"Title", L"starter_svg");

        /* Load the graphics */
        graphics = p.load_graphics(L"auto", graphicsfile, L"");
        if (graphics == -1) {
            wcerr << "Error: " << p.get_errmsg() << endl;
            return 2;
        }


        p.begin_page_ext(0, 0, L"width=a4.width height=a4.height");

        /* ------------------------------------------------------
         * Fit the graphics into a box with proportional resizing
         * ------------------------------------------------------
         */

        /* The "boxsize" option defines a box with a given width and height 
         * and its lower left corner located at the reference point.
         * "position={center}" positions the graphics in the center of the
         * box.
         * "fitmethod=meet" resizes the graphics proportionally until its 
         * height or width completely fits into the box.
         * The "showborder" option is used to illustrate the borders of the 
         * box 
         */
        optlist.str(L"");
        optlist << "boxsize={ " << boxwidth << " " << boxheight <<
                  "} position={center} fitmethod=meet showborder";

        /* Before actually fitting the graphics we check whether fitting is
         * possible.
         */
        if (p.info_graphics(graphics, L"fittingpossible", optlist.str()) == 1)
        {
            p.fit_graphics(graphics, x, y, optlist.str());
        }
        else
        {
            wcerr << L"Cannot place graphics: " << p.get_errmsg() << endl;
        }

        p.end_page_ext(L"");

        p.close_graphics(graphics);

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
