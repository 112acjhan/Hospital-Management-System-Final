/*---------------------------------------------------------------------------*
 |          Copyright (c) 1997-2022 PDFlib GmbH. All rights reserved.        |
 +---------------------------------------------------------------------------+
 |    This software may not be copied or distributed except as expressly     |
 |    authorized by PDFlib GmbH's general license agreement or a custom      |
 |    license agreement signed by PDFlib GmbH.                               |
 |    For more information about licensing please refer to www.pdflib.com.   |
 *---------------------------------------------------------------------------*/

//
// C++ wrapper for PDFlib
//

#ifndef PDFLIB_HPP
#define PDFLIB_HPP

#include <string>
#include <sstream>
#include <iostream>
#include <stdexcept>


/*
 * PDFCPP_DL
 *
 * The pdflib.hpp header can be used for static linking against the PDFlib
 * library, or it can be configured for loading the PDFlib DLL dynamically at
 * runtime.
 *
 * The default is to compile for static linking against the PDFlib library. For
 * dynamic loading, define PDFCPP_DL as 1. In that case the resulting program
 * must not be linked against the PDFlib library. Instead the pdflibdl.c module
 * from the "bind/c" directory must be compiled and linked to the application.
 */
#ifndef PDFCPP_DL
#define PDFCPP_DL 0
#endif

namespace pdflib {

#include "pdflib.h"

#if PDFCPP_DL
#include "pdflibdl.h"
#endif


template<class pstring, class conv> class basic_PDFlib;

/**
 * The "do-nothing" converter that has the effect that the basic_PDFlib class
 * behaves in the same way as the PDFlib 7.x wrapper.
 */
template<class pstring>
class NoOpConverter
{
public:
    static bool do_conversion()
    {
        return false;
    }

    static void convert_to_pdf_bytes(
        const basic_PDFlib<pstring, NoOpConverter<pstring> >&,
        const pstring&, std::string&)
    {
        throw std::logic_error(
                "NoOpConverter::convert_to_pdf_bytes: internal error: "
                "converter called although do_conversion() returns false");
    }

    static void convert_to_pdf_utf8(
            const basic_PDFlib<pstring, NoOpConverter<pstring> >&,
            const pstring&, std::string&)
    {
        throw std::logic_error(
                "NoOpConverter::convert_to_pdf_utf8: internal error: "
                "converter called although do_conversion() returns false");
    }

    static void convert_to_pdf_utf16(
            const basic_PDFlib<pstring, NoOpConverter<pstring> >&,
            const pstring&, std::string&)
    {
        throw std::logic_error(
                "NoOpConverter::convert_to_pdf_utf16: internal error: "
                "converter called although do_conversion() returns false");
    }

    static void convert_to_pstring(
            const basic_PDFlib<pstring, NoOpConverter<pstring> >&,
            const char *, pstring&)
    {
        throw std::logic_error(
                "NoOpConverter::convert_to_pstring: internal error: "
                "converter called although do_conversion() returns false");
    }
};

#if PDFCPP_DL
#define MPDFLIB_API m_PDFlib_api->

#define PDFCPP_TRY      PDF_TRY_DL(m_PDFlib_api, p)

#define PDFCPP_CATCH  \
PDF_CATCH_DL(m_PDFlib_api, p) { \
    pstring message, apiname; \
    convert_exception_strings(message, apiname); \
    throw Exception(message, m_PDFlib_api->PDF_get_errnum(p), apiname, m_PDFlib_api->PDF_get_opaque(p)); \
}

#else
#define MPDFLIB_API

#define PDFCPP_TRY      PDF_TRY(p)

#define PDFCPP_CATCH  \
PDF_CATCH(p) { \
    pstring message, apiname; \
    convert_exception_strings(message, apiname); \
    throw Exception(message, PDF_get_errnum(p), apiname, PDF_get_opaque(p)); \
}

#endif // PDFCPP_DL

// The C++ class wrapper for PDFlib

template<class pstring, class conv>
class basic_PDFlib
{
    friend class NoOpConverter<pstring>;

public:
    /* The string type used for option lists and return values. */
    typedef pstring string_type;

    class Exception
    {
    public:
	Exception(const pstring& errmsg, int errnum, const pstring& apiname,
	    void *opaque) :
	        m_errmsg(errmsg), m_errnum(errnum), m_apiname(apiname),
	        m_opaque(opaque)
        {
        }
	pstring get_errmsg() const { return m_errmsg; }
	int get_errnum() const { return m_errnum; }
	pstring get_apiname() const { return m_apiname; }
	const void *get_opaque() const { return m_opaque; }
    private:
	pstring m_errmsg;
	int m_errnum;
	pstring m_apiname;
	void * m_opaque;
    }; // Exception

#if PDFCPP_DL
    class dl_load_error: public std::runtime_error
    {
    public:
        explicit dl_load_error() :
            std::runtime_error("Couldn't load PDFlib DLL")
        {
        }
    };

    /*
     * The constructor variant for dynamic loading does require the "opaque"
     * parameter to be a NULL pointer, as it is used internally. This way the
     * signature is compatible with the non-DL constructor variant.
     */
    basic_PDFlib(allocproc_t allocproc = NULL,
        reallocproc_t reallocproc = NULL,
        freeproc_t freeproc = NULL,
        void *opaque = NULL)
    {
        if (opaque)
        {
            throw std::invalid_argument(
                    "In the dynamic loading variant of the PDFlib C++ binding "
                    "the 'opaque' parameter must be NULL");
        }

        m_PDFlib_api = PDF_new2_dl(NULL, allocproc, reallocproc, freeproc, &p);

        if (!m_PDFlib_api)
        {
            throw dl_load_error();
        }

        check_api(NULL);

        set_cpp_binding_options();
    }

    ~basic_PDFlib() throw()
    {
        PDF_delete_dl(m_PDFlib_api, p);
    }
#else
    basic_PDFlib(allocproc_t allocproc = NULL,
	reallocproc_t reallocproc = NULL,
	freeproc_t freeproc = NULL,
        void *opaque = NULL)
    {
        p = PDF_new2(NULL, allocproc, reallocproc, freeproc, opaque);

        if (!p)
        {
            throw std::bad_alloc();
        }

        set_cpp_binding_options();
    }

    ~basic_PDFlib() throw()
    {
        PDF_delete(p);
    }
#endif // PDFCPP_DL

#include "cpp_wrapped.h"

    void begin_document_callback(writeproc_t writeproc, const pstring& optlist)
    {
        std::string optlist_param;
        const char *p_optlist_param;
        param_to_utf8(optlist, optlist_param, p_optlist_param);

        PDFCPP_TRY
            MPDFLIB_API PDF_begin_document_callback(p, writeproc,
                                                    p_optlist_param);
        PDFCPP_CATCH;
    }

    void *get_opaque()
    {
        void *retval = NULL;

        PDFCPP_TRY
            retval = MPDFLIB_API PDF_get_opaque(p);
        PDFCPP_CATCH;

        return retval;
    }


    /* Convert an arbitrary encoded string to a Unicode string in several formats. */
    std::string
    convert_to_unicode(const pstring& inputformat, const std::string& inputstring, const pstring& optlist) const
    {
        std::string retval;

        std::string inputformat_param;
        const char *p_inputformat_param;
        param_to_bytes(inputformat, inputformat_param, p_inputformat_param);

        std::string optlist_param;
        const char *p_optlist_param;
        param_to_utf8(optlist, optlist_param, p_optlist_param);

	PDFCPP_TRY {
            int outputlen;
            const char * const buf =
                    MPDFLIB_API PDF_convert_to_unicode(p, p_inputformat_param,
                        inputstring.data(), static_cast<int>(inputstring.length()),
                        &outputlen, p_optlist_param);
            if (buf)
                retval.assign(buf, static_cast<size_t>(outputlen));
	}
	PDFCPP_CATCH;

	return retval;
    }

/* Figure out whether or not we're running on an EBCDIC-based machine */
#define PDFCPP_ASCII_A                 0x41
#define PDFCPP_PLATFORM_A              'A'
#define PDFCPP_EBCDIC_A                0xC1

#if (PDFCPP_ASCII_A != PDFCPP_PLATFORM_A \
                && PDFCPP_EBCDIC_A == PDFCPP_PLATFORM_A)
#define PDFCPP_INTERNAL_OPTLIST \
    "escapesequence=false charref=false bom=optimize outputformat=ebcdicutf8"
#else
#define PDFCPP_INTERNAL_OPTLIST \
    "escapesequence=false charref=false bom=optimize outputformat=utf8"
#endif

private:
    PDF *p;
#if PDFCPP_DL
    const PDFlib_api *m_PDFlib_api;
#endif // PDFCPP_DL

    enum
    {
        utf16_wchar_t_size = 2,
        utf32_wchar_t_size = 4
    };

#if PDFCPP_DL
    void check_api(void *opaque)
    {
        if (m_PDFlib_api->sizeof_PDFlib_api != sizeof(PDFlib_api) ||
            m_PDFlib_api->major != PDFLIB_MAJORVERSION ||
            m_PDFlib_api->minor != PDFLIB_MINORVERSION)
        {
            pstring message;
            pstring apiname; /* stays empty */

            switch (sizeof(typename pstring::value_type))
            {
            case sizeof(char):
                apiretval_to_pstring("loaded wrong version of PDFlib library", message);
                break;

            case utf16_wchar_t_size:
            case utf32_wchar_t_size:
                apiretval_to_pstring(reinterpret_cast<const char *>(L"loaded wrong version of PDFlib library"), message);
                break;

            default:
                bad_wchar_size("basic_PDFlib<pstring, conv>::check_api");
            }

            throw Exception(message, -1, apiname, opaque);
        }
    }
#endif // PDFCPP_DL

    void set_cpp_binding_options(void)
    {
        PDFCPP_TRY
        {
            MPDFLIB_API PDF_set_option(p, "objorient=true");
            if (conv::do_conversion())
            {
                MPDFLIB_API PDF_set_option(p, "binding={C++ conv}");
                set_unicaplang_parameters();
            }
            else
            {
                switch (sizeof(typename pstring::value_type))
                {
                case sizeof(char):
                    MPDFLIB_API PDF_set_option(p, "apitextformat=utf8 binding={C++}");
                    set_unicaplang_parameters();
                    break;

                case utf16_wchar_t_size:
                    MPDFLIB_API PDF_set_option(p, "apitextformat=utf16 binding={C++}");
                    set_unicaplang_parameters();
                    break;

                case utf32_wchar_t_size:
                    MPDFLIB_API PDF_set_option(p, "apitextformat=utf32 binding={C++}");
                    set_unicaplang_parameters();
                    break;

                default:
                    bad_wchar_size("basic_PDFlib<pstring, conv>::basic_PDFlib");
                }
            }
        }
#if PDFCPP_DL
        PDF_CATCH_DL(m_PDFlib_api, p) {
            pstring message, apiname;

            prepare_bad_env_diagnostics(message, apiname);

            throw Exception(message, 999, apiname, m_PDFlib_api->PDF_get_opaque(p));
        }
#else // PDFCPP_DL
        PDF_CATCH(p) {
            pstring message, apiname;

            prepare_bad_env_diagnostics(message, apiname);

            throw Exception(message, 999, apiname, PDF_get_opaque(p));
        }
#endif // PDFCPP_DL
    }

    /**
     * Prepare the special diagnostic information for the situation that
     * the constructor fails because of a syntactically incorrect PDFLIBLOGGING
     * environment variable or from the corresponding Windows registry entry.
     */
    void prepare_bad_env_diagnostics(pstring& message, pstring& apiname)
    {
        switch (sizeof(typename pstring::value_type))
        {
        case sizeof(char):
            apiretval_to_pstring("Syntax error in environment variable 'PDFLIBLOGGING'"
#ifdef _WIN32
                    " or in the Windows registry entry 'pdfliblogging'"
#endif
                    , message);
            apiretval_to_pstring("constructor PDFlib", apiname);
            break;

        case utf16_wchar_t_size:
        case utf32_wchar_t_size:
            apiretval_to_pstring(reinterpret_cast<const char *>(
                    L"Syntax error in environment variable 'PDFLIBLOGGING'"
#ifdef _WIN32
                    L" or in the Windows registry entry 'pdfliblogging'"
#endif
                    ), message);
            apiretval_to_pstring(reinterpret_cast<const char *>(
                    L"constructor PDFlib"), apiname);
            break;
        }
    }

    void set_unicaplang_parameters() const
    {
        MPDFLIB_API PDF_set_option(p, "unicaplang=true");
        MPDFLIB_API PDF_set_option(p, "textformat=auto2");
        MPDFLIB_API PDF_set_option(p, "hypertextformat=auto2");
        MPDFLIB_API PDF_set_option(p, "hypertextencoding={}");
    }

    void bad_wchar_size(const char *apiname) const
    {
        std::ostringstream exception_text;
        exception_text << apiname << ": unsupported wchar_t size: "
                        << sizeof(typename pstring::value_type);

        throw std::logic_error(exception_text.str());
    }

    void param_to_utf8(const pstring& param, std::string& pdflib_param,
                        const char *& pdflib_ptr) const
    {
        if (conv::do_conversion())
        {
            conv::convert_to_pdf_utf8(*this, param, pdflib_param);
            pdflib_ptr = pdflib_param.c_str();
        }
        else
        {
            const char * const s = reinterpret_cast<const char *>(param.c_str());
            int outputlen;
            const size_t param_length = param.length();

            switch (sizeof(typename pstring::value_type))
            {
            case sizeof(char):
                /*
                 * UTF-8: Must be converted nevertheless as it might be
                 * necessary to add the UTF-8 BOM.
                 */
                MPDFLIB_API PDF_set_internal_option(p, "wrappercall=true");
                pdflib_ptr = MPDFLIB_API PDF_convert_to_unicode(p, "utf8",
                    s, static_cast<int>(param_length * sizeof(char)),
                    &outputlen, PDFCPP_INTERNAL_OPTLIST);
                break;

            case utf16_wchar_t_size:
                if (param_length > 0) {
                    MPDFLIB_API PDF_set_internal_option(p, "wrappercall=true");
                    pdflib_ptr = MPDFLIB_API PDF_convert_to_unicode(p, "utf16",
                            s, static_cast<int>(param_length * utf16_wchar_t_size),
                            &outputlen, PDFCPP_INTERNAL_OPTLIST);
                } else {
                    pdflib_ptr = "";
                }
                break;

            case utf32_wchar_t_size:
                if (param_length > 0) {
                    MPDFLIB_API PDF_set_internal_option(p, "wrappercall=true");
                    pdflib_ptr = MPDFLIB_API PDF_convert_to_unicode(p, "utf32",
                        s, static_cast<int>(param_length * utf32_wchar_t_size),
                        &outputlen, PDFCPP_INTERNAL_OPTLIST);
                } else {
                    pdflib_ptr = "";
                }
                break;

            default:
                bad_wchar_size("basic_PDFlib<pstring, conv>::param_to_utf8");
            }
        }
    }

    void param_to_0utf16(const pstring& param, std::string& pdflib_param,
                        const char *& pdflib_ptr, int& len) const
    {
        if (conv::do_conversion())
        {
            conv::convert_to_pdf_utf16(*this, param, pdflib_param);
            pdflib_ptr = pdflib_param.c_str();
            len = static_cast<int>(pdflib_param.length());
        }
        else
        {
            const char * const s = reinterpret_cast<const char *>(param.c_str());
            const size_t param_length = param.length();

            switch (sizeof(typename pstring::value_type))
            {
            case sizeof(char):
                /*
                 * UTF-8 encoding. Must be converted as it might be necessary
                 * to add the UTF-8 BOM.
                 */
                MPDFLIB_API PDF_set_internal_option(p, "wrappercall=true");
                pdflib_ptr = MPDFLIB_API PDF_convert_to_unicode(p, "utf8",
                        s, static_cast<int>(param_length),
                        &len, "escapesequence=false charref=false outputformat=utf16");
                break;

            case utf16_wchar_t_size:
                /*
                 * UTF-16 can be passed through directly
                 */
                pdflib_ptr = s;
                len = static_cast<int>(param_length * utf16_wchar_t_size);
                break;

            case utf32_wchar_t_size:
                if (param_length > 0)
                {
                    MPDFLIB_API PDF_set_internal_option(p, "wrappercall=true");
                    pdflib_ptr = MPDFLIB_API PDF_convert_to_unicode(p, "utf32",
                            s, static_cast<int>(param_length * utf32_wchar_t_size),
                            &len, "escapesequence=false charref=false outputformat=utf16");
                }
                else
                {
                    static const char utf16_eos[] = { 0, 0 };

                    pdflib_ptr = utf16_eos;
                    len = 0;
                }
                break;

            default:
                bad_wchar_size("basic_PDFlib<pstring, conv>::param_to_0utf16");
            }
        }
    }

    void param_to_utf16(const pstring& param, std::string& pdflib_param,
                        const char *& pdflib_ptr, int& len) const
    {
        if (conv::do_conversion())
        {
            conv::convert_to_pdf_utf16(*this, param, pdflib_param);
            pdflib_ptr = pdflib_param.c_str();
            len = static_cast<int>(pdflib_param.length());
        }
        else
        {
            const char *s = reinterpret_cast<const char *>(param.c_str());
            const size_t param_length = param.length();

            switch (sizeof(typename pstring::value_type))
            {
            case sizeof(char):
                /*
                 * UTF-8 encoded. Must be converted as it might be necessary
                 * to add the UTF-8 BOM.
                 */
                MPDFLIB_API PDF_set_internal_option(p, "wrappercall=true");
                pdflib_ptr = MPDFLIB_API PDF_convert_to_unicode(p, "utf8",
                            s, static_cast<int>(param_length),
                            &len, "escapesequence=false charref=false outputformat=utf16");
                break;

            case utf16_wchar_t_size:
                /*
                 * UTF-16 can be passed through directly
                 */
                pdflib_ptr = s;
                len = static_cast<int>(param_length * utf16_wchar_t_size);
                break;

            case utf32_wchar_t_size:
                if (param_length > 0)
                {
                    MPDFLIB_API PDF_set_internal_option(p, "wrappercall=true");
                    pdflib_ptr = MPDFLIB_API PDF_convert_to_unicode(p, "utf32",
                            s, static_cast<int>(param_length * utf32_wchar_t_size),
                            &len, "escapesequence=false charref=false outputformat=utf16");
                }
                else
                {
                    static const char utf16_eos[] = { 0, 0 };

                    pdflib_ptr = utf16_eos;
                    len = 0;
                }
                break;

            default:
                bad_wchar_size("basic_PDFlib<pstring, conv>::param_to_utf16");
            }
        }
    }

    void param_to_bytes(const pstring& param, std::string& pdflib_param,
                        const char *& pdflib_ptr) const
    {
        if (conv::do_conversion())
        {
            conv::convert_to_pdf_bytes(*this, param, pdflib_param);
            pdflib_ptr = pdflib_param.c_str();
        }
        else
        {
            const size_t size = sizeof(typename pstring::value_type);
            const char *s = reinterpret_cast<const char *>(param.c_str());

            switch (size)
            {
            case sizeof(char):
                pdflib_ptr = s;
                break;

            case utf16_wchar_t_size:
            case utf32_wchar_t_size:
                {
                    const size_t param_length = param.length();

                    if (param_length > 0)
                    {
                        int highchar;

                        const char * const deflated =
                            MPDFLIB_API PDF_deflate_unicode(p, s,
                                    static_cast<int>(param_length * size),
                                    size, &highchar);

                        if (!deflated)
                        {
                            std::ostringstream exception_text;

                            exception_text
                                << "basic_PDFlib::param_to_bytes: high "
                                    "Unicode character '0x"
                                << std::hex << highchar
                                << "' is not supported in this character string";

                            throw std::runtime_error(exception_text.str().c_str());
                        }

                        pdflib_ptr = deflated;
                    }
                    else
                    {
                        pdflib_ptr = "";
                    }
                }
                break;

            default:
                bad_wchar_size("basic_PDFlib<pstring, conv>::param_to_bytes");
            }
        }
    }

    void apiretval_to_pstring(const char * const pdflib_retval,
                        pstring& cpp_retval) const
    {
        if (conv::do_conversion())
        {
            if (pdflib_retval)
            {
                conv::convert_to_pstring(*this, pdflib_retval, cpp_retval);
            }
            else
            {
                cpp_retval.erase();
            }
        }
        else
        {
            if (pdflib_retval)
            {
                cpp_retval.assign(reinterpret_cast
                                    <const typename pstring::value_type *>
                                        (pdflib_retval));
            }
            else
            {
                cpp_retval.erase();
            }
        }
    }

    void
    convert_exception_strings(pstring& message, pstring& apiname) const
    {
        if (conv::do_conversion())
        {
            conv::convert_to_pstring(*this, MPDFLIB_API PDF_get_errmsg(p),
                                        message);
            conv::convert_to_pstring(*this, MPDFLIB_API PDF_get_apiname(p),
                                        apiname);
        }
        else
        {
            /*
             * Without custom converter the PDFlib API returns the error message
             * and the API name in the expected encoding, so just put this
             * string into the output string.
             */
            message = reinterpret_cast<const typename pstring::value_type *>
                                        (MPDFLIB_API PDF_get_errmsg(p));
            apiname = reinterpret_cast<const typename pstring::value_type *>
                                        (MPDFLIB_API PDF_get_apiname(p));
        }
    }

    /*
     * Prevent copy construction and assignment.
     */
    basic_PDFlib(const basic_PDFlib&);
    basic_PDFlib& operator=(const basic_PDFlib&);
};


#if defined(PDFLIB_CPP_STRING)

typedef basic_PDFlib<std::string, NoOpConverter<std::string> > PDFlib;

#elif defined(PDFLIB_CPP_U8STRING)

// Visual Studio by default does not provide the __cplusplus macro with the
// actually supported C++ version but provides an old C++ version number
// for compatibility reasons. Therefore an extra check for
// Visual Studio 2019 version 16.1 or newer is implemented.
#if __cplusplus >= 202002L || (defined(_MSC_VER) && _MSC_VER >= 1921)
typedef basic_PDFlib<std::u8string, NoOpConverter<std::u8string> > PDFlib;
#else
#error "C++20 support required for std::u8string-based PDFlib class"
#endif

#elif defined(PDFLIB_CPP_U16STRING)

// Visual Studio by default does not provide the __cplusplus macro with the
// actually supported C++ version but provides an old C++ version number
// for compatibility reasons. Therefore an extra check for Visual Studio 2015
// or newer is implemented.

#if __cplusplus >= 201103L || (defined(_MSC_VER) && _MSC_VER >= 1910)
typedef basic_PDFlib<std::u16string, NoOpConverter<std::u16string> > PDFlib;
#else
#error "C++11 support required for std::u16string-based PDFlib class"
#endif

#elif defined(PDFLIB_CPP_U32STRING)

#if __cplusplus >= 201103L || (defined(_MSC_VER) && _MSC_VER >= 1910)
typedef basic_PDFlib<std::u32string, NoOpConverter<std::u32string> > PDFlib;
#else
#error "C++11 support required for std::u32string-based PDFlib class"
#endif

#else

typedef basic_PDFlib<std::wstring, NoOpConverter<std::wstring> > PDFlib;

#endif
} // end of PDFlib namespace

#endif	// PDFLIB_HPP
