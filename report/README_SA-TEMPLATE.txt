===========================
SPECIAL ASSIGNMENT TEMPLATE
===========================

Aalto University
School of Science
Department of Applied Physics

Basic template developed by Jarmo Ala-Heikkilä (2017)
Based on FF-report template by Risto Vanhanen and Jarmo Ala-Heikkilä (2011)


1. General
----------

The official instructions for Physics Special Assignment (PHYS-E0441)
are available at
https://mycourses.aalto.fi/course/view.php?id=18446&lang=en(permanent

If this template is contradictory with the MyCourses instructions, fix
the template accordingly. An attempt is made to maintain the template,
but use it at your own risk.


2. Initial verification
-----------------------

In order to check that the SA template works in your computer
environment, do the initial verification by building the example
report. This is done with the following commands (or the
corresponding functionalities of a graphical LaTeX environment):

$ unzip SA-latex-template.zip
$ cd SA-template
$ pdflatex sa
$ bibtex sa
$ pdflatex sa
$ pdflatex sa
$ pdflatex sa

Now you should have the example SA in pdf-format, i.e., sa.pdf.
Check with a pdf-reader that the report looks OK, including its
metadata (menu File -> Properties).

If you receive error messages concerning missing sty-packages,
you may need to install additional LaTeX packages. For example,
the package texlive-science with its dependencies had to be added
to a standard TeX installation in Ubuntu 10.04.


3. Writing your Special Assignment
----------------------------------

A new Special Assignment can be written by replacing the parts of
the example SA with new contents. Some hints:

a. The master file is sa.tex. There are some MAIN DEFINITIONS in the
beginning of the file that need to be updated, e.g., report language,
author and title. If you wish to split your tex-source in a different
manner than in the template, adjust the corresponding \input-commands.

b. In the SA template, the bibliography has been made with BiBTeX.
Use of BiBTeX can be recommended, but if the bibliography has been
written directly in LaTeX, modify the corresponding \input-command
in sa.tex.

c. The LaTeX packages that are used in the SA are defined in
packages.tex. If other packages are needed, they can be
added to the list. Note, however, that all packages are not
compatible with each other.

d. Some shorthand commands are defined in commands.tex.
You may add your own favorite shorthands to this file.

e. The report cover is defined in cover.tex. It is
made automatically using the main definitions in sa.tex.

f. The abstract page is defined by abstract.tex. It is
otherwise made automatically, but the actual abstract text must
be written in the tagged position.

g. The main text is written in main.tex and its
appendixes as appendixA.tex. If you do not have
any appendixes, comment out the corresponding \input-command
from sa.tex.

h. The master file sa.tex is processed with pdflatex that
produces the final pdf-report without going via PostScript.
This means that eps-graphics files are not supported, but
figures must be in pdf, png or jpg format and included using
a following type of command:
\includegraphics[height=80mm]{nicepicture}


4. Processing your Special Assignment
-------------------------------------

A new SA is processed through LaTeX with the commands in Sec. 2.

