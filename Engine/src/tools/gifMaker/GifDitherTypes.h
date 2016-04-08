#pragma once

// done for consistency. feel free to use Freeimage versions
namespace kubik
{
	enum ditherTypes
	{
		OFX_GIF_DITHER_NONE = -1,
		OFX_GIF_DITHER_FS = 1,           // Floyd & Steinberg error diffusion
		OFX_GIF_DITHER_BAYER4x4 = 2,		// Bayer ordered dispersed dot dithering (order 2 dithering matrix)
		OFX_GIF_DITHER_BAYER8x8 = 3, 	// Bayer ordered dispersed dot dithering (order 3 dithering matrix)
		OFX_GIF_DITHER_CLUSTER6x6 = 4,	// Ordered clustered dot dithering (order 3 - 6x6 matrix)
		OFX_GIF_DITHER_CLUSTER8x8 = 5,	// Ordered clustered dot dithering (order 4 - 8x8 matrix)
		OFX_GIF_DITHER_CLUSTER16x16 = 6, // Ordered clustered dot dithering (order 8 - 16x16 matrix)
		OFX_GIF_DITHER_BAYER16x16 = 7,	// Bayer ordered dispersed dot dithering (order 4 dithering matrix)
	};
}
