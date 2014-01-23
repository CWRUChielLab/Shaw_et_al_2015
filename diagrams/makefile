SNAPSHOTS=protraction-closing-snapshot.png  protraction-open-snapshot.png \
		  retraction-closed-snapshot.png
MUSCLES=$(SNAPSHOTS:snapshot.png=muscles.png)
NEURONS=$(SNAPSHOTS:snapshot.png=neurons.png)

all: model_diagram.pdf model_storyboard.pdf
eps: model_diagram.eps model_storyboard.eps

clean:
	rm -f $(MUSCLES) $(NEURONS) model_diagram.pdf model_storyboard.pdf

model_diagram.pdf: protraction-open-muscles.png protraction-open-neurons.png

model_storyboard.pdf: $(MUSCLES)

%muscles.png : %snapshot.png
	convert $< -extent 620x480+375+150 -alpha set -virtual-pixel transparent \
		-channel A -blur 0x20 -level 50%,100% +channel -background white \
		-flatten $@

%neurons.png : %snapshot.png
	convert $< -crop 401x290+255+670 $@

%.pdf : %_inkscape.svg
	inkscape --export-pdf $@ --export-text-to-path --export-area-page $<

%.eps : %_inkscape.svg
	inkscape --export-eps $@ --export-text-to-path --export-area-page $<

