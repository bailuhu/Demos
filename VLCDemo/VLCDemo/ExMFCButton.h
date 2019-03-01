#pragma once

#define FLAT_UI	// ±âÆ½·ç¸ñUI

#ifdef FLAT_UI
	#define MFCButton ExMFCButton
#else
	#define MFCButton CMFCButton
#endif
// ExMFCButton

#define COLOR_RED			(RGB(255,0,0))
#define COLOR_DARK_RED		(RGB(139,0,0))
#define COLOR_GREEN			(RGB(0,255,0))
#define COLOR_DARK_GREEN	(RGB(0,100,0))
#define COLOR_LIGHT_GREEN	(RGB(144,238,144))
#define COLOR_SKY			(RGB(66,162,255))
#define COLOR_BLUE			(RGB(0,0,255))	
#define COLOR_LIGHT_BLUE	(RGB(173,216,230))
#define COLOR_LIGHT_BLUE1	(RGB(191,239,255))
#define COLOR_LIGHT_BLUE2	(RGB(178,223,238))
#define COLOR_LIGHT_BLUE3	(RGB(154,192,205))
#define COLOR_LIGHT_BLUE4	(RGB(104,131,139))
#define COLOR_YELLOW		(RGB(255,255,0))
#define COLOR_LIGHT_YELLOW	(RGB(255,255,224))
#define COLOR_MAGENTA		(RGB(255,0,255))	// Æ·ºì
#define COLOR_CYAN			(RGB(0,255,255))	// À¶ÂÌ
#define COLOR_CYAN2			(RGB(0,238,238))	// À¶ÂÌ
#define COLOR_CYAN3			(RGB(0,205,205))	// À¶ÂÌ
#define COLOR_CYAN4			(RGB(0,139,139))	// À¶ÂÌ
#define COLOR_GREEN_WHITE	(RGB(100,240,100))	
#define COLOR_WHITE			(RGB(255,255,255))
#define COLOR_BLACK			(RGB(0, 0, 0))
#define COLOR_PINK			(RGB(255,50,255))
#define COLOR_MISTY_ROSE1	(RGB(255,228,225))
#define COLOR_MISTY_ROSE2	(RGB(238,213,210))
#define COLOR_MISTY_ROSE3	(RGB(205,183,181))
#define COLOR_MISTY_ROSE4	(RGB(139,125,123))
#define COLOR_SYS			(RGB(236,233,215))
#define COLOR_LIMIT			(RGB(0,128,128))
#define COLOR_BROWN			(RGB(128, 0, 0))
#define COLOR_ORANGE		(RGB(255,128,64))
#define COLOR_GREENRED		(RGB(128,128,0))
#define COLOR_GREENBLUE		(RGB(0,128,128))
#define COLOR_GREY			(RGB(232,232,232))
#define COLOR_DARKGRAY		(RGB(192,192,192))
#define COLOR_DIM_GREY		(RGB(105,105,105))
#define COLOR_GREY21		(RGB(54,54,54))
#define COLOR_PALE_TURQUOISE (RGB(175,238,238))
#define COLOR_DARK_TURQUOISE (RGB(0,206,209))
#define COLOR_MEDIUM_TURQUOISE (RGB(72,209,204))
#define COLOR_TURQUOISE		(RGB(64,224,208))
#define COLOR_DARK_GOLDENROD (RGB(184,134,11))	// Dark Yellow

class ExMFCButton : public CMFCButton
{
	DECLARE_DYNAMIC(ExMFCButton)

public:
	ExMFCButton();
	virtual ~ExMFCButton();

	void DrawBorder(CDC* pDC, CRect& rectClient, UINT uiState); // ÖØÔØCMFCButton

protected:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDIS);
	virtual void OnDrawBorder(CDC* pDC, CRect& rectClient, UINT uiState);
	DECLARE_MESSAGE_MAP()
	
};


