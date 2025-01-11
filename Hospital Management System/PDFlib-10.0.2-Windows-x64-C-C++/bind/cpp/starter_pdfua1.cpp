/* 
 * Tagged PDF starter:
 * Create document with structure information for reflow and accessibility
 *
 * Required data: font, image file
 */

#include <iostream>
#include <string>

#include "pdflib.hpp"

using namespace std;
using namespace pdflib;

int
main(void)
{
    try {
        /* This is where the data files are. Adjust as necessary. */
        const wstring searchpath = L"../data";
        const wstring imagefile = L"lionel.jpg";

        PDFlib p;
        int id, image, font, action;
        wostringstream optlist;

        /*
         * errorpolicy=exception means that the program will stop
         * if one of the API function runs into a problem.
         */
        optlist.str(L"");
        optlist << L"errorpolicy=exception searchPath={{" << searchpath << L"}}";
        p.set_option(optlist.str());

        p.begin_document(L"starter_pdfua1.pdf",
                L"pdfua=PDF/UA-1 lang=en " 
                L"tag={tagname=Document Title={PDFlib PDF/UA-1 demo}}") ;

        p.set_info(L"Creator", L"starter_pdfua1");
        p.set_info(L"Title", L"PDFlib PDF/UA-1 demo");

        /* Automatically create spaces between chunks of text */
        p.set_option(L"autospace=true");

        p.begin_page_ext(0, 0,
                L"width=a4.width height=a4.height taborder=structure");

        p.create_bookmark(L"PDF/UA-1 demo", L"");

        font = p.load_font(L"NotoSerif-Regular", L"unicode", L"");

        p.setfont(font, 24.0);

        /* =================== Simple text  ======================== */

        /* Use abbreviated tagging with the "tag" option */
        p.fit_textline(L"Introduction to Paper Planes",
            50, 700, L"tag={tagname=H1 Title={Introduction}} fontsize=24");

        p.fit_textline(
            L"Paper planes can be made from any kind of paper.",
            50, 675, L"tag={tagname=P} fontsize=12");

        p.fit_textline(L"Most paper planes don't have an engine.",
            50, 650, L"tag={tagname=P} fontsize=12");

        /* =================== Interactive Link ======================== */
        /* Open both P and Link elements in a single call */
        id = p.begin_item(L"P", L"tag={tagname=Link Title={Kraxi on the Web}}");

        /* Create visible content which represents the link */
        p.fit_textline(L"Learn more on the Kraxi website.",
            50, 625,
            L"matchbox={name={kraxi.com}} fontsize=12 " 
            L"strokecolor=blue fillcolor=blue underline");

        /* Create URI action */
        action = p.create_action(L"URI", L"url={http://www.kraxi.com}");

        /* Create Link annotation on named matchbox "kraxi.com".
         * This automatically creates an OBJR (object reference) element.
         */
        optlist.str(L"");
        optlist << L"linewidth=0 usematchbox={kraxi.com} " <<
            L"contents={Link to Kraxi Inc. Web site} " <<
            L"action={activate=" << action << L" } ";
        p.create_annotation(0, 0, 0, 0, L"Link", optlist.str());

        /* This closes the Link and P structure elements */
        p.end_item(id);

        /* =================== Image  ======================== */
        /* A grouping element is required as container for Figure and Caption */
        id = p.begin_item(L"Sect", L"");

        image = p.load_image(L"auto", imagefile, L"");

        /* The "Placement" attribute is recommended for Figure elements
         * as children of grouping elements.
         */
        p.fit_image(image, 50, 400,
            L"tag={tagname=Figure Placement=Block Alt={Image of Kraxi waiting for customers.}} "
            L"scale=0.5");
        p.close_image(image);
        /* Caption text below the image; Caption element follows Figure.
        * Since Caption doesn't allow direct content in PDF 1.7 we create
        * an additional P element.
        */
        p.fit_textline(L"Kraxi waiting for customers.", 
                50, 375,
        L"tag={tagname=Caption tag={tagname=P}} fontsize=12 ");

        p.end_item(id);
        /* =================== Artifact  ======================== */
        p.fit_textline(L"Page 1", 250, 100,
            L"tag={tagname=Artifact} fontsize=12");

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
