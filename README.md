BANCStar Source Code (rjl@applied-math.org)
====================

Code Samples!
-------------

The file [`C16LNAPP.SCN`](https://github.com/jloughry/BANCStar/blob/master/C16LNAPP.SCN)
is (presently only the first page of) some production code used in First Interstate Bank
of Washington around 1991.  It is not used any more, and is of some historical interest,
so with consent of the participants I have published it here.

As a side project at the bank, I wrote a sophisticated formatter dubbed LIST whose
output can be seen [here](https://github.com/jloughry/BANCStar/blob/master/C16LNAPP.pdf).
In addition to the syntax highlighting and flow notation shown, LIST produced a view on
the code tailored to the programmer's need, generating cross-references, statistics, and
screen shots for documentation.  When I have time I'll put the rest of the code up here.

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
downstream GOTOs must be found and retargeted.

> A few more details I remember:

> BANCStar actually came with a "screen generator" that was supposed to be used to construct
applications. But the 5.1c version of the generator was so limited that experienced programmers
soon began to pry off the covers and modify directly the intermediate code that the run-time module
actually executed.

> When I arrived in Seattle in early 1990, I joined a team of seven programmers who by now routinely
wrote directly in BancSTAR machine language, completely ignoring the screen generator. The first
time they showed me the language, I thought it was a joke. But within a few weeks I was reading and
writing the code, with the aid of every BancSTAR programmer's favorite tools: a dot-matrix printer,
lots and lots of different color highlighters, and a three-ring binder called the Prompt File,
stuffed with printouts of the dozens of tables in the system, and religiously updated anytime
anyone changed anything of significance. (I wasn't kidding about reusing storage; if you needed a
constant integer 1000, and you could find a place where somebody else had once used that same
value, you linked your code to his and hoped it never changed.)

> We developed some in-house tools for programming large applications in BancSTAR, and at one point
attempted to interest Broadway & Seymour in them, but I think they never really believed us, that
we were writing directly to their internal, low-level, undocumented machine code.

