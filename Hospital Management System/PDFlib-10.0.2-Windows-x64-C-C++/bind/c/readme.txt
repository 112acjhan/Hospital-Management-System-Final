Notes on the PDFlib C binding
-----------------------------

Applications which use the PDFlib binding for C must be linked with
a C++ linker since the PDFlib library includes some parts which are
implemented in C++. Using a C linker may result in unresolved externals
unless the application is explicitly linked against the required C++
support libraries.

Supported compiler/linker versions for all supported platforms are
listed in system-requirements.txt. This document also lists additional
libraries which may be required for linking PDFlib applications on some
platforms.


Windows
-------

The DLL pdflib.dll is supplied with the corresponding import library
pdflib.lib in ~/pdflib. In order to build and run the supplied C/C++
samples copy these files to the bind/c or bind/cpp directories.


macOS
-----
The package for C/C++ contains the following libraries:

- static library
- PDFlib.framework (must be installed manually in /Library/Frameworks)

