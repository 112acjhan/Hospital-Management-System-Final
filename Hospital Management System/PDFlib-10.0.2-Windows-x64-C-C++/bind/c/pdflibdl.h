/*---------------------------------------------------------------------------*
 |          Copyright (c) 1997-2022 PDFlib GmbH. All rights reserved.        |
 +---------------------------------------------------------------------------+
 |    This software may not be copied or distributed except as expressly     |
 |    authorized by PDFlib GmbH's general license agreement or a custom      |
 |    license agreement signed by PDFlib GmbH.                               |
 |    For more information about licensing please refer to www.pdflib.com.   |
 *---------------------------------------------------------------------------*/

/* 
 * Function prototypes for dynamically loading the PDFlib DLL at runtime
 *
 */

#include "pdflib.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Notes for using the PDFlib DLL loading mechanism:
 *
 * - PDF_TRY_DL()/PDF_CATCH_DL() must be used instead of the standard
 *   exception handling macros.
 * - PDF_new_dl() must be used instead of PDF_new()/PDF_new2().
 * - PDF_delete_dl() must be used instead of PDF_delete().
 * - PDF_get_opaque() must not be used.
 */

/* Load the PDFlib DLL, and fetch pointers to all exported functions. */
PDFLIB_API const PDFlib_api * PDFLIB_CALL
PDF_new_dl(PDF **pp);

/* Load the PDFlib DLL, and fetch pointers to all exported functions,
 * with same signature as the PDF_new2() call, except for the "opaque" pointer,
 * which is used internally by PDF_new2_dl to store the DLL handle.
 */
PDFLIB_API const PDFlib_api * PDFLIB_CALL
PDF_new2_dl(errorproc_t errorhandler, allocproc_t allocproc,
        reallocproc_t reallocproc, freeproc_t freeproc, PDF **pp);

/* Unload the previously loaded PDFlib DLL */
PDFLIB_API void PDFLIB_CALL
PDF_delete_dl(const PDFlib_api *PDFlib, PDF *p);


#define PDF_TRY_DL(PDFlib, p)	\
    if (p) { if (setjmp(PDFlib->pdf_jbuf(p)->jbuf) == 0)

/* Inform the exception machinery that a PDF_TRY_DL() will be left without
   entering the corresponding PDF_CATCH_DL( ) clause. */
#define PDF_EXIT_TRY_DL(PDFlib, p)		PDFlib->pdf_exit_try(p)

/* Catch an exception; must always be paired with PDF_TRY(). */
#define PDF_CATCH_DL(PDFlib, p)		} if (PDFlib->pdf_catch(p))

/* Re-throw an exception to another handler. */
#define PDF_RETHROW_DL(PDFlib, p)		PDFlib->pdf_rethrow(p)

#ifdef __cplusplus
}	/* extern "C" */
#endif
