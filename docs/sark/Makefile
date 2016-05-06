
sark.pdf: sark.tex ../appnote.tex sdram.pdf sysram.pdf tcm.pdf
	pdflatex sark.tex

%.pdf: %.fig
	fig2dev -Lpdf $< $@

clean:
	/bin/rm -f *.aux *.log *.pdf *.bak
