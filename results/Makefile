all: blue_gene_bounces_plot.eps blue_gene_pixels_plot.eps taro_desktop_plot.eps timings-test1.eps timings-test2.eps

%.eps: %.png
	convert $< $@

%.png: %.gnuplot %.txt
	gnuplot $<

clean:
	rm *.png *.eps

.PHONY: clean
