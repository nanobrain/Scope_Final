/**
  ******************************************************************************
  * @file    GUI/Userspace.c 
  * @author  Adrian Kurylak
  * @version V1.0.0
  * @date    25-November-2016
  * @brief   Definitions of userspace buttons and widgets
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "cmsis_os.h"                   // CMSIS RTOS header file
#include "main.h"
#include "GUI.h"
#include "FRAMEWIN.h"
#include "DIALOG.h"
#include "Relays.h"
#include "ADC_Config.h"
#include "VGA_Config.h"
#include "DAC_Config.h"
#include "Scope_GUI.h"

void createWindow(WM_HWIN a_hDlg);
void onClick(WM_HWIN a_hDlg, int a_Id);
void onValueChange(WM_HWIN a_hDlg, int a_Id);

void UserSpaceCallback(WM_MESSAGE * pMsg)
{
  WM_HWIN				hDlg;
	int						NCode, Id;

  hDlg = pMsg->hWin;
	
  switch (pMsg->MsgId)
	{
	
	case WM_INIT_DIALOG:
	{
		createWindow(hDlg);
	}break; // WM_INIT_DIALOG
	
	
	case WM_NOTIFY_PARENT:
	{
		Id = WM_GetId(pMsg->hWinSrc);
		NCode = pMsg->Data.v;
		
		
		switch(NCode)
		{
			
				case WM_NOTIFICATION_CLICKED: // Intentionaly it is not WM_NOTIFICATION_RELEASED
				{
					onClick(hDlg,Id);
				}break;
				
				
				case WM_NOTIFICATION_VALUE_CHANGED:
				{
					onValueChange(hDlg,Id);
				}break;
			
		}
	}break;
	
	default:
	WM_DefaultProc(pMsg);
	
	}
} // END

void createWindow(WM_HWIN a_hDlg)
{
	WM_HWIN 	hButton, hSlider;
	uint8_t		x0, y0, offsetY, offsetX;
	
	/**** Create widgets ****/
	// Offset slider
	hSlider = SLIDER_CreateEx(USERSPACEPADDINGX,USERSPACEPADDINGY,GRAPHUNITSIZE,USERSPACESIZEY,a_hDlg,WM_CF_SHOW,SLIDER_CF_VERTICAL,GUI_ID_MAIN_OFFSET);
	SLIDER_SetRange(hSlider, 0, 655);
	SLIDER_SetNumTicks(hSlider, 27);
	SLIDER_SetValue(hSlider, 327); // Default position
	
	// ACDC text and buttons
	offsetX = GRAPHUNITSIZE*1.5;
	offsetY = 0;
	x0 = USERSPACEPADDINGX + offsetX;
	y0 = USERSPACEPADDINGY;
	TEXT_CreateEx(x0,y0,USERSPACESIZEX,TEXT_HEIGHT,a_hDlg,WM_CF_SHOW | TEXT_CF_HCENTER,0,0,"Coupling");
	x0 = x0;
	y0 = USERSPACEPADDINGY * 2 + TEXT_HEIGHT;
	hButton = BUTTON_CreateEx(x0,y0,SBUTTON_WIDTH,SBUTTON_HEIGHT,a_hDlg, WM_CF_SHOW, 0, GUI_ID_USERSPACE_AC);
	BUTTON_SetText(hButton,"AC");
	WM_EnableWindow(hButton);
	x0 += USERSPACEPADDINGX+SBUTTON_WIDTH;
	y0 = y0;
	hButton = BUTTON_CreateEx(x0,y0,SBUTTON_WIDTH,SBUTTON_HEIGHT,a_hDlg, WM_CF_SHOW, 0, GUI_ID_USERSPACE_DC);
	BUTTON_SetText(hButton,"DC");
	WM_DisableWindow(hButton);
	
	// GND/IN text and buttons
	offsetX = offsetX;
	offsetY += TEXT_HEIGHT + SBUTTON_HEIGHT + 3 * USERSPACEPADDINGY;
	x0 = USERSPACEPADDINGX + offsetX;
	y0 = USERSPACEPADDINGY + offsetY;
	TEXT_CreateEx(x0,y0,USERSPACESIZEX,TEXT_HEIGHT,a_hDlg,WM_CF_SHOW | TEXT_CF_HCENTER,0,0,"Input");
	x0 = x0;
	y0 = USERSPACEPADDINGY*2+TEXT_HEIGHT + offsetY;
	hButton = BUTTON_CreateEx(x0,y0,SBUTTON_WIDTH,SBUTTON_HEIGHT,a_hDlg, WM_CF_SHOW, 0, GUI_ID_USERSPACE_GND);
	BUTTON_SetText(hButton,"GND");
	WM_DisableWindow(hButton);
	x0 += USERSPACEPADDINGX+SBUTTON_WIDTH;
	y0 = y0;
	hButton = BUTTON_CreateEx(x0,y0,SBUTTON_WIDTH,SBUTTON_HEIGHT,a_hDlg, WM_CF_SHOW, 0, GUI_ID_USERSPACE_IN);
	BUTTON_SetText(hButton,"BNC");
	WM_EnableWindow(hButton);
	
	// GND/IN text and buttons
	offsetX = offsetX;
	offsetY += TEXT_HEIGHT + SBUTTON_HEIGHT + 3 * USERSPACEPADDINGY;
	x0 = USERSPACEPADDINGX + offsetX;
	y0 = USERSPACEPADDINGY + offsetY;
	TEXT_CreateEx(x0,y0,USERSPACESIZEX,TEXT_HEIGHT,a_hDlg,WM_CF_SHOW | TEXT_CF_HCENTER,0,0,"Attenuation");
	x0 = x0;
	y0 = USERSPACEPADDINGY * 2 + TEXT_HEIGHT + offsetY;
	hButton = BUTTON_CreateEx(x0,y0,SBUTTON_WIDTH,SBUTTON_HEIGHT,a_hDlg, WM_CF_SHOW, 0, GUI_ID_USERSPACE_ATT1);
	BUTTON_SetText(hButton,"1:1");
	WM_EnableWindow(hButton);
	x0 += USERSPACEPADDINGX+SBUTTON_WIDTH;
	y0 = y0;
	hButton = BUTTON_CreateEx(x0,y0,SBUTTON_WIDTH,SBUTTON_HEIGHT,a_hDlg, WM_CF_SHOW, 0, GUI_ID_USERSPACE_ATT10);
	BUTTON_SetText(hButton,"1:10");
	WM_DisableWindow(hButton);
	
	// Gain Up/Down buttons
	offsetX = offsetX;
	offsetY += TEXT_HEIGHT + SBUTTON_HEIGHT + 3*USERSPACEPADDINGY;
	x0 = USERSPACEPADDINGX + offsetX;
	y0 = USERSPACEPADDINGY + offsetY;
	TEXT_CreateEx(x0,y0,USERSPACESIZEX,TEXT_HEIGHT,a_hDlg,WM_CF_SHOW | TEXT_CF_HCENTER,0,0,"Gain");
	x0 = x0;
	y0 += USERSPACEPADDINGY+TEXT_HEIGHT;
	hButton = BUTTON_CreateEx(x0,y0,BBUTTON_WIDTH,2*BBUTTON_HEIGHT,a_hDlg, WM_CF_SHOW, 0, GUI_ID_USERSPACE_GAINUP);
	BUTTON_SetText(hButton,"Up");
	x0 = x0;
	y0 += 2 * BBUTTON_HEIGHT + USERSPACEPADDINGY;
	hButton = BUTTON_CreateEx(x0,y0,BBUTTON_WIDTH,2*BBUTTON_HEIGHT,a_hDlg, WM_CF_SHOW, 0, GUI_ID_USERSPACE_GAINDOWN);
	BUTTON_SetText(hButton,"Down");
}

void onValueChange(WM_HWIN a_hDlg, int a_Id)
{
	uint16_t	value;
	WM_HWIN 	hSlider;
	
	switch(a_Id)
	{
		
		case GUI_ID_MAIN_OFFSET:
		{
			hSlider = WM_GetDialogItem(a_hDlg,GUI_ID_MAIN_OFFSET);
			value = (65535 - SLIDER_GetValue(hSlider))*100;
			DAC_Set_Value(value);
		}break;
		
		default:
		{
			/*pass*/
		}
	}
}

void onClick(WM_HWIN a_hDlg, int a_Id)
{
	WM_HWIN hButton;
	
	switch(a_Id)
	{
		case GUI_ID_USERSPACE_AC:
		{
			hButton = WM_GetDialogItem(a_hDlg,GUI_ID_USERSPACE_AC);
			WM_DisableWindow(hButton);
			hButton = WM_GetDialogItem(a_hDlg,GUI_ID_USERSPACE_DC);
			WM_EnableWindow(hButton);
			Relay_ACDC(AC);
		}break;

		case GUI_ID_USERSPACE_DC:
		{
			hButton = WM_GetDialogItem(a_hDlg,GUI_ID_USERSPACE_AC);
			WM_EnableWindow(hButton);
			hButton = WM_GetDialogItem(a_hDlg,GUI_ID_USERSPACE_DC);
			WM_DisableWindow(hButton);
			Relay_ACDC(DC);
		}break;

		case GUI_ID_USERSPACE_GND:
		{
			hButton = WM_GetDialogItem(a_hDlg,GUI_ID_USERSPACE_GND);
			WM_DisableWindow(hButton);
			hButton = WM_GetDialogItem(a_hDlg,GUI_ID_USERSPACE_IN);
			WM_EnableWindow(hButton);
			Relay_Input(GND);
		}break;

		case GUI_ID_USERSPACE_IN:
		{
			hButton = WM_GetDialogItem(a_hDlg,GUI_ID_USERSPACE_GND);
			WM_EnableWindow(hButton);
			hButton = WM_GetDialogItem(a_hDlg,GUI_ID_USERSPACE_IN);
			WM_DisableWindow(hButton);
			Relay_Input(INPUT);
		}break;

		case GUI_ID_USERSPACE_ATT1:
		{
			hButton = WM_GetDialogItem(a_hDlg,GUI_ID_USERSPACE_ATT1);
			WM_DisableWindow(hButton);
			hButton = WM_GetDialogItem(a_hDlg,GUI_ID_USERSPACE_ATT10);
			WM_EnableWindow(hButton);
			Relay_Attenuator(DB0);
		}break;

		case GUI_ID_USERSPACE_ATT10:
		{
			hButton = WM_GetDialogItem(a_hDlg,GUI_ID_USERSPACE_ATT1);
			WM_EnableWindow(hButton);
			hButton = WM_GetDialogItem(a_hDlg,GUI_ID_USERSPACE_ATT10);
			WM_DisableWindow(hButton);
			Relay_Attenuator(DB20);
		}break;

		case GUI_ID_USERSPACE_GAINUP:
		{
			if( VGA_Step_Gain(STEP_UP) != 0 )
				WM_DisableWindow(WM_GetDialogItem(a_hDlg,GUI_ID_USERSPACE_GAINUP));
			else
			{
				WM_EnableWindow(WM_GetDialogItem(a_hDlg,GUI_ID_USERSPACE_GAINDOWN));
				WM_EnableWindow(WM_GetDialogItem(a_hDlg,GUI_ID_USERSPACE_GAINUP));
			}
		}break;

		case GUI_ID_USERSPACE_GAINDOWN:
		{
			if( VGA_Step_Gain(STEP_DOWN) )
				WM_DisableWindow(WM_GetDialogItem(a_hDlg,GUI_ID_USERSPACE_GAINDOWN));
			else
			{
				WM_EnableWindow(WM_GetDialogItem(a_hDlg,GUI_ID_USERSPACE_GAINDOWN));
				WM_EnableWindow(WM_GetDialogItem(a_hDlg,GUI_ID_USERSPACE_GAINUP));
			}
		}break;
		default:
		{
			/*pass*/
		}
	}
}
/**** END OF FILE ****/
