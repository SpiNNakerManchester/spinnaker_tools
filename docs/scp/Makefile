
scp.pdf: scp.tex ../appnote.tex sdp-scp.pdf cmd-ver.pdf cmd-read.pdf cmd-write.pdf
	pdflatex scp.tex

%.pdf: %.fig
	fig2dev -Lpdf $< $@

clean:
	/bin/rm -f *.aux *.log *.pdf *.bak
