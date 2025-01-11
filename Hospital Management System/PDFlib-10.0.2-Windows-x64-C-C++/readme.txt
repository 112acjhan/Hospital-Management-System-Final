================================================
PDFlib - A library for generating PDF on the fly
================================================

The PDFlib family comprises the following products:

- PDFlib includes functions for generating PDF output with text, raster
  images and vector graphics.

- PDFlib+PDI includes all PDFlib functions plus the PDF Import Library (PDI)
  for including existing PDF pages in the generated output. It also includes
  the pCOS interface for querying PDF objects.

- PDFlib Personalization Server (PPS) includes PDFlib+PDI plus functions for
  automatically filling PDFlib Blocks. A PPS license also covers the
  PDFlib Block Plugin for creating Blocks interactively with Adobe Acrobat
  on macOS and Windows.

An overview of product features can be found in the PDFlib Tutorial.

PDFlib is available on a wide variety of operating system platforms and
supports many programming languages and development environments:

- C
- C++
- Java
- .NET
- Objective-C
- Perl
- PHP
- Python
- RPG
- Ruby

PDFlib, PDFlib+PDI, and PPS are available in binary form and require
a commercial license. All of these products are available in a single
combined library, and can be evaluated without a commercial license. However,
unless a valid license key is applied a demo stamp will be generated
across all pages, and the pCOS facility (included in PDFlib+PDI and PPS)
is restricted to small input documents.


How to Start?
=============

--- Install the appropriate library for your target language binding

Platform specific notes can be found in system-requirements.txt.

Specific notes for each language binding are available in the PDFlib Tutorial.


--- Play with the starter samples

The PDFlib distribution contains a number of samples which demonstrate
the basic use of many features for all supported language bindings.
Run the samples, take a look at the generated PDF documents and play with
the source code. All major programming paradigms regarding the PDFlib API
are demonstrated in the starter samples.


--- Browse the documentation

While the PDFlib Tutorial explains many concepts and provides a general
framework, the PDFlib API Reference is your bible regarding the programming
interface. It thoroughly documents all API methods, parameters and options.
Keep in mind that many support requests can be avoided simply by reading
the PDFlib documentation.


--- Get more ideas from the PDFlib Cookbook

The PDFlib Cookbook, which is available at www.pdflib.com, contains
programming samples for a wide variety of tasks along with the generated
PDF output documents. The Cookbook contains hundreds of topics which
demonstrate PDFlib API use. These topics are available in the Java and PHP
languages, but the underlying concepts apply to all other language bindings
as well.



Opening a Support Case
======================
We offer support agreements in combination with our product licenses.
They provide many advantages over the lifetime of a purchased product,
see https://www.pdflib.com/licensing-support/support for details.

If you run into a problem you should first make sure that you are using the
latest maintenance release for the product you licensed. Maintenance
releases are available for download from the www.pdflib.com Web site.
Customers with active support can use all maintenance releases for free.


If you have trouble with a PDFlib product, please send the following
information to support@pdflib.com:

- Your company name and (unless you are still evaluating the product)
  your license key

- A description of the problem and your expectation.

- Exact product version (including maintenance release and possibly
  patchlevel number), the operating system platform and language binding
  
- Relevant code snippets for reproducing the problem.
  
- Sample data files if necessary, e.g. image files or PDF input files
  We guarantee full confidentiality for data supplied with support cases.

- In some cases PDFlib logging output may be required. Logging can be
  enabled as follows:
  
  command-line: export PDFLIBLOGGING="filename=trace.log"
  source code: p.set_option("logging={filename=trace.log}");
  (or similar for other shells and language bindings).
  
- Details of the PDF viewer (if relevant) where the problem occurs


Licensing
=========
Please contact us if you are interested in obtaining a commercial license:

PDFlib GmbH
Licensing Department
Franziska-Bilek-Weg 9
80339 Munich, Germany
www.pdflib.com

License inquiries: sales@pdflib.com
Support requests: support@pdflib.com
