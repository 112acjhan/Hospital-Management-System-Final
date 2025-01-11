/* 
 * formfields starter:
 * Create a linearized PDF (for fast delivery over the Web, also known
 * as "fast Web view") which is encrypted and contains some form fields.
 * A few lines of JavaScript are inserted as "page open" action to
 * automatically populate the date field with the current date.
 *
 * Required data: font file
 */

#include <iostream>

#include "pdflib.hpp"

using namespace std;
using namespace pdflib;

int
main(void)
{
    try {
        PDFlib p;
        wostringstream optlist;
        /* This is where the data files are. Adjust as necessary. */
        const wstring searchpath = L"../data";
        int font, action;
        double llx=150, lly=550, urx=350, ury=575;

        /* JavaScript for automatically filling the date into a form field */
        const wstring js =
            L"var d = util.printd(\"mm/dd/yyyy\", new Date());"
            L"var date = this.getField(\"date\");"
            L"date.value = d;";

        //  This means we must check return values of load_font() etc.
        optlist.str(L"");
        optlist << L"errorpolicy=return searchpath={{" << searchpath << L"}}";
        p.set_option(optlist.str());

        if (p.begin_document(L"starter_formfields.pdf",
                                    L"linearize masterpassword=pdflib "
                                    L"permissions={nomodify}") == -1) {
            wcerr << L"Error: " << p.get_errmsg() << endl;
            return 2;
        }

        p.set_info(L"Creator", L"PDFlib starter sample");
        p.set_info(L"Title", L"starter_formfields");

        optlist.str(L"");
        optlist << L"script[" << js.length() << L"]={" << js << L"}";
        action = p.create_action(L"JavaScript", optlist.str());

        optlist.str(L"");
        optlist << L"width=a4.width height=a4.height action={open=" << action << L"}";
        p.begin_page_ext(0, 0, optlist.str());

        font = p.load_font(L"NotoSerif-Regular", L"winansi", L"simplefont nosubsetting");
        if (font == -1) {
            wcerr << L"Error: " << p.get_errmsg() << endl;
            return 2;
        }

        p.setfont(font, 24);

        p.fit_textline(L"Date: ", 125, lly+5, L"position={right bottom}");

        /* The tooltip will be used as rollover text for the field */
            optlist.str(L"");
            optlist << L"tooltip={Date (will be filled automatically)} "
                    L"bordercolor={gray 0} font=" << font;
        p.create_field(llx, lly, urx, ury, L"date", L"textfield",
                            optlist.str());

        lly-=100; ury-=100;
        p.fit_textline(L"Name: ", 125, lly+5, L"position={right bottom}");

            optlist.str(L"");
            optlist << L"tooltip={Enter your name here} "
                    L"bordercolor={gray 0} font=" << font;
        p.create_field(llx, lly, urx, ury, L"name", L"textfield",
                            optlist.str());

        p.end_page_ext(L"");

        p.end_document(L"");
    }
    catch (PDFlib::Exception &ex) {
        wcerr << L"PDFlib exception occurred: " << endl
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
