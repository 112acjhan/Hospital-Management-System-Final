/*
 * Starter sample for path objects:
 * Create some basic examples of path object construction and use
 *
 * Required data: font
 */

#include <iostream>
#include <string>

#include "pdflib.hpp"

using namespace std;
using namespace pdflib;

int
main(void)
{
    const wstring outfile = L"starter_path.pdf";

    try {
        /* This is where the data files are. Adjust as necessary. */
        const wstring searchpath = L"../data";

        PDFlib p;
        int path, tf, n;
        wstring result;
        double x, y;
        wostringstream optlist;

        const wstring text =
L"Lorem ipsum dolor sit amet, consectetur adipisicing elit, \
sed do eiusmod tempor incididunt ut labore et dolore magna \
aliqua. Ut enim ad minim veniam, quis nostrud exercitation \
ullamco laboris nisi ut aliquip ex ea commodo consequat. \
Duis aute irure dolor in reprehenderit in voluptate velit esse cillum \
dolore eu fugiat nulla pariatur. Excepteur sint occaecat \
cupidatat non proident, sunt in culpa qui officia deserunt mollit anim \
id est laborum. ";

        /* This means we must check return values of load_font() etc. */
        p.set_option(L"errorpolicy=return");

        // Set the search path for fonts and PDF files
        optlist.str(L"");
        optlist << L"searchpath={{" << searchpath << L"}}";
        p.set_option(optlist.str());

        if (p.begin_document(outfile, L"") == -1) {
            wcerr << L"Error: " << p.get_errmsg() << endl;
            return 2;
        }

        p.set_info(L"Creator", L"PDFlib starter sample");
        p.set_info(L"Title", L"starter_path");

        /* Start an A4 page */
        p.begin_page_ext(0, 0, L"width=a4.width height=a4.height");

        /* Construct a path object for an arrow shape */
        path = -1;

        /* The tip of the arrow gets rounded corners */
        path = p.add_path_point(path, 200.0,  25.0, L"move", L"round=10");
        path = p.add_path_point(path, 200.0,  75.0, L"line", L"");
        /* assign a name to the arrow's tip */
        path = p.add_path_point(path, 300.0,   0.0, L"line", L"name=tip");
        path = p.add_path_point(path, 200.0, -75.0, L"line", L"");
        path = p.add_path_point(path, 200.0, -25.0, L"line", L"");

        /* Start a new subpath for the straight base of the arrow */
        path = p.add_path_point(path, 200.0, -25.0, L"move", L"");
        path = p.add_path_point(path,   0.0, -25.0, L"line", L"");

        /* The center of the base can serve as a named attachment point */
        path = p.add_path_point(path,   0.0,   0.0, L"line", L"name=base");
        path = p.add_path_point(path,   0.0,  25.0, L"line", L"");
        path = p.add_path_point(path, 200.0,  25.0, L"line", L"");

        x = 100.0;
        y = 850.0;

        /* ----------------------------------------
         * Place arrow in its original direction
         * ----------------------------------------
         */
        y -= 100.0;
        p.draw_path(path, x, y,
            L"stroke linewidth=3 fill fillcolor=Turquoise "
            L"linecap=projecting attachmentpoint=base ");

        /* ----------------------------------------
         * Scale down arrow and align it to north east
         * ----------------------------------------
         */
        y -= 200.0;
        p.draw_path(path, x, y,
            L"stroke linewidth=3 fill fillcolor=Turquoise "
            L"linecap=projecting attachmentpoint=base scale=0.5 align={1 1}");

        /* ----------------------------------------
         * Scale to 50%, use the arrow tip as attachment point,
         * and align the arrow to the left
         * ----------------------------------------
         */
        y -= 100.0;
        p.draw_path(path, x, y,
            L"stroke linewidth=3 fill fillcolor=Turquoise "
            L"linecap=projecting attachmentpoint=tip scale=0.5 align={-1 0}");

        /* ----------------------------------------
         * Place text on the path; round all corners to
         * allow smoother text at the corners
         * ----------------------------------------
         */
        y -= 100.0;

        optlist.str(L"");
        optlist << L"textpath={path=" << path << L" round=10} "
                L"position={center bottom} "
                L"fontname=NotoSerif-Regular encoding=winansi fontsize=8";
        p.fit_textline(text, x, y, optlist.str());

        /* ----------------------------------------
         * Use the path as clipping path for a Textflow
         * ----------------------------------------
        */
        y -= 300.0;

        /* Feed the text to the Textflow object */
        tf = p.add_textflow(-1, text,
            L"fontname=NotoSerif-Regular fontsize=10 encoding=winansi "
            L"alignment=justify");
        /* Use text twice to fill the arrow */
        tf = p.add_textflow(tf, text,
            L"fontname=NotoSerif-Regular fontsize=10 encoding=winansi "
            L"alignment=justify");
        if (tf == -1) {
            wcerr << L"Error: " << p.get_errmsg() << endl;
            return 2;
        }

        /* Attach the path's reference point to the middle left (0%, 50%)
         * of the fitbox, and wrap the text inside the path (inversefill)
         */
        optlist.str(L"");
        optlist << L"wrap={inversefill paths={{path=" << path
                << L" refpoint={0% 50%} scale=1.5 }}}";
        result = p.fit_textflow(tf, x, y, x+450, y+225, optlist.str());

        if (result == L"_stop")
        {
            /* In this example we don't care about overflow text */
        }
        p.delete_textflow(tf);

        /* ----------------------------------------
         * Query information about the path object
         * ----------------------------------------
        */
        n = (int) p.info_path(path, L"numpoints", L"");
        (void)n; // to silence warning

        p.delete_path(path);
        p.end_page_ext(L"");
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
