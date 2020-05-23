ImageServer: ImageFilter
		gcc -o ImgServer ImageServer.c -lulfius -lorcania -ljansson -lyder

ImageFilter: filters
		 gcc -o dirTest dirprueba.c imageFilters.a -ljansson `pkg-config --cflags --libs MagickWand`

filters:
		gcc -c imageFilters.c `pkg-config --cflags --libs MagickWand`
		ar rv imageFilters.a imageFilters.o
		ranlib imageFilters.a