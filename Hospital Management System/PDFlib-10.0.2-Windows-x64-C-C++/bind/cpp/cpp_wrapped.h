

/* Activate a previously created structure element or other content item. */
void
activate_item(int id)
{
    PDFCPP_TRY {
	MPDFLIB_API PDF_activate_item(p, id);
    }
    PDFCPP_CATCH;
}


/* Create a named destination on a page in the document. */
void
add_nameddest(const pstring& name, const pstring& optlist)
{
    std::string name_param;
    const char *p_name_param;
    int len_name;
    param_to_utf16(name, name_param, p_name_param, len_name);
    std::string optlist_param;
    const char *p_optlist_param;
    param_to_utf8(optlist, optlist_param, p_optlist_param);

    PDFCPP_TRY {
	MPDFLIB_API PDF_add_nameddest(p, p_name_param, len_name, p_optlist_param);
    }
    PDFCPP_CATCH;
}


/* Add a point to a new or existing path object. */
int
add_path_point(int path, double x, double y, const pstring& type, const pstring& optlist)
{
    int volatile retval = 0;

    std::string type_param;
    const char *p_type_param;
    param_to_bytes(type, type_param, p_type_param);
    std::string optlist_param;
    const char *p_optlist_param;
    param_to_utf8(optlist, optlist_param, p_optlist_param);

    PDFCPP_TRY {
	retval = MPDFLIB_API PDF_add_path_point(p, path, x, y, p_type_param, p_optlist_param);
    }
    PDFCPP_CATCH;

    return retval;
}


/* Add a file to a portfolio folder or a package. */
int
add_portfolio_file(int folder, const pstring& filename, const pstring& optlist)
{
    int volatile retval = 0;

    std::string filename_param;
    const char *p_filename_param;
    int len_filename;
    param_to_0utf16(filename, filename_param, p_filename_param, len_filename);
    std::string optlist_param;
    const char *p_optlist_param;
    param_to_utf8(optlist, optlist_param, p_optlist_param);

    PDFCPP_TRY {
	retval = MPDFLIB_API PDF_add_portfolio_file(p, folder, p_filename_param, len_filename, p_optlist_param);
    }
    PDFCPP_CATCH;

    return retval;
}


/* Add a folder to a new or existing portfolio. */
int
add_portfolio_folder(int parent, const pstring& foldername, const pstring& optlist)
{
    int volatile retval = 0;

    std::string foldername_param;
    const char *p_foldername_param;
    int len_foldername;
    param_to_0utf16(foldername, foldername_param, p_foldername_param, len_foldername);
    std::string optlist_param;
    const char *p_optlist_param;
    param_to_utf8(optlist, optlist_param, p_optlist_param);

    PDFCPP_TRY {
	retval = MPDFLIB_API PDF_add_portfolio_folder(p, parent, p_foldername_param, len_foldername, p_optlist_param);
    }
    PDFCPP_CATCH;

    return retval;
}


/* Add a cell to a new or existing table. */
int
add_table_cell(int table, int column, int row, const pstring& text, const pstring& optlist)
{
    int volatile retval = 0;

    std::string text_param;
    const char *p_text_param;
    int len_text;
    param_to_utf16(text, text_param, p_text_param, len_text);
    std::string optlist_param;
    const char *p_optlist_param;
    param_to_utf8(optlist, optlist_param, p_optlist_param);

    PDFCPP_TRY {
	retval = MPDFLIB_API PDF_add_table_cell(p, table, column, row, p_text_param, len_text, p_optlist_param);
    }
    PDFCPP_CATCH;

    return retval;
}


/* Create a Textflow object, or add text and explicit options to an existing Textflow. */
int
add_textflow(int textflow, const pstring& text, const pstring& optlist)
{
    int volatile retval = 0;

    std::string text_param;
    const char *p_text_param;
    int len_text;
    param_to_utf16(text, text_param, p_text_param, len_text);
    std::string optlist_param;
    const char *p_optlist_param;
    param_to_utf8(optlist, optlist_param, p_optlist_param);

    PDFCPP_TRY {
	retval = MPDFLIB_API PDF_add_textflow(p, textflow, p_text_param, len_text, p_optlist_param);
    }
    PDFCPP_CATCH;

    return retval;
}


/* Align the coordinate system with a relative vector. */
void
align(double dx, double dy)
{
    PDFCPP_TRY {
	MPDFLIB_API PDF_align(p, dx, dy);
    }
    PDFCPP_CATCH;
}


/* Draw a counterclockwise circular arc segment. */
void
arc(double x, double y, double r, double alpha, double beta)
{
    PDFCPP_TRY {
	MPDFLIB_API PDF_arc(p, x, y, r, alpha, beta);
    }
    PDFCPP_CATCH;
}


/* Draw a clockwise circular arc segment. */
void
arcn(double x, double y, double r, double alpha, double beta)
{
    PDFCPP_TRY {
	MPDFLIB_API PDF_arcn(p, x, y, r, alpha, beta);
    }
    PDFCPP_CATCH;
}


/* Create a new PDF file subject to various options. */
int
begin_document(const pstring& filename, const pstring& optlist)
{
    int volatile retval = 0;

    std::string filename_param;
    const char *p_filename_param;
    int len_filename;
    param_to_0utf16(filename, filename_param, p_filename_param, len_filename);
    std::string optlist_param;
    const char *p_optlist_param;
    param_to_utf8(optlist, optlist_param, p_optlist_param);

    PDFCPP_TRY {
	retval = MPDFLIB_API PDF_begin_document(p, p_filename_param, len_filename, p_optlist_param);
    }
    PDFCPP_CATCH;

    return retval;
}


/* Create a new node in the document part hierarchy (requires PDF/VT or   PDF 2.0). */
void
begin_dpart(const pstring& optlist)
{
    std::string optlist_param;
    const char *p_optlist_param;
    param_to_utf8(optlist, optlist_param, p_optlist_param);

    PDFCPP_TRY {
	MPDFLIB_API PDF_begin_dpart(p, p_optlist_param);
    }
    PDFCPP_CATCH;
}


/* Start a Type 3 font definition. */
void
begin_font(const pstring& fontname, double a, double b, double c, double d, double e, double f, const pstring& optlist)
{
    std::string fontname_param;
    const char *p_fontname_param;
    int len_fontname;
    param_to_0utf16(fontname, fontname_param, p_fontname_param, len_fontname);
    std::string optlist_param;
    const char *p_optlist_param;
    param_to_utf8(optlist, optlist_param, p_optlist_param);

    PDFCPP_TRY {
	MPDFLIB_API PDF_begin_font(p, p_fontname_param, len_fontname, a, b, c, d, e, f, p_optlist_param);
    }
    PDFCPP_CATCH;
}


/* Start a glyph definition for a Type 3 font. */
void
begin_glyph_ext(int uv, const pstring& optlist)
{
    std::string optlist_param;
    const char *p_optlist_param;
    param_to_utf8(optlist, optlist_param, p_optlist_param);

    PDFCPP_TRY {
	MPDFLIB_API PDF_begin_glyph_ext(p, uv, p_optlist_param);
    }
    PDFCPP_CATCH;
}


/* Open a structure element or other content item with attributes supplied as options. */
int
begin_item(const pstring& tagname, const pstring& optlist)
{
    int volatile retval = 0;

    std::string tagname_param;
    const char *p_tagname_param;
    param_to_bytes(tagname, tagname_param, p_tagname_param);
    std::string optlist_param;
    const char *p_optlist_param;
    param_to_utf8(optlist, optlist_param, p_optlist_param);

    PDFCPP_TRY {
	retval = MPDFLIB_API PDF_begin_item(p, p_tagname_param, p_optlist_param);
    }
    PDFCPP_CATCH;

    return retval;
}


/* Start a layer for subsequent output on the page. */
void
begin_layer(int layer)
{
    PDFCPP_TRY {
	MPDFLIB_API PDF_begin_layer(p, layer);
    }
    PDFCPP_CATCH;
}


/* Begin a marked content sequence with optional properties. */
void
begin_mc(const pstring& tagname, const pstring& optlist)
{
    std::string tagname_param;
    const char *p_tagname_param;
    param_to_bytes(tagname, tagname_param, p_tagname_param);
    std::string optlist_param;
    const char *p_optlist_param;
    param_to_utf8(optlist, optlist_param, p_optlist_param);

    PDFCPP_TRY {
	MPDFLIB_API PDF_begin_mc(p, p_tagname_param, p_optlist_param);
    }
    PDFCPP_CATCH;
}


/* Add a new page to the document, and specify various options. */
void
begin_page_ext(double width, double height, const pstring& optlist)
{
    std::string optlist_param;
    const char *p_optlist_param;
    param_to_utf8(optlist, optlist_param, p_optlist_param);

    PDFCPP_TRY {
	MPDFLIB_API PDF_begin_page_ext(p, width, height, p_optlist_param);
    }
    PDFCPP_CATCH;
}


/* Start a pattern definition with options. */
int
begin_pattern_ext(double width, double height, const pstring& optlist)
{
    int volatile retval = 0;

    std::string optlist_param;
    const char *p_optlist_param;
    param_to_utf8(optlist, optlist_param, p_optlist_param);

    PDFCPP_TRY {
	retval = MPDFLIB_API PDF_begin_pattern_ext(p, width, height, p_optlist_param);
    }
    PDFCPP_CATCH;

    return retval;
}


/* Start a template definition. */
int
begin_template_ext(double width, double height, const pstring& optlist)
{
    int volatile retval = 0;

    std::string optlist_param;
    const char *p_optlist_param;
    param_to_utf8(optlist, optlist_param, p_optlist_param);

    PDFCPP_TRY {
	retval = MPDFLIB_API PDF_begin_template_ext(p, width, height, p_optlist_param);
    }
    PDFCPP_CATCH;

    return retval;
}


/* Draw a circle. */
void
circle(double x, double y, double r)
{
    PDFCPP_TRY {
	MPDFLIB_API PDF_circle(p, x, y, r);
    }
    PDFCPP_CATCH;
}


/* Draw a circular arc segment defined by three points. */
void
circular_arc(double x1, double y1, double x2, double y2)
{
    PDFCPP_TRY {
	MPDFLIB_API PDF_circular_arc(p, x1, y1, x2, y2);
    }
    PDFCPP_CATCH;
}


/* Use the current path as clipping path, and terminate the path. */
void
clip()
{
    PDFCPP_TRY {
	MPDFLIB_API PDF_clip(p);
    }
    PDFCPP_CATCH;
}


/* Close an open font handle which has not yet been used in the document. */
void
close_font(int font)
{
    PDFCPP_TRY {
	MPDFLIB_API PDF_close_font(p, font);
    }
    PDFCPP_CATCH;
}


/* Close vector graphics. */
void
close_graphics(int graphics)
{
    PDFCPP_TRY {
	MPDFLIB_API PDF_close_graphics(p, graphics);
    }
    PDFCPP_CATCH;
}


/* Close an image or template. */
void
close_image(int image)
{
    PDFCPP_TRY {
	MPDFLIB_API PDF_close_image(p, image);
    }
    PDFCPP_CATCH;
}


/* Close all open PDI page handles, and close the input PDF document. */
void
close_pdi_document(int doc)
{
    PDFCPP_TRY {
	MPDFLIB_API PDF_close_pdi_document(p, doc);
    }
    PDFCPP_CATCH;
}


/* Close the page handle and free all page-related resources. */
void
close_pdi_page(int page)
{
    PDFCPP_TRY {
	MPDFLIB_API PDF_close_pdi_page(p, page);
    }
    PDFCPP_CATCH;
}


/* Close the current path. */
void
closepath()
{
    PDFCPP_TRY {
	MPDFLIB_API PDF_closepath(p);
    }
    PDFCPP_CATCH;
}


/* Close the path, fill, and stroke it. */
void
closepath_fill_stroke()
{
    PDFCPP_TRY {
	MPDFLIB_API PDF_closepath_fill_stroke(p);
    }
    PDFCPP_CATCH;
}


/* Close the path, and stroke it. */
void
closepath_stroke()
{
    PDFCPP_TRY {
	MPDFLIB_API PDF_closepath_stroke(p);
    }
    PDFCPP_CATCH;
}


/* Apply a transformation matrix to the current coordinate system. */
void
concat(double a, double b, double c, double d, double e, double f)
{
    PDFCPP_TRY {
	MPDFLIB_API PDF_concat(p, a, b, c, d, e, f);
    }
    PDFCPP_CATCH;
}


/* Print text at the next line. */
void
continue_text(const pstring& text)
{
    std::string text_param;
    const char *p_text_param;
    int len_text;
    param_to_utf16(text, text_param, p_text_param, len_text);

    PDFCPP_TRY {
	MPDFLIB_API PDF_continue_text2(p, p_text_param, len_text);
    }
    PDFCPP_CATCH;
}


/* Create a 3D view. */
int
create_3dview(const pstring& username, const pstring& optlist)
{
    int volatile retval = 0;

    std::string username_param;
    const char *p_username_param;
    int len_username;
    param_to_0utf16(username, username_param, p_username_param, len_username);
    std::string optlist_param;
    const char *p_optlist_param;
    param_to_utf8(optlist, optlist_param, p_optlist_param);

    PDFCPP_TRY {
	retval = MPDFLIB_API PDF_create_3dview(p, p_username_param, len_username, p_optlist_param);
    }
    PDFCPP_CATCH;

    return retval;
}


/* Create an action which can be applied to various objects and events. */
int
create_action(const pstring& type, const pstring& optlist)
{
    int volatile retval = 0;

    std::string type_param;
    const char *p_type_param;
    param_to_bytes(type, type_param, p_type_param);
    std::string optlist_param;
    const char *p_optlist_param;
    param_to_utf8(optlist, optlist_param, p_optlist_param);

    PDFCPP_TRY {
	retval = MPDFLIB_API PDF_create_action(p, p_type_param, p_optlist_param);
    }
    PDFCPP_CATCH;

    return retval;
}


/* Create an annotation on the current page. */
void
create_annotation(double llx, double lly, double urx, double ury, const pstring& type, const pstring& optlist)
{
    std::string type_param;
    const char *p_type_param;
    param_to_bytes(type, type_param, p_type_param);
    std::string optlist_param;
    const char *p_optlist_param;
    param_to_utf8(optlist, optlist_param, p_optlist_param);

    PDFCPP_TRY {
	MPDFLIB_API PDF_create_annotation(p, llx, lly, urx, ury, p_type_param, p_optlist_param);
    }
    PDFCPP_CATCH;
}


/* Create a DeviceN colorspace with an arbitrary number of color components. */
int
create_devicen(const pstring& optlist)
{
    int volatile retval = 0;

    std::string optlist_param;
    const char *p_optlist_param;
    param_to_utf8(optlist, optlist_param, p_optlist_param);

    PDFCPP_TRY {
	retval = MPDFLIB_API PDF_create_devicen(p, p_optlist_param);
    }
    PDFCPP_CATCH;

    return retval;
}


/* Create a bookmark subject to various options. */
int
create_bookmark(const pstring& text, const pstring& optlist)
{
    int volatile retval = 0;

    std::string text_param;
    const char *p_text_param;
    int len_text;
    param_to_utf16(text, text_param, p_text_param, len_text);
    std::string optlist_param;
    const char *p_optlist_param;
    param_to_utf8(optlist, optlist_param, p_optlist_param);

    PDFCPP_TRY {
	retval = MPDFLIB_API PDF_create_bookmark(p, p_text_param, len_text, p_optlist_param);
    }
    PDFCPP_CATCH;

    return retval;
}


/* Create a new form field or fill an imported form field. */
void
create_field(double llx, double lly, double urx, double ury, const pstring& name, const pstring& type, const pstring& optlist)
{
    std::string name_param;
    const char *p_name_param;
    int len_name;
    param_to_utf16(name, name_param, p_name_param, len_name);
    std::string type_param;
    const char *p_type_param;
    param_to_bytes(type, type_param, p_type_param);
    std::string optlist_param;
    const char *p_optlist_param;
    param_to_utf8(optlist, optlist_param, p_optlist_param);

    PDFCPP_TRY {
	MPDFLIB_API PDF_create_field(p, llx, lly, urx, ury, p_name_param, len_name, p_type_param, p_optlist_param);
    }
    PDFCPP_CATCH;
}


/* Create a form field group subject to various options. */
void
create_fieldgroup(const pstring& name, const pstring& optlist)
{
    std::string name_param;
    const char *p_name_param;
    int len_name;
    param_to_utf16(name, name_param, p_name_param, len_name);
    std::string optlist_param;
    const char *p_optlist_param;
    param_to_utf8(optlist, optlist_param, p_optlist_param);

    PDFCPP_TRY {
	MPDFLIB_API PDF_create_fieldgroup(p, p_name_param, len_name, p_optlist_param);
    }
    PDFCPP_CATCH;
}


/* Create a graphics state object subject to various options. */
int
create_gstate(const pstring& optlist)
{
    int volatile retval = 0;

    std::string optlist_param;
    const char *p_optlist_param;
    param_to_utf8(optlist, optlist_param, p_optlist_param);

    PDFCPP_TRY {
	retval = MPDFLIB_API PDF_create_gstate(p, p_optlist_param);
    }
    PDFCPP_CATCH;

    return retval;
}


/* Create a named virtual read-only file from data provided in memory. */
void
create_pvf(const pstring& filename, const void * data, size_t data_len, const pstring& optlist)
{
    std::string filename_param;
    const char *p_filename_param;
    int len_filename;
    param_to_0utf16(filename, filename_param, p_filename_param, len_filename);
    std::string optlist_param;
    const char *p_optlist_param;
    param_to_utf8(optlist, optlist_param, p_optlist_param);

    PDFCPP_TRY {
	MPDFLIB_API PDF_create_pvf(p, p_filename_param, len_filename, data, data_len, p_optlist_param);
    }
    PDFCPP_CATCH;
}


/* Create a Textflow object from text contents, inline options, and explicit options. */
int
create_textflow(const pstring& text, const pstring& optlist)
{
    int volatile retval = 0;

    std::string text_param;
    const char *p_text_param;
    int len_text;
    param_to_utf16(text, text_param, p_text_param, len_text);
    std::string optlist_param;
    const char *p_optlist_param;
    param_to_utf8(optlist, optlist_param, p_optlist_param);

    PDFCPP_TRY {
	retval = MPDFLIB_API PDF_create_textflow(p, p_text_param, len_text, p_optlist_param);
    }
    PDFCPP_CATCH;

    return retval;
}


/* Draw a Bezier curve from the current point, using 3 more control points. */
void
curveto(double x1, double y1, double x2, double y2, double x3, double y3)
{
    PDFCPP_TRY {
	MPDFLIB_API PDF_curveto(p, x1, y1, x2, y2, x3, y3);
    }
    PDFCPP_CATCH;
}


/* Create a new layer definition. */
int
define_layer(const pstring& name, const pstring& optlist)
{
    int volatile retval = 0;

    std::string name_param;
    const char *p_name_param;
    int len_name;
    param_to_utf16(name, name_param, p_name_param, len_name);
    std::string optlist_param;
    const char *p_optlist_param;
    param_to_utf8(optlist, optlist_param, p_optlist_param);

    PDFCPP_TRY {
	retval = MPDFLIB_API PDF_define_layer(p, p_name_param, len_name, p_optlist_param);
    }
    PDFCPP_CATCH;

    return retval;
}


/* Delete a path object. */
void
delete_path(int path)
{
    PDFCPP_TRY {
	MPDFLIB_API PDF_delete_path(p, path);
    }
    PDFCPP_CATCH;
}


/* Delete a named virtual file and free its data structures (but not the contents). */
int
delete_pvf(const pstring& filename)
{
    int volatile retval = 0;

    std::string filename_param;
    const char *p_filename_param;
    int len_filename;
    param_to_0utf16(filename, filename_param, p_filename_param, len_filename);

    PDFCPP_TRY {
	retval = MPDFLIB_API PDF_delete_pvf(p, p_filename_param, len_filename);
    }
    PDFCPP_CATCH;

    return retval;
}


/* Delete a table and all associated data structures. */
void
delete_table(int table, const pstring& optlist)
{
    std::string optlist_param;
    const char *p_optlist_param;
    param_to_utf8(optlist, optlist_param, p_optlist_param);

    PDFCPP_TRY {
	MPDFLIB_API PDF_delete_table(p, table, p_optlist_param);
    }
    PDFCPP_CATCH;
}


/* Delete a textflow and all associated data structures. */
void
delete_textflow(int textflow)
{
    PDFCPP_TRY {
	MPDFLIB_API PDF_delete_textflow(p, textflow);
    }
    PDFCPP_CATCH;
}


/* Download data from a network resource and store it in a disk-based or virtual file. */
int
download(const pstring& filename, const pstring& optlist)
{
    int volatile retval = 0;

    std::string filename_param;
    const char *p_filename_param;
    int len_filename;
    param_to_0utf16(filename, filename_param, p_filename_param, len_filename);
    std::string optlist_param;
    const char *p_optlist_param;
    param_to_utf8(optlist, optlist_param, p_optlist_param);

    PDFCPP_TRY {
	retval = MPDFLIB_API PDF_download(p, p_filename_param, len_filename, p_optlist_param);
    }
    PDFCPP_CATCH;

    return retval;
}


/* Draw a path object. */
void
draw_path(int path, double x, double y, const pstring& optlist)
{
    std::string optlist_param;
    const char *p_optlist_param;
    param_to_utf8(optlist, optlist_param, p_optlist_param);

    PDFCPP_TRY {
	MPDFLIB_API PDF_draw_path(p, path, x, y, p_optlist_param);
    }
    PDFCPP_CATCH;
}


/* Draw an ellipse. */
void
ellipse(double x, double y, double rx, double ry)
{
    PDFCPP_TRY {
	MPDFLIB_API PDF_ellipse(p, x, y, rx, ry);
    }
    PDFCPP_CATCH;
}


/* Draw an elliptical arc segment from the current point. */
void
elliptical_arc(double x, double y, double rx, double ry, const pstring& optlist)
{
    std::string optlist_param;
    const char *p_optlist_param;
    param_to_utf8(optlist, optlist_param, p_optlist_param);

    PDFCPP_TRY {
	MPDFLIB_API PDF_elliptical_arc(p, x, y, rx, ry, p_optlist_param);
    }
    PDFCPP_CATCH;
}


/* Add a glyph name and/or Unicode value to a custom 8-bit encoding. */
void
encoding_set_char(const pstring& encoding, int slot, const pstring& glyphname, int uv)
{
    std::string encoding_param;
    const char *p_encoding_param;
    param_to_bytes(encoding, encoding_param, p_encoding_param);
    std::string glyphname_param;
    const char *p_glyphname_param;
    param_to_bytes(glyphname, glyphname_param, p_glyphname_param);

    PDFCPP_TRY {
	MPDFLIB_API PDF_encoding_set_char(p, p_encoding_param, slot, p_glyphname_param, uv);
    }
    PDFCPP_CATCH;
}


/* Close the generated PDF document and apply various options. */
void
end_document(const pstring& optlist)
{
    std::string optlist_param;
    const char *p_optlist_param;
    param_to_utf8(optlist, optlist_param, p_optlist_param);

    PDFCPP_TRY {
	MPDFLIB_API PDF_end_document(p, p_optlist_param);
    }
    PDFCPP_CATCH;
}


/* Close a node in the document part hierarchy (requires PDF/VT or PDF 2.0). */
void
end_dpart(const pstring& optlist)
{
    std::string optlist_param;
    const char *p_optlist_param;
    param_to_utf8(optlist, optlist_param, p_optlist_param);

    PDFCPP_TRY {
	MPDFLIB_API PDF_end_dpart(p, p_optlist_param);
    }
    PDFCPP_CATCH;
}


/* Terminate a Type 3 font definition. */
void
end_font()
{
    PDFCPP_TRY {
	MPDFLIB_API PDF_end_font(p);
    }
    PDFCPP_CATCH;
}


/* Terminate a glyph definition for a Type 3 font. */
void
end_glyph()
{
    PDFCPP_TRY {
	MPDFLIB_API PDF_end_glyph(p);
    }
    PDFCPP_CATCH;
}


/* Close a structure element or other content item. */
void
end_item(int id)
{
    PDFCPP_TRY {
	MPDFLIB_API PDF_end_item(p, id);
    }
    PDFCPP_CATCH;
}


/* Deactivate all active layers. */
void
end_layer()
{
    PDFCPP_TRY {
	MPDFLIB_API PDF_end_layer(p);
    }
    PDFCPP_CATCH;
}


/* End the least recently opened marked content sequence. */
void
end_mc()
{
    PDFCPP_TRY {
	MPDFLIB_API PDF_end_mc(p);
    }
    PDFCPP_CATCH;
}


/* Finish a page, and apply various options. */
void
end_page_ext(const pstring& optlist)
{
    std::string optlist_param;
    const char *p_optlist_param;
    param_to_utf8(optlist, optlist_param, p_optlist_param);

    PDFCPP_TRY {
	MPDFLIB_API PDF_end_page_ext(p, p_optlist_param);
    }
    PDFCPP_CATCH;
}


/* Finish a pattern definition. */
void
end_pattern()
{
    PDFCPP_TRY {
	MPDFLIB_API PDF_end_pattern(p);
    }
    PDFCPP_CATCH;
}


/* Finish a template definition. */
void
end_template_ext(double width, double height)
{
    PDFCPP_TRY {
	MPDFLIB_API PDF_end_template_ext(p, width, height);
    }
    PDFCPP_CATCH;
}


/* End the current path without filling or stroking it. */
void
endpath()
{
    PDFCPP_TRY {
	MPDFLIB_API PDF_endpath(p);
    }
    PDFCPP_CATCH;
}


/* Fill the interior of the path with the current fill color. */
void
fill()
{
    PDFCPP_TRY {
	MPDFLIB_API PDF_fill(p);
    }
    PDFCPP_CATCH;
}


/* Fill a graphics block with variable data according to its properties. */
int
fill_graphicsblock(int page, const pstring& blockname, int graphics, const pstring& optlist)
{
    int volatile retval = 0;

    std::string blockname_param;
    const char *p_blockname_param;
    param_to_utf8(blockname, blockname_param, p_blockname_param);
    std::string optlist_param;
    const char *p_optlist_param;
    param_to_utf8(optlist, optlist_param, p_optlist_param);

    PDFCPP_TRY {
	retval = MPDFLIB_API PDF_fill_graphicsblock(p, page, p_blockname_param, graphics, p_optlist_param);
    }
    PDFCPP_CATCH;

    return retval;
}


/* Fill an image block with variable data according to its properties. */
int
fill_imageblock(int page, const pstring& blockname, int image, const pstring& optlist)
{
    int volatile retval = 0;

    std::string blockname_param;
    const char *p_blockname_param;
    param_to_utf8(blockname, blockname_param, p_blockname_param);
    std::string optlist_param;
    const char *p_optlist_param;
    param_to_utf8(optlist, optlist_param, p_optlist_param);

    PDFCPP_TRY {
	retval = MPDFLIB_API PDF_fill_imageblock(p, page, p_blockname_param, image, p_optlist_param);
    }
    PDFCPP_CATCH;

    return retval;
}


/* Fill a PDF block with variable data according to its properties. */
int
fill_pdfblock(int page, const pstring& blockname, int contents, const pstring& optlist)
{
    int volatile retval = 0;

    std::string blockname_param;
    const char *p_blockname_param;
    param_to_utf8(blockname, blockname_param, p_blockname_param);
    std::string optlist_param;
    const char *p_optlist_param;
    param_to_utf8(optlist, optlist_param, p_optlist_param);

    PDFCPP_TRY {
	retval = MPDFLIB_API PDF_fill_pdfblock(p, page, p_blockname_param, contents, p_optlist_param);
    }
    PDFCPP_CATCH;

    return retval;
}


/* Fill a Textline or Textflow Block with variable data according to its properties. */
void
fill_stroke()
{
    PDFCPP_TRY {
	MPDFLIB_API PDF_fill_stroke(p);
    }
    PDFCPP_CATCH;
}


/* Fill a Textline or Textflow Block with variable data according to its properties. */
int
fill_textblock(int page, const pstring& blockname, const pstring& text, const pstring& optlist)
{
    int volatile retval = 0;

    std::string blockname_param;
    const char *p_blockname_param;
    param_to_utf8(blockname, blockname_param, p_blockname_param);
    std::string text_param;
    const char *p_text_param;
    int len_text;
    param_to_utf16(text, text_param, p_text_param, len_text);
    std::string optlist_param;
    const char *p_optlist_param;
    param_to_utf8(optlist, optlist_param, p_optlist_param);

    PDFCPP_TRY {
	retval = MPDFLIB_API PDF_fill_textblock(p, page, p_blockname_param, p_text_param, len_text, p_optlist_param);
    }
    PDFCPP_CATCH;

    return retval;
}


/* Place vector graphics on a content stream, subject to various options. */
void
fit_graphics(int graphics, double x, double y, const pstring& optlist)
{
    std::string optlist_param;
    const char *p_optlist_param;
    param_to_utf8(optlist, optlist_param, p_optlist_param);

    PDFCPP_TRY {
	MPDFLIB_API PDF_fit_graphics(p, graphics, x, y, p_optlist_param);
    }
    PDFCPP_CATCH;
}


/* Place an image or template on the page, subject to various options. */
void
fit_image(int image, double x, double y, const pstring& optlist)
{
    std::string optlist_param;
    const char *p_optlist_param;
    param_to_utf8(optlist, optlist_param, p_optlist_param);

    PDFCPP_TRY {
	MPDFLIB_API PDF_fit_image(p, image, x, y, p_optlist_param);
    }
    PDFCPP_CATCH;
}


/* Place an imported PDF page on the page subject to various options. */
void
fit_pdi_page(int page, double x, double y, const pstring& optlist)
{
    std::string optlist_param;
    const char *p_optlist_param;
    param_to_utf8(optlist, optlist_param, p_optlist_param);

    PDFCPP_TRY {
	MPDFLIB_API PDF_fit_pdi_page(p, page, x, y, p_optlist_param);
    }
    PDFCPP_CATCH;
}


/* Fully or partially place a table on the page. */
pstring
fit_table(int table, double llx, double lly, double urx, double ury, const pstring& optlist)
{
    const char * volatile retval = NULL;
    pstring pstring_retval;

    std::string optlist_param;
    const char *p_optlist_param;
    param_to_utf8(optlist, optlist_param, p_optlist_param);

    PDFCPP_TRY {
	retval = MPDFLIB_API PDF_fit_table(p, table, llx, lly, urx, ury, p_optlist_param);
    }
    PDFCPP_CATCH;

    apiretval_to_pstring(retval, pstring_retval);

    return pstring_retval;
}


/* Format the next portion of a Textflow. */
pstring
fit_textflow(int textflow, double llx, double lly, double urx, double ury, const pstring& optlist)
{
    const char * volatile retval = NULL;
    pstring pstring_retval;

    std::string optlist_param;
    const char *p_optlist_param;
    param_to_utf8(optlist, optlist_param, p_optlist_param);

    PDFCPP_TRY {
	retval = MPDFLIB_API PDF_fit_textflow(p, textflow, llx, lly, urx, ury, p_optlist_param);
    }
    PDFCPP_CATCH;

    apiretval_to_pstring(retval, pstring_retval);

    return pstring_retval;
}


/* Place a single line of text at position (x, y) subject to various options. */
void
fit_textline(const pstring& text, double x, double y, const pstring& optlist)
{
    std::string text_param;
    const char *p_text_param;
    int len_text;
    param_to_utf16(text, text_param, p_text_param, len_text);
    std::string optlist_param;
    const char *p_optlist_param;
    param_to_utf8(optlist, optlist_param, p_optlist_param);

    PDFCPP_TRY {
	MPDFLIB_API PDF_fit_textline(p, p_text_param, len_text, x, y, p_optlist_param);
    }
    PDFCPP_CATCH;
}


/* Get the name of the API method which threw the last exception or failed. */
pstring
get_apiname()
{
    const char * volatile retval = NULL;
    pstring pstring_retval;

    PDFCPP_TRY {
	retval = MPDFLIB_API PDF_get_apiname(p);
    }
    PDFCPP_CATCH;

    apiretval_to_pstring(retval, pstring_retval);

    return pstring_retval;
}


/* Get the contents of the PDF output buffer. */
const char *
get_buffer(long *outputlen)
{
    const char *volatile retval = NULL;

    PDFCPP_TRY {
	retval = MPDFLIB_API PDF_get_buffer(p, outputlen);
    }
    PDFCPP_CATCH;

    return (const char*)retval;
}


/* Get the text of the last thrown exception or the reason of a failed method call. */
pstring
get_errmsg()
{
    const char * volatile retval = NULL;
    pstring pstring_retval;

    PDFCPP_TRY {
	retval = MPDFLIB_API PDF_get_errmsg(p);
    }
    PDFCPP_CATCH;

    apiretval_to_pstring(retval, pstring_retval);

    return pstring_retval;
}


/* Get the number of the last thrown exception or the reason of a failed method call. */
int
get_errnum()
{
    int volatile retval = 0;

    PDFCPP_TRY {
	retval = MPDFLIB_API PDF_get_errnum(p);
    }
    PDFCPP_CATCH;

    return retval;
}


/* Retrieve some option or other value. */
double
get_option(const pstring& keyword, const pstring& optlist)
{
    double volatile retval = 0;

    std::string keyword_param;
    const char *p_keyword_param;
    param_to_bytes(keyword, keyword_param, p_keyword_param);
    std::string optlist_param;
    const char *p_optlist_param;
    param_to_utf8(optlist, optlist_param, p_optlist_param);

    PDFCPP_TRY {
	retval = MPDFLIB_API PDF_get_option(p, p_keyword_param, p_optlist_param);
    }
    PDFCPP_CATCH;

    return retval;
}


/* Retrieve a string value. */
pstring
get_string(int idx, const pstring& optlist)
{
    const char * volatile retval = NULL;
    pstring pstring_retval;

    std::string optlist_param;
    const char *p_optlist_param;
    param_to_utf8(optlist, optlist_param, p_optlist_param);

    PDFCPP_TRY {
	retval = MPDFLIB_API PDF_get_string(p, idx, p_optlist_param);
    }
    PDFCPP_CATCH;

    apiretval_to_pstring(retval, pstring_retval);

    return pstring_retval;
}


/* Query detailed information about a loaded font. */
double
info_font(int font, const pstring& keyword, const pstring& optlist)
{
    double volatile retval = 0;

    std::string keyword_param;
    const char *p_keyword_param;
    param_to_bytes(keyword, keyword_param, p_keyword_param);
    std::string optlist_param;
    const char *p_optlist_param;
    param_to_utf8(optlist, optlist_param, p_optlist_param);

    PDFCPP_TRY {
	retval = MPDFLIB_API PDF_info_font(p, font, p_keyword_param, p_optlist_param);
    }
    PDFCPP_CATCH;

    return retval;
}


/* Format vector graphics and query metrics and other properties. */
double
info_graphics(int graphics, const pstring& keyword, const pstring& optlist)
{
    double volatile retval = 0;

    std::string keyword_param;
    const char *p_keyword_param;
    param_to_bytes(keyword, keyword_param, p_keyword_param);
    std::string optlist_param;
    const char *p_optlist_param;
    param_to_utf8(optlist, optlist_param, p_optlist_param);

    PDFCPP_TRY {
	retval = MPDFLIB_API PDF_info_graphics(p, graphics, p_keyword_param, p_optlist_param);
    }
    PDFCPP_CATCH;

    return retval;
}


/* Format an image and query metrics and other image properties. */
double
info_image(int image, const pstring& keyword, const pstring& optlist)
{
    double volatile retval = 0;

    std::string keyword_param;
    const char *p_keyword_param;
    param_to_bytes(keyword, keyword_param, p_keyword_param);
    std::string optlist_param;
    const char *p_optlist_param;
    param_to_utf8(optlist, optlist_param, p_optlist_param);

    PDFCPP_TRY {
	retval = MPDFLIB_API PDF_info_image(p, image, p_keyword_param, p_optlist_param);
    }
    PDFCPP_CATCH;

    return retval;
}


/* Query information about a matchbox on the current page. */
double
info_matchbox(const pstring& boxname, int num, const pstring& keyword)
{
    double volatile retval = 0;

    std::string boxname_param;
    const char *p_boxname_param;
    int len_boxname;
    param_to_0utf16(boxname, boxname_param, p_boxname_param, len_boxname);
    std::string keyword_param;
    const char *p_keyword_param;
    param_to_bytes(keyword, keyword_param, p_keyword_param);

    PDFCPP_TRY {
	retval = MPDFLIB_API PDF_info_matchbox(p, p_boxname_param, len_boxname, num, p_keyword_param);
    }
    PDFCPP_CATCH;

    return retval;
}


/* Query the results of drawing a path object without actually drawing it. */
double
info_path(int path, const pstring& keyword, const pstring& optlist)
{
    double volatile retval = 0;

    std::string keyword_param;
    const char *p_keyword_param;
    param_to_bytes(keyword, keyword_param, p_keyword_param);
    std::string optlist_param;
    const char *p_optlist_param;
    param_to_utf8(optlist, optlist_param, p_optlist_param);

    PDFCPP_TRY {
	retval = MPDFLIB_API PDF_info_path(p, path, p_keyword_param, p_optlist_param);
    }
    PDFCPP_CATCH;

    return retval;
}


/* Perform formatting calculations for a PDI page and query the resulting metrics. */
double
info_pdi_page(int page, const pstring& keyword, const pstring& optlist)
{
    double volatile retval = 0;

    std::string keyword_param;
    const char *p_keyword_param;
    param_to_bytes(keyword, keyword_param, p_keyword_param);
    std::string optlist_param;
    const char *p_optlist_param;
    param_to_utf8(optlist, optlist_param, p_optlist_param);

    PDFCPP_TRY {
	retval = MPDFLIB_API PDF_info_pdi_page(p, page, p_keyword_param, p_optlist_param);
    }
    PDFCPP_CATCH;

    return retval;
}


/* Query properties of a virtual file or the PDFlib Virtual Filesystem (PVF). */
double
info_pvf(const pstring& filename, const pstring& keyword)
{
    double volatile retval = 0;

    std::string filename_param;
    const char *p_filename_param;
    int len_filename;
    param_to_0utf16(filename, filename_param, p_filename_param, len_filename);
    std::string keyword_param;
    const char *p_keyword_param;
    param_to_bytes(keyword, keyword_param, p_keyword_param);

    PDFCPP_TRY {
	retval = MPDFLIB_API PDF_info_pvf(p, p_filename_param, len_filename, p_keyword_param);
    }
    PDFCPP_CATCH;

    return retval;
}


/* Query table information related to the most recently placed table instance. */
double
info_table(int table, const pstring& keyword)
{
    double volatile retval = 0;

    std::string keyword_param;
    const char *p_keyword_param;
    param_to_bytes(keyword, keyword_param, p_keyword_param);

    PDFCPP_TRY {
	retval = MPDFLIB_API PDF_info_table(p, table, p_keyword_param);
    }
    PDFCPP_CATCH;

    return retval;
}


/* Query the current state of a Textflow. */
double
info_textflow(int textflow, const pstring& keyword)
{
    double volatile retval = 0;

    std::string keyword_param;
    const char *p_keyword_param;
    param_to_bytes(keyword, keyword_param, p_keyword_param);

    PDFCPP_TRY {
	retval = MPDFLIB_API PDF_info_textflow(p, textflow, p_keyword_param);
    }
    PDFCPP_CATCH;

    return retval;
}


/* Perform textline formatting without creating output and query the resulting metrics. */
double
info_textline(const pstring& text, const pstring& keyword, const pstring& optlist)
{
    double volatile retval = 0;

    std::string text_param;
    const char *p_text_param;
    int len_text;
    param_to_utf16(text, text_param, p_text_param, len_text);
    std::string keyword_param;
    const char *p_keyword_param;
    param_to_bytes(keyword, keyword_param, p_keyword_param);
    std::string optlist_param;
    const char *p_optlist_param;
    param_to_utf8(optlist, optlist_param, p_optlist_param);

    PDFCPP_TRY {
	retval = MPDFLIB_API PDF_info_textline(p, p_text_param, len_text, p_keyword_param, p_optlist_param);
    }
    PDFCPP_CATCH;

    return retval;
}


/* Draw a line from the current point to another point. */
void
lineto(double x, double y)
{
    PDFCPP_TRY {
	MPDFLIB_API PDF_lineto(p, x, y);
    }
    PDFCPP_CATCH;
}


/* Load a 3D model from a disk-based or virtual file. */
int
load_3ddata(const pstring& filename, const pstring& optlist)
{
    int volatile retval = 0;

    std::string filename_param;
    const char *p_filename_param;
    int len_filename;
    param_to_0utf16(filename, filename_param, p_filename_param, len_filename);
    std::string optlist_param;
    const char *p_optlist_param;
    param_to_utf8(optlist, optlist_param, p_optlist_param);

    PDFCPP_TRY {
	retval = MPDFLIB_API PDF_load_3ddata(p, p_filename_param, len_filename, p_optlist_param);
    }
    PDFCPP_CATCH;

    return retval;
}


/* Load a multimedia asset or file attachment from a file or URL. */
int
load_asset(const pstring& type, const pstring& filename, const pstring& optlist)
{
    int volatile retval = 0;

    std::string type_param;
    const char *p_type_param;
    param_to_bytes(type, type_param, p_type_param);
    std::string filename_param;
    const char *p_filename_param;
    int len_filename;
    param_to_0utf16(filename, filename_param, p_filename_param, len_filename);
    std::string optlist_param;
    const char *p_optlist_param;
    param_to_utf8(optlist, optlist_param, p_optlist_param);

    PDFCPP_TRY {
	retval = MPDFLIB_API PDF_load_asset(p, p_type_param, p_filename_param, len_filename, p_optlist_param);
    }
    PDFCPP_CATCH;

    return retval;
}


/* Search for a font and prepare it for later use. */
int
load_font(const pstring& fontname, const pstring& encoding, const pstring& optlist)
{
    int volatile retval = 0;

    std::string fontname_param;
    const char *p_fontname_param;
    int len_fontname;
    param_to_0utf16(fontname, fontname_param, p_fontname_param, len_fontname);
    std::string encoding_param;
    const char *p_encoding_param;
    param_to_bytes(encoding, encoding_param, p_encoding_param);
    std::string optlist_param;
    const char *p_optlist_param;
    param_to_utf8(optlist, optlist_param, p_optlist_param);

    PDFCPP_TRY {
	retval = MPDFLIB_API PDF_load_font(p, p_fontname_param, len_fontname, p_encoding_param, p_optlist_param);
    }
    PDFCPP_CATCH;

    return retval;
}


/* Open a disk-based or virtual vector graphics file subject to various options. */
int
load_graphics(const pstring& type, const pstring& filename, const pstring& optlist)
{
    int volatile retval = 0;

    std::string type_param;
    const char *p_type_param;
    param_to_bytes(type, type_param, p_type_param);
    std::string filename_param;
    const char *p_filename_param;
    int len_filename;
    param_to_0utf16(filename, filename_param, p_filename_param, len_filename);
    std::string optlist_param;
    const char *p_optlist_param;
    param_to_utf8(optlist, optlist_param, p_optlist_param);

    PDFCPP_TRY {
	retval = MPDFLIB_API PDF_load_graphics(p, p_type_param, p_filename_param, len_filename, p_optlist_param);
    }
    PDFCPP_CATCH;

    return retval;
}


/* Search for an ICC profile, and prepare it for later use. */
int
load_iccprofile(const pstring& profilename, const pstring& optlist)
{
    int volatile retval = 0;

    std::string profilename_param;
    const char *p_profilename_param;
    int len_profilename;
    param_to_0utf16(profilename, profilename_param, p_profilename_param, len_profilename);
    std::string optlist_param;
    const char *p_optlist_param;
    param_to_utf8(optlist, optlist_param, p_optlist_param);

    PDFCPP_TRY {
	retval = MPDFLIB_API PDF_load_iccprofile(p, p_profilename_param, len_profilename, p_optlist_param);
    }
    PDFCPP_CATCH;

    return retval;
}


/* Open a disk-based or virtual image file subject to various options. */
int
load_image(const pstring& imagetype, const pstring& filename, const pstring& optlist)
{
    int volatile retval = 0;

    std::string imagetype_param;
    const char *p_imagetype_param;
    param_to_bytes(imagetype, imagetype_param, p_imagetype_param);
    std::string filename_param;
    const char *p_filename_param;
    int len_filename;
    param_to_0utf16(filename, filename_param, p_filename_param, len_filename);
    std::string optlist_param;
    const char *p_optlist_param;
    param_to_utf8(optlist, optlist_param, p_optlist_param);

    PDFCPP_TRY {
	retval = MPDFLIB_API PDF_load_image(p, p_imagetype_param, p_filename_param, len_filename, p_optlist_param);
    }
    PDFCPP_CATCH;

    return retval;
}


/* Find a built-in spot color name, or make a named spot color from the current fill color. */
int
makespotcolor(const pstring& spotname)
{
    int volatile retval = 0;

    std::string spotname_param;
    const char *p_spotname_param;
    param_to_utf8(spotname, spotname_param, p_spotname_param);

    PDFCPP_TRY {
	retval = MPDFLIB_API PDF_makespotcolor(p, p_spotname_param, 0);
    }
    PDFCPP_CATCH;

    return retval;
}


/* Add a marked content point with optional properties. */
void
mc_point(const pstring& tagname, const pstring& optlist)
{
    std::string tagname_param;
    const char *p_tagname_param;
    param_to_bytes(tagname, tagname_param, p_tagname_param);
    std::string optlist_param;
    const char *p_optlist_param;
    param_to_utf8(optlist, optlist_param, p_optlist_param);

    PDFCPP_TRY {
	MPDFLIB_API PDF_mc_point(p, p_tagname_param, p_optlist_param);
    }
    PDFCPP_CATCH;
}


/* Set the current point for graphics output. */
void
moveto(double x, double y)
{
    PDFCPP_TRY {
	MPDFLIB_API PDF_moveto(p, x, y);
    }
    PDFCPP_CATCH;
}


/* Open a disk-based or virtual PDF document and prepare it for later use. */
int
open_pdi_document(const pstring& filename, const pstring& optlist)
{
    int volatile retval = 0;

    std::string filename_param;
    const char *p_filename_param;
    int len_filename;
    param_to_0utf16(filename, filename_param, p_filename_param, len_filename);
    std::string optlist_param;
    const char *p_optlist_param;
    param_to_utf8(optlist, optlist_param, p_optlist_param);

    PDFCPP_TRY {
	retval = MPDFLIB_API PDF_open_pdi_document(p, p_filename_param, len_filename, p_optlist_param);
    }
    PDFCPP_CATCH;

    return retval;
}


/* Prepare a page for later use with PDF_fit_pdi_page(). */
int
open_pdi_page(int doc, int pagenumber, const pstring& optlist)
{
    int volatile retval = 0;

    std::string optlist_param;
    const char *p_optlist_param;
    param_to_utf8(optlist, optlist_param, p_optlist_param);

    PDFCPP_TRY {
	retval = MPDFLIB_API PDF_open_pdi_page(p, doc, pagenumber, p_optlist_param);
    }
    PDFCPP_CATCH;

    return retval;
}


/* Get the value of a pCOS path with type number or boolean. */
double
pcos_get_number(int doc, const pstring& path)
{
    double volatile retval = 0;

    std::string path_param;
    const char *p_path_param;
    param_to_utf8(path, path_param, p_path_param);

    PDFCPP_TRY {
	retval = MPDFLIB_API PDF_pcos_get_number(p, doc, "%s", p_path_param);
    }
    PDFCPP_CATCH;

    return retval;
}


/* Get the value of a pCOS path with type name, number, string, or boolean. */
pstring
pcos_get_string(int doc, const pstring& path)
{
    const char * volatile retval = NULL;
    pstring pstring_retval;

    std::string path_param;
    const char *p_path_param;
    param_to_utf8(path, path_param, p_path_param);

    PDFCPP_TRY {
	retval = MPDFLIB_API PDF_pcos_get_string(p, doc, "%s", p_path_param);
    }
    PDFCPP_CATCH;

    apiretval_to_pstring(retval, pstring_retval);

    return pstring_retval;
}


/* Get the contents of a pCOS path with type stream, fstream, or string. */
const unsigned char *
pcos_get_stream(int doc, int *outputlen, const pstring& optlist, const pstring& path)
{
    const unsigned char *volatile retval = NULL;

    std::string optlist_param;
    const char *p_optlist_param;
    param_to_utf8(optlist, optlist_param, p_optlist_param);
    std::string path_param;
    const char *p_path_param;
    param_to_utf8(path, path_param, p_path_param);

    PDFCPP_TRY {
	retval = MPDFLIB_API PDF_pcos_get_stream(p, doc, outputlen, p_optlist_param, "%s", p_path_param);
    }
    PDFCPP_CATCH;

    return (const unsigned char*)retval;
}


/* Delete a PDF container object. */
void
poca_delete(int container, const pstring& optlist)
{
    std::string optlist_param;
    const char *p_optlist_param;
    param_to_utf8(optlist, optlist_param, p_optlist_param);

    PDFCPP_TRY {
	MPDFLIB_API PDF_poca_delete(p, container, p_optlist_param);
    }
    PDFCPP_CATCH;
}


/* Insert a simple or container object in a PDF container object. */
void
poca_insert(int container, const pstring& optlist)
{
    std::string optlist_param;
    const char *p_optlist_param;
    param_to_utf8(optlist, optlist_param, p_optlist_param);

    PDFCPP_TRY {
	MPDFLIB_API PDF_poca_insert(p, container, p_optlist_param);
    }
    PDFCPP_CATCH;
}


/* Create a new PDF container object of type dictionary or array and insert objects. */
int
poca_new(const pstring& optlist)
{
    int volatile retval = 0;

    std::string optlist_param;
    const char *p_optlist_param;
    param_to_utf8(optlist, optlist_param, p_optlist_param);

    PDFCPP_TRY {
	retval = MPDFLIB_API PDF_poca_new(p, p_optlist_param);
    }
    PDFCPP_CATCH;

    return retval;
}


/* Remove a simple or container object from a PDF container object. */
void
poca_remove(int container, const pstring& optlist)
{
    std::string optlist_param;
    const char *p_optlist_param;
    param_to_utf8(optlist, optlist_param, p_optlist_param);

    PDFCPP_TRY {
	MPDFLIB_API PDF_poca_remove(p, container, p_optlist_param);
    }
    PDFCPP_CATCH;
}


/* Process certain elements of an imported PDF document. */
int
process_pdi(int doc, int page, const pstring& optlist)
{
    int volatile retval = 0;

    std::string optlist_param;
    const char *p_optlist_param;
    param_to_utf8(optlist, optlist_param, p_optlist_param);

    PDFCPP_TRY {
	retval = MPDFLIB_API PDF_process_pdi(p, doc, page, p_optlist_param);
    }
    PDFCPP_CATCH;

    return retval;
}


/* Draw a rectangle. */
void
rect(double x, double y, double width, double height)
{
    PDFCPP_TRY {
	MPDFLIB_API PDF_rect(p, x, y, width, height);
    }
    PDFCPP_CATCH;
}


/* Restore the most recently saved graphics state from the stack. */
void
restore()
{
    PDFCPP_TRY {
	MPDFLIB_API PDF_restore(p);
    }
    PDFCPP_CATCH;
}


/* Resume a page to add more content to it. */
void
resume_page(const pstring& optlist)
{
    std::string optlist_param;
    const char *p_optlist_param;
    param_to_utf8(optlist, optlist_param, p_optlist_param);

    PDFCPP_TRY {
	MPDFLIB_API PDF_resume_page(p, p_optlist_param);
    }
    PDFCPP_CATCH;
}


/* Rotate the coordinate system. */
void
rotate(double phi)
{
    PDFCPP_TRY {
	MPDFLIB_API PDF_rotate(p, phi);
    }
    PDFCPP_CATCH;
}


/* Save the current graphics state to a stack. */
void
save()
{
    PDFCPP_TRY {
	MPDFLIB_API PDF_save(p);
    }
    PDFCPP_CATCH;
}


/* Scale the coordinate system. */
void
scale(double sx, double sy)
{
    PDFCPP_TRY {
	MPDFLIB_API PDF_scale(p, sx, sy);
    }
    PDFCPP_CATCH;
}


/* Set one or more graphics appearance options. */
void
set_graphics_option(const pstring& optlist)
{
    std::string optlist_param;
    const char *p_optlist_param;
    param_to_utf8(optlist, optlist_param, p_optlist_param);

    PDFCPP_TRY {
	MPDFLIB_API PDF_set_graphics_option(p, p_optlist_param);
    }
    PDFCPP_CATCH;
}


/* Activate a graphics state object. */
void
set_gstate(int gstate)
{
    PDFCPP_TRY {
	MPDFLIB_API PDF_set_gstate(p, gstate);
    }
    PDFCPP_CATCH;
}


/* Fill document information field key with value. */
void
set_info(const pstring& key, const pstring& value)
{
    std::string key_param;
    const char *p_key_param;
    param_to_utf8(key, key_param, p_key_param);
    std::string value_param;
    const char *p_value_param;
    int len_value;
    param_to_utf16(value, value_param, p_value_param, len_value);

    PDFCPP_TRY {
	MPDFLIB_API PDF_set_info2(p, p_key_param, p_value_param, len_value);
    }
    PDFCPP_CATCH;
}


/* Define layer relationships. */
void
set_layer_dependency(const pstring& type, const pstring& optlist)
{
    std::string type_param;
    const char *p_type_param;
    param_to_bytes(type, type_param, p_type_param);
    std::string optlist_param;
    const char *p_optlist_param;
    param_to_utf8(optlist, optlist_param, p_optlist_param);

    PDFCPP_TRY {
	MPDFLIB_API PDF_set_layer_dependency(p, p_type_param, p_optlist_param);
    }
    PDFCPP_CATCH;
}


/* Set one or more global options. */
void
set_option(const pstring& optlist)
{
    std::string optlist_param;
    const char *p_optlist_param;
    param_to_utf8(optlist, optlist_param, p_optlist_param);

    PDFCPP_TRY {
	MPDFLIB_API PDF_set_option(p, p_optlist_param);
    }
    PDFCPP_CATCH;
}


/* Set one or more text filter or text appearance options for simple text output methods. */
void
set_text_option(const pstring& optlist)
{
    std::string optlist_param;
    const char *p_optlist_param;
    param_to_utf8(optlist, optlist_param, p_optlist_param);

    PDFCPP_TRY {
	MPDFLIB_API PDF_set_text_option(p, p_optlist_param);
    }
    PDFCPP_CATCH;
}


/* Set the position for simple text output on the page. */
void
set_text_pos(double x, double y)
{
    PDFCPP_TRY {
	MPDFLIB_API PDF_set_text_pos(p, x, y);
    }
    PDFCPP_CATCH;
}


/* Set the color space and color for the graphics and text state.. */
void
setcolor(const pstring& fstype, const pstring& colorspace, double c1, double c2, double c3, double c4)
{
    std::string fstype_param;
    const char *p_fstype_param;
    param_to_bytes(fstype, fstype_param, p_fstype_param);
    std::string colorspace_param;
    const char *p_colorspace_param;
    param_to_bytes(colorspace, colorspace_param, p_colorspace_param);

    PDFCPP_TRY {
	MPDFLIB_API PDF_setcolor(p, p_fstype_param, p_colorspace_param, c1, c2, c3, c4);
    }
    PDFCPP_CATCH;
}


/* Set the current font in the specified size. */
void
setfont(int font, double fontsize)
{
    PDFCPP_TRY {
	MPDFLIB_API PDF_setfont(p, font, fontsize);
    }
    PDFCPP_CATCH;
}


/* Set the current linewidth. */
void
setlinewidth(double width)
{
    PDFCPP_TRY {
	MPDFLIB_API PDF_setlinewidth(p, width);
    }
    PDFCPP_CATCH;
}


/* Explicitly set the current transformation matrix. */
void
setmatrix(double a, double b, double c, double d, double e, double f)
{
    PDFCPP_TRY {
	MPDFLIB_API PDF_setmatrix(p, a, b, c, d, e, f);
    }
    PDFCPP_CATCH;
}


/* Define a shading (color gradient) between two or more colors. */
int
shading(const pstring& type, double x0, double y0, double x1, double y1, double c1, double c2, double c3, double c4, const pstring& optlist)
{
    int volatile retval = 0;

    std::string type_param;
    const char *p_type_param;
    param_to_bytes(type, type_param, p_type_param);
    std::string optlist_param;
    const char *p_optlist_param;
    param_to_utf8(optlist, optlist_param, p_optlist_param);

    PDFCPP_TRY {
	retval = MPDFLIB_API PDF_shading(p, p_type_param, x0, y0, x1, y1, c1, c2, c3, c4, p_optlist_param);
    }
    PDFCPP_CATCH;

    return retval;
}


/* Define a shading pattern using a shading object. */
int
shading_pattern(int shading, const pstring& optlist)
{
    int volatile retval = 0;

    std::string optlist_param;
    const char *p_optlist_param;
    param_to_utf8(optlist, optlist_param, p_optlist_param);

    PDFCPP_TRY {
	retval = MPDFLIB_API PDF_shading_pattern(p, shading, p_optlist_param);
    }
    PDFCPP_CATCH;

    return retval;
}


/* Fill an area with a shading, based on a shading object. */
void
shfill(int shading)
{
    PDFCPP_TRY {
	MPDFLIB_API PDF_shfill(p, shading);
    }
    PDFCPP_CATCH;
}


/* Print text in the current font and size at the current position. */
void
show(const pstring& text)
{
    std::string text_param;
    const char *p_text_param;
    int len_text;
    param_to_utf16(text, text_param, p_text_param, len_text);

    PDFCPP_TRY {
	MPDFLIB_API PDF_show2(p, p_text_param, len_text);
    }
    PDFCPP_CATCH;
}


/* Print text in the current font at the specified position. */
void
show_xy(const pstring& text, double x, double y)
{
    std::string text_param;
    const char *p_text_param;
    int len_text;
    param_to_utf16(text, text_param, p_text_param, len_text);

    PDFCPP_TRY {
	MPDFLIB_API PDF_show_xy2(p, p_text_param, len_text, x, y);
    }
    PDFCPP_CATCH;
}


/* Skew the coordinate system. */
void
skew(double alpha, double beta)
{
    PDFCPP_TRY {
	MPDFLIB_API PDF_skew(p, alpha, beta);
    }
    PDFCPP_CATCH;
}


/* Calculate the width of text in an arbitrary font. */
double
stringwidth(const pstring& text, int font, double fontsize)
{
    double volatile retval = 0;

    std::string text_param;
    const char *p_text_param;
    int len_text;
    param_to_utf16(text, text_param, p_text_param, len_text);

    PDFCPP_TRY {
	retval = MPDFLIB_API PDF_stringwidth2(p, p_text_param, len_text, font, fontsize);
    }
    PDFCPP_CATCH;

    return retval;
}


/* Stroke the path with the current color and line width, and clear it. */
void
stroke()
{
    PDFCPP_TRY {
	MPDFLIB_API PDF_stroke(p);
    }
    PDFCPP_CATCH;
}


/* Suspend the current page so that it can later be resumed. */
void
suspend_page(const pstring& optlist)
{
    std::string optlist_param;
    const char *p_optlist_param;
    param_to_utf8(optlist, optlist_param, p_optlist_param);

    PDFCPP_TRY {
	MPDFLIB_API PDF_suspend_page(p, p_optlist_param);
    }
    PDFCPP_CATCH;
}


/* Translate the origin of the coordinate system. */
void
translate(double tx, double ty)
{
    PDFCPP_TRY {
	MPDFLIB_API PDF_translate(p, tx, ty);
    }
    PDFCPP_CATCH;
}
