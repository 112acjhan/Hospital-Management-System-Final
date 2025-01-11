/* 
 * Starter layer:
 * Define several layers, output images and text to them and define
 * particular layers to be visible when opening the document
 *
 * Define two layers for RGB or Grayscale images and two layers for English or
 * German image captions. Output images and text on the various layers and
 * open the document with the RGB images and English captions visible.
 *
 * Required data: grayscale and RGB images
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
        const wstring outfile = L"starter_layer.pdf";

        PDFlib p;
        const wstring rgb = L"nesrin.jpg";
        const wstring gray = L"nesrin_gray.jpg";

        wostringstream buf;
        wostringstream optlist;
        int font, imageRGB, imageGray, layerRGB, layerGray, layerEN, layerDE;


        //  This means we must check return values of load_font() etc.
        p.set_option(L"errorpolicy=return");

        // Set the search path for fonts and PDF files
        optlist.str(L"");
        optlist << L"searchpath={{" << searchpath << L"}}";
        p.set_option(optlist.str());


        /* Open the document with the "Layers" navigation tab visible */
        if (p.begin_document(outfile, L"openmode=layers") == -1) {
            wcerr << L"Error: " << p.get_errmsg() << endl;
            return 2;
        }

        p.set_info(L"Creator", L"PDFlib starter sample");
        p.set_info(L"Title", L"starter_layer");


        /* Load the font */
        font = p.load_font(L"NotoSerif-Regular", L"unicode", L"");

        if (font == -1) {
            wcerr << L"Error: " << p.get_errmsg() << endl;
            return 2;
        }

        /* Load the Grayscale image */
        imageGray = p.load_image(L"auto", gray, L"");
        if (imageGray == -1) {
            wcerr << L"Error: " << p.get_errmsg() << endl;
            return 2;
        }

        /* Load the RGB image */
        imageRGB = p.load_image(L"auto", rgb, L"");
        if (imageRGB == -1) {
            wcerr << L"Error: " << p.get_errmsg() << endl;
            return 2;
        }

        /*
        * Define all layers which will be used, and their relationships.
            * This should be done before the first page if the layers are
        * used on more than one page.
        */

        /* Define the layer "RGB" */
        layerRGB = p.define_layer(L"RGB", L"");

        /* Define the layer "Grayscale" which is hidden when opening the
         * document or printing it.
         */
        layerGray = p.define_layer(L"Grayscale",
                    L"initialviewstate=false initialprintstate=false");

        /* At most one of the "Grayscale" and "RGB" layers should be visible */
        buf.str(L"");
        buf << L"group={" << layerGray << L" " << layerRGB << L"}";
        p.set_layer_dependency(L"Radiobtn", buf.str());

        /* Define the layer L"English" */
        layerEN = p.define_layer(L"English", L"");

        /* Define the layer L"German" which is hidden when opening the document
         * or printing it.
         */
        layerDE = p.define_layer(L"German",
                    L"initialviewstate=false initialprintstate=false");

        /* At most one of the "English" and "German" layers should be visible */
        buf.str(L"");
        buf << L"group={" << layerEN << L" " << layerDE << L"}";
        p.set_layer_dependency(L"Radiobtn", buf.str());

        /* Start page */
        p.begin_page_ext(0, 0, L"width=a4.width height=a4.height");

        /* Place the RGB image on the layer L"RGB" */
        p.begin_layer(layerRGB);
        p.fit_image(imageRGB, 100, 400,
                    L"boxsize={400 300} fitmethod=meet");

        /* Place the Grayscale image on the layer L"Grayscale" */
        p.begin_layer(layerGray);
        p.fit_image(imageGray, 100, 400,
                    L"boxsize={400 300} fitmethod=meet");

        /* Common options for fit_textline() */
        buf.str(L"");
        buf << L"font=" << font << L" fontsize=20";

        /* Place an English image caption on the layer "English" */
        p.begin_layer(layerEN);
        p.fit_textline(L"This is the Nesrin image.", 100, 370, buf.str());

        /* Place a German image caption on the layer "German".  */
        p.begin_layer(layerDE);
        p.fit_textline(L"Das ist das Nesrin-Bild.", 100, 370, buf.str());

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
