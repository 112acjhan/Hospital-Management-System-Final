/*
 * PDF impose:
 * Import all pages from one more existing PDFs and place col x row pages
 * on each sheet of the output PDF (imposition). If there are annotations
 * on an imported page these are also imported and scaled or rotated as
 * required.
 *
 * Required software: PDFlib+PDI or PDFlib Personalization Server (PPS)
 * Required data: PDF documents
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

        /* This is where the data files are. Adjust as necessary. */
        const wstring searchpath = L"../data";

        /* By default annotations are also imported. In some cases this
         * requires the Noto fonts for creating annotation appearance streams.
         */
        const wstring fontpath = L"../../resource/font";

        const wstring outfile = L"starter_pdfimpose.pdf";
        const wstring title = L"PDF Impose";

        const wstring pdffiles[] =
        {
            L"markup_annotations.pdf",
            L"PLOP-datasheet.pdf",
            L"pCOS-datasheet.pdf"
        };
        const int filecount = sizeof(pdffiles) / sizeof(pdffiles[0]);
        int i, col = 0, row = 0;
        wostringstream optlist;
        double scale = 1;          // scaling factor of a page
        double rowheight;          // row height for the page to be placed
        double colwidth;           // column width for the page to be placed

        double sheetwidth = 595;   // width of the sheet
        double sheetheight = 842;  // height of the sheet
        int maxcol=2, maxrow=2;    // maxcol x maxrow pages will be placed on one sheet

        // Set the search path for fonts and PDF files
        optlist.str(L"");
        optlist << L"searchpath={{" << searchpath << L"}}";
        p.set_option(optlist.str());

        optlist.str(L"");
        optlist << L"searchpath={{" << fontpath << L"}}";
        p.set_option(optlist.str());


        /* This means we must check return values of load_font() etc. */
        p.set_option(L"errorpolicy=return");

        if (p.begin_document(L"starter_pdfimpose.pdf", L"") == -1) {
            wcerr << L"Error: " << p.get_errmsg() << endl;
                return 2;
        }

        p.set_info(L"Creator", L"PDFlib starter sample");
        p.set_info(L"Title", title);
        
        /* ---------------------------------------------------------------------
         * Define the sheet width and height,  the number of rows and columns
         * and calculate the scaling factor and cell dimensions for the 
         * multi-page imposition.
         * ---------------------------------------------------------------------
         */
        if (maxrow > maxcol)
        scale = 1.0 / maxrow;
        else
        scale = 1.0 / maxcol;

        rowheight = sheetheight * scale;
        colwidth = sheetwidth * scale;

        bool pageopen = false; // is a page open that must be closed?
        
        /* Loop over all input documents */
        for (i = 0; i < filecount; i++) 
        {
            int indoc, maxpage, pageno, page;

            /* Open the input PDF */
            indoc = p.open_pdi_document(pdffiles[i], L"");
            if (indoc == -1){
                wcerr << L"Error: " << p.get_errmsg() << endl;
                continue;
            }
        
            maxpage = (int) p.pcos_get_number(indoc, L"length:pages");
            
            /* Loop over all pages of the input document */
            for (pageno = 1; pageno <= maxpage; pageno++)
            {
                page = p.open_pdi_page(indoc, pageno, L"");

                if (page == -1) {
                    wcerr << L"Error: " << p.get_errmsg() << endl;
                    continue;
                }
                
                /* Start a new page */
                if (!pageopen) {
                    p.begin_page_ext(sheetwidth, sheetheight, L"");
                    pageopen = true;
                }
                
                /* The save/restore pair is required to get an independent
                 * clipping area for each mini page. Note that clipping
                 * is not required for the imported pages, but affects
                 * the rectangle around each page. Without clipping we
                 * would have to move the rectangle a bit inside the
                 * imported page to avoid drawing outside its area.
                 */
                p.save();
                
                // Clipping path for the rectangle
                p.rect(col * colwidth, sheetheight - (row + 1) * rowheight,
                colwidth, rowheight);
                p.clip();
                optlist.str(L"");
                optlist << L"boxsize={" << colwidth << L" " << rowheight << L"} fitmethod=meet";
                
                // Fit the imported page in the box
                p.fit_pdi_page(page, col * colwidth, 
                sheetheight - (row + 1) * rowheight, optlist.str());

                p.close_pdi_page(page);
                
                /* Draw a frame around the mini page */ 
                optlist.str(L"");
                optlist << L"linewidth=" << scale;
                p.set_graphics_option(optlist.str());
                p.rect(col * colwidth, sheetheight - (row + 1) * rowheight,
                colwidth, rowheight);
                p.stroke();
            
                p.restore();

                col++;
                
                // Start a new row if the current row is full
                if (col == maxcol) {
                    col = 0;
                    row++;
                }
                
                // Close the page if it is full
                if (row == maxrow) {
                    row = 0;
                    p.end_page_ext(L"");
                    pageopen = false;
                }
            }
            p.close_pdi_document(indoc);
        }
        
        if (pageopen) {
            p.end_page_ext(L"");
        }
        
        p.end_document(L"");

    }
    catch (PDFlib::Exception &ex) {
        wcerr << L"PDFlib exception occurred in starter_pdfimpose: " << endl
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
