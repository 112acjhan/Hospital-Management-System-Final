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

#include <iostream>
#include <vector>

#include "pdflib.hpp"

using namespace std;
using namespace pdflib;

int
main(void)
{
    /* This is where the data files are. Adjust as necessary. */
    const wstring searchpath = L"../data";

    /* By default annotations are also imported. In some cases this
     * requires the Noto fonts for creating annotation appearance streams.
     */
    const wstring fontpath = L"../../resource/font";

    const wstring outfile = L"starter_block.pdf";
    const wstring infile = L"block_template.pdf";

    PDFlib p;
    int indoc;

    const int number_of_recipients = 3;

    typedef struct {
    wstring name;           /* Blockname */
        wstring contents[number_of_recipients];
                            /* image Block: array with image file name for each recipient
                             *image Block: array with image file name for each recipient */
    } blockdata_s;

    /* Static text simulates database-driven main contents */
    const blockdata_s TextBlocks[] = {
        {
            L"recipient",
            {          // address data for each recipient 
                L"Mr Maurizio Moroni\nStrada Provinciale 124\nReggio Emilia",

                L"Ms Dominique Perrier\n25, Rue Lauriston\nParis",

                L"Mr Liu Wong\n55 Grizzly Peak Rd.\nButte"
            }
        },
        {
            L"announcement",
            {           // greeting for each recipient 
                L"Dear Paper Planes Fan,\n\n" 
                L"we are happy to present our <fillcolor=red>best price offer" 
                L"<fillcolor=black> especially for you:\n",
                
                L"Bonjour,\n\n" 
                L"here comes your personal <fillcolor=red>best price offer:\n",
                
                L"Dear Paper Folder,\n\n" 
                L"here's another exciting new paper plane especially for you. " 
                L"We can supply this <fillcolor=red>best price offer" 
                L"<fillcolor=black> only for a limited time and in limited quantity. " 
                L"Don't hesitate and order your new plane today!\n",
            }
        },
        {
            L"specialoffers",
            {           // personalized offer for each recipient 
                L"<fillcolor=red>Long Distance Glider<fillcolor=black>\n" 
                L"With this paper rocket you can send all your " 
                L"messages even when sitting in a hall or in the cinema pretty near " 
                L"the back.\n",
                
                L"<fillcolor=red>Giant Wing<fillcolor=black>\n" 
                L"An unbelievable sailplane! It is amazingly robust and " 
                L"can even do aerobatics. But it is best suited to gliding.\n" 
                L"This paper arrow can be thrown with big swing. " 
                L"We launched it from the roof of a hotel. It stayed in the air a " 
                L"long time and covered a considerable distance.\n",
                
                L"<fillcolor=red>Super Dart<fillcolor=black>\n" 
                L"The super dart can fly giant loops with a radius of 4 " 
                L"or 5 meters and cover very long distances. Its heavy cone point is " 
                L"slightly bowed upwards to get the lift required for loops.\n"
            }
        },
        {
            L"goodbye",
            {          // bye bye phrase for each recipient 
                L"Visit us on our Web site at <fillcolor=blue>www.kraxi.com<fillcolor=black>!\n\n" 
                L"Yours sincerely,\nVictor Kraxi",
                L"Make sure to order quickly at <fillcolor=blue>www.kraxi.com<fillcolor=black> " 
                L"since we will soon run out of stock!\n\n" 
                L"Yours sincerely,\nVictor Kraxi",

                L"Make sure to order soon at <fillcolor=blue>www.kraxi.com<fillcolor=black>!\n\n" 
                L"Your friendly staff at Kraxi Systems Paper Planes",
            }
        }
    };

    // Names and contents of image Block(s)
    const blockdata_s ImageBlocks[] = {
        {
            L"icon",
            {          // image file name for each recipient
                L"plane1.png", L"plane2.png", L"plane3.png"
            }
        }
    };

    const int no_of_text_blocks = sizeof(TextBlocks) / sizeof(TextBlocks[0]);
    const int no_of_image_blocks = sizeof(ImageBlocks) / sizeof(ImageBlocks[0]);
    wostringstream buf;

    try {

        // This means we must check return values of load_font() etc.
        // Set the search path for fonts and images etc.

        buf.str(L"");
        buf << L"errorpolicy=return searchpath={{" << searchpath << L"}}";
        p.set_option(buf.str());

        buf.str(L"");
        buf << L"searchpath={{" << fontpath << L"}}";
        p.set_option(buf.str());

        if (p.begin_document(outfile,
            L"destination={type=fitwindow} pagelayout=singlepage") == -1)
        {
            wcerr << L"Error: " << p.get_errmsg() << endl;
            return 2;
        }

        p.set_info(L"Creator", L"PDFlib starter sample");
        p.set_info(L"Title", L"starter_block");

        // Open the Block template with PDFlib Blocks
        indoc = p.open_pdi_document(infile, L"");
        if (indoc == -1)
        {
            wcerr << L"Error: " << p.get_errmsg() << endl;
            return 2;
        }

        int no_of_input_pages =
                (int) p.pcos_get_number(indoc, L"length:pages");

        // Preload all pages of the Block template. We assume a small
        // number of input pages and a large number of generated output
        // pages. Therefore it makes sense to keep the input pages
        // open instead of opening them again for each recipient.
        // Add 1 because we use the 1-based page number as array index.

        vector<int> pagehandles(no_of_input_pages + 1);

        int pageno;
        for (pageno = 1; pageno <= no_of_input_pages; pageno++)
        {
            // Open template page and prepare it for cloning the page size
            pagehandles[pageno] = p.open_pdi_page(indoc, pageno,
                                        L"cloneboxes");

            if (pagehandles[pageno] == -1)
            {
                wcerr << L"Error: " << p.get_errmsg() << endl;
                return 2;
            }
        }

        // For each recipient: place template page(s) and fill Blocks
        for (int recipient = 0; recipient < number_of_recipients; recipient++) {

            // Process all pages of the template document
            for (int pageno = 1; pageno <= no_of_input_pages; pageno++) {
                // Accumulated unused or excess space in Textflow Blocks:
                // - if positive, the previous Blocks didn't use their space, so
                //   we move up the Block
                // - if negative, the previous Blocks used excess space, so
                //   we move down the Block
                double accumulated_offset_y = 0;

                // Start the next output page. The page size will be
                // replaced with the cloned size of the input page.
                p.begin_page_ext(0, 0, L"width=a4.width height=a4.height");

                // Place the imported page on the output page, and clone all
                // page boxes which are present in the input page; this will
                // override the dummy size used in begin_page_ext().
                p.fit_pdi_page(pagehandles[pageno], 0, 0, L"cloneboxes");

                int b;

                // Process all Textflow Blocks
                for (b=0; b < no_of_text_blocks; b++){
                    wstring objtype;
                    wostringstream optlist, blockpath, pcos_optlist;

                    // The Textflow Blocks in the template use "fitmethod=nofit"
                    // which means we allow the Textflow to overflow the Block.
                    wstring baseopt = L"";
                    optlist << baseopt;

                    // pCOS path for the current Block 
                    
                    blockpath << L"pages[" << (pageno-1) << L"]/blocks/" << TextBlocks[b].name;

                    // Retrieve y coordinate of the Block's lower left corner
                    
                    pcos_optlist.str(L"");
                    pcos_optlist << blockpath.str() << "/Rect[1]";
                    double lly = p.pcos_get_number(indoc, pcos_optlist.str());

                    // Adjust the vertical Block position by accumulated offset
                    // and make sure we don't fall off the bottom page edge.
                    // Similarly we could adjust the horizontal position.
                    double adjusted_lly = lly + accumulated_offset_y;
                    if (adjusted_lly < 0){ 
                        wcerr << L"Error for recipient " << recipient << 
                            L" (input page " << pageno << L"): " << endl <<
                            L"Too much text in Textflow Blocks";
                        return 2;
                    }
                    // The "refpoint" option overrides the Blocks's original
                    // position. We use relative coordinates (suffix "r") to move
                    // the Block up or down if the previous Blocks didn't use up
                    // their area or exceeded the Block area.
                    optlist << L" refpoint={ 0r " << accumulated_offset_y << L"r }";

                    // Create a matchbox for the Block contents, using the Block name as matchbox name
                    optlist << L" matchbox={name={" << TextBlocks[b].name  << "}}";

                    // Fill the text Block.
                    if (p.fill_textblock(pagehandles[pageno], TextBlocks[b].name,
                        TextBlocks[b].contents[recipient], optlist.str()) == -1){
                        wcerr << L"Warning: " << p.get_errmsg() << endl;
                    } else{
                        // Calculate the height which wasn't used inside the Block
                        // or was used in excess outside the Block. This will be used
                        // for adjusting the position of the next Block.
                        
                        if ((int) p.info_matchbox(TextBlocks[b].name, 1, L"exists") == 1) {
                            // We successfully filled a Textflow Block. Retrieve the bottom edge
                            // of the matchbox which gives the vertical end position of the contents...
                            double content_lly = p.info_matchbox(TextBlocks[b].name, 1, L"y1");		// (x1, y1) = lower left corner

                            // ...and use the distance from the bottom edge of the Block as offset
                            accumulated_offset_y += (content_lly - adjusted_lly);
                        } else {
                            // If the Block is empty, no corresponding matchbox exists.
                            // We use the Block height as offset to skip the whole Block,
                            // not taking into account possible space between the Blocks.
                            pcos_optlist.str(L"");
                            pcos_optlist << blockpath.str() << "/Rect[3]";
                            double ury = p.pcos_get_number(indoc, pcos_optlist.str());
                            accumulated_offset_y += (ury - lly);
                        }

                    }
                }

                // Process all image Blocks
                for (b=0; b < no_of_image_blocks; b++){
                    int image = p.load_image(L"auto", ImageBlocks[b].contents[recipient], L"");
                    if (image == -1) {
                        wcerr << L"Error: " << p.get_errmsg() << endl;
                        return(2);
                    }

                    // Fill image Block
                    if (p.fill_imageblock(pagehandles[pageno], ImageBlocks[b].name, image, L"") == -1)
                        wcerr << L"Warning: " << p.get_errmsg() << endl;
                    p.close_image(image);
                }

                p.end_page_ext(L"");
            }
        }

        // Close the preloaded template pages
        for (int pageno = 1; pageno <= no_of_input_pages; pageno++) {
            p.close_pdi_page(pagehandles[pageno]);
        }
        p.close_pdi_document(indoc);

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
