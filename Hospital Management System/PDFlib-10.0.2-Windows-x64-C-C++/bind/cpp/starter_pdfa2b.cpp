/* 
 * PDF/A-2b starter:
 * Create PDF/A-2b conforming output with layers, transparency, annotation and
 * PDF/A attachment.
 *
 * Required data: font file, image file
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
    const wstring attachments[] = { L"lionel.pdf"};
    const size_t attachment_count  =
            sizeof(attachments) / sizeof(attachments[0]);
    wostringstream optlist;
    PDFlib p;
    int font, image;
    int layer_english, layer_german, layer_french, layer_image;
    int textflow;
    double width, height;
    size_t i;

    try {
        //  This means we must check return values of load_font() etc.
        p.set_option(L"errorpolicy=return");

        // Set the search path for fonts and PDF files
        optlist.str(L"");
        optlist << L"searchpath={{" << searchpath << L"}}";
        p.set_option(optlist.str());


        /* Initially display the layer panel and show the full page */
        if (p.begin_document(L"starter_pdfa2b.pdf",
                L"openmode=layers viewerpreferences={fitwindow=true} "
                L"pdfa=PDF/A-2b") == -1) {
            wcerr << L"Error: " << p.get_errmsg() << endl;
            return 2;
        }

        p.set_info(L"Creator", L"PDFlib starter sample");
        p.set_info(L"Title", L"starter_pdfa2b");

        /* Define the layers, with only English and image layers switched on. */
        layer_english = p.define_layer(L"English text", L"");
        layer_german  = p.define_layer(L"German text", L"defaultstate=false");
        layer_french  = p.define_layer(L"French text", L"defaultstate=false");
        layer_image   = p.define_layer(L"Images", L"");

        /* Define a radio button relationship for the language layers, so only
         * one language can be switched on at a time.
         */
        optlist.str(L"");
        optlist << L"group={" << layer_english << L" " << layer_german
		<< L" " << layer_french << L"}";
        p.set_layer_dependency(L"Radiobtn", optlist.str());


        p.begin_page_ext(0,0, L"width=a4.width height=a4.height");

        font = p.load_font(L"NotoSerif-Regular", L"winansi", L"");

        if (font == -1) {
            wcerr << L"Error: " << p.get_errmsg() << endl;
            return 2;
        }

        /* Create a Stamp annotation */
        optlist.str(L"");
        optlist << L"iconname=approved contents={PDF/A} font=" << font;
        p.create_annotation( 50, 700, 300, 840, L"Stamp", optlist.str());

        optlist.str(L"");
        optlist << L"font=" << font << " fontsize=24";

        p.begin_layer(layer_english);
        textflow = p.create_textflow(
                L"PDF/A-2b starter sample with layers, transparency, "
                L"annotation and attachment", optlist.str());
        p.fit_textflow(textflow, 50, 650, 550, 700, L"");
        p.delete_textflow(textflow);

        p.begin_layer(layer_german);
        textflow = p.create_textflow(
                L"PDF/A-2b Starter-Beispiel mit Ebenen, Transparenz, "
                L"Anmerkung und Anlage", optlist.str());
        p.fit_textflow(textflow, 50, 650, 550, 700, L"");
        p.delete_textflow(textflow);

        p.begin_layer(layer_french);
        textflow = p.create_textflow(
                L"PDF/A-2b starter exemple avec des calques, "
                L"transparence, commentaire et attachement", optlist.str());
        p.fit_textflow(textflow, 50, 650, 550, 700, L"");
        p.delete_textflow(textflow);

        p.begin_layer(layer_image);

        image = p.load_image(L"auto", imagefile, L"");
        if (image == -1) {
            wcerr << L"Error: " << p.get_errmsg() << endl;
            return 2;
        }

        width = p.info_image(image, L"width", L"");
        height = p.info_image(image, L"height", L"");

        /* Place the image on the page and close it */
        p.fit_image(image, 0.0, 0.0, L"");
        p.close_image(image);

        /* Place a transparent diagonal stamp across the image area, in
         * different colors
         */
        optlist.str(L"");
        optlist << L"boxsize={" << width << L" " << height << 
            L"} stamp=ll2ur font=" << font << 
            L" fillcolor={lab 100 0 0} gstate={opacityfill=0.5}";

        p.begin_layer(layer_english);
        p.fit_textline(L"Transparent text", 0, 0, optlist.str());

        p.begin_layer(layer_german);
        p.fit_textline(L"Transparenter Text", 0, 0, optlist.str());

        p.begin_layer(layer_french);
        p.fit_textline(L"Texte transparent", 0, 0, optlist.str());

        /* Close all layers */
        p.end_layer();

        p.end_page_ext(L"");

        /* Construct option list with attachment handles. The attachments must
         * be PDF/A-1 or PDF/A-2 files.
         */
        optlist.str(L"");
        optlist << L"attachments={";
        for (i = 0; i < attachment_count; i += 1) {
            const int attachment_handle =
                    p.load_asset(L"Attachment", attachments[i],
                                L"description={This is a PDF/A attachment}");

            if (attachment_handle == -1) {
                wcerr << L"Error loading attachment: " << p.get_errmsg() << endl;
                return 2;
            }

            optlist << " " << attachment_handle;
        }
        optlist << "}";

        p.end_document(optlist.str());
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
