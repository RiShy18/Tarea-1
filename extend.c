// Last updated 2008/11/04 10:53

// convert logo: -background blue -extent 1024x768-192-144 logo_extent.jpg
// Read an image and centre it on a larger 1024x768, extended canvas.
// The input image must be no larger than 1024x768 because this code does not
// check for errors. 

#include <windows.h>
#include <wand/magick_wand.h>

void test_wand(void)
{
	MagickWand *m_wand = NULL;
	PixelWand *p_wand;
	int w,h;

	MagickWandGenesis();

	/* Create a wand */
	m_wand = NewMagickWand();
	p_wand = NewPixelWand();

	// Change this to whatever colour you like - e.g. "none"
	PixelSetColor(p_wand, "blue");
	/* Read the input image */
	MagickReadImage(m_wand,"logo:");
	w = MagickGetImageWidth(m_wand);
	h = MagickGetImageHeight(m_wand);
	MagickSetImageBackgroundColor(m_wand,p_wand);
	// This centres the original image on the new canvas.
	// Note that the extent's offset is relative to the 
	// top left corner of the *original* image, so adding an extent
	// around it means that the offset will be negative
	MagickExtentImage(m_wand,1024,768,-(1024-w)/2,-(768-h)/2);
	MagickWriteImage(m_wand,"logo_extent.jpg");

	/* Tidy up */
	m_wand = DestroyMagickWand(m_wand);
	p_wand = DestroyPixelWand(p_wand);
	MagickWandTerminus();
}
