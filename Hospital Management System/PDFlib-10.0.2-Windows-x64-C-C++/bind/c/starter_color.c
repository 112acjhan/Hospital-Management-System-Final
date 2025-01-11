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
 */

#include <stdio.h>
#include <stdlib.h>

#include "pdflib.h"


int
main(void)
{
    /* This is where the data files are. Adjust as necessary. */
    const char* searchpath = "../data";
    const char* outfile = "starter_color.pdf";

    PDF * p;
    int font, devicen, sh, shp, pattern;
    int y = 800, x = 50, xoffset1=80, xoffset2 = 100, yoffset = 70, r = 30;
    double w, h;
    int icchandle;

    /* create a new PDFlib object */
    if ((p = PDF_new()) == (PDF *) 0) {
        printf("Couldn't create PDFlib object (out of memory)\n");
        return(2);
    }

    PDF_TRY(p) {
#define OPTLISTSIZE 256
        char optlist[OPTLISTSIZE];
#define BUFSIZE 512
        char textbuf[BUFSIZE];

        snprintf(optlist, OPTLISTSIZE, "SearchPath={{%s}}", searchpath);
        PDF_set_option(p, optlist);

        /* This means that errors in function calls trigger an exception. */
        PDF_set_option(p, "errorpolicy=exception textformat=utf8");

        /* For this important function we prefer an error return value */
        if (PDF_begin_document(p, outfile, 0, "errorpolicy=return") == -1) {
            printf("Error: %s\n", PDF_get_errmsg(p));
            PDF_delete(p);
            return(2);
        }

        PDF_set_info(p, "Creator", "PDFlib starter sample");
        PDF_set_info(p, "Title", "starter_color");

        /* Load the font */
        font = PDF_load_font(p, "NotoSerif-Regular", 0, "unicode", "");

        /* Start the page */
        PDF_begin_page_ext(p, 0, 0, "width=a4.width height=a4.height");

        PDF_setfont(p, font, 14);


        /* -------------------------------------------------------------------
         * Use default colors
         *
         * If no special color is set the default values will be used. The
         * default values are restored at the beginning of the page.
         * 0=black in the Gray color space is the default fill and stroke
         * color in many cases, as shown in our sample.
         * -------------------------------------------------------------------
         */

        /* Fill a circle with the default black fill color */
        PDF_circle(p, x, y-=yoffset, r);
        PDF_fill(p);

        /* Output text with default black fill color */
        PDF_fit_textline(p,
                "Circle and text with default color {gray 0}", 0,
                x+xoffset2, y, "");

        PDF_fit_textline(p, "1.", 0,  x+xoffset1, y, "");

        /* -------------------------------------------------------------------
         * Use the Gray color space
         *
         * Gray color is defined by gray values between 0=black and 1=white.
         * -------------------------------------------------------------------
         */

        /* Set the current fill color to light gray 0.5 = 50% gray.
         */
        snprintf(optlist, OPTLISTSIZE, "fillcolor={gray 0.5}");
        PDF_set_graphics_option(p, optlist);

        /* Fill a circle with the current fill color defined above */
        PDF_circle(p, x, y-=yoffset, r);
        PDF_fill(p);

        /* Output text with the gray color defined above */
        snprintf(textbuf, BUFSIZE, "Circle and text with %s", optlist);
        PDF_fit_textline(p, textbuf, 0, x+xoffset2, y, "");

        /* Alternatively you can set the fill color directly in the call to
         * fit_textline(). This sets the color just for a single function call.
         */
        PDF_fit_textline(p, "2.", 0, x+xoffset1, y, optlist);


        /* --------------------------------------------------------------------
         * Use the RGB color space
         *
         * RGB color is defined by RGB triples, i.e. three values between 0 and
         * 1 specifying the percentage of red, green, and blue.
         * (0, 0, 0) is black and (1, 1, 1) is white. The commonly used RGB
         * color values in the range 0-255 must be divided by 255 in order to
         * scale them to the range 0-1 as required by PDFlib and PDF.
         * --------------------------------------------------------------------
         */

        /* Set the fill color to a green tint with RGB values (0.2, 0.8, 0.2).
         * Alternatively you can use HTML color names, e.g. "fillcolor=limegreen"
         */
        snprintf(optlist, OPTLISTSIZE, "fillcolor={rgb 0.2 0.8 0.2}");
        PDF_set_graphics_option(p, optlist);

        /* Draw a circle with the current fill color defined above */
        PDF_circle(p, x, y-=yoffset, r);
        PDF_fill(p);

        /* Output a text line with the RGB fill color defined above */
        snprintf(textbuf, BUFSIZE, "Circle and text with %s", optlist);
        PDF_fit_textline(p, textbuf, 0, x+xoffset2, y, "");

        /* Alternatively you can set the fill color directly in the call to
         * fit_textline(). This sets the color just for a single function call.
         */
        PDF_fit_textline(p, "3.", 0, x+xoffset1, y, optlist);


        /* --------------------------------------------------------------------
         * Use the CMYK color space
         *
         * CMYK color is defined by four CMYK values between 0 = no color and
         * 1 = full color representing Cyan, magenta, yellow, and black values;
         * (0, 0, 0, 0) is white and (0, 0, 0, 1) is black.
         * --------------------------------------------------------------------
         */

        /* Set the current fill color to a pale orange represented by
         * (0.1, 0.7, 0.7, 0.1) which defines 10% cyan, 70% magenta, 70% yellow,
         * and 10% black.
         */
        snprintf(optlist, OPTLISTSIZE, "fillcolor={cmyk 0.1 0.7 0.7 0.1}");
        PDF_set_graphics_option(p, optlist);

        /* Fill a circle with the current fill color defined above */
        PDF_circle(p, x, y-=yoffset, r);
        PDF_fill(p);

        /* Output a text line with the CMYK fill color defined above */
        snprintf(textbuf, BUFSIZE, "Circle and text with %s", optlist);
        PDF_fit_textline(p, textbuf, 0, x+xoffset2, y, "");

        /* Alternatively you can set the fill color directly in the call to
         * fit_textline(). This sets the color just for a single function call.
         */
        PDF_fit_textline(p, "4.", 0, x+xoffset1, y, optlist);


        /* --------------------------------------------------------------------
         * Use the Lab color space
         *
         * Device-independent color in the CIE L*a*b* color space is specified
         * by a luminance value in the range 0-100 and two color values in the
         * range -127 to 128. The a value contains the green-red axis,
         * while the b value contains the blue-yellow axis.
         * --------------------------------------------------------------------
         */

        /* Set the current fill color to a loud blue represented by
         * (100, -127, -127).
         */
        snprintf(optlist, OPTLISTSIZE, "fillcolor={lab 100 -127 -127}");
        PDF_set_graphics_option(p, optlist);

        /* Fill a circle with the Lab fill color defined above */
        PDF_circle(p, x, y-=yoffset, r);
        PDF_fill(p);

        snprintf(textbuf, BUFSIZE, "Circle and text with %s", optlist);
        PDF_fit_textline(p, textbuf, 0, x+xoffset2, y, "");

        /* Alternatively you can set the fill color directly in the call to
         * fit_textline(). This sets the color just for a single function call.
         */
        PDF_fit_textline(p, "5.", 0, x+xoffset1, y, optlist);


        /* ---------------------------------------------------------------
         * Use ICC-based color space
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
        icchandle = PDF_load_iccprofile(p, "sRGB", 0, "usage=iccbased");
        if (icchandle == -1) {
            printf("Error: %s\n", PDF_get_errmsg(p));
            PDF_delete(p);
            return(2);
        }

        /*
         * You can use similar syntax for CMYK and grayscale profiles with
         * the corresponding number of four or one color values.
         */
        snprintf(optlist, OPTLISTSIZE,
            "fillcolor={iccbased %d 0.1 0.95 0.3}", icchandle);
        PDF_set_graphics_option(p, optlist);

        /* Fill a circle with the ICC based RGB color defined above */
        PDF_circle(p, x, y-=yoffset, r);
        PDF_fill(p);

        /* Output a text line with the ICC based RGB fill color defined above.
         */
        snprintf(textbuf, BUFSIZE, "Circle and text with %s", optlist);
        PDF_fit_textline(p, textbuf, 0, x+xoffset2, y, "");

        /* Alternatively you can set the fill color directly in the call to
         * fit_textline(). This sets the color just for a single function call.
         */
        PDF_fit_textline(p, "6.", 0, x+xoffset1, y, optlist);


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
         *     spot = PDF_makespotcolor(p, "PANTONE 281 U", 0);
         *     snprintf(optlist, OPTLISTSIZE, "fillcolor={spot %d 1}", spot);
         *
         * See PDFlib Tutorial for defining custom spot colors.
         */
        snprintf(optlist, OPTLISTSIZE, "fillcolor={spotname {PANTONE 281 U} 1}");
        PDF_set_graphics_option(p, optlist);

        /* Fill a circle with the spot color defined above */
        PDF_circle(p, x, y-=yoffset, r);
        PDF_fill(p);

        snprintf(textbuf, BUFSIZE, "Circle and text with %s", optlist);
        PDF_fit_textline(p, textbuf, 0, x+xoffset2, y, "");

        /* Alternatively you can set the fill color directly in the call to
         * fit_textline(). This sets the color just for a single function call.
         */
        PDF_fit_textline(p, "7.", 0, x+xoffset1, y, optlist);


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
        devicen = PDF_create_devicen(p,
            "names={Magenta Yellow} alternate=devicecmyk transform={{0 0 4 1 roll}}");
        snprintf(optlist, OPTLISTSIZE, "fillcolor={devicen %d 0.5 1}", devicen);
        PDF_set_graphics_option(p, optlist);

        /* Fill a circle with the DeviceN fill color defined above */
        PDF_circle(p, x, y-=yoffset, r);
        PDF_fill(p);

        snprintf(textbuf, BUFSIZE, "Circle and text with %s", optlist);
        PDF_fit_textline(p, textbuf, 0, x+xoffset2, y, "");

        /* Alternatively you can set the fill color directly in the call to
         * fit_textline(). This sets the color just for a single function call.
         */
        PDF_fit_textline(p, "8.", 0, x+xoffset1, y, optlist);


        /* --------------------------------------------------------------------
         * Use Shading colorspace to draw a color gradient
         * --------------------------------------------------------------------
         */

        /* Create axial shading from red to blue */
        sh = PDF_shading(p, "axial", 10, 10, 400, 300, 0, 0, 0, 0,
             "startcolor=red endcolor=blue");

        shp = PDF_shading_pattern(p, sh, "");
        snprintf(optlist, OPTLISTSIZE, "fillcolor={pattern %d}", shp);
        PDF_set_graphics_option(p, optlist);

        /* Fill a circle with the shading defined above */
        PDF_circle(p, x, y-=yoffset, r);
        PDF_fill(p);

        snprintf(textbuf, BUFSIZE, "Circle and text with %s", optlist);
        PDF_fit_textline(p, textbuf, 0, x+xoffset2, y, "");

        /* Alternatively you can set the fill color directly in the call to
         * fit_textline(). This sets the color just for a single function call.
         */
        PDF_fit_textline(p, "9.", 0, x+xoffset1, y, optlist);


        /* --------------------------------------------------------------------
         * Use Pattern colorspace to fill objects with a geometric pattern
         * --------------------------------------------------------------------
         */
        w = 5;
        h = 10;

        /* Create a pattern containing geometric objects */
        pattern = PDF_begin_pattern_ext(p, w, h, "");

        /* Use RGB color for the pattern */
        PDF_setcolor(p, "stroke", "rgb", 0.4, 0.5, 0.2, 0);
        PDF_setlinewidth(p, w / 10);

        /* Set the line cap beyond the line end */
        PDF_set_graphics_option(p, "linecap=2");

        /* Draw the pattern objects */
        PDF_moveto(p, 0, 0);
        PDF_lineto(p, w, h / 2);
        PDF_lineto(p, 0, h);
        PDF_stroke(p);

        PDF_moveto(p, 0, h / 2);
        PDF_lineto(p, w / 2, h / 4);
        PDF_stroke(p);

        PDF_moveto(p, w, h);
        PDF_lineto(p, w / 2, 3 * h / 4);
        PDF_stroke(p);

        PDF_end_pattern(p);

        /* Now use the pattern colorspace */
        snprintf(optlist, OPTLISTSIZE, "fillcolor={pattern %d}", pattern);
        PDF_set_graphics_option(p, optlist);

        /* Fill a circle with the pattern color defined above */
        PDF_circle(p, x, y-=yoffset, r);
        PDF_fill(p);


        /* Set text rendering to "fill and stroke text" to ensure that
         * patterned text remains readable.
         */
        PDF_set_text_option(p, "textrendering=2");

        snprintf(textbuf, BUFSIZE, "Circle and text with %s", optlist);
        PDF_fit_textline(p, textbuf, 0, x+xoffset2, y, "");

        /* Alternatively you can set the fill color directly in the call to
         * fit_textline(). This sets the color just for a single function call.
         */
        PDF_fit_textline(p, "10.", 0, x+xoffset1, y, optlist);

        PDF_end_page_ext(p, "");

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
