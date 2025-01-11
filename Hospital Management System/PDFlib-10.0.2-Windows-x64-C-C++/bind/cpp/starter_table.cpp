/* 
 * Table starter:
 * Create table which may span multiple pages.
 * The table cells are filled with various content types including
 * Textflow, Textline, image, SVG, stamp, Web link (annotation) and
 * form field.
 *
 * Required data: image and SVG files, font
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

        const wstring imagefile = L"nesrin.jpg";
        const wstring graphicsfile = L"PDFlib-logo.svg";

        int row, col, font, fieldfont, image, graphics, tf=-1, tbl=-1;
        int rowmax=50, colmax=4;
        PDFlib p;
        double llx= 50, lly=50, urx=550, ury=800;
        const wstring headertext =
                                L"Table header (row 1)";
        wstring result;
        wostringstream optlist;

        /* Dummy text for filling a cell with multi-line Textflow */
        const wstring tf_text = 
L"Lorem ipsum dolor sit amet, consectetur adi&shy;pi&shy;sicing elit, "
L"sed do eius&shy;mod tempor incidi&shy;dunt ut labore et dolore magna "
L"ali&shy;qua. Ut enim ad minim ve&shy;niam, quis nostrud exer&shy;citation "
L"ull&shy;amco la&shy;bo&shy;ris nisi ut ali&shy;quip ex ea commodo "
L"con&shy;sequat. Duis aute irure dolor in repre&shy;henderit in voluptate "
L"velit esse cillum dolore eu fugiat nulla pari&shy;atur. Excep&shy;teur "
L"sint occae&shy;cat cupi&shy;datat non proident, sunt in culpa qui officia "
L"dese&shy;runt mollit anim id est laborum. ";

        //  This means we must check return values of load_font() etc.
        p.set_option(L"errorpolicy=return");

        // Set the search path for fonts and PDF files
        optlist.str(L"");
        optlist << L"searchpath={{" << searchpath << L"}}";
        p.set_option(optlist.str());

        if (p.begin_document(L"starter_table.pdf", L"") == -1) {
            wcerr << L"Error: " << p.get_errmsg() << endl;
            return 2;
        }
        p.set_info(L"Creator", L"PDFlib starter sample");
        p.set_info(L"Title", L"starter_table");

        /* -------------------- Add table cells -------------------- */

        /* ---------- Row 1: table header (spans all columns) */
        row = 1; col = 1;
        font = p.load_font(L"NotoSerif-Regular", L"unicode", L"");
        if (font == -1) {
            wcerr << L"Error: " << p.get_errmsg() << endl;
            return(2);
        }

        optlist.str(L"");
        optlist << L"fittextline={position=center font=" << font
                << L" fontsize=14} colspan=" << colmax;

        tbl = p.add_table_cell(tbl, col, row, headertext, optlist.str());
        if (tbl == -1) {
            wcerr << L"Error adding cell: " << p.get_errmsg() << endl;
            return 2;
        }

        /* ---------- Row 2: various kinds of content */
        row++; col=1;
        /* ----- Simple text cell */

        optlist.str(L"");
        optlist << L"fittextline={font=" << font
                << L" fontsize=12 orientate=west}";

        tbl = p.add_table_cell(tbl, col, row, L"vertical line", optlist.str());
        if (tbl == -1) {
            wcerr << L"Error adding cell: " << p.get_errmsg() << endl;
            return 2;
        }

        /* ----- Colorized background */
        col++;

        optlist.str(L"");
        optlist << L"fittextline={font=" << font
                << L" fontsize=12 fillcolor=white} matchbox={fillcolor={orange}}";

        tbl = p.add_table_cell(tbl, col, row, L"colorized cell", optlist.str()); 

        /* ----- Multi-line text with Textflow */
        col++;

        tf = p.add_textflow(tf, tf_text,
                L"charref fontname=NotoSerif-Regular fontsize=8 "
                L"alignment=justify");
        if (tf == -1) {
            wcerr << L"Error: " << p.get_errmsg() << endl; return(2);
        }

        optlist.str(L"");
        optlist << L"marginleft=2 marginright=2 margintop=2 marginbottom=2 "
                    L"textflow=" << tf;

        tbl = p.add_table_cell(tbl, col, row, L"", optlist.str());
        if (tbl == -1) {
            wcerr << L"Error adding cell: " << p.get_errmsg() << endl;
            return 2;
        }

        /* ----- Rotated image */
        col++;

        image = p.load_image(L"auto", imagefile, L"");
        if (image == -1) {
            wcerr << L"Couldn't load image: " << p.get_errmsg() << endl;
            return 2;
        }

        optlist.str(L"");
        optlist << L"image=" << image << L" fitimage={orientate=west} "
                << L"fittextline={fontname=NotoSerif-Regular fontsize=12 fillcolor=white}";

        tbl = p.add_table_cell(tbl, col, row, L"rotated image", optlist.str());
        if (tbl == -1) {
            wcerr << L"Error adding cell: " << p.get_errmsg() << endl;
            return 2;
        }
        /* ---------- Row 3: various kinds of content */
        row++;


        /* ----- Diagonal stamp */
        col=1;

        optlist.str(L"");
        optlist << L"rowheight=50 fittextline={font=" << font
                << L" fontsize=10 stamp=ll2ur}";

        tbl = p.add_table_cell(tbl, col, row, L"diagonal stamp", optlist.str());
        if (tbl == -1) {
            wcerr << L"Error: " << p.get_errmsg() << endl;
            return 2;
        }

        /* ----- SVG graphics */
        col++;

        /* Load the graphics */
        graphics = p.load_graphics(L"auto", graphicsfile, L"");
        if (graphics == -1){
            wcerr << L"Error: " << p.get_errmsg() << endl;
            return 2;
        }
        
        optlist.str(L"");
        optlist << L"margin=5 graphics=" << graphics << L" ";

        tbl = p.add_table_cell(tbl, col, row, L"", optlist.str());

        if (tbl == -1)
        {
            wcerr << L"Error adding cell: " << p.get_errmsg() << endl;
            return 2;
        }

        /* ----- Annotation: Web link */
        col++;
        
        int action = p.create_action(L"URI", L"url={https://www.pdflib.com}");
        
        optlist.str(L"");
        optlist << "margin=5 fittextline={fontname=NotoSerif-Regular fontsize=14 "
                << L"fillcolor=blue} annotationtype=Link " 
                << L"fitannotation={action={activate " << action << L"} linewidth=0}";

        tbl = p.add_table_cell(tbl, col, row, L"Web link", optlist.str());

        if (tbl == -1)
        {
            wcerr << L"Error adding cell: " << p.get_errmsg() << endl;
            return 2;
        }
        
        /* ----- Form field */
        col++;
        fieldfont = p.load_font(L"NotoSerif-Regular", L"winansi", L"simplefont nosubsetting");
        if (fieldfont == -1) {
            wcerr << L"Error: " << p.get_errmsg() << endl;
            return(2);
        } 
        optlist.str(L"");
        optlist << "margin=5 fieldtype=textfield fieldname={name} " 
                << L"fitfield={multiline linewidth=0 font=" << fieldfont 
                << " fontsize=12 alignment=center currentvalue={text field} "
                << L"scrollable=false}";

        tbl = p.add_table_cell(tbl, col, row, L"", optlist.str());

        if (tbl == -1)
        {
            wcerr << L"Error adding cell: " << p.get_errmsg() << endl;
            return 2;
        }


        /* ---------- Fill row 3 and above with their numbers */
        for (row++; row <= rowmax; row++) {
            for (col = 1; col <= colmax; col++) {
                wostringstream num;

                num << L"Col " << col << L"/Row " << row;

                optlist.str(L"");
                optlist << L"colwidth=25% fittextline={font=" << font
                        <<  L" fontsize=10}";

                tbl = p.add_table_cell(tbl, col, row, num.str(), optlist.str());
            }
        }

        /* ---------- Place the table on one or more pages ---------- */

        /*
         * Loop until all of the table is placed; create new pages
         * as long as more table instances need to be placed.
         */
        do {
            p.begin_page_ext(0, 0, L"width=a4.width height=a4.height");

            /* Shade every other row; draw lines for all table cells.
             * Add "showcells showborder" to visualize cell borders 
             */

            /* Place the table instance */
            result = p.fit_table(tbl, llx, lly, urx, ury, 
                        L"header=1 rowheightdefault=auto "
                        L"fill={{area=rowodd fillcolor={gray 0.9}}} "
                        L"stroke={{line=other}}");
            if (result == L"_error") {
                wcerr << L"Couldn't place table: " << p.get_errmsg() << endl;
                return 2;
            }

            p.end_page_ext(L"");

        } while (result == L"_boxfull");

        /* Check the result; "_stop" means all is ok. */
        if (result != L"_stop") {
            if (result == L"_error") {
                wcerr << L"Error when placing table: " << p.get_errmsg()
                        << endl;
                return 2;
            }
            else {
                /* Any other return value is a user exit caused by
                 * the "return" option; this requires dedicated code to
                 * deal with.
                 */
                wcerr << L"User return found in Table" << endl;
                return 2;
            }
        }
        /* This will also delete Textflow handles used in the table */
        p.delete_table(tbl, L"");

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
