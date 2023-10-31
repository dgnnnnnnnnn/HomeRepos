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

POINT startPointBox = { 0 };  
POINT endPointBox = { 0 };

POINT startPointCircle = { 0 };
POINT endPointCircle = { 0 };

POINT startPointLion = { 0 };  // Lion용 Point
POINT endPointLion = { 0 };

POINT startPointCube = { 0 };
POINT endPointCube = { 0 };
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
int radius;  // 원의 반지름

RECT drawArea;

// 기본 크기 및 위치 설정
const int DEFAULT_FACE_WIDTH = 100; // 얼굴 기본 너비
const int DEFAULT_FACE_HEIGHT = 100; // 얼굴 기본 높이

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





bool InRect(INT mmx, INT mmy, RECT rect1)    // 현재커서위치, 지정할 사각형 영역
{
	if ((mmx > rect1.left && mmx < rect1.right) &&
		(mmy > rect1.top && mmy < rect1.bottom))
	{
		return 1; // 도형과 겹침
	}
	return 0; // 두 RECT가 겹치지 않음
}

BOOL InCircle(int x, int y, POINT center, int radius) {
	int dx = x - center.x;
	int dy = y - center.y;
	return (dx * dx + dy * dy) <= (radius * radius);
}


bool InDrawArea(INT x, INT y, RECT rect2)    // 현재커서위치, 지정할 사각형 영역
{
	if ((x > rect2.left && x < rect2.right) &&
		(y > rect2.top && y < rect2.bottom))
	{
		return 1; // 도형과 겹침
	}
	return 0; // 두 RECT가 겹치지 않음
}


void OnMouseDrag(HDC hdc, POINT startPoint, POINT endPoint)
{
	
	// 스케일링 비율 계산
	float scaleX = (float)(endPoint.x - startPoint.x) / DEFAULT_FACE_WIDTH;
	float scaleY = (float)(endPoint.y - startPoint.y) / DEFAULT_FACE_HEIGHT;

	// 조정된 크기 및 위치로 그리기 : 얼굴, 귀 2개
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

	// 눈 2개
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

	// 코 2개
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

	// 눈썹의 길이와 두께를 설정 (이 값을 조정하여 눈썹의 크기를 변경할 수 있습니다)
	int eyebrowLength = radius * 0.35; // 예: 반지름의 30%만큼의 길이
	int eyebrowThickness = radius * 0.03; // 예: 반지름의 10%만큼의 두께

	// 왼쪽 눈썹의 시작점과 끝점 계산
	POINT leftEyebrowStart = { centerX - radius * 0.5, centerY - radius * 0.3 };
	POINT leftEyebrowEnd = { leftEyebrowStart.x + eyebrowLength, leftEyebrowStart.y - eyebrowThickness };

	// 오른쪽 눈썹의 시작점과 끝점 계산
	POINT rightEyebrowStart = { centerX + radius * 0.5, centerY - radius * 0.3 };
	POINT rightEyebrowEnd = { rightEyebrowStart.x - eyebrowLength, rightEyebrowStart.y - eyebrowThickness };

	// 눈썹 그리기
	MoveToEx(hdc, leftEyebrowStart.x, leftEyebrowStart.y, NULL);
	LineTo(hdc, leftEyebrowEnd.x, leftEyebrowEnd.y);

	MoveToEx(hdc, rightEyebrowStart.x, rightEyebrowStart.y, NULL);
	LineTo(hdc, rightEyebrowEnd.x, rightEyebrowEnd.y);

	MoveToEx(hdc, leftEyebrowStart.x, leftEyebrowStart.y+3, NULL);
	LineTo(hdc, leftEyebrowEnd.x, leftEyebrowEnd.y+2);

	MoveToEx(hdc, rightEyebrowStart.x, rightEyebrowStart.y+3, NULL);
	LineTo(hdc, rightEyebrowEnd.x, rightEyebrowEnd.y+2);
}


void DrawBox(HDC hdc, POINT startPoint, POINT endPoint) {
	// Define the offset for the parallel projection
	const int offset_x = 30;
	const int offset_y = 30;

	// Define the points of the cube
	POINT cubePoints[8] = {
		{startPoint.x, startPoint.y},
		{endPoint.x, startPoint.y},
		{endPoint.x, endPoint.y},
		{startPoint.x, endPoint.y},
		{startPoint.x + offset_x, startPoint.y - offset_y},
		{endPoint.x + offset_x, startPoint.y - offset_y},
		{endPoint.x + offset_x, endPoint.y - offset_y},
		{startPoint.x + offset_x,endPoint.y - offset_y}
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
		DrawBox(hdc, startPointBox, endPointBox);
	}
}

// 윈도우의 이벤트를 처리하는 콜백(Callback) 함수.
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	SetTimer(hwnd, 1, 100, NULL);

	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(hwnd, &ps);

	switch (uMsg)         // window message 종류별 수행
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
			InvalidateRect(hwnd, NULL, TRUE);
		}
		break;

	case WM_TIMER:   // 보노보노 눈 동적 변화
		if (GetAsyncKeyState(VK_SPACE) & 0x8000) {
			// 스페이스바가 눌려져 있을 때
			BonobonoSwitch = true;
			InvalidateRect(hwnd, NULL, TRUE);
		}
		else {
			// 스페이스바가 눌려져 있지 않을 때
			BonobonoSwitch = false;
			InvalidateRect(hwnd, NULL, TRUE);
		}
		break;


	//case WM_KEYDOWN:     폐기! BeginPaint에서 keydown을 받아오기 어려움
	//	// OutputDebugString(L"WM_KEYDOWN received\n");
	//	if (wParam == VK_SPACE) {
	//		BonobonoSwitch = true;
	//		InvalidateRect(hwnd, NULL, TRUE); // 화면을 다시 그리도록 요청
	//	}
	//	break;

	//case WM_KEYUP:
	//	if (wParam == VK_SPACE) {
	//		BonobonoSwitch = false;
	//		InvalidateRect(hwnd, NULL, TRUE); // 화면을 다시 그리도록 요청
	//	}
	//	break;

	case WM_LBUTTONDOWN:  // 마우스 왼쪽 클릭시
	{
		///** 사각형 그리기
		if (isMouseOnDrawingBox && isDrawingBox) {  // Cube
			isMouseLButtonPressed = 1;

			startPointBox.x = LOWORD(lParam);
			startPointBox.y = HIWORD(lParam);
			endPointBox.x = startPointBox.x;
			endPointBox.y = startPointBox.y;


		}
		///** 원 그리기
		if (isMouseOnDrawingBox && isDrawingCircle) {
			startPointCircle = { 0 };
			endPointCircle = { 0 };

			startPointCircle.x = LOWORD(lParam);
			startPointCircle.y = HIWORD(lParam);
			endPointCircle.x = startPointCircle.x;   // 끝점 초기화(없을 시 다시 그릴 때 기존 끝점값이 사용되는 오류)
			endPointCircle.y = startPointCircle.y;

			isMouseLButtonPressed = 1;
			InvalidateRect(hwnd, NULL, TRUE);
		}

		if (isMouseOnDrawingBox && isDrawingLion) {  // Lion 
			startPointLion.x = LOWORD(lParam);
			startPointLion.y = HIWORD(lParam);
			isMouseLButtonPressed = 1;
		}

	}
	break;

	case WM_RBUTTONDOWN: // 마우스 우클릭 시작
	{
		if (isMouseOnDrawingBox && isDrawingBox) {

			bbx = bx = LOWORD(lParam);
			bby = by = HIWORD(lParam);

			bx = bbx;
			by = bby;

			isMouseRButtonPressed = 1;

			if (InRect(mmx, mmy, rect)) {
				//사각형의 영역내에 있다면
				isMouseRButtonPressed = 1;
			}
			// InvalidateRect(hwnd, NULL, TRUE);
		}

		if (isMouseOnDrawingBox && isDrawingCircle) {
			// Circle 우클릭 
			mmx = mmx = 0;
			mmx = mx = LOWORD(lParam);
			mmy = my = HIWORD(lParam);

			if (InCircle(mmx, mmy, startPointCircle, radius)) { // 우클릭이 원 내부에서 발생한 경우
				isMouseRButtonPressed = 1;
			}
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

			// 드래그가 완료되었으므로 화면을 다시 그리도록 요청
			// InvalidateRect(hwnd, NULL, TRUE);
		}

		//if (isMouseOnDrawingBox && isDrawingCube) {
		//	endPointCube.x = LOWORD(lParam);
		//	endPointCube.y = HIWORD(lParam);

		//	// 드래그가 완료되었으므로 화면을 다시 그리도록 요청
		//	// InvalidateRect(hwnd, NULL, TRUE);
		//}
		isMouseLButtonPressed = 0;




	}
	break;

	case WM_MOUSEMOVE:
	{

		if (isMouseRButtonPressed && isMouseOnDrawingBox && isDrawingBox) {  // 우클릭 move 이벤트
			bx = LOWORD(lParam);
			by = HIWORD(lParam);

			int dx = bx - bbx;
			int dy = by - bby;
			bbx = bx;  // 초기화가 없으면 첫 rButtonDown에서 설정한 bbx값을 계속 가져오게 됨!
			bby = by;

	
			if (dx != 0 || dy != 0) {


				startPointBox.x += dx;
				startPointBox.y += dy;
				endPointBox.x += dx;
				endPointBox.y += dy;

				InvalidateRect(hwnd, NULL, TRUE); // 창을 다시 그립니다.

				/*startPointBox.x = bx;
				startPointBox.y = by;*/
			}
		}
		else if (isMouseRButtonPressed && isMouseOnDrawingBox && isDrawingCircle) {
			int currentX = LOWORD(lParam);
			int dx = currentX - mmx; // 드래그로 이동한 거리( 지금위치 - 처음위치 )

			// 원의 크기를 조절. 100px당 2배/0.5배 증감
			if (dx > 0) {
				double factor = 1.0 + dx / 100.0;
				radius *= factor * factor;  // 100px 이동시 2배 증가
			}
			else if (dx < 0) {
				radius *= (1 - abs(dx) / 200.0); // 100px 이동시 0.5배 감소
			}

			// 원의 크기가 너무 작아지는 것을 방지
			if (radius < 10) radius = 10;

			mmx = currentX; // 현재 위치 업데이트
			InvalidateRect(hwnd, NULL, TRUE); // WM_PAINT 호출
		}

		else if (isMouseLButtonPressed && isMouseOnDrawingBox && isDrawingBox) {
			endPointBox.x = LOWORD(lParam);
			endPointBox.y = HIWORD(lParam);
			InvalidateRect(hwnd, NULL, TRUE); // WM_PAINT 호출
		}
		else if (isMouseLButtonPressed && isMouseOnDrawingBox && isDrawingCircle) {
			endPointCircle.x = LOWORD(lParam);
			endPointCircle.y = HIWORD(lParam);
			InvalidateRect(hwnd, NULL, TRUE); // WM_PAINT 호출
		}
		else if (isMouseLButtonPressed && isMouseOnDrawingBox && isDrawingLion) {
			endPointLion.x = LOWORD(lParam);
			endPointLion.y = HIWORD(lParam);
			InvalidateRect(hwnd, NULL, TRUE); // WM_PAINT 호출
		}


		
		hdc = (HDC)wParam;   // 전체 gui RECT의 크기를 가져옴
		RECT rect;
		GetClientRect(hwnd, &rect);

		RECT drawArea = {
		rect.left + 8,
		rect.top + 8 + 8 + 8 + 64,
		rect.right - 8,
		rect.bottom - 8
		};

		POINT pt;  // drawArea 체크용 마우스 좌표
		pt.x = LOWORD(lParam);
		pt.y = HIWORD(lParam);

		if (PtInRect(&drawArea, pt))
		{
			// 마우스의 위치가 사각형 안에 있으면 커서의 모양을 +(CROSS) 모양으로 변경
			isMouseOnDrawingBox = 1;
			SetCursor(LoadCursor(NULL, IDC_CROSS));
		}
		else
		{
			isMouseOnDrawingBox = 0;
			// 마우스의 위치가 사각형 안에 있지 않으면 기본 커서 모양으로 변경
			SetCursor(LoadCursor(NULL, IDC_ARROW));
		}

	}
	break;

	case WM_PAINT:
	{
		// 윈도우의 그래픽 컨텍스트를 얻고, 그림을 그릴 수 있게 함
		// 화면이 새로고침 될 때마다 지우개질
		HBRUSH hBrush_eraser = CreateSolidBrush(RGB(255, 255, 255)); // 지우개

		RECT rect_eraser;
		GetClientRect(hwnd, &rect_eraser);
		FillRect(hdc, &rect, hBrush_eraser);




		if (isMouseLButtonPressed && isMouseOnDrawingBox && isDrawingBox)
		{
			DrawShape(hwnd, hdc);
		}

		else if (isMouseLButtonPressed && isMouseOnDrawingBox && isDrawingCircle)
		{
			/* 원을 채우려면, Ellipse() 함수를 사용할 때 채우기를 원하는 브러시를 디바이스 컨텍스트 (DC)에 선택 */
			HBRUSH hBrushCircle = CreateSolidBrush(RGB(255, 216, 190)); // 핑크 브러시 생성
			HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, hBrushCircle);

			HPEN hTransparentPen = CreatePen(PS_NULL, 0, RGB(0, 0, 0)); // 투명한 펜 생성
			HPEN hOldPen = (HPEN)SelectObject(hdc, hTransparentPen); // 투명한 펜을 디바이스 컨텍스트에 선택


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


		else if (isMouseRButtonPressed && isMouseOnDrawingBox && isDrawingBox) {

			// 현재 상태에 따라 도형을 그릴 수 있습니다.
			DrawShape(hwnd, hdc);
		}
		
		else if (isMouseRButtonPressed && isMouseOnDrawingBox && isDrawingCircle) {
			HBRUSH hBrushCircle = CreateSolidBrush(RGB(255, 216, 190));
			HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, hBrushCircle);
			HPEN hTransparentPen = CreatePen(PS_NULL, 0, RGB(0, 0, 0)); // 투명한 펜 생성
			HPEN hOldPen = (HPEN)SelectObject(hdc, hTransparentPen); // 투명한 펜을 디바이스 컨텍스트에 선택

			SelectObject(hdc, hBrushCircle);

			Ellipse(hdc, startPointCircle.x - radius, startPointCircle.y - radius,
				startPointCircle.x + radius, startPointCircle.y + radius);

			SelectObject(hdc, hOldPen);
			SelectObject(hdc, hOldBrush);
			DeleteObject(hTransparentPen);
			DeleteObject(hBrushCircle);
		}

		// 보노보노
		else if (isDrawingBonobono) {  
			HBRUSH Bono1 = CreateSolidBrush(RGB(127, 200, 255));
			HBRUSH OldBrush1 = (HBRUSH)SelectObject(hdc, Bono1);  
			Ellipse(hdc, 280, 115, 520, 355);
			SelectObject(hdc, OldBrush1);
			DeleteObject(Bono1);



			// 입
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

			// 코, 양쪽 눈
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
		}


		else {   // 화면 초기화(현재는 버튼클릭시, 도형이 우클릭 이동시 드로잉영역을 벗어날때 사용 중)
			InvalidateRect(hwnd, NULL, TRUE);
		}


		EndPaint(hwnd, &ps);
	}
	break;


	case WM_DESTROY:
	{


		PostQuitMessage(0);
		FreeConsole();  // GUI를 닫으면 cmd창도 같이 종료
		ExitProcess(0); // 잔여 프로세스도 종료합니다.
	}
	break;

	case WM_GETMINMAXINFO:  // 확대축소 불가
	{
		MINMAXINFO* pMinMaxInfo = (MINMAXINFO*)lParam;
		pMinMaxInfo->ptMinTrackSize.x = 800; // 최소 가로 크기
		pMinMaxInfo->ptMinTrackSize.y = 480; // 최소 세로 크기
		pMinMaxInfo->ptMaxTrackSize.x = 800; // 최대 가로 크기
		pMinMaxInfo->ptMaxTrackSize.y = 480; // 최대 세로 크기
	}
	break;

	case WM_ERASEBKGND:    // 배경색, 박스 change
	{
		HDC hdc = (HDC)wParam;
		RECT rect;
		GetClientRect(hwnd, &rect);
		HBRUSH hBrush = CreateSolidBrush(RGB(255, 240, 200)); // 크림색 변경
		FillRect(hdc, &rect, hBrush);
		DeleteObject(hBrush);

		// margin과 padding을 반영하여 사각형의 위치와 크기 계산
		RECT boxRect = {
			rect.left + 8,    // left margin
			rect.top + 8,     // top margin
			rect.right - 8,   // right margin
			rect.bottom - 8   // bottom margin
		};

		// 사각형 그리기
		HBRUSH hBrushBox = CreateSolidBrush(RGB(255, 240, 200));  // 박스 색상
		FillRect(hdc, &boxRect, hBrushBox);
		FrameRect(hdc, &boxRect, CreateSolidBrush(RGB(0, 0, 0)));
		DeleteObject(hBrushBox);

		// drawing box 부분
		RECT drawArea = {
			rect.left + 8,    // left margin
			rect.top + 8 + 8 + 8 + 64,     // top margin
			rect.right - 8,   // right margin
			rect.bottom - 8   // bottom margin
		};
		HBRUSH hBrushDrawArea = CreateSolidBrush(RGB(255, 255, 255));  // 박스 색상
		FillRect(hdc, &drawArea, hBrushDrawArea);
		FrameRect(hdc, &drawArea, CreateSolidBrush(RGB(0, 0, 0)));
		DeleteObject(hBrushDrawArea);

		return 1; // 처리되었음을 알려줌
	}
	break;

	case WM_DRAWITEM:    // 버튼 이벤트 관련
	{
		LPDRAWITEMSTRUCT pDIS = (LPDRAWITEMSTRUCT)lParam;
		if (pDIS->CtlType == ODT_BUTTON && pDIS->CtlID == 1 | 2 | 3 | 4 | 5)
		{
			// 그림자 효과를 그립니다.
			HPEN hShadowPen = CreatePen(PS_SOLID, 1, RGB(128, 128, 128));  // 회색 펜
			SelectObject(pDIS->hDC, hShadowPen);
			POINT shadowPoints[] =
			{
				{ pDIS->rcItem.right, pDIS->rcItem.top + 2 },
				{ pDIS->rcItem.right, pDIS->rcItem.bottom },
				{ pDIS->rcItem.left + 2, pDIS->rcItem.bottom }
			};
			Polyline(pDIS->hDC, shadowPoints, 3);
			DeleteObject(hShadowPen);

			// 외곽선을 그립니다.
			HPEN hBorderPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));  // 검은색 펜
			SelectObject(pDIS->hDC, hBorderPen);
			Rectangle(pDIS->hDC, pDIS->rcItem.left, pDIS->rcItem.top, pDIS->rcItem.right, pDIS->rcItem.bottom);
			DeleteObject(hBorderPen);

			// 버튼의 텍스트를 그립니다.
			SIZE size;
			wchar_t text[256];
			GetWindowText(pDIS->hwndItem, text, 256);
			GetTextExtentPoint32(pDIS->hDC, text, wcslen(text), &size);
			SetTextColor(pDIS->hDC, RGB(30, 28, 28));  // 텍스트 색상 (하얀색)
			TextOut(pDIS->hDC,
				(pDIS->rcItem.right - size.cx) / 2,
				(pDIS->rcItem.bottom - size.cy) / 2,
				text,
				wcslen(text));

			return TRUE;  // 메시지 처리 완료
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



	/* 윈도우 클래스 선언.*/
	WNDCLASS wc;
	ZeroMemory(&wc, sizeof(wc));	// 모두 0으로 초기화.

	// 윈도우 클래스 값 설정
	wc.hInstance = hInstance;
	wc.lpszClassName = TEXT("Computer Software");
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WindowProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.lpszClassName = L"ButtonWindowClass";

	// 윈도우 클래스 등록.
	if (RegisterClass(&wc) == 0)
	{
		MessageBox(NULL, L"RegisterClass failed!", L"Error", MB_ICONERROR);
		exit(-1);	//예외
	}

	// Window viewport 영역 조정
	RECT rect = { 0, 0, 800, 480 };
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, 0);
	int width = rect.right - rect.left;
	int height = rect.bottom - rect.top;

	// 윈도우 생성
	hwnd = CreateWindow(
		wc.lpszClassName,
		TEXT("곰돌이"),
		WS_OVERLAPPEDWINDOW,
		0, 0,
		width, height,
		NULL, NULL,
		hInstance,
		NULL
	);


	// 오류 검사.
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


	// 창 보이기.
	ShowWindow(hwnd, SW_SHOW); // 창 띄우고
	UpdateWindow(hwnd); // 업데이트해야 보임. 한 쌍으로 쓴다고 보면 됨.



	// 메시지 처리 루프.
	MSG msg;
	ZeroMemory(&msg, sizeof(msg));



	while (msg.message != WM_QUIT)
	{
		// 메시지 처리.
		if (GetMessage(&msg, hwnd, 0, 0))
		{

			// 메시지 해석해줘.
			TranslateMessage(&msg);
			// 메시지를 처리해야할 곳에 전달해줘.
			DispatchMessage(&msg);

		}
	}
	UnregisterClass(wc.lpszClassName, wc.hInstance);

	//종료 메시지 보내기
	return (int)msg.wParam;

}
