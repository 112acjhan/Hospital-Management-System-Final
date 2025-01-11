/*---------------------------------------------------------------------------*
 |          Copyright (c) 2002-2022 PDFlib GmbH. All rights reserved.        |
 +---------------------------------------------------------------------------+
 |    This software may not be copied or distributed except as expressly     |
 |    authorized by PDFlib GmbH's general license agreement or a custom      |
 |    license agreement signed by PDFlib GmbH.                               |
 |    For more information about licensing please refer to www.pdflib.com.   |
 *---------------------------------------------------------------------------*/

/*
 * C wrapper code for dynamically loading DLLs at runtime. This is used
 * for all PDFlib products that support dynamic loading of the respective
 * DLLs.
 *
 * For build-technical reasons the static functions are included in a header
 * file, to be included from the product-specific wrapper for loading the DLL.
 *
 * This module is not supported on all platforms.
 */

/* ---------------------------------- WIN32 ----------------------------- */

#ifdef WIN32

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <winbase.h>
#undef WIN32_LEAN_AND_MEAN

static void *
pdfdl_dlopen(const char *filename)
{
    return (void *) LoadLibrary(filename);
}

static void *
pdfdl_dlsym(void *handle, const char *funcname)
{
    return (void *) GetProcAddress((HINSTANCE) handle, funcname);
}

static void
pdfdl_dlclose(void *handle)
{
    (void) FreeLibrary((HINSTANCE) handle);
}

/* ---------------------------------- MVS ----------------------------- */

#elif defined(__MVS__)

#include <dynit.h>
#include <dll.h>

static void *
pdfdl_dlopen(const char *filename)
{
    return dllload(filename);
}

static void *
pdfdl_dlsym(void *handle, const char *funcname)
{
    return dllqueryfn((dllhandle *) handle, funcname);
}

static void
pdfdl_dlclose(void *handle)
{
    (void) dllfree((dllhandle *) handle);
}

/* ------------------ Linux & Mac OS X and other Unixes --------------------- */

#elif defined(__linux__) || defined(linux) || defined(__APPLE__) \
    || defined (__FreeBSD__) || defined (_AIX) || defined(__hpux) || defined(sun)

#include <dlfcn.h>

static void *
pdfdl_dlopen(const char *filename)
{
    static void *ret;

    ret = dlopen(filename, RTLD_LAZY);

#ifdef DEBUG
    if (!ret)
        fprintf(stderr, "dlopen() failed: '%s'\n", dlerror());
#endif

    return ret;
}

static void *
pdfdl_dlsym(void *handle, const char *funcname)
{
    return dlsym(handle, funcname);
}

static void
pdfdl_dlclose(void *handle)
{
    (void) dlclose(handle);
}

/* ---------------------------------- AS/400  ----------------------------- */

#elif defined __ILEC400__

#include <string.h>
#include <stdlib.h>
#include <miptrnam.h>
#include <qleawi.h>
#include <qusec.h>

static void *
pdfdl_dlopen(const char *filename);
static void *
pdfdl_dlsym(void *handle, const char *funcname);

static void
pdfdl_dlclose(void *handle);

static void *
pdfdl_dlopen(char *filename)
{
    char libName[11], objName[11], *s;
    void *handle;
    _SYSPTR pSrvPgm;
    Qle_ABP_Info_t actInfo;
    int actInfoLen;
    Qus_EC_t errCode;

    memset(libName, '\0', sizeof(libName));
    if ((s = strchr(filename, '/')) == NULL) {
        s = filename;
    } else {
        memcpy(libName, filename,  s - filename);
        if (!strcmp(libName, "*LIBL"))
            libName[0] = '\0';
        s += 1;
    }
    strcpy(objName, s);

    /* Get system pointer to service program */
    pSrvPgm = rslvsp(WLI_SRVPGM, objName, libName, _AUTH_NONE);

    /* Activate Bound Program */
    handle = malloc(sizeof(int));
    actInfoLen = sizeof(actInfo);
    errCode.Bytes_Provided = sizeof(errCode);
    QleActBndPgm(&pSrvPgm, handle, &actInfo, &actInfoLen, &errCode);

    if (errCode.Bytes_Available > 0)
        return NULL;

    return (void *) handle;
}

static void *
pdfdl_dlsym(void *handle, char *funcname)
{
    int expID;
    int expNameLen;
    void *ret;
    int expType;
    Qus_EC_t errCode;

    expID = 0;
    expNameLen = strlen(funcname);
    errCode.Bytes_Provided = sizeof(errCode);
    QleGetExp( handle,
        &expID, &expNameLen, funcname, &ret, &expType, &errCode);

    if (errCode.Bytes_Available > 0)
        return NULL;

    return ret;
}

static void
pdfdl_dlclose(void *handle)
{
    free( handle);
}

/* ---------------------------------- unknown  ----------------------------- */

#else

#error No DLL loading code for this platform available!

#endif
