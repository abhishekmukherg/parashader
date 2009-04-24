parallel_raytracing: parallel_raytracing.tex parashader.bib color_shades.eps \
		normal_gray_scale.eps original_color.eps \
		overly_lit_gray_scale.eps red_scale.eps red_shades.eps \
		blue_gene_bounces_plot.eps blue_gene_pixels_plot.eps \
		taro_desktop_plot.eps
	latex $<
	bibtex $(<:.tex=)
	latex $<
	latex $<

blue_gene_bounces_plot.png:
	ln -s ../render/$@
blue_gene_pixels_plot.png:
	ln -s ../render/$@
taro_desktop_plot.png:
	ln -s ../render/$@

%.eps: %.png
	convert $< $@

clean:
	rm -f *.eps *.aux *.bbl *.blg *.dvi *.log *.out
