parallel_raytracing: parallel_raytracing.tex parashader.bib color_shades.eps \
		normal_gray_scale.eps original_color.eps \
		overly_lit_gray_scale.eps red_scale.eps red_shades.eps
	latex $<
	bibtex $(<:.tex=)
	latex $<
	latex $<

color_shades.eps: color_shades.png
	convert $< $@

normal_gray_scale.eps: normal_gray_scale.png
	convert $< $@

original_color.eps: original_color.png
	convert $< $@

overly_lit_gray_scale.eps: overly_lit_gray_scale.png
	convert $< $@

red_scale.eps: red_scale.png
	convert $< $@

red_shades.eps: red_shades.png
	convert $< $@

