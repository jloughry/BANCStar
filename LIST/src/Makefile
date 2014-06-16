SRCDIR=.
SHIMDIR=../shim

# use modern Windows API calls to simulate <graph.h> functionality
SHIM=$(SHIMDIR)/graph.c

# select the printer implementation to use: ANSI_SCREEN, PROPRINTER, LASERJET, PAGEPRINTER, GENERIC_PRINTER
PRINTER=ANSI_SCREEN

CC=bcc32
CFLAGS=-I$(SHIMDIR) -I$(SRCDIR) -D$(PRINTER)

RM=rm

# skip src/L_COLORS.C
SOURCES= \
	$(SRCDIR)/L_MAIN.C \
	$(SRCDIR)/L_ANSI.C \
	$(SRCDIR)/L_ARROWS.C \
	$(SRCDIR)/L_BBDD.C \
	$(SRCDIR)/L_CHECK.C \
	$(SRCDIR)/L_CHECK1.C \
	$(SRCDIR)/L_CHKLIN.C \
	$(SRCDIR)/L_CHKRPT.C \
	$(SRCDIR)/L_COMMTS.C \
	$(SRCDIR)/L_CONVER.C \
	$(SRCDIR)/L_DISPPR.C \
	$(SRCDIR)/L_DOC.C \
	$(SRCDIR)/L_FOOTER.C \
	$(SRCDIR)/L_GETBIT.C \
	$(SRCDIR)/L_GETPR.C \
	$(SRCDIR)/L_GETTBL.C \
	$(SRCDIR)/L_HEADER.C \
	$(SRCDIR)/L_HELP.C \
	$(SRCDIR)/L_IBMPR.C \
	$(SRCDIR)/L_INDENT.C \
	$(SRCDIR)/L_LASERJ.C \
	$(SRCDIR)/L_LIST.C \
	$(SRCDIR)/L_LONGST.C \
	$(SRCDIR)/L_MODPR.C \
	$(SRCDIR)/L_NUMBER.C \
	$(SRCDIR)/L_NUMSTR.C \
	$(SRCDIR)/L_OPTION.C \
	$(SRCDIR)/L_PAGEBR.C \
	$(SRCDIR)/L_PAGENM.C \
	$(SRCDIR)/L_PAGEPR.C \
	$(SRCDIR)/L_POINTR.C \
	$(SRCDIR)/L_PRINTR.C \
	$(SRCDIR)/L_RDMAIN.C \
	$(SRCDIR)/L_RDPROM.C \
	$(SRCDIR)/L_READLN.C \
	$(SRCDIR)/L_REPORT.C \
	$(SRCDIR)/L_RUNNER.C \
	$(SRCDIR)/L_SCNPRT.C \
	$(SRCDIR)/L_SECRET.C \
	$(SRCDIR)/L_SETBLD.C \
	$(SRCDIR)/L_SETCON.C \
	$(SRCDIR)/L_SETITA.C \
	$(SRCDIR)/L_SETUP.C \
	$(SRCDIR)/L_SHORTP.C \
	$(SRCDIR)/L_SPACES.C \
	$(SRCDIR)/L_SRCODE.C \
	$(SRCDIR)/L_TABS.C \
	$(SRCDIR)/L_WINDOW.C \
	$(SRCDIR)/L_ZEROST.C

all: list

clean:
	-$(RM) *.obj
	-$(RM) *.exe
	-$(RM) *.tds

list: $(SOURCES) $(SHIM)
	$(CC) $(CFLAGS) $+

%.obj: src/%.c
	$(CC) $(CFLAGS) -c -o $@ $+

shim/graph.c: shim/graph.h
graph.obj: shim/graph.c
	$(CC) $(CFLAGS) -c -o $@ $+
