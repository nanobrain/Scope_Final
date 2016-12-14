/**
  ******************************************************************************
  * @file    GUI/Scope_GUI.h 
  * @author  Adrian Kurylak
  * @version V1.0.0
  * @date    25-November-2016
  * @brief   Header for GUI
  ******************************************************************************
  */

#ifndef __SCOPE_GUI_H
#define __SCOPE_GUI_H

extern void GraphCallback(WM_MESSAGE * pMsg);
extern void UserSpaceCallback(WM_MESSAGE * pMsg);
extern void TriggerLineCallback(WM_MESSAGE * pMsg);

typedef struct GUI_Colors{
	GUI_COLOR BackGround;
	GUI_COLOR GraphBackGround;
	GUI_COLOR Grid;
	GUI_COLOR Label;
	GUI_COLOR Border;
	GUI_COLOR Frame;
	GUI_COLOR Waveform;
}GUI_Colors;

/* Windows IDs */
#define GUI_HDLG_ID_FRAMEWIN 1
#define GUI_HDLG_ID_GRAPH_WIN 2

/* Screen & windows sizes */
// Scope graph
#define GRIDLS						GUI_LS_DOT
#define GRAPHCOUNTX 			10
#define GRAPHCOUNTY				8
#define GRAPHUNITSIZE			34
#define GRAPHSIZEX				GRAPHCOUNTX * GRAPHUNITSIZE
#define GRAPHSIZEY				GRAPHCOUNTY * GRAPHUNITSIZE
#define GRAPHSTARTX				0
#define GRAPHSTARTY				0
#define GRAPHENDX					GRAPHSTARTX + GRAPHSIZEX
#define GRAPHENDY					GRAPHSTARTY + GRAPHSIZEY

// User space
#define SCREENSIZEX				480
#define SCREENSIZEY				272
#define USERSPACESTARTX		GRAPHENDX
#define USERSPACESTARTY		GRAPHSTARTY
#define USERSPACEENDX			SCREENSIZEX
#define USERSPACEENDY			SCREENSIZEY
#define USERSPACESIZEX		SCREENSIZEX - GRAPHSIZEX // Better is USERSPACEENDX-USERSPACESTARTX, but it doesn't work. Dunno why...
#define USERSPACESIZEY		USERSPACEENDY - USERSPACESTARTY
#define USERSPACEPADDINGX	5
#define USERSPACEPADDINGY	5

// Small button
#define SBUTTON_WIDTH				(GRAPHUNITSIZE * 2.5 - USERSPACEPADDINGX * 3) / 2
#define SBUTTON_HEIGHT			20

// Big button
#define BBUTTON_WIDTH				GRAPHUNITSIZE * 2.5 - USERSPACEPADDINGX * 2
#define BBUTTON_HEIGHT			20

// Text
#define TEXT_HEIGHT					15

/* GUI elements IDs */
#define GUI_ID_MAIN_SCOPE								GUI_ID_USER
#define GUI_ID_MAIN_SCOPE_GRAPH					GUI_ID_USER + 1
#define GUI_ID_USERSPACE								GUI_ID_USER + 2
#define GUI_ID_USERSPACE_AC							GUI_ID_USER + 3
#define GUI_ID_USERSPACE_DC							GUI_ID_USER + 4
#define GUI_ID_USERSPACE_GND						GUI_ID_USER + 5
#define GUI_ID_USERSPACE_IN							GUI_ID_USER + 6
#define GUI_ID_USERSPACE_ATT1						GUI_ID_USER + 7
#define GUI_ID_USERSPACE_ATT10					GUI_ID_USER + 8
#define GUI_ID_USERSPACE_GAINUP					GUI_ID_USER + 9
#define GUI_ID_USERSPACE_GAINDOWN				GUI_ID_USER + 10
#define GUI_ID_MAIN_OFFSET							GUI_ID_USER + 11

#endif /* __SCOPE_GUI_H */

/**** END OF FILE ****/
