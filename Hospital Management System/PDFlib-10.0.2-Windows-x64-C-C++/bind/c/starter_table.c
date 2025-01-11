/* 
 * Table starter:
 * Create table which may span multiple pages.
 * The table cells are filled with various content types including
 * Textflow, Textline, image, SVG, stamp, Web link (annotation) and
 * form field.
 *
 * Required data: image and SVG files, font
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "pdflib.h"


int
main(void)
{

    /* This is where the data files are. Adjust as necessary. */
    const char * searchpath = "../data";
    const char * imagefile = "nesrin.jpg";
    const char * graphicsfile = "PDFlib-logo.svg";

    PDF * p;

    int row, col, font, fieldfont, image, graphics, action, tf=-1, tbl=-1;
    int rowmax=50, colmax=4;

    double llx= 50, lly=50, urx=550, ury=800;
    const char * headertext = "Table header (row 1)";
    const char * result;
#define OPTLISTSIZE 256
    char optlist[OPTLISTSIZE];

    /* Dummy text for filling a cell with multi-line Textflow */
    const char * tf_text =
"Lorem ipsum dolor sit amet, consectetur adi&shy;pi&shy;sicing elit, \
sed do eius&shy;mod tempor incidi&shy;dunt ut labore et dolore magna \
ali&shy;qua. Ut enim ad minim ve&shy;niam, quis nostrud exer&shy;citation \
ull&shy;amco la&shy;bo&shy;ris nisi ut ali&shy;quip ex ea commodo \
con&shy;sequat. \
Duis aute irure dolor in repre&shy;henderit in voluptate velit esse \
cillum dolore \
eu fugiat nulla pari&shy;atur. Excep&shy;teur sint occae&shy;cat \
cupi&shy;datat \
non proident, sunt in culpa qui officia dese&shy;runt mollit anim id est \
laborum. ";

    /* create a new PDFlib object */
    if ((p = PDF_new()) == (PDF *) 0) {
        printf("Couldn't create PDFlib object (out of memory)\n");
        return(2);
    }

    PDF_TRY(p) {
        /* This means we must check return values of load_font() etc. */
        PDF_set_option(p, "errorpolicy=return");

        snprintf(optlist, OPTLISTSIZE, "SearchPath={{%s}} textformat=utf8", searchpath);
        PDF_set_option(p, optlist);

        if (PDF_begin_document(p, "starter_table.pdf", 0, "") == -1) {
            printf("Error: %s\n", PDF_get_errmsg(p));
            PDF_delete(p);
            return(2);
        }

        PDF_set_info(p, "Creator", "PDFlib starter sample");
        PDF_set_info(p, "Title", "starter_table");

        /* -------------------- Add table cells -------------------- */

        /* ---------- Row 1: table header (spans all columns) */
        row = 1; col = 1;
        PDF_set_option(p, "textformat=utf8");
        font = PDF_load_font(p, "NotoSerif-Regular", 0, "unicode", "");

        if (font == -1) {
            printf("Error: %s\n", PDF_get_errmsg(p));
            PDF_delete(p);
            return(2);
        }

        snprintf(optlist, OPTLISTSIZE,
            "fittextline={position=center font=%d fontsize=14} colspan=%d",
            font, colmax);

        tbl = PDF_add_table_cell(p, tbl, col, row, headertext, 0, optlist);

        if (tbl == -1) {
            printf("Error: adding cell: %s\n", PDF_get_errmsg(p));
            PDF_delete(p);
            return(2);
        }

        /* ---------- Row 2: various kinds of content */
        row++; 

        /* ----- Simple text cell */
        col=1;

        snprintf(optlist, OPTLISTSIZE, 
            "fittextline={font=%d fontsize=12 orientate=west}",
            font);

        tbl = PDF_add_table_cell(p, tbl, col, row, "vertical line", 0, optlist);

        if (tbl == -1) {
            printf("Error: adding cell: %s\n", PDF_get_errmsg(p));
            PDF_delete(p);
            return(2);
        }

        /* ----- Colorized background */
        col++;

        snprintf(optlist, OPTLISTSIZE, 
            "fittextline={font=%d fontsize=12 fillcolor=white} "
            "matchbox={fillcolor={orange}}", font);

        tbl = PDF_add_table_cell(p, tbl, col, row, "colorized cell", 0, optlist);

        if (tbl == -1) {
            printf("Error: adding cell: %s\n", PDF_get_errmsg(p));
            PDF_delete(p);
            return(2);
        }

        /* ----- Multi-line text with Textflow */
        col++;

        snprintf(optlist, OPTLISTSIZE, 
            "charref fontname=NotoSerif-Regular fontsize=8 alignment=justify");

        tf = PDF_add_textflow(p, tf, tf_text, 0, optlist);
        if (tf == -1) {
            printf("Error: adding cell: %s\n", PDF_get_errmsg(p));
            PDF_delete(p);
            return(2);
        }

        snprintf(optlist, OPTLISTSIZE, "margin=2 textflow=%d", tf);

        tbl = PDF_add_table_cell(p, tbl, col, row, "", 0, optlist);

        if (tbl == -1) {
            printf("Error: adding cell: %s\n", PDF_get_errmsg(p));
            PDF_delete(p);
            return(2);
        }

        /* ----- Rotated image */
        col++;

        image = PDF_load_image(p, "auto", imagefile, 0, "");

        if (image == -1) {
            printf("Couldn't load image: %s\n", PDF_get_errmsg(p));
            PDF_delete(p);
            return(2);
        }

        snprintf(optlist, OPTLISTSIZE, "image=%d fitimage={orientate=west} "
            "fittextline={fontname=NotoSerif-Regular fontsize=12 fillcolor=white}",
            image);

        tbl = PDF_add_table_cell(p, tbl, col, row, "rotated image", 0, optlist);

        if (tbl == -1) {
            printf("Error: adding cell: %s\n", PDF_get_errmsg(p));
            PDF_delete(p);
            return(2);
        }

        /* ---------- Row 3: various kinds of content */
        row++;

        /* ----- Diagonal stamp */
        col=1;

        snprintf(optlist, OPTLISTSIZE,
            "rowheight=50 fittextline={font=%d fontsize=10 stamp=ll2ur}", font);

        tbl = PDF_add_table_cell(p, tbl, col, row, "diagonal stamp", 0, optlist);

        if (tbl == -1) {
            printf("Error: adding cell: %s\n", PDF_get_errmsg(p));
            PDF_delete(p);
            return(2);
        }

        /* ----- SVG graphics */
        col++;

        /* Load the graphics */
        graphics = PDF_load_graphics(p, "auto", graphicsfile, 0, "");
        if (graphics == -1){
            printf("Error: %s\n", PDF_get_errmsg(p));
            PDF_delete(p);
            return(2);
        }
        
        snprintf(optlist, OPTLISTSIZE,  "margin=5 graphics=%d", graphics );

        tbl = PDF_add_table_cell(p, tbl, col, row, "", 0, optlist);

        if (tbl == -1) {
            printf("Error: adding cell: %s\n", PDF_get_errmsg(p));
            PDF_delete(p);
            return(2);
        }

        /* ----- Annotation: Web link*/
        col++;

        action = PDF_create_action(p, "URI", "url={https://www.pdflib.com}");

        snprintf(optlist, OPTLISTSIZE,  
            "margin=5 fittextline={fontname=NotoSerif-Regular fontsize=14 fillcolor=blue} "  
            "annotationtype=Link fitannotation={action={activate %d} linewidth=0}",
            action);

        tbl = PDF_add_table_cell(p, tbl, col, row, "Web link", 0, optlist);

        if (tbl == -1) {
            printf("Error: adding cell: %s\n", PDF_get_errmsg(p));
            PDF_delete(p);
            return(2);
        }

        /* ----- Form field */
        col++;
        fieldfont = PDF_load_font(p, "NotoSerif-Regular", 0, "winansi", "simplefont nosubsetting");

        if (fieldfont == -1) {
            printf("Error: %s\n", PDF_get_errmsg(p));
            PDF_delete(p);
            return(2);
        }
        snprintf(optlist, OPTLISTSIZE,  
            "margin=5 fieldtype=textfield fieldname={name} "
            "fitfield={multiline linewidth=0 font=%d fontsize=12 "
            "alignment=center currentvalue={text field} scrollable=false}",
            fieldfont);

        tbl = PDF_add_table_cell(p, tbl, col, row, "", 0, optlist);

        if (tbl == -1) {
            printf("Error: adding cell: %s\n", PDF_get_errmsg(p));
            PDF_delete(p);
            return(2);
        }

        /* ---------- Fill row 3 and above with their numbers */
        for (row++; row <= rowmax; row++)
        {
            for (col = 1; col <= colmax; col++)
            {
#define BUFSIZE 256  
                char num[BUFSIZE];

                snprintf(num, BUFSIZE, "Col %d/Row %d", col, row);
                snprintf(optlist, OPTLISTSIZE,
                    "colwidth=25%% fittextline={font=%d fontsize=10}", font);
                tbl = PDF_add_table_cell(p, tbl, col, row, num, 0, optlist);

                if (tbl == -1) {
                    printf("Error: adding cell: %s\n", PDF_get_errmsg(p));
                    PDF_delete(p);
                    return(2);
                }
            }
        }

        /* ---------- Place the table on one or more pages ---------- */

        /*
         * Loop until all of the table is placed; create new pages
         * as long as more table instances need to be placed.
         */
        do {
            PDF_begin_page_ext(p, 0, 0, "width=a4.width height=a4.height");

            /* Shade every other row; draw lines for all table cells.
             * Add "showcells showborder" to visualize cell borders.
             */
            snprintf(optlist, OPTLISTSIZE, "header=1 rowheightdefault=auto "
                    "fill={{area=rowodd fillcolor={gray 0.9}}} "
                    "stroke={{line=other}}");

            /* Place the table instance */
            result = PDF_fit_table(p, tbl, llx, lly, urx, ury, optlist);

            if (!strcmp(result, "_error"))
            {
                printf("Couldn't place table: %s\n", PDF_get_errmsg(p));
                PDF_delete(p);
                return(2);
            }

            PDF_end_page_ext(p, "");

        } while (!strcmp(result, "_boxfull"));

        /* Check the result; "_stop" means all is ok. */
        if (strcmp(result, "_stop"))
        {
            if (!strcmp(result, "_error"))
            {
                printf("Error when placing table: %s\n", PDF_get_errmsg(p));
                PDF_delete(p);
                return(2);
            }
            else
            {
                /* Any other return value is a user exit caused by
                 * the "return" option; this requires dedicated code to
                 * deal with.
                 */
                printf("User return found in Table\n");
                PDF_delete(p);
                return(2);
            }
        }

        /* This will also delete Textflow handles used in the table */
        PDF_delete_table(p, tbl, "");

        PDF_end_document(p, "");
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
