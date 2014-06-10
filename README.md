BANCStar (joe.loughry@gmail.com)
========

The Worst Programming Environment in the World?
-----------------------------------------------

This is BANCStar source code:

    2999,,,
    8600,,,1
    3000,829,6,30089
    10829,2446,22245,22012
    11585,15855,22002,22002
    11586,15865,22002,22002
    11587,15875,22002,22002
    11588,15885,22002,22002
    11596,15965,22002,22002
    11597,15975,22002,22002
    11598,15985,22002,22002
    11599,15995,22002,22002
    11600,16005,22002,22002
    11601,16015,22002,22002
    11602,16025,22002,22002
    11603,16035,22002,22002
    11604,16045,22002,22002
    11605,16055,22002,22002
    11606,16065,22002,22002
    11607,16075,22002,22002
    11608,16085,22002,22002
    11609,16095,22002,22002
    11610,16105,22002,22002
    11611,16115,22002,22002
    11612,16125,22002,22002
    11613,16135,22002,22002
    11614,16145,22002,22002
    11615,16155,22002,22002
    11616,16165,22002,22002
    11617,16175,22002,22002
    11618,16185,22002,22002
    11619,16195,22002,22002
    11620,16205,22002,22002
    11621,16215,22002,22002
    11622,16225,22002,22002
    11623,16235,22002,22002
    11624,16245,22002,22002
    11625,16255,22002,22002
    11626,16265,22002,22002
    11627,16275,22002,22002
    11628,16285,22002,22002
    11629,16295,22002,22002
    11630,16305,22002,22002
    11631,16315,22002,22002
    11632,16325,22002,22002
    11638,16385,22002,22002
    11639,16395,22002,22002
    11677,22002,22002,22002
    11678,22002,22002,22002
    11679,22002,22002,22002
    11680,22002,22002,22002
    11691,22002,22002,22002
    11693,22002,22002,22002
    11707,22002,22002,22002
    11749,22002,22002,22002
    3001,351,2,
    10190,3512,22002,22002
    10191,3522,22002,22002
    10192,3522,22002,22002
    10193,3522,22002,22002
    3000,192,1,
    10193,1902,1912,1922
    10863,3552,22002,22002
    10864,3562,22002,22002
    10865,3572,22002,22002
    10866,3582,22002,22002
    10867,3592,22002,22002
    10505,3662,22002,22002
    10414,3912,22002,22002
    10415,3922,22002,22002
    10416,3932,22002,22002
    10563,4072,22002,22002
    10562,4082,22002,22002
    10566,4102,22002,22002,
    3001,,,
    31597,10001,716,108
    31598,10001,785,108
    31599,10001,717,108
    31600,10001,710,107
    31601,10001,786,108
    31602,10001,715,108
    31603,10001,787,108
    31604,10001,714,108
    31605,10001,713,108
    31606,10001,712,108
    31607,10001,711,108
    31608,10001,765,108
    31609,10001,766,108
    31617,10001,767,108
    31618,10001,768,108
    31619,10001,769,108
    31620,10001,770,108
    31621,10001,771,108
    31622,10001,772,108
    31623,10001,776,108
    31624,10001,777,108
    31625,10001,778,108
    31626,10001,779,108
    31627,10001,780,108
    31628,10001,781,108
    31629,10001,782,108
    31630,10001,797,108
    31631,10001,763,108
    31632,10001,764,108
    8500,,2,
    1254,2301,1,-2301

Imagine being introduced to that code your first day on the job. In February of 1990, I was.
A team of five programmers maintained hundreds of files of this code in production for years
(we didn't invent it; we only had to work with it). It took about two weeks to become fluent
in the language. As a historical curiosity, I present here some of the surviving
documentation on the <a name="LIST-output"/>tool I made to cope with it, which duplicated as
closely as possible the highlighter-pens-and-paper method that earlier programmers used to
work on the code.

![screen shot](https://github.com/jloughry/BANCStar/raw/master/screenshot.jpeg)

BANCStar Source Code
--------------------

The file [`C16LNAPP.SCN`](https://github.com/jloughry/BANCStar/blob/master/C16LNAPP.SCN)
is (presently only the first page of) some production code used in First Interstate Bank
of Washington around 1991.  It is not used any more, and is of some historical interest,
so with consent of the participants I have published it here.

As a side project at the bank, I wrote a sophisticated formatter dubbed LIST whose
output can be seen [here](https://github.com/jloughry/BANCStar/blob/master/C16LNAPP.pdf?raw=true).
In addition to the syntax highlighting and flow notation shown, LIST produced a view on
the code tailored to the programmer's need, generating cross-references, statistics, and
screen shots for documentation.  When I have time I'll put the rest of the code up here.

More BANCStar Source Code
-------------------------

[`MM1SM1.SCN`](https://github.com/jloughry/BANCStar/blob/master/MM1SM1.SCN) ("Main Menu
1 Sub-Menu 1") was the top-level programme in the system. When I get the [LIST](#LIST)
utility running again, I'll post a translated copy here.

The only [prompt file](https://github.com/jloughry/BANCStar/tree/master/LIST/test_data/LIAB.PFL)
I have is the 'liabilities' one.

The `LIST` Utility<a name="LIST"/>
------------------

LIST is the programme that translates
[screen code](https://github.com/jloughry/BANCStar/blob/master/C16LNAPP.SCN) into
something [readable for humans](#LIST-output).

The complete [source code](https://github.com/jloughry/BANCStar/tree/master/LIST/src/)
to LIST has been recovered. It was written for a now-obsolete Microsoft C compiler on MS-DOS,
so it doesn't compile now, but should be easy to port to any POSIX operating system.
Presently, the source code looks for a few DOS-specific `#include` files like `dos.h`
and `conio.h`, but I don't think it uses any obsolete memory model features like near
and far pointers. There isn't a `Makefile`, sorry.

The 1991-vintage executable still sort of works on Microsoft Windows in 2014 (in a
CMD prompt window).

`LIST.EXE` needs two data files to work: some
[screen code](https://github.com/jloughry/BANCStar/tree/master/LIST/test_data/MM1SM1.SCN) and a
[prompt file](https://github.com/jloughry/BANCStar/tree/master/LIST/test_data/LIAB.PFL). Run it
like this:

````
LIST Screen File Formatter Version 2.2a                             Revision 11
Copyright (C) 1990, 1991 Joe Loughry. All rights reserved.

Usage:list [/prompt][/sN][/eN][/a][/d][/f][/h][/n][/x][/z] filename.ext

The options, which may appear in any order, are as follows:

     /icl      Use the prompt file ICL.PFL
     /liab     Use the prompt file LIAB.PFL
     /other    Use the prompt file OTHER.PFL
     /rates    Use the prompt file RATES.PFL
     /pNAME    Use the prompt file specified by NAME
     /sN       Start listing at line N of the source file.
     /eN       Stop listing at line N of the source file.
     /a        Print listing only; do not print List Report.
     /d        Format output for the IBM 4019 Laser Printer and send to file.
     /f        Send output to the file C:\LISTFILE.RJL -- do not print.
     /h        Display this Help message.
     /n        Do not perform syntax checking.
     /x        Print extended Reference List report. [default]
     /z        Print List report only; do not print listing.

$ list -liab -f mm1sm1.scn
````

Wikipedia page
--------------

The BANCStar programming language has its own [Wikipedia page]
(http://en.wikipedia.org/wiki/BANCStar_programming_language).

My 1997 article on the language
-------------------------------

*I wrote the following article in 1997.  It was published by Brian Connors in the*
[Turing Tarpit](http://reocities.com/ResearchTriangle/station/2266/tarpit/bancstar.html)
*and updated in 2000.  Further samples of BANCStar source code will be collected here
as they are found.*

> I think the world is ready to be introduced to BANCStar.

> The following actual sample of BANCStar source code was taken from a production system. I'm not too
worried about revealing a lot of proprietary information here, as only about ten people in the
world can read this code:

    8607,,,1
    11547,15475,22002,22002
    1316,1629,1,1649 
    3001,1316,3,30078
    11528,22052,22002,22002
    9301,0,1528,1528
    31568,10001,800,107
    8560,,,1568
    8550,210,,
    3001,,,
    3100,1316,3,30089
    11547,15475,22002,22002
    3001,1316,3,30089
    3001,1317,3,10000
    8400,,,
    8550,700,801,
    3001,,,
    9301,0,522,522
    3000,1284,3,10001
    8500,,3,
    8500,,5,
    1547,,1,-2301

> I think we had something like 1,350 files of this code, most files a few hundred lines long, though
some were over 1,000 lines. The system ran the retail branch operations of a major commercial bank
in the early 1990s.

> Interesting features of BANCStar included:

> - The only legal characters in a BANCStar program are the digits 0 through 9, comma, minus sign,
and carriage return. Blank lines are not allowed. If a "." appears anywhere in the file, the
compiler will crash.

> - Comments are strictly prohibited.

> - Control structures available include the 3000 ("conditional"), 3001 ("block conditional"), and
3101 ("reverse block conditional"), as well as 8500 ("GOTO") and 8550 ("combination GOTO").

> - There is a hard limit of 2000 variables + constants in the entire system. Anything to be
displayed on screen or printed on a form must be defined as a constant, thereby taking up part of
this valuable space. As a result, most calculations are done with a block of twenty or so "working
storage" variables, which are continually reused. Everything in the system is global. New projects
always started off with the programmer searching for a handful of working storage numbers that
could be "borrowed" long enough to complete the calculation, then restored to their original values
before the rightful owner noticed that they were gone.

> - Labels within the code are absolute, so any time a new page was added or removed, all
downstream GOTOs must be found and re-targeted.

> A few more details I remember:

> BANCStar actually came with a "screen generator" that was supposed to be used to construct
applications. But the 5.1c version of the generator was so limited that experienced programmers
soon began to pry off the covers and modify directly the intermediate code that the run-time module
actually executed.

> When I arrived in Seattle in early 1990, I joined a team of seven programmers who by now routinely
wrote directly in BANCStar machine language, completely ignoring the screen generator. The first
time they showed me the language, I thought it was a joke. But within a few weeks I was reading and
writing the code, with the aid of every BANCStar programmer's favourite tools: a dot-matrix printer,
lots and lots of different colour highlighters, and a three-ring binder called the Prompt File,
stuffed with printouts of the dozens of tables in the system, and religiously updated anytime
anyone changed anything of significance. (I wasn't kidding about reusing storage; if you needed a
constant integer 1000, and you could find a place where somebody else had once used that same
value, you linked your code to his and hoped it never changed.)

> We developed some in-house tools for programming large applications in BANCStar, and at one point
attempted to interest Broadway & Seymour in them, but I think they never really believed us, that
we were writing directly to their internal, low-level, undocumented machine code.

