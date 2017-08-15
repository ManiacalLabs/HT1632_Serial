// #include "MatrixDisplay.h"
// #include "DisplayToolbox.h"
//
//
// //DEFINES
// #define MAXMSG 129
// #define MAXCHAR MAXMSG - 1
//
// // Macro to make it the initDisplay function a little easier to understand
// #define setMaster(dispNum, CSPin) initDisplay(dispNum,CSPin,true)
// #define setSlave(dispNum, CSPin) initDisplay(dispNum,CSPin,false)
// //****************
//
// //GLOBALS
// bool _updateAvailable = false;
//
// uint8_t _dispBuf[MAXCHAR];
//
// // Prepare boundaries
// uint8_t X_MAX = 0;
// uint8_t Y_MAX = 0;
//
// //****************
//
// //GLOBAL OBJECTS
// MatrixDisplay disp(4,6,7, false); //Matrix controller
// DisplayToolbox toolbox(&disp); //Display tools
//
// //****************
//
// void setup()
// {
// 	memset(_dispBuf, 0b11001100, MAXCHAR);
// 	_updateAvailable = true;
//
// 	Serial.begin(115200);
// 	Serial.flush();
//
// 	// Fetch bounds (dynamically work out how large this display is)
// 	X_MAX = disp.getTotalWidth();
// 	Y_MAX = disp.getTotalHeight();
//
// 	// Prepare displays
// 	disp.setMaster(0,5);
// 	disp.setSlave(1,4);
// 	disp.setSlave(2,3);
// 	disp.setSlave(3,2);
//
// 	disp.clear();
// 	disp.syncDisplays();
// 	toolbox.setBrightness(16);
// }
//
// void UpdateDisplay()
// {
// 	if(_updateAvailable)
// 	{
// 		// _updateAvailable = false;
//
// 		//disp.clear();
//
// 		static uint8_t col;
// 		static uint8_t row;
// 		static uint8_t dots;
// 		for (col=0; col< X_MAX; col++)
// 		{
// 			dots = _dispBuf[col];
// 			for (row=0; row < Y_MAX; row++) {
// 				if (dots & (128>>row))   	     // only 8 rows.
// 					toolbox.setPixel(col, row, 1);
// 				else
// 					toolbox.setPixel(col, row, 0);
// 			}
// 		}
//
// 		disp.syncDisplays();
//
// 		Serial.print("#"); //So we know the display has been updated
// 	}
// }
//
// void loop()
// {
// 	UpdateDisplay();
//     delay(250);
// }
