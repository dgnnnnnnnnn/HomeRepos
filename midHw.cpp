#ifdef UNICODE
#pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console")
#else
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
#endif

#include <cmath>
#include <windows.h>

bool isDrawingBox = false;
bool isDrawingCircle = false;
bool isDrawingBonobono = false;
bool isDrawingLion = false;
bool isDrawingCube = false;
bool wasDrawingLion = false;
bool BonobonoSwitch = false;

BOOL CUBE_DRAW_MODE = false;
BOOL CUBE_RESIZE_MODE = false;


POINT startPointBox = { 0 };  
POINT endPointBox = { 0 };

POINT startPointCircle = { 0 };
POINT endPointCircle = { 0 };

POINT startPointLion = { 0 };  // Lionìš© Point
POINT endPointLion = { 0 };

POINT startPointCube = { 0 };
POINT endPointCube = { 0 };
POINT cubeResizePoint = { 0 };
POINT cubePoints[8]; // Àü¿ªº¯¼ö resize cube¿ë


int isMouseLButtonPressed = 0;
int isMouseRButtonPressed = 0;
int isMouseOnDrawingBox = 0;
RECT rect = { 0 };

const wchar_t* textCrash = L"Crash!!!";
int mx, my;
int emx, emy;
int mmx, mmy;
int bx, by;
int bbx, bby;
<<<<<<< HEAD
int ccx, ccy;
int radius;  // ¿øÀÇ ¹İÁö¸§
=======
int radius;  // ì›ì˜ ë°˜ì§€ë¦„
>>>>>>> 261dcef258f61636d46f713d9a2618932561a998

RECT drawArea;

// ê¸°ë³¸ í¬ê¸° ë° ìœ„ì¹˜ ì„¤ì •
const int DEFAULT_FACE_WIDTH = 100; // ì–¼êµ´ ê¸°ë³¸ ë„ˆë¹„
const int DEFAULT_FACE_HEIGHT = 100; // ì–¼êµ´ ê¸°ë³¸ ë†’ì´

const int DEFAULT_EAR1_WIDTH = 40;
const int DEFAULT_EAR1_HEIGHT = 40;
const int DEFAULT_EAR1_X = -5;
const int DEFAULT_EAR1_Y = -10;

const int DEFAULT_EAR2_WIDTH = 40;
const int DEFAULT_EAR2_HEIGHT = 40;
const int DEFAULT_EAR2_X = 65;
const int DEFAULT_EAR2_Y = -10;

const int DEFAULT_EYE1_WIDTH = 6;
const int DEFAULT_EYE1_HEIGHT = 6;
const int DEFAULT_EYE1_X = 30;
const int DEFAULT_EYE1_Y = 40;

const int DEFAULT_EYE2_WIDTH = 6;
const int DEFAULT_EYE2_HEIGHT = 6;
const int DEFAULT_EYE2_X = 65;
const int DEFAULT_EYE2_Y = 40;

const int DEFAULT_NOSE1_WIDTH = 11;
const int DEFAULT_NOSE1_HEIGHT = 9;
const int DEFAULT_NOSE1_X = 40;
const int DEFAULT_NOSE1_Y = 55;

const int DEFAULT_NOSE2_WIDTH = 11;
const int DEFAULT_NOSE2_HEIGHT = 9;
const int DEFAULT_NOSE2_X = 51;
const int DEFAULT_NOSE2_Y = 55;




//bool InRect(INT mmx, INT mmy, RECT rect1)    // ÇöÀçÄ¿¼­À§Ä¡, ÁöÁ¤ÇÒ »ç°¢Çü ¿µ¿ª
//{
//	if ((mmx > rect1.left && mmx < rect1.right) &&
//		(mmy > rect1.top && mmy < rect1.bottom))
//	{
//		return 1; // µµÇü°ú °ãÄ§
//	}
//	return 0; // µÎ RECT°¡ °ãÄ¡Áö ¾ÊÀ½
//}

<<<<<<< HEAD
bool InBox(int mmx, int mmy, POINT startPointBox, POINT endPointBox) {

	/* 3Â÷¿ø Box¸¦ 2Â÷¿ø ÇüÅÂ·Î ±¸ÇöÇÏ¸é 3°³ÀÇ µµÇüÀÌ ³ª¿È -> °¢°¢ front,top,rightFaceÀÇ Æ÷ÀÎÆ®¸¦ °¡Áü 
	   ÆòÇà»çº¯ÇüÀº 2°³ÀÇ »ï°¢ÇüÀ¸·Î ÂÉ°³ °Ë»çÇØ¾ßÇÏÁö¸¸ »ı·«ÇÏ°í ¸®ÀüÇÔ¼ö »ç¿ë */

	POINT startPoint = { min(startPointBox.x, endPointBox.x), min(startPointBox.y, endPointBox.y) };
	POINT endPoint = { max(startPointBox.x, endPointBox.x), max(startPointBox.y, endPointBox.y) };

	// µå·¡±× ¹æÇâÀ» ±âÁØÀ¸·Î ÆòÇà Åõ¿µ¿¡ ´ëÇÑ ¿ÀÇÁ¼Â °è»ê -> ½ÃÀÛÆ÷ÀÎÆ®º¸´Ù Á¾·áÆ÷ÀÎÆ®°¡ ´õ ÀÛÀº °æ¿ìµµ ÀÖÀ¸´Ï±î
	const int offset_x = (startPoint.x <= endPoint.x) ? 30 : -30;
	const int offset_y = (startPoint.y <= endPoint.y) ? -30 : 30;

	POINT cubePoints[8] = {
		{startPoint.x, startPoint.y},
		{endPoint.x, startPoint.y},
		{endPoint.x, endPoint.y},
		{startPoint.x, endPoint.y},
		{startPoint.x + offset_x, startPoint.y + offset_y},
		{endPoint.x + offset_x, startPoint.y + offset_y},
		{endPoint.x + offset_x, endPoint.y + offset_y},
		{startPoint.x + offset_x,endPoint.y + offset_y}
	};

	POINT frontFace[4] = { cubePoints[0], cubePoints[1], cubePoints[2], cubePoints[3] };

	POINT topFace[4] = { cubePoints[0], cubePoints[1], cubePoints[5], cubePoints[4] };
	HRGN hRgnOne = CreatePolygonRgn(topFace, 4, WINDING);      // ¸®Àü(À§ÂÊ ÆòÇà»çº¯Çü) »ı¼º
	BOOL bInsideOne = PtInRegion(hRgnOne, mmx, mmy);           // PtInRegionÇÔ¼ö) mmx, mmyÀÌ ¸®Àü¿¡ ÀÖ´ÂÁö °Ë»çÇØ true-false ¹İÈ¯

	POINT rightFace[4] = { cubePoints[1], cubePoints[2], cubePoints[6], cubePoints[5] };
	HRGN hRgnTwo = CreatePolygonRgn(rightFace, 4, WINDING);  
	BOOL bInsideTwo = PtInRegion(hRgnTwo, mmx, mmy);        


	if ((mmx > startPoint.x && mmx < endPoint.x && mmy > startPoint.y && mmy < endPoint.y)
		|| bInsideOne || bInsideTwo) {
		return true;
	}
	return false;
=======
bool InRect(INT mmx, INT mmy, RECT rect1)    // í˜„ì¬ì»¤ì„œìœ„ì¹˜, ì§€ì •í•  ì‚¬ê°í˜• ì˜ì—­
{
	if ((mmx > rect1.left && mmx < rect1.right) &&
		(mmy > rect1.top && mmy < rect1.bottom))
	{
		return 1; // ë„í˜•ê³¼ ê²¹ì¹¨
	}
	return 0; // ë‘ RECTê°€ ê²¹ì¹˜ì§€ ì•ŠìŒ
>>>>>>> 261dcef258f61636d46f713d9a2618932561a998
}

BOOL InCircle(int x, int y, POINT center, int radius) {
	int dx = x - center.x;
	int dy = y - center.y;
	return (dx * dx + dy * dy) <= (radius * radius);
}

<<<<<<< HEAD
bool InDrawArea(INT x, INT y, RECT rect2)    // ÇöÀçÄ¿¼­À§Ä¡, ÁöÁ¤ÇÒ »ç°¢Çü ¿µ¿ª
=======

bool InDrawArea(INT x, INT y, RECT rect2)    // í˜„ì¬ì»¤ì„œìœ„ì¹˜, ì§€ì •í•  ì‚¬ê°í˜• ì˜ì—­
>>>>>>> 261dcef258f61636d46f713d9a2618932561a998
{
	if ((x > rect2.left && x < rect2.right) &&
		(y > rect2.top && y < rect2.bottom))
	{
		return 1; // ë„í˜•ê³¼ ê²¹ì¹¨
	}
	return 0; // ë‘ RECTê°€ ê²¹ì¹˜ì§€ ì•ŠìŒ
}

void OnMouseDrag(HDC hdc, POINT startPoint, POINT endPoint)
{
	
	// ìŠ¤ì¼€ì¼ë§ ë¹„ìœ¨ ê³„ì‚°
	float scaleX = (float)(endPoint.x - startPoint.x) / DEFAULT_FACE_WIDTH;
	float scaleY = (float)(endPoint.y - startPoint.y) / DEFAULT_FACE_HEIGHT;

	// ì¡°ì •ëœ í¬ê¸° ë° ìœ„ì¹˜ë¡œ ê·¸ë¦¬ê¸° : ì–¼êµ´, ê·€ 2ê°œ
	HBRUSH Lion1 = CreateSolidBrush(RGB(255, 200, 15));
	HBRUSH OldBrush1 = (HBRUSH)SelectObject(hdc, Lion1);
	Ellipse(hdc, startPoint.x + DEFAULT_EAR1_X * scaleX,
		startPoint.y + DEFAULT_EAR1_Y * scaleY,
		startPoint.x + (DEFAULT_EAR1_X + DEFAULT_EAR1_WIDTH) * scaleX,
		startPoint.y + (DEFAULT_EAR1_Y + DEFAULT_EAR1_HEIGHT) * scaleY);

	Ellipse(hdc, startPoint.x + DEFAULT_EAR2_X * scaleX,
		startPoint.y + DEFAULT_EAR2_Y * scaleY,
		startPoint.x + (DEFAULT_EAR2_X + DEFAULT_EAR2_WIDTH) * scaleX,
		startPoint.y + (DEFAULT_EAR2_Y + DEFAULT_EAR2_HEIGHT) * scaleY);

	Ellipse(hdc, startPoint.x, startPoint.y,
		startPoint.x + DEFAULT_FACE_WIDTH * scaleX, startPoint.y + DEFAULT_FACE_HEIGHT * scaleY);

	SelectObject(hdc, OldBrush1);
	DeleteObject(Lion1);

	// ëˆˆ 2ê°œ
	HBRUSH Lion2 = CreateSolidBrush(RGB(0, 0 ,0));
	HBRUSH OldBrush2 = (HBRUSH)SelectObject(hdc, Lion2);
	Ellipse(hdc, startPoint.x + DEFAULT_EYE1_X * scaleX,
		startPoint.y + DEFAULT_EYE1_Y * scaleY,
		startPoint.x + (DEFAULT_EYE1_X + DEFAULT_EYE1_WIDTH) * scaleX,
		startPoint.y + (DEFAULT_EYE1_Y + DEFAULT_EYE1_HEIGHT) * scaleY);

	Ellipse(hdc, startPoint.x + DEFAULT_EYE2_X * scaleX,
		startPoint.y + DEFAULT_EYE2_Y * scaleY,
		startPoint.x + (DEFAULT_EYE2_X + DEFAULT_EYE2_WIDTH) * scaleX,
		startPoint.y + (DEFAULT_EYE2_Y + DEFAULT_EYE2_HEIGHT) * scaleY);
	SelectObject(hdc, OldBrush2);
	DeleteObject(Lion2);

	// ì½” 2ê°œ
	Ellipse(hdc, startPoint.x + DEFAULT_NOSE1_X * scaleX,
		startPoint.y + DEFAULT_NOSE1_Y * scaleY,
		startPoint.x + (DEFAULT_NOSE1_X + DEFAULT_NOSE1_WIDTH) * scaleX,
		startPoint.y + (DEFAULT_NOSE1_Y + DEFAULT_NOSE1_HEIGHT) * scaleY);

	Ellipse(hdc, startPoint.x + DEFAULT_NOSE2_X * scaleX,
		startPoint.y + DEFAULT_NOSE2_Y * scaleY,
		startPoint.x + (DEFAULT_NOSE2_X + DEFAULT_NOSE2_WIDTH) * scaleX,
		startPoint.y + (DEFAULT_NOSE2_Y + DEFAULT_NOSE2_HEIGHT) * scaleY);

	int centerX = (startPoint.x + endPoint.x) / 2;
	int centerY = (startPoint.y + endPoint.y) / 2;
	int radius = abs(endPoint.x - startPoint.x) / 2;

	// ëˆˆì¹ì˜ ê¸¸ì´ì™€ ë‘ê»˜ë¥¼ ì„¤ì • (ì´ ê°’ì„ ì¡°ì •í•˜ì—¬ ëˆˆì¹ì˜ í¬ê¸°ë¥¼ ë³€ê²½í•  ìˆ˜ ìˆìŠµë‹ˆë‹¤)
	int eyebrowLength = radius * 0.35; // ì˜ˆ: ë°˜ì§€ë¦„ì˜ 30%ë§Œí¼ì˜ ê¸¸ì´
	int eyebrowThickness = radius * 0.03; // ì˜ˆ: ë°˜ì§€ë¦„ì˜ 10%ë§Œí¼ì˜ ë‘ê»˜

	// ì™¼ìª½ ëˆˆì¹ì˜ ì‹œì‘ì ê³¼ ëì  ê³„ì‚°
	POINT leftEyebrowStart = { centerX - radius * 0.5, centerY - radius * 0.3 };
	POINT leftEyebrowEnd = { leftEyebrowStart.x + eyebrowLength, leftEyebrowStart.y - eyebrowThickness };

	// ì˜¤ë¥¸ìª½ ëˆˆì¹ì˜ ì‹œì‘ì ê³¼ ëì  ê³„ì‚°
	POINT rightEyebrowStart = { centerX + radius * 0.5, centerY - radius * 0.3 };
	POINT rightEyebrowEnd = { rightEyebrowStart.x - eyebrowLength, rightEyebrowStart.y - eyebrowThickness };

	// ëˆˆì¹ ê·¸ë¦¬ê¸°
	MoveToEx(hdc, leftEyebrowStart.x, leftEyebrowStart.y, NULL);
	LineTo(hdc, leftEyebrowEnd.x, leftEyebrowEnd.y);

	MoveToEx(hdc, rightEyebrowStart.x, rightEyebrowStart.y, NULL);
	LineTo(hdc, rightEyebrowEnd.x, rightEyebrowEnd.y);

	MoveToEx(hdc, leftEyebrowStart.x, leftEyebrowStart.y+3, NULL);
	LineTo(hdc, leftEyebrowEnd.x, leftEyebrowEnd.y+2);

	MoveToEx(hdc, rightEyebrowStart.x, rightEyebrowStart.y+3, NULL);
	LineTo(hdc, rightEyebrowEnd.x, rightEyebrowEnd.y+2);
}

void DrawBox(HDC hdc, POINT startPointBox, POINT endPointBox) {

	// ÀÛÀº ÁÂÇ¥¸¦ startPoint·Î ¼öÁ¤
	POINT startPoint = { min(startPointBox.x, endPointBox.x), min(startPointBox.y, endPointBox.y) };
	POINT endPoint = { max(startPointBox.x, endPointBox.x), max(startPointBox.y, endPointBox.y) };

	// µå·¡±× ¹æÇâÀ» ±âÁØÀ¸·Î ÆòÇà Åõ¿µ¿¡ ´ëÇÑ ¿ÀÇÁ¼Â °è»ê -> ½ÃÀÛÆ÷ÀÎÆ®º¸´Ù Á¾·áÆ÷ÀÎÆ®°¡ ´õ ÀÛÀº °æ¿ìµµ ÀÖÀ¸´Ï±î
	const int offset_x = (startPoint.x <= endPoint.x) ? 30 : -30;
	const int offset_y = (startPoint.y <= endPoint.y) ? -30 : 30;

	POINT cubePoints[8] = {
		{startPoint.x, startPoint.y},
		{endPoint.x, startPoint.y},
		{endPoint.x, endPoint.y},
		{startPoint.x, endPoint.y},
		{startPoint.x + offset_x, startPoint.y + offset_y},
		{endPoint.x + offset_x, startPoint.y + offset_y},
		{endPoint.x + offset_x, endPoint.y + offset_y},
		{startPoint.x + offset_x,endPoint.y + offset_y}
	};

	// Define a brush with a color
	HBRUSH hBrush = CreateSolidBrush(RGB(198, 219, 218)); //  cobalt color
	SelectObject(hdc, hBrush);

	// Fill the front face
	POINT frontFace[4] = { cubePoints[0], cubePoints[1], cubePoints[2], cubePoints[3] };
	Polygon(hdc, frontFace, 4);
	// Fill the top face
	POINT topFace[4] = { cubePoints[0], cubePoints[1], cubePoints[5], cubePoints[4] };
	Polygon(hdc, topFace, 4);
	// Fill the right face
	POINT rightFace[4] = { cubePoints[1], cubePoints[2], cubePoints[6], cubePoints[5] };
	Polygon(hdc, rightFace, 4);

	// You can add other faces if needed

	// Draw lines between the points to form the cube
	HPEN hPen = CreatePen(PS_SOLID, 2, RGB(0, 0, 0)); // Black pen
	SelectObject(hdc, hPen);



	DeleteObject(hPen);
	DeleteObject(hBrush); // Don't forget to delete the brush
}

void DrawCube(HDC hdc, POINT startPointCube, POINT endPointCube) {

	// ÀÛÀº ÁÂÇ¥¸¦ startPoint·Î ¼öÁ¤
	POINT startPoint = { min(startPointCube.x, endPointCube.x), min(startPointCube.y, endPointCube.y) };
	POINT endPoint = { max(startPointCube.x, endPointCube.x), max(startPointCube.y, endPointCube.y) };

	// µå·¡±× ¹æÇâÀ» ±âÁØÀ¸·Î ÆòÇà Åõ¿µ¿¡ ´ëÇÑ ¿ÀÇÁ¼Â °è»ê -> ½ÃÀÛÆ÷ÀÎÆ®º¸´Ù Á¾·áÆ÷ÀÎÆ®°¡ ´õ ÀÛÀº °æ¿ìµµ ÀÖÀ¸´Ï±î
	const int offset_x = (startPoint.x <= endPoint.x) ? 30 : -30;
	const int offset_y = (startPoint.y <= endPoint.y) ? -30 : 30;

	POINT cubePoints[8] = {
		{startPoint.x, startPoint.y},
		{endPoint.x, startPoint.y},
		{endPoint.x, endPoint.y},
		{startPoint.x, endPoint.y},
		{startPoint.x + offset_x, startPoint.y + offset_y},
		{endPoint.x + offset_x, startPoint.y + offset_y},
		{endPoint.x + offset_x, endPoint.y + offset_y},
		{startPoint.x + offset_x,endPoint.y + offset_y}
	};

	// Define a brush with a color
	HBRUSH hBrush = CreateSolidBrush(RGB(200, 200, 200)); // Light gray color
	SelectObject(hdc, hBrush);

	// Fill the front face
	POINT frontFace[4] = { cubePoints[0], cubePoints[1], cubePoints[2], cubePoints[3] };
	Polygon(hdc, frontFace, 4);
	// Fill the top face
	POINT topFace[4] = { cubePoints[0], cubePoints[1], cubePoints[5], cubePoints[4] };
	Polygon(hdc, topFace, 4);
	// Fill the right face
	POINT rightFace[4] = { cubePoints[1], cubePoints[2], cubePoints[6], cubePoints[5] };
	Polygon(hdc, rightFace, 4);

	// You can add other faces if needed

	// Draw lines between the points to form the cube
	HPEN hPen = CreatePen(PS_SOLID, 2, RGB(0, 0, 0)); // Black pen
	SelectObject(hdc, hPen);

	for (int i = 0; i < 4; ++i) {
		MoveToEx(hdc, cubePoints[i].x, cubePoints[i].y, NULL);
		LineTo(hdc, cubePoints[(i + 1) % 4].x, cubePoints[(i + 1) % 4].y);
		LineTo(hdc, cubePoints[(i + 1) % 4 + 4].x, cubePoints[(i + 1) % 4 + 4].y);
		LineTo(hdc, cubePoints[i + 4].x, cubePoints[i + 4].y);
		LineTo(hdc, cubePoints[i].x, cubePoints[i].y);
	}

	DeleteObject(hPen);
	DeleteObject(hBrush); // Don't forget to delete the brush
}

void DrawShape(HWND hwnd, HDC hdc) {
	if (isDrawingBox && (isMouseLButtonPressed || isMouseRButtonPressed)) {
		DrawBox(hdc, startPointBox, endPointBox);     // ¹Ú½º »ı¼º(ÁÂÅ¬¸¯, ¿ìÅ¬¸¯ ¸ğµÎ »ç¿ëÁß)
	}
	if (isDrawingCube && (isMouseLButtonPressed || isMouseRButtonPressed)) {
		DrawCube(hdc, startPointCube, endPointCube);  // Å¥ºê »ı¼º
	}
}

// ìœˆë„ìš°ì˜ ì´ë²¤íŠ¸ë¥¼ ì²˜ë¦¬í•˜ëŠ” ì½œë°±(Callback) í•¨ìˆ˜.
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	SetTimer(hwnd, 1, 300, NULL);

	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(hwnd, &ps);

	switch (uMsg)         // window message ì¢…ë¥˜ë³„ ìˆ˜í–‰
	{


	case WM_COMMAND:
		if (LOWORD(wParam) == 1) {
			isDrawingBox = true;
			isDrawingCircle = false;
			isDrawingBonobono = false;
			isDrawingLion = false;
			isDrawingCube = false;
			InvalidateRect(hwnd, NULL, TRUE);
		}
		else if (LOWORD(wParam) == 2) {
			isDrawingBox = false;
			isDrawingCircle = true;
			isDrawingBonobono = false;
			isDrawingLion = false;
			InvalidateRect(hwnd, NULL, TRUE);
		}
		else if (LOWORD(wParam) == 3) {
			isDrawingBox = false;
			isDrawingCircle = false;
			isDrawingBonobono = true;
			isDrawingLion = false;
			InvalidateRect(hwnd, NULL, TRUE);
		}
		else if (LOWORD(wParam) == 4) {
			isDrawingBox = false;
			isDrawingCircle = false;
			isDrawingBonobono = false;
			isDrawingLion = true;
			startPointLion = { 0, 0 };
			endPointLion = { 0, 0 };
			InvalidateRect(hwnd, NULL, TRUE);
		}
		else if (LOWORD(wParam) == 5) {
			isDrawingBox = false;
			isDrawingCircle = false;
			isDrawingBonobono = false;
			isDrawingLion = false;
			isDrawingCube = true;
			CUBE_DRAW_MODE = true;     // ¹öÆ°À» Å¬¸¯ÇßÀ»¶§´Â È­¸éÀÌ ÃÊ±âÈ­ »óÅÂÀÌ¹Ç·Î DRAW¸ğµå
			CUBE_RESIZE_MODE = false;
			InvalidateRect(hwnd, NULL, TRUE);
		}
		break;

	case WM_TIMER:   // ë³´ë…¸ë³´ë…¸ ëˆˆ ë™ì  ë³€í™”
		if (GetAsyncKeyState(VK_SPACE) & 0x8000) {
			// ìŠ¤í˜ì´ìŠ¤ë°”ê°€ ëˆŒë ¤ì ¸ ìˆì„ ë•Œ
			BonobonoSwitch = true;
			InvalidateRect(hwnd, NULL, TRUE);
		}
		else {
			// ìŠ¤í˜ì´ìŠ¤ë°”ê°€ ëˆŒë ¤ì ¸ ìˆì§€ ì•Šì„ ë•Œ
			BonobonoSwitch = false;
			InvalidateRect(hwnd, NULL, TRUE);
		}
		break;


	//case WM_KEYDOWN:     íê¸°! BeginPaintì—ì„œ keydownì„ ë°›ì•„ì˜¤ê¸° ì–´ë ¤ì›€
	//	// OutputDebugString(L"WM_KEYDOWN received\n");
	//	if (wParam == VK_SPACE) {
	//		BonobonoSwitch = true;
	//		InvalidateRect(hwnd, NULL, TRUE); // í™”ë©´ì„ ë‹¤ì‹œ ê·¸ë¦¬ë„ë¡ ìš”ì²­
	//	}
	//	break;

	//case WM_KEYUP:
	//	if (wParam == VK_SPACE) {
	//		BonobonoSwitch = false;
	//		InvalidateRect(hwnd, NULL, TRUE); // í™”ë©´ì„ ë‹¤ì‹œ ê·¸ë¦¬ë„ë¡ ìš”ì²­
	//	}
	//	break;

	case WM_LBUTTONDOWN:  // ë§ˆìš°ìŠ¤ ì™¼ìª½ í´ë¦­ì‹œ
	{
<<<<<<< HEAD
		///** »ç°¢Çü ±×¸®±â
		if (isMouseOnDrawingBox && isDrawingBox) {  // Box
=======
		///** ì‚¬ê°í˜• ê·¸ë¦¬ê¸°
		if (isMouseOnDrawingBox && isDrawingBox) {  // Cube
>>>>>>> 261dcef258f61636d46f713d9a2618932561a998
			isMouseLButtonPressed = 1;

			startPointBox.x = LOWORD(lParam);
			startPointBox.y = HIWORD(lParam);
			endPointBox.x = startPointBox.x;
			endPointBox.y = startPointBox.y;


		}
		///** ì› ê·¸ë¦¬ê¸°
		if (isMouseOnDrawingBox && isDrawingCircle) {
			startPointCircle = { 0 };
			endPointCircle = { 0 };

			startPointCircle.x = LOWORD(lParam);
			startPointCircle.y = HIWORD(lParam);
			endPointCircle.x = startPointCircle.x;   // ëì  ì´ˆê¸°í™”(ì—†ì„ ì‹œ ë‹¤ì‹œ ê·¸ë¦´ ë•Œ ê¸°ì¡´ ëì ê°’ì´ ì‚¬ìš©ë˜ëŠ” ì˜¤ë¥˜)
			endPointCircle.y = startPointCircle.y;

			isMouseLButtonPressed = 1;
		//	InvalidateRect(hwnd, NULL, TRUE);
		}

		///** ¶óÀÌ¾ğ ±×¸®±â
		if (isMouseOnDrawingBox && isDrawingLion) {  // Lion 
			startPointLion.x = LOWORD(lParam);
			startPointLion.y = HIWORD(lParam);
			isMouseLButtonPressed = 1;
		}

		///** Å¥ºê ±×¸®±â
		if (isMouseOnDrawingBox && isDrawingCube) {  // Box
			isMouseLButtonPressed = 1;

			ccx = LOWORD(lParam);
			ccy = HIWORD(lParam);
			if (InBox(ccx, ccy, startPointCube, endPointCube)) {   // Cube¾È¿¡ Ä¿¼­°¡ ÀÖ´ÂÁö °Ë»ç
				CUBE_DRAW_MODE = false;
				CUBE_RESIZE_MODE = true;
			}
			else {
				CUBE_DRAW_MODE = true;
				CUBE_RESIZE_MODE = false;
				InvalidateRect(hwnd, NULL, TRUE);

				startPointCube.x = LOWORD(lParam);
				startPointCube.y = HIWORD(lParam);
				endPointCube.x = startPointCube.x;
				endPointCube.y = startPointCube.y;
			}

		}

	}
	break;

	case WM_RBUTTONDOWN: // ë§ˆìš°ìŠ¤ ìš°í´ë¦­ ì‹œì‘
	{
		if (isMouseOnDrawingBox && isDrawingBox) {

			bbx = bx = LOWORD(lParam);
			bby = by = HIWORD(lParam);

			bx = bbx;
			by = bby;

<<<<<<< HEAD
			if (InBox(bbx, bby, startPointBox, endPointBox)) {
				// Box ¿µ¿ª³»¿¡ ÀÖ´Ù¸é ´­¸²»óÅÂ·Î
=======
			isMouseRButtonPressed = 1;

			if (InRect(mmx, mmy, rect)) {
				//ì‚¬ê°í˜•ì˜ ì˜ì—­ë‚´ì— ìˆë‹¤ë©´
>>>>>>> 261dcef258f61636d46f713d9a2618932561a998
				isMouseRButtonPressed = 1;
			}
			// InvalidateRect(hwnd, NULL, TRUE);
		}

		if (isMouseOnDrawingBox && isDrawingCircle) {
			// Circle ìš°í´ë¦­ 
			mmx = mmx = 0;
			mmx = mx = LOWORD(lParam);
			mmy = my = HIWORD(lParam);

			if (InCircle(mmx, mmy, startPointCircle, radius)) { // ìš°í´ë¦­ì´ ì› ë‚´ë¶€ì—ì„œ ë°œìƒí•œ ê²½ìš°
				isMouseRButtonPressed = 1;
			}
		}

		if (isMouseOnDrawingBox && isDrawingCube) {

			bbx = bx = LOWORD(lParam);
			bby = by = HIWORD(lParam);

			bx = bbx;
			by = bby;

			if (InBox(bbx, bby, startPointCube, endPointCube)) {
				// Cube ¿µ¿ª³»¿¡ ÀÖ´Ù¸é ´­¸²»óÅÂ·Î
				isMouseRButtonPressed = 1;
			}
			// InvalidateRect(hwnd, NULL, TRUE);
		}
	}
	break;

	case WM_RBUTTONUP:
	{
		bbx = bby = 0;
		mmx = mmy = 0;
		isMouseRButtonPressed = 0;
		// InvalidateRgn(hwnd, NULL, TRUE);


	}
	break;


	case WM_LBUTTONUP:
	{
		if (isMouseOnDrawingBox && isDrawingLion) {
			endPointLion.x = LOWORD(lParam);
			endPointLion.y = HIWORD(lParam);

			// ë“œë˜ê·¸ê°€ ì™„ë£Œë˜ì—ˆìœ¼ë¯€ë¡œ í™”ë©´ì„ ë‹¤ì‹œ ê·¸ë¦¬ë„ë¡ ìš”ì²­
			// InvalidateRect(hwnd, NULL, TRUE);
		}

		//if (isMouseOnDrawingBox && isDrawingCube) {
		//	endPointCube.x = LOWORD(lParam);
		//	endPointCube.y = HIWORD(lParam);

		//	// ë“œë˜ê·¸ê°€ ì™„ë£Œë˜ì—ˆìœ¼ë¯€ë¡œ í™”ë©´ì„ ë‹¤ì‹œ ê·¸ë¦¬ë„ë¡ ìš”ì²­
		//	// InvalidateRect(hwnd, NULL, TRUE);
		//}
		isMouseLButtonPressed = 0;




	}
	break;

	case WM_MOUSEMOVE:
	{

		if (isMouseRButtonPressed && isMouseOnDrawingBox && isDrawingBox) {  // ìš°í´ë¦­ move ì´ë²¤íŠ¸
			bx = LOWORD(lParam);
			by = HIWORD(lParam);

			int dx = bx - bbx;
			int dy = by - bby;
			bbx = bx;  // ì´ˆê¸°í™”ê°€ ì—†ìœ¼ë©´ ì²« rButtonDownì—ì„œ ì„¤ì •í•œ bbxê°’ì„ ê³„ì† ê°€ì ¸ì˜¤ê²Œ ë¨!
			bby = by;

	
			if (dx != 0 || dy != 0) {


				startPointBox.x += dx;
				startPointBox.y += dy;
				endPointBox.x += dx;
				endPointBox.y += dy;

				InvalidateRect(hwnd, NULL, TRUE); // ì°½ì„ ë‹¤ì‹œ ê·¸ë¦½ë‹ˆë‹¤.

				/*startPointBox.x = bx;
				startPointBox.y = by;*/
			}
		}
		else if (isMouseRButtonPressed && isMouseOnDrawingBox && isDrawingCircle) {
			int currentX = LOWORD(lParam);
			int dx = currentX - mmx; // ë“œë˜ê·¸ë¡œ ì´ë™í•œ ê±°ë¦¬( ì§€ê¸ˆìœ„ì¹˜ - ì²˜ìŒìœ„ì¹˜ )

			// ì›ì˜ í¬ê¸°ë¥¼ ì¡°ì ˆ. 100pxë‹¹ 2ë°°/0.5ë°° ì¦ê°
			if (dx > 0) {
				double factor = 1.0 + dx / 100.0;
				radius *= factor * factor;  // 100px ì´ë™ì‹œ 2ë°° ì¦ê°€
			}
			else if (dx < 0) {
				radius *= (1 - abs(dx) / 200.0); // 100px ì´ë™ì‹œ 0.5ë°° ê°ì†Œ
			}

			// ì›ì˜ í¬ê¸°ê°€ ë„ˆë¬´ ì‘ì•„ì§€ëŠ” ê²ƒì„ ë°©ì§€
			if (radius < 10) radius = 10;

			mmx = currentX; // í˜„ì¬ ìœ„ì¹˜ ì—…ë°ì´íŠ¸
			InvalidateRect(hwnd, NULL, TRUE); // WM_PAINT í˜¸ì¶œ
		}
		else if (isMouseRButtonPressed && isMouseOnDrawingBox && isDrawingCube) { 
			bx = LOWORD(lParam);
			by = HIWORD(lParam);

			int dx = bx - bbx;
			int dy = by - bby;
			bbx = bx;  // ÃÊ±âÈ­°¡ ¾øÀ¸¸é Ã¹ rButtonDown¿¡¼­ ¼³Á¤ÇÑ bbx°ªÀ» °è¼Ó °¡Á®¿À°Ô µÊ!
			bby = by;


			if (dx != 0 || dy != 0) {


				startPointCube.x += dx;
				startPointCube.y += dy;
				endPointCube.x += dx;
				endPointCube.y += dy;

				InvalidateRect(hwnd, NULL, TRUE); // Ã¢À» ´Ù½Ã ±×¸³´Ï´Ù.

				/*startPointCube.x = bx;
				startPointCube.y = by;*/
			}
		}

		else if (isMouseLButtonPressed && isMouseOnDrawingBox && isDrawingBox) {
			endPointBox.x = LOWORD(lParam);
			endPointBox.y = HIWORD(lParam);
			InvalidateRect(hwnd, NULL, TRUE); // WM_PAINT í˜¸ì¶œ
		}
		else if (isMouseLButtonPressed && isMouseOnDrawingBox && isDrawingCircle) {
			endPointCircle.x = LOWORD(lParam);
			endPointCircle.y = HIWORD(lParam);
			InvalidateRect(hwnd, NULL, TRUE); // WM_PAINT í˜¸ì¶œ
		}
		else if (isMouseLButtonPressed && isMouseOnDrawingBox && isDrawingLion) {
			endPointLion.x = LOWORD(lParam);
			endPointLion.y = HIWORD(lParam);
			InvalidateRect(hwnd, NULL, TRUE); // WM_PAINT í˜¸ì¶œ
		}
		else if (isMouseLButtonPressed && isMouseOnDrawingBox && isDrawingCube && CUBE_DRAW_MODE) {
			endPointCube.x = LOWORD(lParam);
			endPointCube.y = HIWORD(lParam);
			InvalidateRect(hwnd, NULL, TRUE); // WM_PAINT È£Ãâ
		}
		else if (isMouseLButtonPressed && isMouseOnDrawingBox && isDrawingCube && CUBE_RESIZE_MODE) {
			POINT startPoint = { min(startPointCube.x, endPointCube.x), min(startPointCube.y, endPointCube.y) };
			POINT endPoint = { max(startPointCube.x, endPointCube.x), max(startPointCube.y, endPointCube.y) };

			// µå·¡±× ¹æÇâÀ» ±âÁØÀ¸·Î ÆòÇà Åõ¿µ¿¡ ´ëÇÑ ¿ÀÇÁ¼Â °è»ê -> ½ÃÀÛÆ÷ÀÎÆ®º¸´Ù Á¾·áÆ÷ÀÎÆ®°¡ ´õ ÀÛÀº °æ¿ìµµ ÀÖÀ¸´Ï±î
			const int offset_x = (startPoint.x <= endPoint.x) ? 30 : -30;
			const int offset_y = (startPoint.y <= endPoint.y) ? -30 : 30;

			cubePoints[0] = { startPoint.x, startPoint.y };
			cubePoints[1] = { endPoint.x, startPoint.y };
			cubePoints[2] = { endPoint.x, endPoint.y };
			cubePoints[3] = { startPoint.x, endPoint.y };
			cubePoints[4] = { startPoint.x + offset_x, startPoint.y + offset_y };
			cubePoints[5] = { endPoint.x + offset_x, startPoint.y + offset_y };
			cubePoints[6] = { endPoint.x + offset_x, endPoint.y + offset_y };
			cubePoints[7] = { startPoint.x + offset_x, endPoint.y + offset_y };

			// Fill the front face
			POINT frontFace[4] = { cubePoints[0], cubePoints[1], cubePoints[2], cubePoints[3] };
			POINT topFace[4] = { cubePoints[0], cubePoints[1], cubePoints[5], cubePoints[4] };
			POINT rightFace[4] = { cubePoints[1], cubePoints[2], cubePoints[6], cubePoints[5] };

			
			int currentX = LOWORD(lParam);
			int currentY = HIWORD(lParam);

			int dx = currentX - startPointCube.x; // xÃà º¯È­·®

			// »óÀÚ Å©±â Á¶Á¤ ·ÎÁ÷
			if (dx != 0) {
				//double scaleFactor = 1 + (dx / 100.0); // ¿¹: µå·¡±×·Î 100px ÀÌµ¿½Ã 2¹è Áõ°¡
				//double scaleFactor = max(0.1, 1.0 + (dx / 100.0));
				for (int i = 0; i < 8; ++i) {
					// Àü¸éÀÇ »óÀÚ Á¡µéÀ» Á¶Á¤


					switch (i) {
					case 0:
						cubePoints[i].x = (int)(cubePoints[i].x - dx);
						cubePoints[i].y = (int)(cubePoints[i].y - dx);
						break;
					case 1:
						cubePoints[i].x = (int)(cubePoints[i].x + dx);
						cubePoints[i].y = (int)(cubePoints[i].y - dx);
						break;
					case 2:
						cubePoints[i].x = (int)(cubePoints[i].x + dx);
						cubePoints[i].y = (int)(cubePoints[i].y + dx);
						break;
					case 3:
						cubePoints[i].x = (int)(cubePoints[i].x - dx);
						cubePoints[i].y = (int)(cubePoints[i].y + dx);
						break;
					case 4:
						cubePoints[i].x = (int)(cubePoints[i].x - dx);
						cubePoints[i].y = (int)(cubePoints[i].y - dx);
						break;
					case 5:
						cubePoints[i].x = (int)(cubePoints[i].x + dx);
						cubePoints[i].y = (int)(cubePoints[i].y - dx);
						break;
					case 6:
						cubePoints[i].x = (int)(cubePoints[i].x + dx);
						cubePoints[i].y = (int)(cubePoints[i].y + dx);
						break;
					case 7:
						cubePoints[i].x = (int)(cubePoints[i].x - dx);
						cubePoints[i].y = (int)(cubePoints[i].y + dx);
						break;
					}
					
		
				

					// ¿·¸éÀÇ »óÀÚ Á¡µé¿¡ ´ëÇÑ Á¶Á¤µµ Ãß°¡
					// ÆòÇà»çº¯ÇüÀÇ ¼öÁ÷ º¯Àº °íÁ¤µÇ°í, ¼öÆò º¯¸¸ scaleFactor¿¡ µû¶ó º¯µ¿µÇ¾î¾ß ÇÔ
				}

				/*
				int currentX = LOWORD(lParam);
				int currentY = HIWORD(lParam);

				int initialWidth = endPointCube.x - startPointCube.x;
				int initialHeight = endPointCube.y - startPointCube.y;

				int dx = currentX - startPointCube.x; // xÃà º¯È­·®
				int dy = currentY - startPointCube.y;

				double scaleX = (initialWidth + dx) / (double)initialWidth;
				double scaleY = (initialHeight + dy) / (double)initialHeight;

				// »óÀÚ Å©±â Á¶Á¤ ·ÎÁ÷
				if (dx != 0) {
					for (int i = 0; i < 8; ++i) {
						int originalX = (i < 4) ? startPointCube.x : (startPointCube.x + offset_x); // Àü¸é°ú ¿·¸é Á¡µéÀÇ ¿ø·¡ x À§Ä¡
						int originalY = (i % 4 == 0 || i % 4 == 3) ? startPointCube.y : (startPointCube.y + offset_y); // Àü¸é°ú ¿·¸é Á¡µéÀÇ ¿ø·¡ y À§Ä¡

						// Á¡ À§Ä¡ Á¶Á¤
						cubePoints[i].x = startPointCube.x + (int)((cubePoints[i].x - originalX) * scaleX);
						cubePoints[i].y = startPointCube.y + (int)((cubePoints[i].y - originalY) * scaleY);
					}
				

					// ¿·¸éÀÇ »óÀÚ Á¡µé¿¡ ´ëÇÑ Á¶Á¤µµ Ãß°¡
					// ÆòÇà»çº¯ÇüÀÇ ¼öÁ÷ º¯Àº °íÁ¤µÇ°í, ¼öÆò º¯¸¸ scaleFactor¿¡ µû¶ó º¯µ¿µÇ¾î¾ß ÇÔ
				}
				*/
			

				//startPointCube.x = currentX; // µå·¡±× ½ÃÀÛ À§Ä¡ ¾÷µ¥ÀÌÆ®
				//startPointCube.y = currentY; 

				InvalidateRect(hwnd, NULL, TRUE); // È­¸é °»½Å
			}
	
	
		}

	
		
		hdc = (HDC)wParam;   // ì „ì²´ gui RECTì˜ í¬ê¸°ë¥¼ ê°€ì ¸ì˜´
		RECT rect;
		GetClientRect(hwnd, &rect);

		RECT drawArea = {
		rect.left + 8,
		rect.top + 8 + 8 + 8 + 64,
		rect.right - 8,
		rect.bottom - 8
		};

		POINT pt;  // drawArea ì²´í¬ìš© ë§ˆìš°ìŠ¤ ì¢Œí‘œ
		pt.x = LOWORD(lParam);
		pt.y = HIWORD(lParam);

		if (PtInRect(&drawArea, pt))
		{
			// ë§ˆìš°ìŠ¤ì˜ ìœ„ì¹˜ê°€ ì‚¬ê°í˜• ì•ˆì— ìˆìœ¼ë©´ ì»¤ì„œì˜ ëª¨ì–‘ì„ +(CROSS) ëª¨ì–‘ìœ¼ë¡œ ë³€ê²½
			isMouseOnDrawingBox = 1;
			SetCursor(LoadCursor(NULL, IDC_CROSS));
		}
		else
		{
			isMouseOnDrawingBox = 0;
			// ë§ˆìš°ìŠ¤ì˜ ìœ„ì¹˜ê°€ ì‚¬ê°í˜• ì•ˆì— ìˆì§€ ì•Šìœ¼ë©´ ê¸°ë³¸ ì»¤ì„œ ëª¨ì–‘ìœ¼ë¡œ ë³€ê²½
			SetCursor(LoadCursor(NULL, IDC_ARROW));
		}

	}
	break;

	case WM_PAINT:
	{
		// ìœˆë„ìš°ì˜ ê·¸ë˜í”½ ì»¨í…ìŠ¤íŠ¸ë¥¼ ì–»ê³ , ê·¸ë¦¼ì„ ê·¸ë¦´ ìˆ˜ ìˆê²Œ í•¨
		// í™”ë©´ì´ ìƒˆë¡œê³ ì¹¨ ë  ë•Œë§ˆë‹¤ ì§€ìš°ê°œì§ˆ
		HBRUSH hBrush_eraser = CreateSolidBrush(RGB(255, 255, 255)); // ì§€ìš°ê°œ

		RECT rect_eraser;
		GetClientRect(hwnd, &rect_eraser);
		FillRect(hdc, &rect, hBrush_eraser);




		if (isMouseLButtonPressed && isMouseOnDrawingBox && isDrawingBox)
		{
			DrawShape(hwnd, hdc);
		}

		else if (isMouseLButtonPressed && isMouseOnDrawingBox && isDrawingCircle)
		{
			/* ì›ì„ ì±„ìš°ë ¤ë©´, Ellipse() í•¨ìˆ˜ë¥¼ ì‚¬ìš©í•  ë•Œ ì±„ìš°ê¸°ë¥¼ ì›í•˜ëŠ” ë¸ŒëŸ¬ì‹œë¥¼ ë””ë°”ì´ìŠ¤ ì»¨í…ìŠ¤íŠ¸ (DC)ì— ì„ íƒ */
			HBRUSH hBrushCircle = CreateSolidBrush(RGB(255, 216, 190)); // í•‘í¬ ë¸ŒëŸ¬ì‹œ ìƒì„±
			HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, hBrushCircle);

			HPEN hTransparentPen = CreatePen(PS_NULL, 0, RGB(0, 0, 0)); // íˆ¬ëª…í•œ íœ ìƒì„±
			HPEN hOldPen = (HPEN)SelectObject(hdc, hTransparentPen); // íˆ¬ëª…í•œ íœì„ ë””ë°”ì´ìŠ¤ ì»¨í…ìŠ¤íŠ¸ì— ì„ íƒ


			radius = sqrt(pow(endPointCircle.x - startPointCircle.x, 2)
				+ pow(endPointCircle.y - startPointCircle.y, 2));
			Ellipse(hdc, startPointCircle.x - radius, startPointCircle.y - radius,
				startPointCircle.x + radius, startPointCircle.y + radius);

			SelectObject(hdc, hOldPen);
			SelectObject(hdc, hOldBrush);
			DeleteObject(hTransparentPen);
			DeleteObject(hBrushCircle);
			EndPaint(hwnd, &ps);
			break;

		}

<<<<<<< HEAD
		else if (isMouseLButtonPressed && isMouseOnDrawingBox && isDrawingCube && CUBE_DRAW_MODE)
		{
=======

		else if (isMouseRButtonPressed && isMouseOnDrawingBox && isDrawingBox) {

			// í˜„ì¬ ìƒíƒœì— ë”°ë¼ ë„í˜•ì„ ê·¸ë¦´ ìˆ˜ ìˆìŠµë‹ˆë‹¤.
>>>>>>> 261dcef258f61636d46f713d9a2618932561a998
			DrawShape(hwnd, hdc);
		}

		else if ((isMouseLButtonPressed || isMouseRButtonPressed) && isMouseOnDrawingBox && isDrawingCube && CUBE_RESIZE_MODE) {
			
			// ÀÛÀº ÁÂÇ¥¸¦ startPoint·Î ¼öÁ¤
			POINT startPoint = { min(startPointCube.x, endPointCube.x), min(startPointCube.y, endPointCube.y) };
			POINT endPoint = { max(startPointCube.x, endPointCube.x), max(startPointCube.y, endPointCube.y) };

			// µå·¡±× ¹æÇâÀ» ±âÁØÀ¸·Î ÆòÇà Åõ¿µ¿¡ ´ëÇÑ ¿ÀÇÁ¼Â °è»ê -> ½ÃÀÛÆ÷ÀÎÆ®º¸´Ù Á¾·áÆ÷ÀÎÆ®°¡ ´õ ÀÛÀº °æ¿ìµµ ÀÖÀ¸´Ï±î
			const int offset_x = (startPoint.x <= endPoint.x) ? 30 : -30;
			const int offset_y = (startPoint.y <= endPoint.y) ? -30 : 30;

			// Define a brush with a color
			HBRUSH hBrush = CreateSolidBrush(RGB(200, 200, 200)); // Light gray color
			SelectObject(hdc, hBrush);

			// Fill the front face
			POINT frontFace[4] = { cubePoints[0], cubePoints[1], cubePoints[2], cubePoints[3] };
			Polygon(hdc, frontFace, 4);
			// Fill the top face
			POINT topFace[4] = { cubePoints[0], cubePoints[1], cubePoints[5], cubePoints[4] };
			Polygon(hdc, topFace, 4);
			// Fill the right face
			POINT rightFace[4] = { cubePoints[1], cubePoints[2], cubePoints[6], cubePoints[5] };
			Polygon(hdc, rightFace, 4);

			// You can add other faces if needed

			// Draw lines between the points to form the cube
			HPEN hPen = CreatePen(PS_SOLID, 2, RGB(0, 0, 0)); // Black pen
			SelectObject(hdc, hPen);

			for (int i = 0; i < 4; ++i) {
				MoveToEx(hdc, cubePoints[i].x, cubePoints[i].y, NULL);
				LineTo(hdc, cubePoints[(i + 1) % 4].x, cubePoints[(i + 1) % 4].y);
				LineTo(hdc, cubePoints[(i + 1) % 4 + 4].x, cubePoints[(i + 1) % 4 + 4].y);
				LineTo(hdc, cubePoints[i + 4].x, cubePoints[i + 4].y);
				LineTo(hdc, cubePoints[i].x, cubePoints[i].y);
			}

			DeleteObject(hPen);
			DeleteObject(hBrush); // Don't forget to delete the brush
		}

		//else if (isMouseRButtonPressed && isMouseOnDrawingBox && isDrawingBox) {

		//	// ÇöÀç »óÅÂ¿¡ µû¶ó µµÇüÀ» ±×¸± ¼ö ÀÖ½À´Ï´Ù.
		//	DrawShape(hwnd, hdc);
		//}
		
		else if (isMouseRButtonPressed && isMouseOnDrawingBox && isDrawingCircle) {
			HBRUSH hBrushCircle = CreateSolidBrush(RGB(255, 216, 190));
			HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, hBrushCircle);
			HPEN hTransparentPen = CreatePen(PS_NULL, 0, RGB(0, 0, 0)); // íˆ¬ëª…í•œ íœ ìƒì„±
			HPEN hOldPen = (HPEN)SelectObject(hdc, hTransparentPen); // íˆ¬ëª…í•œ íœì„ ë””ë°”ì´ìŠ¤ ì»¨í…ìŠ¤íŠ¸ì— ì„ íƒ

			SelectObject(hdc, hBrushCircle);

			Ellipse(hdc, startPointCircle.x - radius, startPointCircle.y - radius,
				startPointCircle.x + radius, startPointCircle.y + radius);

			SelectObject(hdc, hOldPen);
			SelectObject(hdc, hOldBrush);
			DeleteObject(hTransparentPen);
			DeleteObject(hBrushCircle);
		}

		// ë³´ë…¸ë³´ë…¸
		else if (isDrawingBonobono) {  
			HBRUSH Bono1 = CreateSolidBrush(RGB(127, 200, 255));
			HBRUSH OldBrush1 = (HBRUSH)SelectObject(hdc, Bono1);  
			Ellipse(hdc, 280, 115, 520, 355);
			SelectObject(hdc, OldBrush1);
			DeleteObject(Bono1);



			// ì…
			HBRUSH Bono2 = CreateSolidBrush(RGB(255, 150, 255));
			HBRUSH OldBrush2 = (HBRUSH)SelectObject(hdc, Bono2);
			Ellipse(hdc, 381, 222, 411, 292);
			SelectObject(hdc, OldBrush2);
			DeleteObject(Bono2);

			Ellipse(hdc, 358, 226, 396, 260);
			Ellipse(hdc, 398, 228, 437, 262);
			MoveToEx(hdc, 348, 236, NULL);
			LineTo(hdc, 370, 240);
			MoveToEx(hdc, 350, 260, NULL);
			LineTo(hdc, 375, 247);
			MoveToEx(hdc, 450, 228, NULL);
			LineTo(hdc, 425, 240);
			MoveToEx(hdc, 425, 249, NULL);
			LineTo(hdc, 450, 255);

			// ì½”, ì–‘ìª½ ëˆˆ
			HBRUSH Bono3 = CreateSolidBrush(RGB(0, 0, 0));
			HBRUSH OldBrush3 = (HBRUSH)SelectObject(hdc, Bono3);
			Ellipse(hdc, 384, 217, 412, 242);

			if (BonobonoSwitch) {
				MoveToEx(hdc, 322, 213, NULL);
				LineTo(hdc, 338, 229);
				MoveToEx(hdc, 319, 240, NULL);
				LineTo(hdc, 338, 227);

				MoveToEx(hdc, 468, 239, NULL);
				LineTo(hdc, 485, 222);
				MoveToEx(hdc, 468, 239, NULL);
				LineTo(hdc, 485, 252);
			}
			else if (!BonobonoSwitch){
				
				Ellipse(hdc, 311, 216, 323, 235);
				Ellipse(hdc, 480, 216, 492, 235);

				HBRUSH Bono4 = CreateSolidBrush(RGB(255, 255, 255));
				HBRUSH OldBrush4 = (HBRUSH)SelectObject(hdc, Bono4);
				Ellipse(hdc, 314, 222, 320, 228);
				Ellipse(hdc, 483, 222, 489, 228);
				SelectObject(hdc, OldBrush4);
				DeleteObject(Bono4);
			}
			SelectObject(hdc, OldBrush3);
			DeleteObject(Bono3);

		}

		else if (isDrawingLion) {
			OnMouseDrag(hdc, startPointLion, endPointLion);
		}
		else if (isDrawingCube) {
			DrawShape(hwnd, hdc);
		}


		else {   // í™”ë©´ ì´ˆê¸°í™”(í˜„ì¬ëŠ” ë²„íŠ¼í´ë¦­ì‹œ, ë„í˜•ì´ ìš°í´ë¦­ ì´ë™ì‹œ ë“œë¡œì‰ì˜ì—­ì„ ë²—ì–´ë‚ ë•Œ ì‚¬ìš© ì¤‘)
			InvalidateRect(hwnd, NULL, TRUE);
		}


		EndPaint(hwnd, &ps);
	}
	break;


	case WM_DESTROY:
	{


		PostQuitMessage(0);
		FreeConsole();  // GUIë¥¼ ë‹«ìœ¼ë©´ cmdì°½ë„ ê°™ì´ ì¢…ë£Œ
		ExitProcess(0); // ì”ì—¬ í”„ë¡œì„¸ìŠ¤ë„ ì¢…ë£Œí•©ë‹ˆë‹¤.
	}
	break;

	case WM_GETMINMAXINFO:  // í™•ëŒ€ì¶•ì†Œ ë¶ˆê°€
	{
		MINMAXINFO* pMinMaxInfo = (MINMAXINFO*)lParam;
		pMinMaxInfo->ptMinTrackSize.x = 800; // ìµœì†Œ ê°€ë¡œ í¬ê¸°
		pMinMaxInfo->ptMinTrackSize.y = 480; // ìµœì†Œ ì„¸ë¡œ í¬ê¸°
		pMinMaxInfo->ptMaxTrackSize.x = 800; // ìµœëŒ€ ê°€ë¡œ í¬ê¸°
		pMinMaxInfo->ptMaxTrackSize.y = 480; // ìµœëŒ€ ì„¸ë¡œ í¬ê¸°
	}
	break;

	case WM_ERASEBKGND:    // ë°°ê²½ìƒ‰, ë°•ìŠ¤ change
	{
		HDC hdc = (HDC)wParam;
		RECT rect;
		GetClientRect(hwnd, &rect);
		HBRUSH hBrush = CreateSolidBrush(RGB(255, 240, 200)); // í¬ë¦¼ìƒ‰ ë³€ê²½
		FillRect(hdc, &rect, hBrush);
		DeleteObject(hBrush);

		// marginê³¼ paddingì„ ë°˜ì˜í•˜ì—¬ ì‚¬ê°í˜•ì˜ ìœ„ì¹˜ì™€ í¬ê¸° ê³„ì‚°
		RECT boxRect = {
			rect.left + 8,    // left margin
			rect.top + 8,     // top margin
			rect.right - 8,   // right margin
			rect.bottom - 8   // bottom margin
		};

		// ì‚¬ê°í˜• ê·¸ë¦¬ê¸°
		HBRUSH hBrushBox = CreateSolidBrush(RGB(255, 240, 200));  // ë°•ìŠ¤ ìƒ‰ìƒ
		FillRect(hdc, &boxRect, hBrushBox);
		FrameRect(hdc, &boxRect, CreateSolidBrush(RGB(0, 0, 0)));
		DeleteObject(hBrushBox);

		// drawing box ë¶€ë¶„
		RECT drawArea = {
			rect.left + 8,    // left margin
			rect.top + 8 + 8 + 8 + 64,     // top margin
			rect.right - 8,   // right margin
			rect.bottom - 8   // bottom margin
		};
		HBRUSH hBrushDrawArea = CreateSolidBrush(RGB(255, 255, 255));  // ë°•ìŠ¤ ìƒ‰ìƒ
		FillRect(hdc, &drawArea, hBrushDrawArea);
		FrameRect(hdc, &drawArea, CreateSolidBrush(RGB(0, 0, 0)));
		DeleteObject(hBrushDrawArea);

		return 1; // ì²˜ë¦¬ë˜ì—ˆìŒì„ ì•Œë ¤ì¤Œ
	}
	break;

	case WM_DRAWITEM:    // ë²„íŠ¼ ì´ë²¤íŠ¸ ê´€ë ¨
	{
		LPDRAWITEMSTRUCT pDIS = (LPDRAWITEMSTRUCT)lParam;
		if (pDIS->CtlType == ODT_BUTTON && pDIS->CtlID == 1 | 2 | 3 | 4 | 5)
		{
			// ê·¸ë¦¼ì íš¨ê³¼ë¥¼ ê·¸ë¦½ë‹ˆë‹¤.
			HPEN hShadowPen = CreatePen(PS_SOLID, 1, RGB(128, 128, 128));  // íšŒìƒ‰ íœ
			SelectObject(pDIS->hDC, hShadowPen);
			POINT shadowPoints[] =
			{
				{ pDIS->rcItem.right, pDIS->rcItem.top + 2 },
				{ pDIS->rcItem.right, pDIS->rcItem.bottom },
				{ pDIS->rcItem.left + 2, pDIS->rcItem.bottom }
			};
			Polyline(pDIS->hDC, shadowPoints, 3);
			DeleteObject(hShadowPen);

			// ì™¸ê³½ì„ ì„ ê·¸ë¦½ë‹ˆë‹¤.
			HPEN hBorderPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));  // ê²€ì€ìƒ‰ íœ
			SelectObject(pDIS->hDC, hBorderPen);
			Rectangle(pDIS->hDC, pDIS->rcItem.left, pDIS->rcItem.top, pDIS->rcItem.right, pDIS->rcItem.bottom);
			DeleteObject(hBorderPen);

			// ë²„íŠ¼ì˜ í…ìŠ¤íŠ¸ë¥¼ ê·¸ë¦½ë‹ˆë‹¤.
			SIZE size;
			wchar_t text[256];
			GetWindowText(pDIS->hwndItem, text, 256);
			GetTextExtentPoint32(pDIS->hDC, text, wcslen(text), &size);
			SetTextColor(pDIS->hDC, RGB(30, 28, 28));  // í…ìŠ¤íŠ¸ ìƒ‰ìƒ (í•˜ì–€ìƒ‰)
			TextOut(pDIS->hDC,
				(pDIS->rcItem.right - size.cx) / 2,
				(pDIS->rcItem.bottom - size.cy) / 2,
				text,
				wcslen(text));

			return TRUE;  // ë©”ì‹œì§€ ì²˜ë¦¬ ì™„ë£Œ
		}
	}
	break;

	default:
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}



	return S_OK;
}




#ifdef UNICODE
int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nShowCmd)
#else
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR pCmdLine, _In_ int nCmdShow)
#endif
{
	HWND hwnd;
	HWND hwndTwo;
	HWND hButton1;
	HWND hButton2;
	HWND hButton3;
	HWND hButton4;
	HWND hButton5;



	/* ìœˆë„ìš° í´ë˜ìŠ¤ ì„ ì–¸.*/
	WNDCLASS wc;
	ZeroMemory(&wc, sizeof(wc));	// ëª¨ë‘ 0ìœ¼ë¡œ ì´ˆê¸°í™”.

	// ìœˆë„ìš° í´ë˜ìŠ¤ ê°’ ì„¤ì •
	wc.hInstance = hInstance;
	wc.lpszClassName = TEXT("Computer Software");
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WindowProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.lpszClassName = L"ButtonWindowClass";

	// ìœˆë„ìš° í´ë˜ìŠ¤ ë“±ë¡.
	if (RegisterClass(&wc) == 0)
	{
		MessageBox(NULL, L"RegisterClass failed!", L"Error", MB_ICONERROR);
		exit(-1);	//ì˜ˆì™¸
	}

	// Window viewport ì˜ì—­ ì¡°ì •
	RECT rect = { 0, 0, 800, 480 };
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, 0);
	int width = rect.right - rect.left;
	int height = rect.bottom - rect.top;

	// ìœˆë„ìš° ìƒì„±
	hwnd = CreateWindow(
		wc.lpszClassName,
		TEXT("ê³°ëŒì´"),
		WS_OVERLAPPEDWINDOW,
		0, 0,
		width, height,
		NULL, NULL,
		hInstance,
		NULL
	);


	// ì˜¤ë¥˜ ê²€ì‚¬.
	if (!hwnd) {
		return FALSE;
	}
	;
	hButton1 = CreateWindow(
		L"BUTTON", L"Box", WS_TABSTOP | WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_OWNERDRAW,
		16, 16, 137, 64, hwnd, (HMENU)1, hInstance, NULL);

	hButton2 = CreateWindow(
		L"BUTTON", L"Circle", WS_TABSTOP | WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_OWNERDRAW,
		169, 16, 137, 64, hwnd, (HMENU)2, hInstance, NULL);

	hButton3 = CreateWindow(
		L"BUTTON", L"Bonobono", WS_TABSTOP | WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_OWNERDRAW,
		322, 16, 137, 64, hwnd, (HMENU)3, hInstance, NULL);

	hButton4 = CreateWindow(
		L"BUTTON", L"Lion", WS_TABSTOP | WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_OWNERDRAW,
		475, 16, 137, 64, hwnd, (HMENU)4, hInstance, NULL);

	hButton5 = CreateWindow(
		L"BUTTON", L"Cube", WS_TABSTOP | WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_OWNERDRAW,
		628, 16, 137, 64, hwnd, (HMENU)5, hInstance, NULL);


	// ì°½ ë³´ì´ê¸°.
	ShowWindow(hwnd, SW_SHOW); // ì°½ ë„ìš°ê³ 
	UpdateWindow(hwnd); // ì—…ë°ì´íŠ¸í•´ì•¼ ë³´ì„. í•œ ìŒìœ¼ë¡œ ì“´ë‹¤ê³  ë³´ë©´ ë¨.



	// ë©”ì‹œì§€ ì²˜ë¦¬ ë£¨í”„.
	MSG msg;
	ZeroMemory(&msg, sizeof(msg));



	while (msg.message != WM_QUIT)
	{
		// ë©”ì‹œì§€ ì²˜ë¦¬.
		if (GetMessage(&msg, hwnd, 0, 0))
		{

			// ë©”ì‹œì§€ í•´ì„í•´ì¤˜.
			TranslateMessage(&msg);
			// ë©”ì‹œì§€ë¥¼ ì²˜ë¦¬í•´ì•¼í•  ê³³ì— ì „ë‹¬í•´ì¤˜.
			DispatchMessage(&msg);

		}
	}
	UnregisterClass(wc.lpszClassName, wc.hInstance);

	//ì¢…ë£Œ ë©”ì‹œì§€ ë³´ë‚´ê¸°
	return (int)msg.wParam;

}
