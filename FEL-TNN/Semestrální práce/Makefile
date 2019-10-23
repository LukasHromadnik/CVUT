PANDOC=pandoc -s Zpráva.md -f markdown -t latex --filter pandoc-citeproc --toc -V lang=cs-CZ

all: html

pdf:
	$(PANDOC) \
		--variable papersize=a4paper \
		-o main.pdf

tex:
	pdflatex main.tex main.pdf
	bibtex main
	pdflatex main.tex main.pdf
	pdflatex main.tex main.pdf

html:
	$(PANDOC) \
		--katex \
		-t html \
		--css github.css \
		-o docs/index.html \
		-V title:""

clean:
	rm -f \
		main.aux \
		main.bbl \
		main.blg \
		main.log \
		main.pdf \
		index.html \
		texput.log
