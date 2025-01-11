/* 
 * PDF/X-4 starter:
 * Create PDF/X-4 conforming output with layers and transparency
 *
 * A low-level layer is created for each of several languages, as well
 * as an image layer
 *
 * The document contains transparent text which is allowed in
 * PDF/X-4, but not earlier PDF/X standards.
 *
 * Required data: font file, image file, ICC output intent profile
 *                (see www.pdflib.com for output intent ICC profiles)
 */

#include <iostream>
#include <string>

#include "pdflib.hpp"

using namespace std;
using namespace pdflib;

int
main(void)
{
    /* This is where the data files are. Adjust as necessary.*/
    const wstring searchpath = L"../data";

    const wstring imagefile = L"zebra.tif";
    wostringstream optlist;
    PDFlib p;
    int font, image;
    int layer_english, layer_german, layer_french, layer_image;
    double width, height;

    try {
        //  This means we must check return values of load_font() etc.
        p.set_option(L"errorpolicy=return");

        // Set the search path for fonts and PDF files
        optlist.str(L"");
        optlist << L"searchpath={{" << searchpath << L"}}";
        p.set_option(optlist.str());


        if (p.begin_document(L"starter_pdfx4.pdf", L"pdfx=PDF/X-4") == -1) {
            wcerr << L"Error: " << p.get_errmsg() << endl;
            return 2;
        }

        p.set_info(L"Creator", L"PDFlib starter sample");
        p.set_info(L"Title", L"starter_pdfx4");


        if (p.load_iccprofile(L"ISOcoated_v2_eci.icc", L"usage=outputintent") == -1)
        {
            wcerr << L"Error: " << p.get_errmsg() << endl;
	    wcerr << L"See www.pdflib.com for output intent ICC profiles." <<endl;
            return 2;
        }

        /* Define the layers; "defaultstate" specifies whether or not
         * the layer is visible when the page is opened.
         */

        layer_english = p.define_layer(L"English text", L"defaultstate=true");
        layer_german  = p.define_layer(L"German text", L"defaultstate=false");
        layer_french  = p.define_layer(L"French text", L"defaultstate=false");

        /* Define a radio button relationship for the language layers  */

        optlist.str(L"");
        optlist << L"group={" << layer_english << L" " << layer_german
            << L" " << layer_french << L"}";
        p.set_layer_dependency(L"Radiobtn", optlist.str());
        layer_image = p.define_layer(L"Images", L"defaultstate=true");

        p.begin_page_ext(0,0, L"width=a4.width height=a4.height");

        font = p.load_font(L"NotoSerif-Regular", L"winansi", L"");

        if (font == -1) {
            wcerr << L"Error: " << p.get_errmsg() << endl;
            return 2;
        }

        p.setfont(font, 24);

        p.begin_layer(layer_english);

        p.fit_textline(L"PDF/X-4 starter sample with layers", 50, 700, L"");

        p.begin_layer(layer_german);
        p.fit_textline(L"PDF/X-4 Starter-Beispiel mit Ebenen", 50, 700, L"");

        p.begin_layer(layer_french);
        p.fit_textline(L"PDF/X-4 Starter exemple avec des calques", 50, 700,
                        L"");

        p.begin_layer(layer_image);

        image = p.load_image(L"auto", imagefile, L"");
        if (image == -1) {
            wcerr << L"Error: " << p.get_errmsg() << endl;
            return 2;
        }

        /* Place the image on the page */
        p.fit_image(image, 0.0, 0.0, L"");

        /* Place a diagonal stamp across the image area */
        width = p.info_image(image, L"width", L"");
        height = p.info_image(image, L"height", L"");

        optlist.str(L"");
        optlist << L"boxsize={" << width << L" " << height << L"} font=" << font << 
            L" stamp=ll2ur fillcolor={lab 100 0 0} gstate={opacityfill=0.5}";
        p.fit_textline(L"Zebra", 0, 0, optlist.str());

        p.close_image(image);

        /* Close all layers */
        p.end_layer();

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
