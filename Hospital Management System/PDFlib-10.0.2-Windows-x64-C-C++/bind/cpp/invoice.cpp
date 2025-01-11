//
// PDFlib client: invoice example in C++
// Required software: PDFlib+PDI or PDFlib Personalization Server (PPS)
//

#include <iostream>
#include <string>

#include <time.h>

#if !defined(WIN32) && !defined(MAC)
#include <unistd.h>
#endif

#include "pdflib.hpp"

using namespace std;
using namespace pdflib;

double x_table = 55;
double tablewidth = 475;

double y_address = 682;
double x_salesrep = 455;
double y_invoice = 542;
double imagesize = 90;

double fontsize = 11;
double fontsizesmall = 9;

const  wstring fontname = L"NotoSerif-Regular";
PDFlib p;

static wostringstream basefontoptions;

struct articledata {
    articledata(wstring n, double pr, int q):
    name(n), price(pr), quantity(q) {}
    wstring name;
    double price;
    int quantity;
};

const articledata dataset[] = {
    articledata(L"Super Kite",         20,     2),
    articledata(L"Turbo Flyer",        40,     5),
    articledata(L"Giga Trash",         180,    1),
    articledata(L"Bare Bone Kit",      50,     3),
    articledata(L"Nitty Gritty",       20,     10),
    articledata(L"Pretty Dark Flyer",  75,     1),
    articledata(L"Free Gift",          0,      1),
};

const int ARTICLECOUNT = sizeof(dataset) / sizeof(dataset[0]);

static const wstring months[] = {
    L"January", L"February", L"March", L"April", L"May", L"June",
    L"July", L"August", L"September", L"October", L"November", L"December"
};


/* -----------------------------------
* Place company stationery as background
* -----------------------------------
*/
void create_stationery(PDFlib& p)
{
    wstring sender =
        L"Kraxi Systems, Inc. &#x2022; 17, Aviation Road &#x2022; Paperfield";
    wstring stationeryfontname= L"NotoSerif-Regular";

    wstring stationeryfilename = L"kraxi_logo.pdf";

    double y_company_logo = 748;

    wstring senderfull =
        L"17, Aviation Road\n" 
        L"Paperfield<nextline leading=50%><nextparagraph leading=120%>" 
        L"Phone 7079-4301\n" 
        L"Fax 7079-4302<nextline leading=50%><nextparagraph leading=120%>" 
        L"info@kraxi.com\n" 
        L"www.kraxi.com\n";

    int         page, stationery;
    wostringstream optlist;
    int         tf;

    stationery = p.open_pdi_document(stationeryfilename, L"");
    page = p.open_pdi_page(stationery, 1, L"");

    p.fit_pdi_page(page, 0, y_company_logo,
            L"boxsize={595 85} position={65 center}");
    p.close_pdi_page(page);
    p.close_pdi_document(stationery);

    optlist.str(L"");
    optlist <<  basefontoptions.str() << L" fontsize=" << fontsizesmall <<
        L" fontname=" << stationeryfontname << L" charref=true";
    p.fit_textline(sender, x_table, y_address + fontsize,
            optlist.str());

    /* ---------------------------------e
    * Print full company contact details
    * -----------------------------------
    */

    optlist.str(L"");
    optlist << basefontoptions.str() <<  L" fontname=" << stationeryfontname <<
        L" leading=125% fillcolor={cmyk 0.64 0.55 0.52 0.27}";
    tf = p.create_textflow(senderfull, optlist.str());
    p.fit_textflow(tf, x_salesrep, y_address,
            x_salesrep+imagesize, y_address + 150, L"verticalalign=bottom");
    p.delete_textflow(tf);
}

/* -----------------------------------
* Place address and header text
* -----------------------------------
*/
void create_header(PDFlib& p)
{
    const wstring salesrepfilename = L"sales_rep4.jpg";
    const wstring salesrepname = L"Lucy Irwin";
    const wstring salesrepcaption = L"Local sales rep:";
    const wstring invoiceheader = L"INVOICE 2012-03";
    time_t        timer;
    struct tm     ltime;


    wstring address =
        L"John Q. Doe\n" 
        L"255 Customer Lane\n" 
        L"Suite B\n" 
        L"12345 User Town\n" 
        L"Everland";

    wostringstream optlist;
    wostringstream date;
    int            salesrepimage;
    int            tf;

    /* -----------------------------------
    * Print address
    * -----------------------------------
    */
    optlist.str(L"");
    optlist << basefontoptions.str() << L" leading=120%";

    tf = p.create_textflow(address, optlist.str());
    p.fit_textflow(tf,
            x_table, y_address, x_table+tablewidth/2, y_address-100, L"");
    p.delete_textflow(tf);

    /* -----------------------------------
    * Place name and image of local sales rep
    * -----------------------------------
    */
    optlist.str(L"");
    optlist << basefontoptions.str() << L" fontsize=" << fontsizesmall;
    p.fit_textline(salesrepcaption, x_salesrep, y_address-fontsizesmall,
            optlist.str());
    p.fit_textline(salesrepname, x_salesrep, y_address-2*fontsizesmall,
            optlist.str());

    salesrepimage = p.load_image(L"auto", salesrepfilename, L"");

    optlist.str(L"");
    optlist << L"boxsize={" << imagesize << L" " <<  imagesize << L"} fitmethod=meet";
    p.fit_image(salesrepimage,
            x_salesrep, y_address-3*fontsizesmall-imagesize, optlist.str());
    p.close_image(salesrepimage);

    /* -----------------------------------
    * Print the header and date
    * -----------------------------------
    */

    /* Add a bookmark with the header text */
    p.create_bookmark(invoiceheader, L"");

    optlist.str(L"");
    optlist << basefontoptions.str();
    p.fit_textline(invoiceheader, x_table, y_invoice, optlist.str());
    time(&timer);
    ltime = *localtime(&timer);

    date.str(L"");
    date << months[ltime.tm_mon].c_str() << " " << ltime.tm_mday
        << ", " << ltime.tm_year + 1900;

    optlist.str(L"");
    optlist <<  L"position {100 0} " << basefontoptions.str();
    p.fit_textline(date.str(), x_table+tablewidth, y_invoice, optlist.str());
}

int
main(void)
{
    try {
        /* This is where font/image/PDF input files live. Adjust as necessary. */
        wstring searchpath = L"../data";

        /* By default annotations are also imported. In some cases this
         * requires the Noto fonts for creating annotation appearance streams.
         */
    
        const wstring fontpath = L"../../resource/font";

        wstring closingtext =
        L"Terms of payment: <fillcolor={rgb 1 0 0}>30 days net. " 
        L"<fillcolor={gray 0}>90 days warranty starting at the day of sale. " 
        L"This warranty covers defects in workmanship only. " 
        L"Kraxi Systems, Inc. will, at its option, repair or replace the " 
        L"product under the warranty. This warranty is not transferable. " 
        L"No returns or exchanges will be accepted for wet products.";

        static const wstring headers[] = {
            L"ITEM", L"DESCRIPTION", L"QUANTITY", L"PRICE", L"AMOUNT"
        };
        const int HEADERSCOUNT = sizeof(headers) / sizeof(headers[0]);

        static const wstring alignments[] = {
            L"right", L"left", L"right", L"right", L"right"
        };

        int            i, pagecount=0;
        int            row, col, tbl;
        int            tf;
        wstring        result;
        double         total;
        wostringstream buf;
        wostringstream optlist;

        optlist.str(L"");
        optlist << L"SearchPath={{" << searchpath << L"}}";

        p.set_option(optlist.str());

        optlist.str(L"");
        optlist << L"SearchPath={{" << fontpath << L"}}";

        p.set_option(optlist.str());

        /* This mean we don't have to check error return values, but will
        * get an exception in case of runtime problems.
        */
        p.set_option(L"errorpolicy=exception");

        p.begin_document(L"invoice.pdf", L"");

        p.set_info(L"Creator", L"invoice");
        p.set_info(L"Author", L"Thomas Merz");
        p.set_info(L"Title", L"PDFlib invoice generation demo");
        basefontoptions.str(L"");
        basefontoptions << L"fontname=" << fontname << L" fontsize=" <<
                fontsize << L"";

        /* -----------------------------------
        * Create and place table with article list
        * -----------------------------------
        */

        /* ---------- Header row */
        row = 1;
        tbl = -1;

        for (col=1; col <= HEADERSCOUNT; col++)
        {
            optlist.str(L"");
            optlist <<  L"fittextline={position={" << alignments[col-1] << 
                L" center} " << basefontoptions.str() << L"} margin=2";
            tbl = p.add_table_cell(tbl, col, row, headers[col-1], 
            optlist.str());
        }
        row++;

        /* ---------- Data rows: one for each article */
        total = 0;

        for (i = 0; i <  ARTICLECOUNT; i++) {
            double sum = dataset[i].price * dataset[i].quantity;
            col = 1;

            /* column 1: ITEM */
            buf.str(L"");
            buf << (i+1);
            optlist.str(L"");
            optlist << L"fittextline={position={" << alignments[col-1] << 
                L" center} " << basefontoptions.str() << L"} margin=2";
            tbl = p.add_table_cell(tbl, col++, row, buf.str(), optlist.str());

            /* column 2: DESCRIPTION */
            optlist.str(L"");
            optlist << L"fittextline={position={" << alignments[col-1] << 
                L" center} " << basefontoptions.str() << L"} colwidth=50% margin=2";
            tbl = p.add_table_cell(tbl, col++, row, dataset[i].name,
                    optlist.str());

            /* column 3: QUANTITY */
            buf.str(L"");
            buf << dataset[i].quantity;
            optlist.str(L"");
            optlist << L"fittextline={position={" << alignments[col-1] << 
                L" center} " << basefontoptions.str() << "} margin=2";
            tbl = p.add_table_cell(tbl, col++, row, buf.str(), optlist.str());

            /* column 4: PRICE */
            buf.str(L"");
            buf.setf(ios_base::fixed, ios_base::floatfield);
            buf.precision(2);
            buf << dataset[i].price ;

            optlist.str(L"");
            optlist << L"fittextline={position={" << alignments[col-1] << 
                L" center} " << basefontoptions.str() << L"} margin=2";
            tbl = p.add_table_cell(tbl, col++, row, buf.str(), optlist.str());

            /* column 5: AMOUNT */
            buf.str(L"");
            buf.setf(ios_base::fixed, ios_base::floatfield);
            buf.precision(2);
            buf << sum;

            optlist.str(L"");
            optlist << L"fittextline={position={" << alignments[col - 1] << 
                L" center} " << basefontoptions.str() << L" " << L"} margin=2";
            tbl = p.add_table_cell(tbl, col++, row, buf.str(), optlist.str());

            total += sum;
            row++;
        }

        /* ---------- Print total in the rightmost column */
        buf.str(L"");
        buf.setf(ios_base::fixed, ios_base::floatfield);
        buf.precision(2);
        buf << total;

        optlist.str(L"");
        optlist << L"fittextline={position={" <<
            alignments[HEADERSCOUNT - 1] << L" center} " <<
            basefontoptions.str() << L"} margin=2";
        tbl = p.add_table_cell(tbl, HEADERSCOUNT, row++, buf.str(), optlist.str());

        /* ---------- Footer row with terms of payment */
        optlist.str(L"");
        optlist << basefontoptions.str() << L" alignment=justify leading=120%";
        tf = p.create_textflow(closingtext, optlist.str());

        optlist.str(L"");
        optlist << L"rowheight=1 margin=2 margintop=" << 2*fontsize << 
            L" textflow=" << tf << L" colspan= " << HEADERSCOUNT;
        tbl = p.add_table_cell(tbl, 1, row++, L"", optlist.str());

        /* ---------- Place the table instance(s), creating pages as required */
        do {
            double top;

            p.begin_page_ext(0, 0, L"width=a4.width height=a4.height");

            if (++pagecount == 1)
            {
                create_stationery(p);
                create_header(p);
                top = y_invoice - 3*fontsize;
            }
            else
            {
                top = 50;
            }

            /* Place the table on the page; Shade every other row. */
            optlist.str(L"");
            optlist << L"header=1 fill={{area=rowodd fillcolor={gray 0.9}}} ";

            result = p.fit_table(tbl,
                    x_table, top, x_table+tablewidth, 20, optlist.str());

            if (result == L"_error") {
                wcerr << L"Couldn't place table: " << p.get_errmsg()
                    << endl;
                return 2;
            }

            p.end_page_ext(L"");
        } while (result == L"_boxfull");

        p.delete_table(tbl, L"");

        p.end_document(L"");
    }

    catch (PDFlib::Exception &ex) {
        wcerr << L"PDFlib exception occurred in invoice sample: " << endl
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
