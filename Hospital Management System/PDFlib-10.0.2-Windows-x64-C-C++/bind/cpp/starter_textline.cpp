/* 
 * Starter text line:
 * Demonstrate various options for placing a text line
 *
 * Place a text line with different font sizes.
 * Output overlined, stroke out, and underlined text.
 * Output text and define character spacing, work spacing, or horizontal
 * scaling.
 * Output text with a defined fill color. Output text including its outlines
 * with a defined stroke color.
 * Place text into a box at various positions. Place text completely into a box
 * with automatic scaling if required.
 *
 * Required data: font
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
        const wstring outfile = L"starter_textline.pdf";

        wstring optlist;
        PDFlib p;
        int font, x = 10, xt = 280, y = 800, yoff = 50;
        const wstring textline = L"Giant Wing Paper Plane";

        wostringstream buf;

        //  This means we must check return values of load_font() etc.
        p.set_option(L"errorpolicy=return");

        // Set the search path for fonts and PDF files
        buf.str(L"");
        buf << L"searchpath={{" << searchpath << L"}}";
        p.set_option(buf.str());


        /* Set an output path according to the name of the topic */
        if (p.begin_document(outfile, L"") == -1) {
            wcerr << L"Error: " << p.get_errmsg() << endl;
            return 2;
        }

        p.set_info(L"Creator", L"PDFlib starter sample");
        p.set_info(L"Title", L"starter_textline");

        /* Start Page */
        p.begin_page_ext(0, 0, L"width=a4.width height=a4.height");

        font = p.load_font(L"NotoSerif-Regular", L"winansi", L"");

        if (font == -1) {
            wcerr << L"Error: " << p.get_errmsg() << endl;
            return 2;
        }

        /* Set the font with a font size of 14 */
        p.setfont(font, 14);


        /* Place the text line without any options applied */
        p.fit_textline(textline, x, y, L"");

        /* Output descriptive text */
        p.fit_textline(L"fit_textline() without any options", xt, y,
            L"fontsize=12");


        /* Place the text with a different font size */
        optlist = L"fontsize=22";

        p.fit_textline(textline, x, y-=yoff, optlist); /* sample text */
        p.fit_textline(optlist, xt, y, L"fontsize=12"); /* description */


        /* Place stroke out text */
        optlist = L"strikeout";

        p.fit_textline(textline, x, y-=yoff, optlist); /* sample text */
        p.fit_textline(optlist, xt, y, L"fontsize=12"); /* description */


        /* Place underlined text */
        optlist = L"underline underlinewidth=7% underlineposition=-20%";

        p.fit_textline(textline, x, y-=yoff, optlist); /* sample text */
        p.fit_textline(optlist, xt, y, L"fontsize=12"); /* description */


        /* Place overlined text */
        optlist = L"overline";

        p.fit_textline(textline, x, y-=yoff, optlist); /* sample text */
        p.fit_textline(optlist, xt, y, L"fontsize=12"); /* description */


        /* Place the text with a horizontal scaling of 150% */
        optlist = L"horizscaling=150%";

        p.fit_textline(textline, x, y-=yoff, optlist); /* sample text */
        p.fit_textline(optlist, xt, y, L"fontsize=12"); /* description */


        /* Place the text with a character spacing of 30% of the font size */
        optlist = L"charspacing=30%";

        p.fit_textline(textline, x, y-=yoff, optlist); /* sample text */
        p.fit_textline(optlist, xt, y, L"fontsize=12"); /* description */


        /* Place the text with a word spacing of 50% of the font size */
        optlist = L"wordspacing=50%";

        p.fit_textline(textline, x, y-=yoff, optlist); /* sample text */
        p.fit_textline(optlist, xt, y, L"fontsize=12"); /* description */


        /* Place the text with a different fill color */
        optlist = L"fillcolor={rgb 0.5 0.2 0.5}";

        p.fit_textline(textline, x, y-=yoff, optlist);
        p.fit_textline(optlist, xt, y, L"fontsize=12");


        /* Place the text including its outlines using a text rendering mode of
         * 2 for L"filling and stroking text" and different fill and stroke
         * colors
         */
        optlist =
            L"fontsize=22 fillcolor={rgb 0.6 0.3 0.6} strokecolor={gray 0} "
            L"strokewidth=0.4 textrendering=2";

        p.fit_textline(textline, x, y-=yoff, optlist);

        /* Output descriptive text */
        p.fit_textline(L"fillcolor={rgb 0.6 0.3 0.6} strokecolor={gray 0} ",
            xt, y+10, L"fontsize=12");
        p.fit_textline(L"strokewidth=0.4 textrendering=2 fontsize=22",
            xt, y-5, L"fontsize=12");


        /* Place the text with its outlines using a text rendering mode of
         * 1 for L"stroking text" and a stroke color of black
         */
        optlist =
            L"fontsize=22 strokecolor={gray 0} strokewidth=0.4 textrendering=1";

        p.fit_textline(textline, x, y-=yoff, optlist);

        /* Output descriptive text */
        p.fit_textline(L"strokecolor={gray 0} strokewidth=0.4", xt, y+10,
            L"fontsize=12");
        p.fit_textline(L"textrendering=1 fontsize=22", xt, y-=5,
            L"fontsize=12");


        /* Place the text in a box with default positioning and fitting */
        optlist = L"boxsize={200 20} showborder";

        p.fit_textline(textline, x, y-=yoff, optlist); /* sample text */
        p.fit_textline(optlist, xt, y+3, L"fontsize=12"); /*description*/


        /* Place the text in a box on the top right */
        optlist = L"boxsize={200 20} position={top right} showborder";

        p.fit_textline(textline, x, y-=yoff, optlist); /* sample text */
        p.fit_textline(optlist, xt, y+3, L"fontsize=12"); /*description*/


        /* Use "fitmethod=clip" to place the text in a box not large enough to
         * show the complete text. The text will be clipped.
         */
        optlist = L"boxsize={130 20} fitmethod=clip showborder";

        p.fit_textline(textline, x, y-=yoff, optlist); /* sample text */
        p.fit_textline(optlist, xt, y+3, L"fontsize=12"); /*description*/


        /* Fit the text into the box automatically with L"fitmethod=auto".
         * In this case, if the text doesn't fit into the box a distortion
         * factor is calculated which makes the text fit into the box. If this
         * factor is larger than the L"shrinklimit" option the text will
         * be distorted by that factor. Otherwise, the font size will be
         * be decreased until until the text fits into the box.
         */

        /* Use "fitmethod=auto" to place the text in a box not large enough to
         * show the complete text. The text will be distorted.
         */
        optlist = L"boxsize={130 20} fitmethod=auto showborder";

        p.fit_textline(textline, x, y-=yoff, optlist); /* sample text */
        p.fit_textline(optlist, xt, y+3, L"fontsize=12"); /*description*/


        /* Use "fitmethod=auto" to place the text in a box too small to show the
         * complete text. The font size will be reduced until the text fits into
         * the box.
         */
        optlist = L"boxsize={100 20} fitmethod=auto showborder";

        p.fit_textline(textline, x, y-=yoff, optlist); /* sample text */
        p.fit_textline(optlist, xt, y+3, L"fontsize=12"); /*description*/

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
