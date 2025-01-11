/* 
 * Starter color:
 * Demonstrate basic use of supported color spaces
 *
 * Apply the following color spaces to text and vector graphics:
 * - Gray
 * - RGB
 * - CMYK
 * - ICC-based gray/rgb/cmyk
 * - spot(separation)
 * - Lab
 * - DeviceN
 * - pattern
 * - shadings
 *
 * Required data: font
 */

#include <iostream>
#include <sstream>

#include "pdflib.hpp"

using namespace std;
using namespace pdflib;

int
main(void)
{
    try {

        /* This is where the data files are. Adjust as necessary. */
        const wstring searchpath = L"../data";
        const wstring outfile = L"starter_color.pdf";

        PDFlib p;
        int font, icchandle, sh, shp, devicen, pattern;
        int y = 800, x = 50, xoffset1=80, xoffset2 = 100, yoffset = 70, r = 30;
        double w, h;
        wostringstream optlist, textbuf;

        /* This means that errors in function calls trigger an exception. */
        p.set_option(L"errorpolicy=exception");

        // Set the search path for fonts and PDF files
        optlist.str(L"");
        optlist << L"searchpath={{" << searchpath << L"}}";
        p.set_option(optlist.str());


        if (p.begin_document(outfile, L"errorpolicy=return") == -1) {
            wcerr << L"Error: " << p.get_errmsg() << endl;
            return 2;
        }

        p.set_info(L"Creator", L"PDFlib starter sample");
        p.set_info(L"Title", L"starter_color");

        /* Load the font */
        font = p.load_font(L"NotoSerif-Regular", L"unicode", L"");


        /* Start the page */
        p.begin_page_ext(0, 0, L"width=a4.width height=a4.height");

        p.setfont(font, 14);

        /*
         * -----------------------------------------------------------------
         * Use default colors
         * 
         * If no special color is set the default values will be used. The
         * default values are restored at the beginning of the page. 0=black
         * in the Gray color space is the default fill and stroke color in
         * many cases, as shown in our sample.
         * -----------------------------------------------------------------
         */

        /* Fill a circle with the default black fill color */
        p.circle(x, y -= yoffset, r);
        p.fill();

        /* Output text with default black fill color */
        p.fit_textline(
                L"Circle and text with default color {gray 0}", x
                        + xoffset2, y, L"");

        p.fit_textline(L"1.", x + xoffset1, y, L"");

        /*
         * -----------------------------------------------------------------
         * Use the Gray color space
         * 
         * Gray color is defined by gray values between 0=black and 1=white.
         * -----------------------------------------------------------------
         */

        /*
         * Set the current fill color to light gray 0.5 = 50% gray.
         */
        optlist.str(L"fillcolor={gray 0.5}");
        p.set_graphics_option(optlist.str());

        /* Fill a circle with the gray color defined above */
        p.circle(x, y -= yoffset, r);
        p.fill();

        /*  Output text with the gray color defined above */
        textbuf.str(L"");
        textbuf << L"Circle and text with " << optlist.str();
        p.fit_textline(textbuf.str(), x + xoffset2, y, L"");

        /* Alternatively you can set the fill color directly in the call to
         * fit_textline(). This sets the color just for a single function call.
         */
        p.fit_textline(L"2.", x + xoffset1, y, optlist.str());

        /*
         * -----------------------------------------------------------------
         * Use the RGB color space
         * 
         * RGB color is defined by RGB triples, i.e. three values between 0
         * and 1 specifying the percentage of red, green, and blue. (0, 0,
         * 0) is black and (1, 1, 1) is white. The commonly used RGB color
         * values in the range 0-255 must be divided by 255 in order to
         * scale them to the range 0-1 as required by PDFlib and PDF.
         * -----------------------------------------------------------------
         */

        /* Set the fill color to a green tint with RGB values (0.2, 0.8, 0.2).
         * Alternatively you can use HTML color names, e.g. "fillcolor=limegreen"
         */
        optlist.str(L"fillcolor={rgb 0.2 0.8 0.2}"); 
        p.set_graphics_option(optlist.str());

        /* Draw a circle with the current fill color defined above */
        p.circle(x, y -= yoffset, r);
        p.fill();

        /* Output a text line with the RGB fill color defined above */
        textbuf.str(L"");
        textbuf << L"Circle and text with " << optlist.str();
        p.fit_textline(textbuf.str(), x + xoffset2, y, L"");

        /* Alternatively you can set the fill color directly in the call to
         * fit_textline(). This sets the color just for a single function call.
         */
        p.fit_textline(L"3.", x + xoffset1, y, optlist.str());

        /*
         * -----------------------------------------------------------------
         * Use the CMYK color space
         * 
         * CMYK color is defined by four CMYK values between 0 = no color
         * and 1 = full color representing cyan, magenta, yellow, and black
         * values; (0, 0, 0, 0) is white and (0, 0, 0, 1) is black.
         * -----------------------------------------------------------------
         */

        /* Set the current fill color to a pale orange represented by
         * (0.1, 0.7, 0.7, 0.1) which defines 10% cyan, 70% magenta, 70% yellow,
         * and 10% black.
         */
        optlist.str(L""); 
        optlist << L"fillcolor={cmyk 0.1 0.7 0.7 0.1}"; 
        p.set_graphics_option(optlist.str());

        /* Fill a circle with the current fill color defined above */
        p.circle(x, y -= yoffset, r);
        p.fill();

        /* Output a text line with the CMYK fill color defined above */
        textbuf.str(L"");
        textbuf << L"Circle and text with " << optlist.str();
        p.fit_textline(textbuf.str(), x + xoffset2, y, L"");


        /* Alternatively you can set the fill color directly in the call to
         * fit_textline(). This sets the color just for a single function call.
         */
        p.fit_textline(L"4.", x + xoffset1, y, optlist.str());

        /*
         * -----------------------------------------------------------------
         * Use the Lab color
         * 
         * Device-independent color in the CIE L*a*b* color space is
         * specified by a luminance value in the range 0-100 and two color
         * values in the range -127 to 128. The a value contains the
         * green-red axis, while the b value contains the blue-yellow
         * axis.
         * -----------------------------------------------------------------
         * 
         */

        /* Set the current fill color to a loud blue represented by
         * (100, -127, -127).
         */
        optlist.str(L"");
        optlist << L"fillcolor={lab 100 -127 -127}"; 
        p.set_graphics_option(optlist.str());

        /* Fill a circle with the Lab fill color defined above */
        p.circle(x, y -= yoffset, r);
        p.fill();

        /* Output a text line with the Lab fill color defined above */
        textbuf.str(L"");
        textbuf << L"Circle and text with " << optlist.str();
        p.fit_textline(textbuf.str(), x + xoffset2, y, L"");


        /* Alternatively you can set the fill color directly in the call to
         * fit_textline(). This sets the color just for a single function call.
         */
        p.fit_textline(L"5.", x + xoffset1, y, optlist.str());

        /*
         * ---------------------------------------------------------------
         * Use ICC based color space
         * 
         * ICC-based colors are specified with the help of an ICC profile.
         * ---------------------------------------------------------------
         */

        /* Set the color based on the sRGB ICC profile to a grass-green
         * represented by the RGB color values (0.1 0.95 0.3) which
         * define 10% red, 95% green, and 30% blue.
         *
         * For the sRGB profile only, the following alternative without
         * any ICC profile handle could be used:
         *      optlist = "fillcolor={iccbased srgb 0.1 0.95 0.3}";
         */

        /* Load the sRGB profile. sRGB is guaranteed to be always available. */
        icchandle = p.load_iccprofile(L"sRGB", L"usage=iccbased");
        if (icchandle == -1) {
            wcerr << L"Error: " << p.get_errmsg() << endl;
            return 2;
        }

        /*
         * You can use similar syntax for CMYK and grayscale profiles with
         * the corresponding number of four or one color values.
         */
        optlist.str(L"");
        optlist << L"fillcolor={iccbased " << icchandle << L" 0.1 0.95 0.3}"; 
        p.set_graphics_option(optlist.str());
    
        /* Fill a circle with the ICC based RGB fill color defined above */
        p.circle(x, y-=yoffset, r);
        p.fill();
    
        /* Output a text line with the ICC based RGB fill color defined above.
         */
        textbuf.str(L"");
        textbuf << L"Circle and text with " << optlist.str();
        p.fit_textline(textbuf.str(), x + xoffset2, y, L"");

    
        /* Alternatively you can set the fill color directly in the call to
         * fit_textline(). This sets the color just for a single function call.
         */
        p.fit_textline(L"6.", x + xoffset1, y, optlist.str());

        
        /* --------------------------------------------------------------------
         * Use spot (separation) color
         *
         * Spot color (separation color space) is a builtin or user-defined
         * named color with an alternate representation in one of the
         * other color spaces above; this is generally used for preparing
         * documents which are intended to be printed on an offset printing
         * machine with one or more custom colors. The tint value (percentage)
         * ranges from 0=no color to 1=maximum intensity of the spot color.
         * --------------------------------------------------------------------
         */

        /* Set spot color "PANTONE 281 U" with a tint value of 1 (=100%)
         * Alternatively the following handle-based approach can be used:
         *
         *     spot = p.makespotcolor(L"PANTONE 281 U", 0);
         *     sprintf(optlist, "fillcolor={spot %d 1}", spot);
         *
         * See PDFlib Tutorial for defining custom spot colors.
         */
        optlist.str(L""); 
        optlist << "fillcolor={spotname {PANTONE 281 U} 1}"; 
        p.set_graphics_option(optlist.str());

        /* Fill a circle with the ICC based RGB fill color defined above */
        p.circle(x, y -= yoffset, r);
        p.fill();

        textbuf.str(L"");
        textbuf << L"Circle and text with " << optlist.str();
        p.fit_textline(textbuf.str(), x + xoffset2, y, L"");

    
        /* Alternatively you can set the fill color directly in the call to
         * fit_textline(). This sets the color just for a single function call.
         */
        p.fit_textline(L"7.", x + xoffset1, y, optlist.str());

        /* --------------------------------------------------------------------
         * Use DeviceN color
         *
         * DeviceN color spaces can use an arbitrary number of color
         * components. If these colorants are not available on the output
         * device, the colors are converted to an alternate color space
         * via a user-supplied transform function. The tint value ranges
         * from 0=no color to 1=maximum intensity.
         * --------------------------------------------------------------------
         */

        /* Set DeviceN color with colorants Magenta and Yellow with
         * alternate color space CMYK. The PostScript transform function
         * simply adds two 0 values for the Cyan and Black channels.
         */
        devicen = p.create_devicen(L"names={Magenta Yellow} alternate=devicecmyk transform={{0 0 4 1 roll}}");
        optlist.str(L"");
        optlist << L"fillcolor={devicen " << devicen << L" 0.5 1}"; 
        p.set_graphics_option(optlist.str());

        /* Fill a circle with the DeviceN fill color defined above */
        p.circle(x, y-=yoffset, r);
        p.fill();

        textbuf.str(L"");
        textbuf << L"Circle and text with " << optlist.str();
        p.fit_textline(textbuf.str(), x + xoffset2, y, L"");


        /* Alternatively you can set the fill color directly in the call to
         * fit_textline(). This sets the color just for a single function call.
         */
        p.fit_textline(L"8.", x+xoffset1, y, optlist.str());


        /* --------------------------------------------------------------------
         * Use Shading colorspace to draw a color gradient
         * --------------------------------------------------------------------
         */

        /* Create axial shading from red to blue */
        sh = p.shading(L"axial", 10, 10, 400, 300, 0, 0, 0, 0,
                L"startcolor=red endcolor=blue");

        shp = p.shading_pattern(sh, L"");
        optlist.str(L"");
        optlist << L"fillcolor={pattern " << shp << L"}";             
        p.set_graphics_option(optlist.str());

        /* Fill a circle with the shading defined above */
        p.circle(x, y-=yoffset, r);
        p.fill();

        textbuf.str(L"");
        textbuf << L"Circle and text with " << optlist.str();
        p.fit_textline(textbuf.str(), x + xoffset2, y, L"");


        /* Alternatively you can set the fill color directly in the call to
         * fit_textline(). This sets the color just for a single function call.
         */
        p.fit_textline(L"9.", x+xoffset1, y, optlist.str());


        /* --------------------------------------------------------------------
         * Use Pattern colorspace to fill objects with a geometric pattern
         * --------------------------------------------------------------------
         */
        w = 5;
        h = 10;

        /* Create a pattern containing geometric objects */
        pattern = p.begin_pattern_ext(w, h, L"");

        /* Use RGB color for the pattern */
        p.setcolor(L"stroke", L"rgb", 0.4, 0.5, 0.2, 0);
        p.setlinewidth(w / 10);

        /* Set the line cap beyond the line end */
        p.set_graphics_option(L"linecap=2");

        /* Draw the pattern objects */
        p.moveto(0, 0);
        p.lineto(w, h / 2);
        p.lineto(0, h);
        p.stroke();

        p.moveto(0, h / 2);
        p.lineto(w / 2, h / 4);
        p.stroke();

        p.moveto(w, h);
        p.lineto(w / 2, 3 * h / 4);
        p.stroke();

        p.end_pattern();

        /* Now use the pattern colorspace */
        optlist.str(L"");
        optlist << L"fillcolor={pattern " << pattern << L"}";            
        p.set_graphics_option(optlist.str());

        /* Fill a circle with the pattern color defined above */
        p.circle(x, y-=yoffset, r);
        p.fill();


        /* Set text rendering to "fill and stroke text" to ensure that
         * patterned text remains readable.
         */
        p.set_text_option(L"textrendering=2");

        textbuf.str(L"");
        textbuf << L"Circle and text with " << optlist.str();
        p.fit_textline(textbuf.str(), x + xoffset2, y, L"");


        /* Alternatively you can set the fill color directly in the call to
         * fit_textline(). This sets the color just for a single function call.
         */
        p.fit_textline(L"10.", x+xoffset1, y, optlist.str());

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
