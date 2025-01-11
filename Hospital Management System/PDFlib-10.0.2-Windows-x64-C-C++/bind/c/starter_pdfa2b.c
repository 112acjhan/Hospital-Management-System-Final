/*
 * PDF/A-2b starter:
 * Create PDF/A-2b conforming output with layers, transparency, annotation and
 * PDF/A attachment.
 *
 * Required data: font file, image file
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "pdflib.h"

int
main(void)
{
    /* This is where the data files are. Adjust as necessary.*/
    const char searchpath[] = "../data";

    PDF *p;
    const char imagefile[] = "zebra.tif";
    const char * const attachments[] = { "lionel.pdf"};

    const size_t attachment_count  =
            sizeof(attachments) / sizeof(attachments[0]);
#define OPTLISTSIZE 256
    char optlist[OPTLISTSIZE];

    int font, image;
    int layer_english, layer_german, layer_french, layer_image;
    int textflow;
    double width, height;
    size_t i;
    char *bufp;

    /* create a new PDFlib object */
    if ((p = PDF_new()) == (PDF *) 0) {
        printf("Couldn't create PDFlib object (out of memory)\n");
        return(2);
    }

    PDF_TRY(p) {
        /* This means we must check return values of load_font() etc. */
        PDF_set_option(p, "errorpolicy=return");

        snprintf(optlist, OPTLISTSIZE, "SearchPath={{%s}}", searchpath);
        PDF_set_option(p, optlist);

        /* Initially display the layer panel and show the full page */
        if (PDF_begin_document(p, "starter_pdfa2b.pdf", 0,
                "openmode=layers viewerpreferences={fitwindow=true} "
                "pdfa=PDF/A-2b") == -1) {
            printf("Error: %s\n", PDF_get_errmsg(p));
            PDF_delete(p);
            return(2);
        }

        PDF_set_info(p, "Creator", "PDFlib starter sample");
        PDF_set_info(p, "Title", "starter_pdfa2b");

        /* Define the layers, with only English and image layers switched on. */
        layer_english = PDF_define_layer(p, "English text", 0, "");
        layer_german  = PDF_define_layer(p, "German text", 0,
                                            "defaultstate=false");
        layer_french  = PDF_define_layer(p, "French text", 0,
                                            "defaultstate=false");
        layer_image   = PDF_define_layer(p, "Images", 0, "");

        /* Define a radio button relationship for the language layers, so only
         * one language can be switched on at a time.
         */
        snprintf(optlist, OPTLISTSIZE,
                "group={%d %d %d}",
                layer_english, layer_german, layer_french);
        PDF_set_layer_dependency(p, "Radiobtn", optlist);

        PDF_begin_page_ext(p, 0,0, "width=a4.width height=a4.height");

        font = PDF_load_font(p, "NotoSerif-Regular", 0, "winansi", "");

        if (font == -1) {
            printf("Error: %s\n", PDF_get_errmsg(p));
            PDF_delete(p);
            return(2);
        }

        // Create a Stamp annotation
        snprintf(optlist, OPTLISTSIZE,
                "iconname=approved contents={PDF/A} font=%d", font);
        PDF_create_annotation(p, 50, 700, 300, 840, "Stamp", optlist);

        snprintf(optlist, OPTLISTSIZE, "font=%d fontsize=24", font);

        PDF_begin_layer(p, layer_english);
        textflow = PDF_create_textflow(p,
                "PDF/A-2b starter sample with layers, transparency, "
                "annotation and attachment", 0, optlist);
        PDF_fit_textflow(p, textflow, 50, 650, 550, 700, "");
        PDF_delete_textflow(p, textflow);

        PDF_begin_layer(p, layer_german);
        textflow = PDF_create_textflow(p,
                "PDF/A-2b Starter-Beispiel mit Ebenen, Transparenz, "
                "Anmerkung und Anlage", 0, optlist);
        PDF_fit_textflow(p, textflow, 50, 650, 550, 700, "");
        PDF_delete_textflow(p, textflow);

        PDF_begin_layer(p, layer_french);
        textflow = PDF_create_textflow(p,
                "PDF/A-2b starter exemple avec des calques, "
                "transparence, commentaire et attachement", 0, optlist);
        PDF_fit_textflow(p, textflow, 50, 650, 550, 700, "");
        PDF_delete_textflow(p, textflow);

        PDF_begin_layer(p, layer_image);

        image = PDF_load_image(p, "auto", imagefile, 0, "");

        if (image == -1) {
            printf("Error: %s\n", PDF_get_errmsg(p));
            PDF_delete(p);
            return(2);
        }

        width = PDF_info_image(p, image, "width", "");
        height = PDF_info_image(p, image, "height", "");

        /* Place the image on the page and close it */
        PDF_fit_image(p, image, 0.0, 0.0, "");
        PDF_close_image(p, image);

        /* Place a transparent diagonal white stamp across the image area in
         * different languages and on different layers.
         */
        snprintf(optlist, OPTLISTSIZE,
            "boxsize={%f %f} stamp=ll2ur font=%d "
            "fillcolor={lab 100 0 0} gstate={opacityfill=0.5}",
            width, height, font);

        PDF_begin_layer(p, layer_english);
        PDF_fit_textline(p, "Transparent text", 0, 0, 0, optlist);

        PDF_begin_layer(p, layer_german);
        PDF_fit_textline(p, "Transparenter Text", 0, 0, 0, optlist);

        PDF_begin_layer(p, layer_french);
        PDF_fit_textline(p, "Texte transparent", 0, 0, 0, optlist);

        /* Close all layers */
        PDF_end_layer(p);

        PDF_end_page_ext(p, "");

        /* Construct option list with attachment handles. The attachments must
         * be PDF/A-1 or PDF/A-2 files.
         */
        bufp = optlist;
        int restsize = OPTLISTSIZE;
        int len = snprintf(bufp, (size_t) restsize, "%s", "attachments={");
        if (len >= restsize)
        {
            printf("option list buffer too small\n");
            PDF_delete(p);
            return(2);
        }
        bufp += len;
        restsize -= len;
        for (i = 0; i < attachment_count; i += 1) {
            const int attachment_handle =
                    PDF_load_asset(p, "Attachment", attachments[i], 0,
                                "description={This is a PDF/A attachment}");

            if (attachment_handle == -1) {
                printf("Error loading attachment: %s\n", PDF_get_errmsg(p));
                PDF_delete(p);
                return(2);
            }
            len = snprintf(bufp, (size_t) restsize, " %d", attachment_handle);
            if (len >= restsize)
            {
                printf("option list buffer too small\n");
                PDF_delete(p);
                return(2);
            }
            bufp += len;
            restsize -= len;
        }
        len = snprintf(bufp, (size_t) restsize, "%s", "}");
        if (len >= restsize)
        {
            printf("option list buffer too small\n");
            PDF_delete(p);
            return(2);
        }

        PDF_end_document(p, optlist);
    }
    PDF_CATCH(p) {
        printf("PDFlib exception occurred:\n");
        printf("[%d] %s: %s\n",
            PDF_get_errnum(p), PDF_get_apiname(p), PDF_get_errmsg(p));
        PDF_delete(p);
        return(2);
    }

    PDF_delete(p);

    return 0;
}
