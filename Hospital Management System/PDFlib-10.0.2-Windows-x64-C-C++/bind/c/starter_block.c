/* 
 * Block starter:
 * Import a PDF page containing Blocks and fill text and image
 * Blocks with some data. For each recipient of the simulated
 * mailing a separate page with personalized information is
 * generated.
 *
 * The Textflow Blocks are processed with variable text lengths in mind:
 * if a Block doesn't fully use its vertical space or requires excess
 * space, the next Block is moved up or down accordingly.
 *
 * Required software: PDFlib Personalization Server (PPS)
 * Required data: Block PDF (template), images, font
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "pdflib.h"


int
main(void)
{
    /* This is where the data files are. Adjust as necessary. */
    const char* searchpath = "../data";

    /* By default annotations are also imported. In some cases this
     * requires the Noto fonts for creating annotation appearance streams.
     * We therefore set the searchpath to also point to the font directory.
     */

    const char * fontpath = "../../resource/font";
    const char *outfile = "starter_block.pdf";
    const char *infile = "block_template.pdf";

    PDF *p;
    int recipient, no_of_input_pages, pageno, *pagehandles, indoc;
#define OPTLISTSIZE 256  
    char optlist[OPTLISTSIZE];
    char baseopt[OPTLISTSIZE];

# define NO_OF_RECIPIENTS 3

    typedef struct {
        const char *name;       /* Block name */
        const char *contents[NO_OF_RECIPIENTS]; 
                                /* text Block: array with a string for each recipient
                                 * image Block: array with image file name for 
                                 *              each recipient 
                                 */
    } blockdata_s;

    /* Static text simulates database-driven main contents */
    const blockdata_s TextBlocks[] = {
        {
            "recipient",
            {   /* address data for each recipient */
                "Mr Maurizio Moroni\nStrada Provinciale 124\nReggio Emilia",

                "Ms Dominique Perrier\n25, Rue Lauriston\nParis",

                "Mr Liu Wong\n55 Grizzly Peak Rd.\nButte"
            }
        },
        {
            "announcement",
            {   /* greeting for each recipient */
                "Dear Paper Planes Fan,\n\n" 
                "we are happy to present our <fillcolor=red>best price offer" 
                "<fillcolor=black> especially for you:\n",
                
                "Bonjour,\n\n" 
                "here comes your personal <fillcolor=red>best price offer:\n",
                
                "Dear Paper Folder,\n\n" 
                "here's another exciting new paper plane especially for you. " 
                "We can supply this <fillcolor=red>best price offer" 
                "<fillcolor=black> only for a limited time and in limited quantity. " 
                "Don't hesitate and order your new plane today!\n",
            }
        },
        {
            "specialoffers",
            {   /* personalized offer for each recipient */
                "<fillcolor=red>Long Distance Glider<fillcolor=black>\n" 
                "With this paper rocket you can send all your " 
                "messages even when sitting in a hall or in the cinema pretty near " 
                "the back.\n",
                
                "<fillcolor=red>Giant Wing<fillcolor=black>\n" 
                "An unbelievable sailplane! It is amazingly robust and " 
                "can even do aerobatics. But it is best suited to gliding.\n" 
                "This paper arrow can be thrown with big swing. " 
                "We launched it from the roof of a hotel. It stayed in the air a " 
                "long time and covered a considerable distance.\n",
                
                "<fillcolor=red>Super Dart<fillcolor=black>\n" 
                "The super dart can fly giant loops with a radius of 4 " 
                "or 5 meters and cover very long distances. Its heavy cone point is " 
                "slightly bowed upwards to get the lift required for loops.\n"
            }
        },
        {
            "goodbye",
            {   /* bye bye phrase for each recipient */
                "Visit us on our Web site at <fillcolor=blue>www.kraxi.com<fillcolor=black>!\n\n" 
                "Yours sincerely,\nVictor Kraxi",
                "Make sure to order quickly at <fillcolor=blue>www.kraxi.com<fillcolor=black> " 
                "since we will soon run out of stock!\n\n" 
                "Yours sincerely,\nVictor Kraxi",

                "Make sure to order soon at <fillcolor=blue>www.kraxi.com<fillcolor=black>!\n\n" 
                "Your friendly staff at Kraxi Systems Paper Planes",
            }
        }
    };
    const blockdata_s ImageBlocks[] = {
        {
            "icon",
            {
                "plane1.png",
                "plane2.png",
                "plane3.png",
            }
        }
    };

    int no_of_text_blocks = sizeof(TextBlocks)/sizeof(TextBlocks[0]);
    int no_of_image_blocks = sizeof(ImageBlocks)/sizeof(ImageBlocks[0]);

    /* create a new PDFlib object */
    if ((p = PDF_new()) == (PDF *) 0)
    {
        printf("Couldn't create PDFlib object (out of memory)\n");
        return(2);
    }

    PDF_TRY(p)
    {
        /* This means we must check return values of load_font() etc.
         * Set the search path for fonts and images etc. */
        snprintf(optlist, OPTLISTSIZE, "errorpolicy=return  SearchPath={{%s}}", searchpath);
        PDF_set_option(p, optlist);

        snprintf(optlist, OPTLISTSIZE, "SearchPath={{%s}}", fontpath);
        PDF_set_option(p, optlist);


        if (PDF_begin_document(p, outfile, 0,
            "destination={type=fitwindow} pagelayout=singlepage") == -1)
        {
            printf("Error: %s\n", PDF_get_errmsg(p));
            PDF_delete(p);
            return(2);
        }

        PDF_set_info(p, "Creator", "PDFlib starter sample");
        PDF_set_info(p, "Title", "starter_block");

        /*  Open the Block template with PDFlib Blocks */
        indoc = PDF_open_pdi_document(p, infile, 0, "");
        if (indoc == -1)
        {
            printf("Error: %s\n", PDF_get_errmsg(p));
            PDF_delete(p);
            return(2);
        }

        no_of_input_pages = (int) PDF_pcos_get_number(p, indoc, "length:pages");

        /* Preload all pages of the Block template. We assume a small
         * number of input pages and a large number of generated output
         * pages. Therefore it makes sense to keep the input pages
         * open instead of opening them again for each recipient.
         * Add 1 because we use the 1-based page number as array index. 
         */

        pagehandles = (int *) malloc((no_of_input_pages+1) * sizeof(int));
        if (pagehandles == NULL)
        {
            printf("Error: couldn't allocate page handle array!\n");
            return(3);
        }

        for (pageno = 1; pageno <= no_of_input_pages; pageno++)
        {
            /* Open the next page and prepare it for cloning the page size */
            pagehandles[pageno] = PDF_open_pdi_page(p, indoc, pageno,
                                        "cloneboxes");

            if (pagehandles[pageno] == -1)
            {
                printf("Error: %s\n", PDF_get_errmsg(p));
                PDF_delete(p);
                return(2);
            }
        }

        /* For each recipient: place template page(s) and fill Blocks */
        for (recipient = 0; recipient < NO_OF_RECIPIENTS; recipient++)
        {
            /* Process all pages of the template document */
            for (pageno = 1; pageno <= no_of_input_pages; pageno++)
            {
                /* Accumulated unused or excess space in Textflow Blocks:
                 * - if positive, the previous Blocks didn't use their space, so
                 *   we move up the Block
                 * - if negative, the previous Blocks used excess space, so
                 *   we move down the Block
                 */

                int b;
                const blockdata_s *block;
                double accumulated_offset_y = 0;

                /* Start the next output page. The dummy size will be
                 * replaced with the cloned size of the input page.
                 */
                PDF_begin_page_ext(p, 0, 0, "width=a4.width height=a4.height");

                /* Place the imported page on the output page and clone all
                 * page boxes which are present in the input page; this will
                 * override the dummy size used in begin_page_ext().
                 */
                PDF_fit_pdi_page(p, pagehandles[pageno], 0, 0, "cloneboxes");

                /* Process all Textflow Blocks */
                for (b=0; b < no_of_text_blocks; b++) {
                    char blockpath[OPTLISTSIZE];
                    char optlistextend[OPTLISTSIZE];
                    double lly;

                    block = &TextBlocks[b];

                    /* The Textflow Blocks in the template use "fitmethod=nofit"
                     * which means we allow the Textflow to overflow the Block. 
                     */
                    strcpy(baseopt, "textformat=utf8");
                    strcpy(optlist, baseopt);

                    /* pCOS path for the current Block  */
                    snprintf(blockpath, OPTLISTSIZE, 
                        "pages[%d]/blocks/%s", (pageno-1), block->name);
                    
                    /* Retrieve y oordinates of the Block's lower left corner */
                    lly = PDF_pcos_get_number(p, indoc, "%s/Rect[1]", blockpath);
                    /* Adjust the vertical Block position by accumulated offset
                     * and make sure we don't fall off the bottom page edge.
                     * Similarly we could adjust the horizontal position.
                     */
                    double adjusted_lly = lly + accumulated_offset_y;
                    if (adjusted_lly < 0){ 
                        printf("Error for recipient %d (input page %d): "  
                            "Too much text in Textflow Blocks", recipient, pageno);
                        PDF_delete(p);
                        return(2);
                    }

                    /* The "refpoint" option overrides the Blocks's original
                     * position. We use relative coordinates (suffix "r") to move
                     * the Block up or down if the previous Blocks didn't use up
                     * their area or exceeded the Block area.
                     */
                    snprintf(optlistextend, OPTLISTSIZE, 
                        " refpoint={ 0r %fr }" , accumulated_offset_y);
                    strcat(optlist, optlistextend);

                    /* Create a matchbox for the Block contents, using the Block name as matchbox name */
                    snprintf(optlistextend, OPTLISTSIZE, 
                        " matchbox={name={%s}}" , block->name);
                    strcat(optlist, optlistextend);

                    /* Fill the text Block. */
                    if (PDF_fill_textblock(p, pagehandles[pageno], 
                            block->name, 
                            block->contents[recipient], 0, optlist) == -1){
                        printf("Warning: %s\n", PDF_get_errmsg(p));
                    }
                    else {
                        /* Calculate the height which wasn't used inside the Block
                         * or was used in excess outside the Block. This will be used
                         * for adjusting the position of the next Block.
                         */
                        if ((int) PDF_info_matchbox(p, block->name, 0, 1, "exists") == 1) {
                            /* We successfully filled a Textflow Block. Retrieve the bottom edge
                             * of the matchbox which gives the vertical end position of the contents...
                             */
                            double content_lly = PDF_info_matchbox(p, block->name, 0, 1, "y1");		/* (x1, y1) = lower left corner */

                            /* ...and use the distance from the bottom edge of the Block as offset */
                            accumulated_offset_y += (content_lly - adjusted_lly);
                        } else {
                            /* If the Block is empty, no corresponding matchbox exists.
                             * We use the Block height as offset to skip the whole Block,
                             * not taking into account possible space between the Blocks.
                             */
                            double ury = PDF_pcos_get_number(p, indoc, "%s/Rect[3]", blockpath);
                            accumulated_offset_y += (ury - lly);
                        }
                                            }
                }

                /* Process all image Blocks */
                for (b=0; b < no_of_image_blocks; b++) {
                    int image;
                    block = &ImageBlocks[b];
                    image = PDF_load_image(p, "auto", block->contents[recipient], 0, "");
                    if (image == -1) {
                        printf("Error: %s\n", PDF_get_errmsg(p));
                        PDF_delete(p);
                        return(2);
                    }

                    /* Fill image Block */
                    if (PDF_fill_imageblock(p, pagehandles[pageno], 
                            ImageBlocks[b].name, image, "") == -1){
                        printf("Error: %s\n", PDF_get_errmsg(p));
                    }
                    PDF_close_image(p, image);
                }
                PDF_end_page_ext(p, "");
            }
        }

        /* Close all input pages */
        for (pageno = 1; pageno <= no_of_input_pages; pageno++)
        {
            PDF_close_pdi_page(p, pagehandles[pageno]);
        }

        free(pagehandles);

        PDF_close_pdi_document(p, indoc);

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
