
all :
	latex introduction.to.signal0x.tex
	latex introduction.to.signal0x.tex
	dvips introduction.to.signal0x.dvi
	ps2pdf14 introduction.to.signal0x.ps
	apvlv introduction.to.signal0x.pdf

.PHONY : clean distclean
clean:
	rm -rf *.log
	rm -rf *.aux
	rm -rf *.dvi
	rm -rf *.ps
distclean: clean
	rm -rf *.pdf
