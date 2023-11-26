#ifdef UNICODE
#pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console")
#else
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
#endif

#include <cmath>
#include <windows.h>
#include "yuhanCG.h"

bool isDrawingBox = false;
bool isDrawingCircle = false;
bool isDrawingBonobono = false;
bool isDrawingLion = false;
bool isDrawingCube = false;
bool wasDrawingLion = false;
int BonobonoSwitch = 0;  // 보노보노 눈

BOOL CUBE_DRAW_MODE = false;
BOOL CUBE_RESIZE_MODE = false;
BOOL CUBE_MOVE_MODE = false;


POINT startPointBox = { 0 };
POINT endPointBox = { 0 };

POINT startPointCircle = { 0 };
POINT endPointCircle = { 0 };

POINT startPointLion = { 0 };  // Lion용 Point
POINT endPointLion = { 0 };

POINT startPointCube = { 0 };
POINT endPointCube = { 0 };
POINT cubeResizePoint = { 0 };
POINT cubePoints[8]; // 전역변수 resize cube용


BOOL isMouseLButtonPressed = 0;
BOOL isMouseRButtonPressed = 0;
int isMouseOnDrawingBox = 0;
RECT rect = { 0 };

const wchar_t* textCrash = L"Crash!!!";
int mx, my;
int emx, emy;
int mmx, mmy;
int bx, by;
int bbx, bby;
int ccx, ccy;
int radius;  // 원의 반지름
int originalCenterX, originalCenterY; // 큐브의 원래 중심 좌표
int originalWidth, originalHeight; // 큐브의 원래 가로, 세로 크기

RECT drawArea;


BOOL InBox(int mmx, int mmy, POINT startPointBox, POINT endPointBox) {

	/* 3차원 Box를 2차원 형태로 구현하면 3개의 도형이 나옴 -> 각각 front,top,rightFace의 포인트를 가짐
	   평행사변형은 2개의 삼각형으로 쪼개 검사해야하지만 생략하고 리전함수 사용 */

	POINT startPoint = { min(startPointBox.x, endPointBox.x), min(startPointBox.y, endPointBox.y) };
	POINT endPoint = { max(startPointBox.x, endPointBox.x), max(startPointBox.y, endPointBox.y) };

	// 드래그 방향을 기준으로 평행 투영에 대한 오프셋 계산 -> 시작포인트보다 종료포인트가 더 작은 경우도 있으니까
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
	HRGN hRgnOne = CreatePolygonRgn(topFace, 4, WINDING);      // 리전(위쪽 평행사변형) 생성
	BOOL bInsideOne = PtInRegion(hRgnOne, mmx, mmy);           // PtInRegion함수) mmx, mmy이 리전에 있는지 검사해 true-false 반환

	POINT rightFace[4] = { cubePoints[1], cubePoints[2], cubePoints[6], cubePoints[5] };
	HRGN hRgnTwo = CreatePolygonRgn(rightFace, 4, WINDING);
	BOOL bInsideTwo = PtInRegion(hRgnTwo, mmx, mmy);


	if ((mmx > startPoint.x && mmx < endPoint.x && mmy > startPoint.y && mmy < endPoint.y)
		|| bInsideOne || bInsideTwo) {
		return true;
	}
	return false;
}

BOOL InCircle(int x, int y, POINT center, int radius) {
	int dx = x - center.x;
	int dy = y - center.y;
	return (dx * dx + dy * dy) <= (radius * radius);
}

BOOL InDrawArea(INT x, INT y, RECT rect2)    // 현재커서위치, 지정할 사각형 영역
{
	if ((x > rect2.left && x < rect2.right) &&
		(y > rect2.top && y < rect2.bottom))
	{
		return 1; // 도형과 겹침
	}
	return 0; // 두 RECT가 겹치지 않음
}


// 윈도우의 이벤트를 처리하는 콜백(Callback) 함수.
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	SetTimer(hwnd, 1, 300, NULL);

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
			isDrawingCube = false;
			InvalidateRect(hwnd, NULL, TRUE);
		}
		else if (LOWORD(wParam) == 3) {
			isDrawingBox = false;
			isDrawingCircle = false;
			isDrawingBonobono = true;
			isDrawingLion = false;
			isDrawingCube = false;
			InvalidateRect(hwnd, NULL, TRUE);
		}
		else if (LOWORD(wParam) == 4) {
			isDrawingBox = false;
			isDrawingCircle = false;
			isDrawingBonobono = false;
			isDrawingLion = true;
			startPointLion = { 0, 0 };
			endPointLion = { 0, 0 };
			BonobonoSwitch = 0;
			isDrawingCube = false;
			InvalidateRect(hwnd, NULL, TRUE);
		}
		else if (LOWORD(wParam) == 5) {
			isDrawingBox = false;
			isDrawingCircle = false;
			isDrawingBonobono = false;
			isDrawingLion = false;
			isDrawingCube = true;
			CUBE_DRAW_MODE = true;     // 버튼을 클릭했을때는 화면이 초기화 상태이므로 DRAW모드
			CUBE_RESIZE_MODE = false;
			InvalidateRect(hwnd, NULL, TRUE);
		}
		break;

	case WM_TIMER:   // 보노보노 눈 동적 변화
		if (GetAsyncKeyState(VK_SPACE) & 0x8000) {
			// 스페이스바가 눌려져 있을 때
			BonobonoSwitch = 1;
			InvalidateRect(hwnd, NULL, TRUE);
		}
		else {
			// 스페이스바가 눌려져 있지 않을 때
			BonobonoSwitch = 0;
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
		if (isMouseOnDrawingBox && isDrawingBox) {  // Box
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
			//	InvalidateRect(hwnd, NULL, TRUE);
		}

		///** 라이언 그리기
		if (isMouseOnDrawingBox && isDrawingLion) {  // Lion 
			startPointLion.x = LOWORD(lParam);
			startPointLion.y = HIWORD(lParam);
			isMouseLButtonPressed = 1;
		}

		///** 큐브 그리기
		if (isMouseOnDrawingBox && isDrawingCube) {  // Cube
			isMouseLButtonPressed = 1;


			ccx = LOWORD(lParam);
			ccy = HIWORD(lParam);
			if (InBox(ccx, ccy, startPointCube, endPointCube)) {   // Cube안에 커서가 있는지 검사
				CUBE_DRAW_MODE = false;
				CUBE_RESIZE_MODE = true;

			}
			else {
				CUBE_DRAW_MODE = true;
				CUBE_RESIZE_MODE = false;
				InvalidateRect(hwnd, NULL, TRUE);

				originalCenterX = (startPointCube.x + endPointCube.x) / 2;
				originalCenterY = (startPointCube.y + endPointCube.y) / 2;
				originalWidth = endPointCube.x - startPointCube.x;
				originalHeight = endPointCube.y - startPointCube.y;

				startPointCube.x = LOWORD(lParam);
				startPointCube.y = HIWORD(lParam);
				endPointCube.x = startPointCube.x;
				endPointCube.y = startPointCube.y;
			}

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

			if (InBox(bbx, bby, startPointBox, endPointBox)) {
				// Box 영역내에 있다면 눌림상태로
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

		if (isMouseOnDrawingBox && isDrawingCube) {

			bbx = bx = LOWORD(lParam);
			bby = by = HIWORD(lParam);

			bx = bbx;
			by = bby;

			if (InBox(bbx, bby, startPointCube, endPointCube)) {
				// Cube 영역내에 있다면 눌림상태로
				isMouseRButtonPressed = 1;
				CUBE_RESIZE_MODE = false;
				CUBE_MOVE_MODE = true;
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
		else if (isMouseRButtonPressed && isMouseOnDrawingBox && isDrawingCube) {
			bx = LOWORD(lParam);
			by = HIWORD(lParam);

			int dx = bx - bbx;
			int dy = by - bby;
			bbx = bx;  // 초기화가 없으면 첫 rButtonDown에서 설정한 bbx값을 계속 가져오게 됨!
			bby = by;


			if (dx != 0 || dy != 0) {


		/*		startPointCube.x += dx;
				startPointCube.y += dy;
				endPointCube.x += dx;
				endPointCube.y += dy;  */

				for (int i = 0; i < 8; i++) {
					cubePoints[i].x += dx;
					cubePoints[i].y += dy;
				}

				InvalidateRect(hwnd, NULL, TRUE); // 창을 다시 그립니다.

				/*startPointCube.x = bx;
				startPointCube.y = by;*/
			}
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
		else if (isMouseLButtonPressed && isMouseOnDrawingBox && isDrawingCube && CUBE_DRAW_MODE) {
			endPointCube.x = LOWORD(lParam);
			endPointCube.y = HIWORD(lParam);
			InvalidateRect(hwnd, NULL, TRUE); // WM_PAINT 호출
		}
		else if (isMouseLButtonPressed && isMouseOnDrawingBox && isDrawingCube && CUBE_RESIZE_MODE) {

			
			POINT startPoint = { min(startPointCube.x, endPointCube.x), min(startPointCube.y, endPointCube.y) };
			POINT endPoint = { max(startPointCube.x, endPointCube.x), max(startPointCube.y, endPointCube.y) };

			// 드래그 방향을 기준으로 평행 투영에 대한 오프셋 계산
			int offset_x, offset_y;

			cubePoints[0] = startPointCube;
			cubePoints[1] = { endPointCube.x, startPointCube.y };
			cubePoints[2] = endPointCube;
			cubePoints[3] = { startPointCube.x, endPointCube.y };

			// 상단면과 측면의 꼭짓점을 드래그 방향에 따라 설정합니다.
			if (startPointCube.x <= endPointCube.x) {
				if (startPointCube.y <= endPointCube.y) {
					// 왼쪽 위에서 오른쪽 아래로 드래그
					offset_x = 30; offset_y = -30;
				}
				else {
					// 왼쪽 아래에서 오른쪽 위로 드래그
					offset_x = 30; offset_y = 30;
				}
			}
			else {
				if (startPointCube.y <= endPointCube.y) {
					// 오른쪽 위에서 왼쪽 아래로 드래그
					offset_x = -30; offset_y = -30;
				}
				else {
					// 오른쪽 아래에서 왼쪽 위로 드래그
					offset_x = -30; offset_y = 30;
				}
			}

			// 오프셋을 적용하여 나머지 꼭짓점을 계산합니다.
			cubePoints[4] = { cubePoints[0].x + offset_x, cubePoints[0].y + offset_y };
			cubePoints[5] = { cubePoints[1].x + offset_x, cubePoints[1].y + offset_y };
			cubePoints[6] = { cubePoints[2].x + offset_x, cubePoints[2].y + offset_y };
			cubePoints[7] = { cubePoints[3].x + offset_x, cubePoints[3].y + offset_y };



			// Fill the front face
			POINT frontFace[4] = { cubePoints[0], cubePoints[1], cubePoints[2], cubePoints[3] };
			POINT topFace[4] = { cubePoints[0], cubePoints[1], cubePoints[5], cubePoints[4] };
			POINT rightFace[4] = { cubePoints[1], cubePoints[2], cubePoints[6], cubePoints[5] };


			int currentX = LOWORD(lParam);
			int currentY = HIWORD(lParam);

			// x축 변화량
			int dx = currentX - ccx;

			// 크기 조절 부분
			int originalWidth = endPoint.x - startPoint.x;
			int originalHeight = endPoint.y - startPoint.y;
			// int newWidth = originalWidth + dx;
			int newWidth = max(1, originalWidth + dx * 2); // 클릭 지점을 중심으로 크기 조절

			// 큐브가 너무 작아지는 걸 방지
			if (newWidth <= 0) {
				dx -= newWidth - 1;
				newWidth = 1;
			}

			// 비율을 맞춰서 조절되도록 
			double scaleFactor = (double)newWidth / (double)originalWidth;

			POINT centerPoint = {
			startPoint.x + (originalWidth / 2),
			startPoint.y + (originalHeight / 2)
			};


			// 상자 크기 조정 로직
			if (dx != 0) {
				//double scaleFactor = 1 + (dx / 100.0); // 예: 드래그로 100px 이동시 2배 증가
				//double scaleFactor = max(0.1, 1.0 + (dx / 100.0));
				// 상자 크기 조정 로직
				for (int i = 0; i < 8; ++i)
				{
					int offsetX = (cubePoints[i].x - centerPoint.x) * scaleFactor;
					int offsetY = (cubePoints[i].y - centerPoint.y) * scaleFactor; // 세로 길이 변경 없음

					cubePoints[i].x = centerPoint.x + offsetX;
					cubePoints[i].y = centerPoint.y + offsetY;
				}

			



				InvalidateRect(hwnd, NULL, TRUE); // 화면 갱신
			}


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
			DrawBox(hwnd, hdc, startPointBox, endPointBox);
		}

		else if (isMouseLButtonPressed && isMouseOnDrawingBox && isDrawingCircle)
		{
			// PAINT 부분에서도 radius값을 설정하므로
			radius = DrawCircle(hwnd, hdc, startPointCircle, endPointCircle, radius, isMouseLButtonPressed);

		}


		// Cube의 DRAW / RESIZE MODE 구분
		else if (isMouseLButtonPressed && isMouseOnDrawingBox && isDrawingCube && CUBE_DRAW_MODE)
		{
			DrawCube(hwnd, hdc, startPointCube, endPointCube, cubePoints, CUBE_RESIZE_MODE);
		}

		else if ((isMouseLButtonPressed || isMouseRButtonPressed) && isMouseOnDrawingBox && isDrawingCube && CUBE_RESIZE_MODE) {

			DrawCube(hwnd, hdc, startPointCube, endPointCube, cubePoints, CUBE_RESIZE_MODE);
		}

		else if (isMouseRButtonPressed && isMouseOnDrawingBox && isDrawingBox) {

			// 현재 상태에 따라 도형을 그릴 수 있습니다.
			DrawBox(hwnd, hdc, startPointBox, endPointBox);
		}

		else if (isMouseRButtonPressed && isMouseOnDrawingBox && isDrawingCircle) {

			// PAINT 부분에서도 radius값을 설정하므로
			radius = DrawCircle(hwnd, hdc, startPointCircle, endPointCircle, radius, isMouseLButtonPressed);
		}

		// 보노보노
		else if (isDrawingBonobono) {
			DrawBonobono(hwnd, hdc, BonobonoSwitch);
		}

		// 라이언
		else if (isDrawingLion) {
			DrawRyan(hwnd, hdc, startPointLion, endPointLion);
		}
		else if (isDrawingCube) {
			DrawCube(hwnd, hdc, startPointCube, endPointCube, cubePoints, CUBE_MOVE_MODE);
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
