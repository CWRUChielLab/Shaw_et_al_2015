MAINTEX = ShawEmpiricalSHCPaper.tex
SOURCES= $(MAINTEX)\
	math.bib \
	neuroscience.bib \
	params.tex \
	reliability.bib \
	ShawEmpiricalSHCPaper_body.tex \
	shc.bib \
	svglov3.clo \
	svjour3.cls

$(MAINTEX:%.tex=%.pdf): $(SOURCES) data_analysis diagram_pdfs model9_figs
	pdflatex $(MAINTEX)
	bibtex $(MAINTEX:%.tex=%.aux)
	pdflatex $(MAINTEX)
	pdflatex $(MAINTEX)

data_analysis:
	$(MAKE) -C data-analysis

diagram_pdfs:
	$(MAKE) -C diagrams

model9_figs:
	$(MAKE) -C model9
