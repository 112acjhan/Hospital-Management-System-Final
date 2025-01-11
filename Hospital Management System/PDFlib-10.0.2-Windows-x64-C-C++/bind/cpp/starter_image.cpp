/* 
 * Starter image:
 * Load and place an image using various options for scaling and positioning
 *
 * Place the image it its original size.
 * Place the image with scaling and orientation to the west.
 * Fit the image into a box with clipping.
 * Fit the image into a box with proportional resizing.
 * Fit the image into a box entirely.
 *
 * Required data: image file
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
        const wstring outfile = L"starter_image.pdf";

        wostringstream buf;
        PDFlib p;
        const wstring imagefile = L"lionel.jpg";
        int font, image;
        int bw = 400, bh = 200;
        int x = 20, y = 580, yoffset = 260;

        wostringstream optlist;

        //  This means we must check return values of load_image() etc.
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
        p.set_info(L"Title", L"starter_image");

        font = p.load_font(L"NotoSerif-Regular", L"winansi", L"");
        if (font == -1) {
            wcerr << L"Error: " << p.get_errmsg() << endl;
            return 2;
        }

        /* Load the image */
        image = p.load_image(L"auto", imagefile, L"");
        if (image == -1) {
            wcerr << L"Error: " << p.get_errmsg() << endl;
            return 2;
        }

        /* Start page 1 */
        p.begin_page_ext(0, 0, L"width=a4.width height=a4.height");
        p.setfont(font, 12);


        /* ------------------------------------
         * Place the image in its original size
         * ------------------------------------
         */

        /* Position the image in its original size with its lower left corner
         * at the reference point (20, 380)
         */
        p.fit_image(image, 20, 380, L"");

        /* Output some descriptive text */
        p.fit_textline(
            L"The image is placed with the lower left corner in its original "
            L"size at reference point (20, 380):", 20, 820, L"");
        p.fit_textline(L"fit_image(image, 20, 380, \"\");", 20, 800, L"");


        /* --------------------------------------------------------
         * Place the image with scaling and orientation to the west
         * --------------------------------------------------------
         */

        /* Position the image with its lower right corner at the reference
         * point (580, 20).
         * L"scale=0.5" scales the image by 0.5.
         * L"orientate=west" orientates the image to the west.
         */
        p.fit_image(image, 580, 20,
            L"scale=0.5 position={right bottom} orientate=west");

        /* Output some descriptive text */
        p.fit_textline(
            L"The image is placed with a scaling of 0.5 and an orientation to "
            L"the west with the lower right corner", 580, 320,
            L"position={right bottom}");
        p.fit_textline(
            L" at reference point (580, 20): fit_image(image, 580, 20, "
            L"\"scale=0.5 orientate=west position={right bottom}\");",
            580, 300, L"position={right bottom}");

        p.end_page_ext(L"");

        /* Start page 2 */
        p.begin_page_ext(0, 0, L"width=a4.width height=a4.height");
        p.setfont(font, 12);


        /* --------------------------------------
         * Fit the image into a box with clipping
         * --------------------------------------
         */

        /* The "boxsize" option defines a box with a given width and height and
         * its lower left corner located at the reference point.
         * "position={right top}" positions the image on the top right of the
         * box.
         * "fitmethod=clip" clips the image to fit it into the box.
         */
        buf.str(L"");
        buf << L"boxsize={" << bw << L" " << bh
            << L"} position={right top} fitmethod=clip";
        p.fit_image(image, x, y, buf.str());

        /* Output some descriptive text */
        p.fit_textline(
            L"fit_image(image, x, y, \"boxsize={400 200} position={right top} "
            L"fitmethod=clip\");", 20, y+bh+10, L"");


        /* ---------------------------------------------------
         * Fit the image into a box with proportional resizing
         * ---------------------------------------------------
         */

        /* The "boxsize" option defines a box with a given width and height and
         * its lower left corner located at the reference point.
         * "position={center}" positions the image in the center of the
         * box.
         * "fitmethod=meet" resizes the image proportionally until its height
         * or width completely fits into the box.
         * The "showborder" option is used to illustrate the borders of the box.
         */
        buf.str(L"");
        buf << L"boxsize={" << bw << L" " << bh
            << L"} position={center} fitmethod=meet showborder";
        p.fit_image(image, x, y-=yoffset, buf.str());

        /* Output some descriptive text */
        p.fit_textline(
            L"fit_image(image, x, y, \"boxsize={400 200} position={center} "
            L"fitmethod=meet showborder\");", 20, y+bh+10, L"");


        /* ---------------------------------
         * Fit the image into a box entirely
         * ---------------------------------
         */

        /* The L"boxsize" option defines a box with a given width and height and
         * its lower left corner located at the reference point.
         * By default, the image is positioned in the lower left corner of the
         * box.
         * L"fitmethod=entire" resizes the image proportionally until its height
         * or width completely fits into the box.
         */
        buf.str(L"");
        buf << L"boxsize={" << bw << L" " << bh << L"} fitmethod=entire";
        p.fit_image(image, x, y-=yoffset, buf.str());

        /* Output some descriptive text */
        p.fit_textline(
            L"fit_image(image, x, y, \"boxsize={400 200} fitmethod=entire\");",
            20, y+bh+10, L"");

        p.end_page_ext(L"");

        p.close_image(image);

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
