/*---------------------------------------------------------------------------*
 |          Copyright (c) 1997-2022 PDFlib GmbH. All rights reserved.        |
 +---------------------------------------------------------------------------+
 |    This software may not be copied or distributed except as expressly     |
 |    authorized by PDFlib GmbH's general license agreement or a custom      |
 |    license agreement signed by PDFlib GmbH.                               |
 |    For more information about licensing please refer to www.pdflib.com.   |
 *---------------------------------------------------------------------------*/

/*
 * C wrapper code for dynamically loading the PDFlib DLL at runtime.
 *
 * This module is not supported on all platforms.
 *
 */

#include <stdlib.h>

#include "pdflibdl.h"

/* enable this to avoid error messages */
/*
#define PDF_SILENT
*/

#include "dl_dl.h"

/* ---------------------------------- WIN32 ----------------------------- */

#ifdef WIN32

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <winbase.h>
#undef WIN32_LEAN_AND_MEAN

#define PDF_DLLNAME                     "pdflib.dll"

/* ---------------------------------- MVS ----------------------------- */

#elif defined(__MVS__)

#define PDF_DLLNAME                     "PDFLIB"

/* ---------------------------------- Linux  ----------------------------- */

#elif defined(__linux__) || defined(linux) \
    || defined (__FreeBSD__) || defined (_AIX) || defined(__hpux) || defined(sun)

#include <dlfcn.h>

#define PDF_DLLNAME                     "libpdf.so"

/* ---------------------------------- Mac OS X  ----------------------------- */

#elif defined(__APPLE__)

#define PDF_DLLNAME                     "libpdf.dylib"

/* ---------------------------------- AS/400  ----------------------------- */

#elif defined __ILEC400__

#define PDF_DLLNAME                     "PDFLIB"

/* ---------------------------------- unknown  ----------------------------- */

#else

#error No DLL loading code for this platform available!

#endif
/* ---------------------------------- generic  ----------------------------- */

static void
pdf_dlerror(const char *msg)
{
#ifndef PDF_SILENT
    puts(msg);
#endif
}

/* Load the PDFlib DLL and fetch the API structure */

PDFLIB_API const PDFlib_api * PDFLIB_CALL
PDF_new_dl(PDF **pp)
{
    return PDF_new2_dl(NULL, NULL, NULL, NULL, pp);
}

PDFLIB_API const PDFlib_api * PDFLIB_CALL
PDF_new2_dl(errorproc_t errorhandler, allocproc_t allocproc,
        reallocproc_t reallocproc, freeproc_t freeproc, PDF **pp)
{
    const PDFlib_api *PDFlib, *(PDFLIB_CALL *get_api)(void);
#define BUFSIZE 256
    char buf[BUFSIZE];

    /* load the PDFLIB DLL... */
    void *handle = pdfdl_dlopen(PDF_DLLNAME);

    if (!handle)
    {
        snprintf(buf, BUFSIZE, "Error: couldn't load PDFlib DLL %s\n", PDF_DLLNAME);
        pdf_dlerror(buf);
        return NULL;
    }

    /* ...and fetch function pointers */
    *(void**) (&get_api) = pdfdl_dlsym(handle, "PDF_get_api");

    if (get_api == NULL)
    {
        pdf_dlerror(
            "Error: couldn't find function PDF_get_api in PDFlib DLL\n");
        pdfdl_dlclose(handle);
        return NULL;
    }

    /* Fetch the API structure. */
    PDFlib = (*get_api)();

    /*
     * Check the version number of the loaded DLL against that of
     * the included header file to avoid version mismatch.
     */
    if (PDFlib->sizeof_PDFlib_api != sizeof(PDFlib_api) ||
        PDFlib->major != PDFLIB_MAJORVERSION ||
        PDFlib->minor != PDFLIB_MINORVERSION) {
        snprintf(buf, BUFSIZE,
        "Error: loaded wrong version of PDFlib DLL\n"
        "Expected version %d.%d (API size %lu), loaded %d.%d (API size %lu)\n",
        PDFLIB_MAJORVERSION, PDFLIB_MINORVERSION,
        (unsigned long) sizeof(PDFlib_api),
        PDFlib->major, PDFlib->minor,
        (unsigned long) PDFlib->sizeof_PDFlib_api);
        pdf_dlerror(buf);
        pdfdl_dlclose(handle);
        return NULL;
    }

    /*
     * Create a new PDFlib object; use PDF_new2() so that we can store
     * the DLL handle within PDFlib and later retrieve it.
     */
    if ((*pp = PDFlib->PDF_new2(errorhandler, allocproc,
            reallocproc, freeproc, handle)) == (PDF *) NULL)
    {
        pdf_dlerror("Couldn't create PDFlib object (out of memory)!\n");
        pdfdl_dlclose(handle);
        return NULL;
    }

    return PDFlib;
}

/* delete the PDFlib object and unload the previously loaded PDFlib DLL */

PDFLIB_API void PDFLIB_CALL
PDF_delete_dl(const PDFlib_api *PDFlib, PDF *p)
{
    if (PDFlib && p)
    {
        /* fetch the DLL handle (previously stored in the PDFlib object) */
        void *handle = PDFlib->PDF_get_opaque(p);

        PDFlib->PDF_delete(p);

        if (handle)
            pdfdl_dlclose(handle);
    }
}
