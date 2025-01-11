/* 
 * PDF/UA-1 starter:
 * Create PDF/UA-1 document with various content types including structure
 * elements, artifacts, and interactive elements.
 *
 * Required data: font file, image
 */

#include <stdio.h>
#include <stdlib.h>

#include "pdflib.h"


int
main(void)
{
    /* This is where the data files are. Adjust as necessary. */
    const char* searchpath = "../data";
    const char *imagefile = "lionel.jpg";

    PDF *p;

    /* create a new PDFlib object */
    if ((p = PDF_new()) == (PDF *) 0) {
        printf("Couldn't create PDFlib object (out of memory)\n");
        return(2);
    }

    PDF_TRY(p)
    {
#define OPTLISTSIZE 256
        char optlist[OPTLISTSIZE];
        int id;
        int image, font, action;

        /* errorpolicy=exception means that the program will stop
        * if one of the API functions runs into a problem.
        */
        snprintf(optlist, OPTLISTSIZE, "errorpolicy=exception SearchPath={{%s}}", searchpath);
        PDF_set_option(p, optlist);

        PDF_set_option(p, "textformat=bytes");

        PDF_begin_document(p, "starter_pdfua1.pdf", 0,
            "pdfua=PDF/UA-1 lang=en "
        "tag={tagname=Document Title={PDFlib PDF/UA-1 demo}}");

        PDF_set_info(p, "Creator", "starter_pdfua1");
        PDF_set_info(p, "Title", "PDFlib PDF/UA-1 demo");

        /* Automatically create spaces between chunks of text */
        PDF_set_option(p, "autospace=true");

        PDF_begin_page_ext(p, 0, 0,
        "width=a4.width height=a4.height taborder=structure");

        PDF_create_bookmark(p, "PDF/UA-1 demo", 0, "");

        font = PDF_load_font(p, "NotoSerif-Regular", 0, "unicode", "");
        PDF_setfont(p, font, 24.0);

        /* =================== Simple text ======================== */

        /* Use abbreviated tagging with the "tag" option */
        PDF_fit_textline(p, "Introduction to Paper Planes", 0,
            50, 700, "tag={tagname=H1 Title={Introduction}} fontsize=24");

        PDF_fit_textline(p,
            "Paper planes can be made from any kind of paper.", 0,
            50, 675, "tag={tagname=P} fontsize=12");

        PDF_fit_textline(p, "Most paper planes don't have an engine.", 0,
            50, 650, "tag={tagname=P} fontsize=12");

        /* =================== Interactive Link ======================== */
        /* Open both P and Link elements in a single call */
        id = PDF_begin_item(p, "P", 
            "tag={tagname=Link Title={Kraxi on the Web}}");

        /* Create visible content which represents the link */
        PDF_fit_textline(p, "Learn more on the Kraxi website.", 0,
            50, 625,
            "matchbox={name={kraxi.com}} fontsize=12 "
            "strokecolor=blue fillcolor=blue underline");

        /* Create URI action */
        action = PDF_create_action(p, "URI", "url={http://www.kraxi.com}");

        /* Create Link annotation on named matchbox "kraxi.com".
        * This automatically creates an OBJR (object reference) element.
        */
        snprintf(optlist, OPTLISTSIZE, "linewidth=0 usematchbox={kraxi.com} "
            "contents={Link to Kraxi Inc. Web site} "
            "action={activate=%d} ", action);
        PDF_create_annotation(p, 0, 0, 0, 0, "Link", optlist);

        /* This closes the Link and P structure elements */
        PDF_end_item(p, id);

        /* =================== Image ======================== */
        /* A grouping element is required as container for Figure and Caption */
        id = PDF_begin_item(p, "Sect", "");
        image = PDF_load_image(p, "auto", imagefile, 0, "");

        /* The "Placement" attribute is recommended for Figure elements
          * as children of grouping elements.
          */ 

        PDF_fit_image(p, image, 50, 400,
            "tag={tagname=Figure Placement=Block "
            "Alt={Image of Kraxi waiting for customers.}} "
            "scale=0.5");
        PDF_close_image(p, image);

        /* Caption text below the image; Caption element follows Figure.
         * Since Caption doesn't allow direct content in PDF 1.7 we create
         * an additional P element.
         */
        PDF_fit_textline(p, "Kraxi waiting for customers.",  0, 
                50, 375,
                "tag={tagname=Caption tag={tagname=P}} fontsize=12 ");

        PDF_end_item(p, id);

        /* =================== Artifact ======================== */
        PDF_fit_textline(p, "Page 1", 0, 250, 100,
            "tag={tagname=Artifact} fontsize=12");

        PDF_end_page_ext(p, "");

        PDF_end_document(p, "");
    }

    PDF_CATCH(p)
    {
        printf("PDFlib exception occurred:\n");
        printf("[%d] %s: %s\n",
            PDF_get_errnum(p), PDF_get_apiname(p), PDF_get_errmsg(p));
        PDF_delete(p);
        return(2);
    }

    PDF_delete(p);

    return 0;
}
