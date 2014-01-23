SOURCES= \
	boxplot-different-preps-protraction-retraction.R \
	boxplot-held-protraction-retraction.R \
	kde-retraction.R

all: $(SOURCES:%.R=%.pdf)

%.pdf : %.R
	Rscript $<
