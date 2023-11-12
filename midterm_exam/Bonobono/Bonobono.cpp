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

POINT startPointLion = { 0 };  // Lion�� Point
POINT endPointLion = { 0 };

POINT startPointCube = { 0 };
POINT endPointCube = { 0 };
POINT cubeResizePoint = { 0 };
POINT cubePoints[8]; // �������� resize cube��


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
int ccx, ccy;
int radius;  // ���� ������

RECT drawArea;

// �⺻ ũ�� �� ��ġ ����
const int DEFAULT_FACE_WIDTH = 100; // �� �⺻ �ʺ�
const int DEFAULT_FACE_HEIGHT = 100; // �� �⺻ ����

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




//bool InRect(INT mmx, INT mmy, RECT rect1)    // ����Ŀ����ġ, ������ �簢�� ����
//{
//	if ((mmx > rect1.left && mmx < rect1.right) &&
//		(mmy > rect1.top && mmy < rect1.bottom))
//	{
//		return 1; // ������ ��ħ
//	}
//	return 0; // �� RECT�� ��ġ�� ����
//}

bool InBox(int mmx, int mmy, POINT startPointBox, POINT endPointBox) {

	/* 3���� Box�� 2���� ���·� �����ϸ� 3���� ������ ���� -> ���� front,top,rightFace�� ����Ʈ�� ����
	   ����纯���� 2���� �ﰢ������ �ɰ� �˻��ؾ������� �����ϰ� �����Լ� ��� */

	POINT startPoint = { min(startPointBox.x, endPointBox.x), min(startPointBox.y, endPointBox.y) };
	POINT endPoint = { max(startPointBox.x, endPointBox.x), max(startPointBox.y, endPointBox.y) };

	// �巡�� ������ �������� ���� ������ ���� ������ ��� -> ��������Ʈ���� ��������Ʈ�� �� ���� ��쵵 �����ϱ�
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
	HRGN hRgnOne = CreatePolygonRgn(topFace, 4, WINDING);      // ����(���� ����纯��) ����
	BOOL bInsideOne = PtInRegion(hRgnOne, mmx, mmy);           // PtInRegion�Լ�) mmx, mmy�� ������ �ִ��� �˻��� true-false ��ȯ

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

bool InDrawArea(INT x, INT y, RECT rect2)    // ����Ŀ����ġ, ������ �簢�� ����
{
	if ((x > rect2.left && x < rect2.right) &&
		(y > rect2.top && y < rect2.bottom))
	{
		return 1; // ������ ��ħ
	}
	return 0; // �� RECT�� ��ġ�� ����
}

void OnMouseDrag(HDC hdc, POINT startPoint, POINT endPoint)
{

	// �����ϸ� ���� ���
	float scaleX = (float)(endPoint.x - startPoint.x) / DEFAULT_FACE_WIDTH;
	float scaleY = (float)(endPoint.y - startPoint.y) / DEFAULT_FACE_HEIGHT;

	// ������ ũ�� �� ��ġ�� �׸��� : ��, �� 2��
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

	// �� 2��
	HBRUSH Lion2 = CreateSolidBrush(RGB(0, 0, 0));
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

	// �� 2��
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

	// ������ ���̿� �β��� ���� (�� ���� �����Ͽ� ������ ũ�⸦ ������ �� �ֽ��ϴ�)
	int eyebrowLength = radius * 0.35; // ��: �������� 30%��ŭ�� ����
	int eyebrowThickness = radius * 0.03; // ��: �������� 10%��ŭ�� �β�

	// ���� ������ �������� ���� ���
	POINT leftEyebrowStart = { centerX - radius * 0.5, centerY - radius * 0.3 };
	POINT leftEyebrowEnd = { leftEyebrowStart.x + eyebrowLength, leftEyebrowStart.y - eyebrowThickness };

	// ������ ������ �������� ���� ���
	POINT rightEyebrowStart = { centerX + radius * 0.5, centerY - radius * 0.3 };
	POINT rightEyebrowEnd = { rightEyebrowStart.x - eyebrowLength, rightEyebrowStart.y - eyebrowThickness };

	// ���� �׸���
	MoveToEx(hdc, leftEyebrowStart.x, leftEyebrowStart.y, NULL);
	LineTo(hdc, leftEyebrowEnd.x, leftEyebrowEnd.y);

	MoveToEx(hdc, rightEyebrowStart.x, rightEyebrowStart.y, NULL);
	LineTo(hdc, rightEyebrowEnd.x, rightEyebrowEnd.y);

	MoveToEx(hdc, leftEyebrowStart.x, leftEyebrowStart.y + 3, NULL);
	LineTo(hdc, leftEyebrowEnd.x, leftEyebrowEnd.y + 2);

	MoveToEx(hdc, rightEyebrowStart.x, rightEyebrowStart.y + 3, NULL);
	LineTo(hdc, rightEyebrowEnd.x, rightEyebrowEnd.y + 2);
}

void DrawBox(HDC hdc, POINT startPointBox, POINT endPointBox) {

	// ���� ��ǥ�� startPoint�� ����
	POINT startPoint = { min(startPointBox.x, endPointBox.x), min(startPointBox.y, endPointBox.y) };
	POINT endPoint = { max(startPointBox.x, endPointBox.x), max(startPointBox.y, endPointBox.y) };

	// �巡�� ������ �������� ���� ������ ���� ������ ��� -> ��������Ʈ���� ��������Ʈ�� �� ���� ��쵵 �����ϱ�
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

	// ���� ��ǥ�� startPoint�� ����
	POINT startPoint = { min(startPointCube.x, endPointCube.x), min(startPointCube.y, endPointCube.y) };
	POINT endPoint = { max(startPointCube.x, endPointCube.x), max(startPointCube.y, endPointCube.y) };

	// �巡�� ������ �������� ���� ������ ���� ������ ��� -> ��������Ʈ���� ��������Ʈ�� �� ���� ��쵵 �����ϱ�
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
		DrawBox(hdc, startPointBox, endPointBox);     // �ڽ� ����(��Ŭ��, ��Ŭ�� ��� �����)
	}
	if (isDrawingCube && (isMouseLButtonPressed || isMouseRButtonPressed)) {
		DrawCube(hdc, startPointCube, endPointCube);  // ť�� ����
	}
}

// �������� �̺�Ʈ�� ó���ϴ� �ݹ�(Callback) �Լ�.
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	SetTimer(hwnd, 1, 300, NULL);

	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(hwnd, &ps);

	switch (uMsg)         // window message ������ ����
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
			isDrawingCube = false;
			InvalidateRect(hwnd, NULL, TRUE);
		}
		else if (LOWORD(wParam) == 5) {
			isDrawingBox = false;
			isDrawingCircle = false;
			isDrawingBonobono = false;
			isDrawingLion = false;
			isDrawingCube = true;
			CUBE_DRAW_MODE = true;     // ��ư�� Ŭ���������� ȭ���� �ʱ�ȭ �����̹Ƿ� DRAW���
			CUBE_RESIZE_MODE = false;
			InvalidateRect(hwnd, NULL, TRUE);
		}
		break;

	case WM_TIMER:   // ���뺸�� �� ���� ��ȭ
		if (GetAsyncKeyState(VK_SPACE) & 0x8000) {
			// �����̽��ٰ� ������ ���� ��
			BonobonoSwitch = true;
			InvalidateRect(hwnd, NULL, TRUE);
		}
		else {
			// �����̽��ٰ� ������ ���� ���� ��
			BonobonoSwitch = false;
			InvalidateRect(hwnd, NULL, TRUE);
		}
		break;


		//case WM_KEYDOWN:     ���! BeginPaint���� keydown�� �޾ƿ��� �����
		//	// OutputDebugString(L"WM_KEYDOWN received\n");
		//	if (wParam == VK_SPACE) {
		//		BonobonoSwitch = true;
		//		InvalidateRect(hwnd, NULL, TRUE); // ȭ���� �ٽ� �׸����� ��û
		//	}
		//	break;

		//case WM_KEYUP:
		//	if (wParam == VK_SPACE) {
		//		BonobonoSwitch = false;
		//		InvalidateRect(hwnd, NULL, TRUE); // ȭ���� �ٽ� �׸����� ��û
		//	}
		//	break;

	case WM_LBUTTONDOWN:  // ���콺 ���� Ŭ����
	{
		///** �簢�� �׸���
		if (isMouseOnDrawingBox && isDrawingBox) {  // Box
			isMouseLButtonPressed = 1;

			startPointBox.x = LOWORD(lParam);
			startPointBox.y = HIWORD(lParam);
			endPointBox.x = startPointBox.x;
			endPointBox.y = startPointBox.y;


		}
		///** �� �׸���
		if (isMouseOnDrawingBox && isDrawingCircle) {
			startPointCircle = { 0 };
			endPointCircle = { 0 };

			startPointCircle.x = LOWORD(lParam);
			startPointCircle.y = HIWORD(lParam);
			endPointCircle.x = startPointCircle.x;   // ���� �ʱ�ȭ(���� �� �ٽ� �׸� �� ���� �������� ���Ǵ� ����)
			endPointCircle.y = startPointCircle.y;

			isMouseLButtonPressed = 1;
			//	InvalidateRect(hwnd, NULL, TRUE);
		}

		///** ���̾� �׸���
		if (isMouseOnDrawingBox && isDrawingLion) {  // Lion 
			startPointLion.x = LOWORD(lParam);
			startPointLion.y = HIWORD(lParam);
			isMouseLButtonPressed = 1;
		}

		///** ť�� �׸���
		if (isMouseOnDrawingBox && isDrawingCube) {  // Box
			isMouseLButtonPressed = 1;

			ccx = LOWORD(lParam);
			ccy = HIWORD(lParam);
			if (InBox(ccx, ccy, startPointCube, endPointCube)) {   // Cube�ȿ� Ŀ���� �ִ��� �˻�
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

	case WM_RBUTTONDOWN: // ���콺 ��Ŭ�� ����
	{
		if (isMouseOnDrawingBox && isDrawingBox) {

			bbx = bx = LOWORD(lParam);
			bby = by = HIWORD(lParam);

			bx = bbx;
			by = bby;

			if (InBox(bbx, bby, startPointBox, endPointBox)) {
				// Box �������� �ִٸ� �������·�
				isMouseRButtonPressed = 1;
			}
			// InvalidateRect(hwnd, NULL, TRUE);
		}

		if (isMouseOnDrawingBox && isDrawingCircle) {
			// Circle ��Ŭ�� 
			mmx = mmx = 0;
			mmx = mx = LOWORD(lParam);
			mmy = my = HIWORD(lParam);

			if (InCircle(mmx, mmy, startPointCircle, radius)) { // ��Ŭ���� �� ���ο��� �߻��� ���
				isMouseRButtonPressed = 1;
			}
		}

		if (isMouseOnDrawingBox && isDrawingCube) {

			bbx = bx = LOWORD(lParam);
			bby = by = HIWORD(lParam);

			bx = bbx;
			by = bby;

			if (InBox(bbx, bby, startPointCube, endPointCube)) {
				// Cube �������� �ִٸ� �������·�
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

			// �巡�װ� �Ϸ�Ǿ����Ƿ� ȭ���� �ٽ� �׸����� ��û
			// InvalidateRect(hwnd, NULL, TRUE);
		}

		//if (isMouseOnDrawingBox && isDrawingCube) {
		//	endPointCube.x = LOWORD(lParam);
		//	endPointCube.y = HIWORD(lParam);

		//	// �巡�װ� �Ϸ�Ǿ����Ƿ� ȭ���� �ٽ� �׸����� ��û
		//	// InvalidateRect(hwnd, NULL, TRUE);
		//}
		isMouseLButtonPressed = 0;




	}
	break;

	case WM_MOUSEMOVE:
	{

		if (isMouseRButtonPressed && isMouseOnDrawingBox && isDrawingBox) {  // ��Ŭ�� move �̺�Ʈ
			bx = LOWORD(lParam);
			by = HIWORD(lParam);

			int dx = bx - bbx;
			int dy = by - bby;
			bbx = bx;  // �ʱ�ȭ�� ������ ù rButtonDown���� ������ bbx���� ��� �������� ��!
			bby = by;


			if (dx != 0 || dy != 0) {


				startPointBox.x += dx;
				startPointBox.y += dy;
				endPointBox.x += dx;
				endPointBox.y += dy;

				InvalidateRect(hwnd, NULL, TRUE); // â�� �ٽ� �׸��ϴ�.

				/*startPointBox.x = bx;
				startPointBox.y = by;*/
			}
		}
		else if (isMouseRButtonPressed && isMouseOnDrawingBox && isDrawingCircle) {
			int currentX = LOWORD(lParam);
			int dx = currentX - mmx; // �巡�׷� �̵��� �Ÿ�( ������ġ - ó����ġ )

			// ���� ũ�⸦ ����. 100px�� 2��/0.5�� ����
			if (dx > 0) {
				double factor = 1.0 + dx / 100.0;
				radius *= factor * factor;  // 100px �̵��� 2�� ����
			}
			else if (dx < 0) {
				radius *= (1 - abs(dx) / 200.0); // 100px �̵��� 0.5�� ����
			}

			// ���� ũ�Ⱑ �ʹ� �۾����� ���� ����
			if (radius < 10) radius = 10;

			mmx = currentX; // ���� ��ġ ������Ʈ
			InvalidateRect(hwnd, NULL, TRUE); // WM_PAINT ȣ��
		}
		else if (isMouseRButtonPressed && isMouseOnDrawingBox && isDrawingCube) {
			bx = LOWORD(lParam);
			by = HIWORD(lParam);

			int dx = bx - bbx;
			int dy = by - bby;
			bbx = bx;  // �ʱ�ȭ�� ������ ù rButtonDown���� ������ bbx���� ��� �������� ��!
			bby = by;


			if (dx != 0 || dy != 0) {


				startPointCube.x += dx;
				startPointCube.y += dy;
				endPointCube.x += dx;
				endPointCube.y += dy;

				InvalidateRect(hwnd, NULL, TRUE); // â�� �ٽ� �׸��ϴ�.

				/*startPointCube.x = bx;
				startPointCube.y = by;*/
			}
		}

		else if (isMouseLButtonPressed && isMouseOnDrawingBox && isDrawingBox) {
			endPointBox.x = LOWORD(lParam);
			endPointBox.y = HIWORD(lParam);
			InvalidateRect(hwnd, NULL, TRUE); // WM_PAINT ȣ��
		}
		else if (isMouseLButtonPressed && isMouseOnDrawingBox && isDrawingCircle) {
			endPointCircle.x = LOWORD(lParam);
			endPointCircle.y = HIWORD(lParam);
			InvalidateRect(hwnd, NULL, TRUE); // WM_PAINT ȣ��
		}
		else if (isMouseLButtonPressed && isMouseOnDrawingBox && isDrawingLion) {
			endPointLion.x = LOWORD(lParam);
			endPointLion.y = HIWORD(lParam);
			InvalidateRect(hwnd, NULL, TRUE); // WM_PAINT ȣ��
		}
		else if (isMouseLButtonPressed && isMouseOnDrawingBox && isDrawingCube && CUBE_DRAW_MODE) {
			endPointCube.x = LOWORD(lParam);
			endPointCube.y = HIWORD(lParam);
			InvalidateRect(hwnd, NULL, TRUE); // WM_PAINT ȣ��
		}
		else if (isMouseLButtonPressed && isMouseOnDrawingBox && isDrawingCube && CUBE_RESIZE_MODE) {
			POINT startPoint = { min(startPointCube.x, endPointCube.x), min(startPointCube.y, endPointCube.y) };
			POINT endPoint = { max(startPointCube.x, endPointCube.x), max(startPointCube.y, endPointCube.y) };

			// �巡�� ������ �������� ���� ������ ���� ������ ��� -> ��������Ʈ���� ��������Ʈ�� �� ���� ��쵵 �����ϱ�
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

			int dx = currentX - startPointCube.x; // x�� ��ȭ��

			// ���� ũ�� ���� ����
			if (dx != 0) {
				//double scaleFactor = 1 + (dx / 100.0); // ��: �巡�׷� 100px �̵��� 2�� ����
				//double scaleFactor = max(0.1, 1.0 + (dx / 100.0));
				for (int i = 0; i < 8; ++i) {
					// ������ ���� ������ ����


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




					// ������ ���� ���鿡 ���� ������ �߰�
					// ����纯���� ���� ���� �����ǰ�, ���� ���� scaleFactor�� ���� �����Ǿ�� ��
				}

				/*
				int currentX = LOWORD(lParam);
				int currentY = HIWORD(lParam);

				int initialWidth = endPointCube.x - startPointCube.x;
				int initialHeight = endPointCube.y - startPointCube.y;

				int dx = currentX - startPointCube.x; // x�� ��ȭ��
				int dy = currentY - startPointCube.y;

				double scaleX = (initialWidth + dx) / (double)initialWidth;
				double scaleY = (initialHeight + dy) / (double)initialHeight;

				// ���� ũ�� ���� ����
				if (dx != 0) {
					for (int i = 0; i < 8; ++i) {
						int originalX = (i < 4) ? startPointCube.x : (startPointCube.x + offset_x); // ����� ���� ������ ���� x ��ġ
						int originalY = (i % 4 == 0 || i % 4 == 3) ? startPointCube.y : (startPointCube.y + offset_y); // ����� ���� ������ ���� y ��ġ

						// �� ��ġ ����
						cubePoints[i].x = startPointCube.x + (int)((cubePoints[i].x - originalX) * scaleX);
						cubePoints[i].y = startPointCube.y + (int)((cubePoints[i].y - originalY) * scaleY);
					}


					// ������ ���� ���鿡 ���� ������ �߰�
					// ����纯���� ���� ���� �����ǰ�, ���� ���� scaleFactor�� ���� �����Ǿ�� ��
				}
				*/


				//startPointCube.x = currentX; // �巡�� ���� ��ġ ������Ʈ
				//startPointCube.y = currentY; 

				InvalidateRect(hwnd, NULL, TRUE); // ȭ�� ����
			}


		}



		hdc = (HDC)wParam;   // ��ü gui RECT�� ũ�⸦ ������
		RECT rect;
		GetClientRect(hwnd, &rect);

		RECT drawArea = {
		rect.left + 8,
		rect.top + 8 + 8 + 8 + 64,
		rect.right - 8,
		rect.bottom - 8
		};

		POINT pt;  // drawArea üũ�� ���콺 ��ǥ
		pt.x = LOWORD(lParam);
		pt.y = HIWORD(lParam);

		if (PtInRect(&drawArea, pt))
		{
			// ���콺�� ��ġ�� �簢�� �ȿ� ������ Ŀ���� ����� +(CROSS) ������� ����
			isMouseOnDrawingBox = 1;
			SetCursor(LoadCursor(NULL, IDC_CROSS));
		}
		else
		{
			isMouseOnDrawingBox = 0;
			// ���콺�� ��ġ�� �簢�� �ȿ� ���� ������ �⺻ Ŀ�� ������� ����
			SetCursor(LoadCursor(NULL, IDC_ARROW));
		}

	}
	break;

	case WM_PAINT:
	{
		// �������� �׷��� ���ؽ�Ʈ�� ���, �׸��� �׸� �� �ְ� ��
		// ȭ���� ���ΰ�ħ �� ������ ���찳��
		HBRUSH hBrush_eraser = CreateSolidBrush(RGB(255, 255, 255)); // ���찳

		RECT rect_eraser;
		GetClientRect(hwnd, &rect_eraser);
		FillRect(hdc, &rect, hBrush_eraser);




		if (isMouseLButtonPressed && isMouseOnDrawingBox && isDrawingBox)
		{
			DrawShape(hwnd, hdc);
		}

		else if (isMouseLButtonPressed && isMouseOnDrawingBox && isDrawingCircle)
		{
			/* ���� ä�����, Ellipse() �Լ��� ����� �� ä��⸦ ���ϴ� �귯�ø� ����̽� ���ؽ�Ʈ (DC)�� ���� */
			HBRUSH hBrushCircle = CreateSolidBrush(RGB(255, 216, 190)); // ��ũ �귯�� ����
			HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, hBrushCircle);

			HPEN hTransparentPen = CreatePen(PS_NULL, 0, RGB(0, 0, 0)); // ������ �� ����
			HPEN hOldPen = (HPEN)SelectObject(hdc, hTransparentPen); // ������ ���� ����̽� ���ؽ�Ʈ�� ����


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

		else if (isMouseLButtonPressed && isMouseOnDrawingBox && isDrawingCube && CUBE_DRAW_MODE)
		{
			DrawShape(hwnd, hdc);
		}

		else if ((isMouseLButtonPressed || isMouseRButtonPressed) && isMouseOnDrawingBox && isDrawingCube && CUBE_RESIZE_MODE) {

			// ���� ��ǥ�� startPoint�� ����
			POINT startPoint = { min(startPointCube.x, endPointCube.x), min(startPointCube.y, endPointCube.y) };
			POINT endPoint = { max(startPointCube.x, endPointCube.x), max(startPointCube.y, endPointCube.y) };

			// �巡�� ������ �������� ���� ������ ���� ������ ��� -> ��������Ʈ���� ��������Ʈ�� �� ���� ��쵵 �����ϱ�
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

		else if (isMouseRButtonPressed && isMouseOnDrawingBox && isDrawingBox) {

			// ���� ���¿� ���� ������ �׸� �� �ֽ��ϴ�.
			DrawShape(hwnd, hdc);
		}

		else if (isMouseRButtonPressed && isMouseOnDrawingBox && isDrawingCircle) {
			HBRUSH hBrushCircle = CreateSolidBrush(RGB(255, 216, 190));
			HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, hBrushCircle);
			HPEN hTransparentPen = CreatePen(PS_NULL, 0, RGB(0, 0, 0)); // ������ �� ����
			HPEN hOldPen = (HPEN)SelectObject(hdc, hTransparentPen); // ������ ���� ����̽� ���ؽ�Ʈ�� ����

			SelectObject(hdc, hBrushCircle);

			Ellipse(hdc, startPointCircle.x - radius, startPointCircle.y - radius,
				startPointCircle.x + radius, startPointCircle.y + radius);

			SelectObject(hdc, hOldPen);
			SelectObject(hdc, hOldBrush);
			DeleteObject(hTransparentPen);
			DeleteObject(hBrushCircle);
		}

		// ���뺸��
		else if (isDrawingBonobono) {
			HBRUSH Bono1 = CreateSolidBrush(RGB(127, 200, 255));
			HBRUSH OldBrush1 = (HBRUSH)SelectObject(hdc, Bono1);
			Ellipse(hdc, 280, 115, 520, 355);
			SelectObject(hdc, OldBrush1);
			DeleteObject(Bono1);



			// ��
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

			// ��, ���� ��
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
			else if (!BonobonoSwitch) {

				Ellipse(hdc, 311, 216, 323, 235);
				Ellipse(hdc, 480, 216, 492, 235);

				Ellipse(hdc, 314, 222, 320, 228);
				Ellipse(hdc, 483, 222, 489, 228);
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


		else {   // ȭ�� �ʱ�ȭ(����� ��ưŬ����, ������ ��Ŭ�� �̵��� ����׿����� ����� ��� ��)
			InvalidateRect(hwnd, NULL, TRUE);
		}


		EndPaint(hwnd, &ps);
	}
	break;


	case WM_DESTROY:
	{


		PostQuitMessage(0);
		FreeConsole();  // GUI�� ������ cmdâ�� ���� ����
		ExitProcess(0); // �ܿ� ���μ����� �����մϴ�.
	}
	break;

	case WM_GETMINMAXINFO:  // Ȯ����� �Ұ�
	{
		MINMAXINFO* pMinMaxInfo = (MINMAXINFO*)lParam;
		pMinMaxInfo->ptMinTrackSize.x = 800; // �ּ� ���� ũ��
		pMinMaxInfo->ptMinTrackSize.y = 480; // �ּ� ���� ũ��
		pMinMaxInfo->ptMaxTrackSize.x = 800; // �ִ� ���� ũ��
		pMinMaxInfo->ptMaxTrackSize.y = 480; // �ִ� ���� ũ��
	}
	break;

	case WM_ERASEBKGND:    // ����, �ڽ� change
	{
		HDC hdc = (HDC)wParam;
		RECT rect;
		GetClientRect(hwnd, &rect);
		HBRUSH hBrush = CreateSolidBrush(RGB(255, 240, 200)); // ũ���� ����
		FillRect(hdc, &rect, hBrush);
		DeleteObject(hBrush);

		// margin�� padding�� �ݿ��Ͽ� �簢���� ��ġ�� ũ�� ���
		RECT boxRect = {
			rect.left + 8,    // left margin
			rect.top + 8,     // top margin
			rect.right - 8,   // right margin
			rect.bottom - 8   // bottom margin
		};

		// �簢�� �׸���
		HBRUSH hBrushBox = CreateSolidBrush(RGB(255, 240, 200));  // �ڽ� ����
		FillRect(hdc, &boxRect, hBrushBox);
		FrameRect(hdc, &boxRect, CreateSolidBrush(RGB(0, 0, 0)));
		DeleteObject(hBrushBox);

		// drawing box �κ�
		RECT drawArea = {
			rect.left + 8,    // left margin
			rect.top + 8 + 8 + 8 + 64,     // top margin
			rect.right - 8,   // right margin
			rect.bottom - 8   // bottom margin
		};
		HBRUSH hBrushDrawArea = CreateSolidBrush(RGB(255, 255, 255));  // �ڽ� ����
		FillRect(hdc, &drawArea, hBrushDrawArea);
		FrameRect(hdc, &drawArea, CreateSolidBrush(RGB(0, 0, 0)));
		DeleteObject(hBrushDrawArea);

		return 1; // ó���Ǿ����� �˷���
	}
	break;

	case WM_DRAWITEM:    // ��ư �̺�Ʈ ����
	{
		LPDRAWITEMSTRUCT pDIS = (LPDRAWITEMSTRUCT)lParam;
		if (pDIS->CtlType == ODT_BUTTON && pDIS->CtlID == 1 | 2 | 3 | 4 | 5)
		{
			// �׸��� ȿ���� �׸��ϴ�.
			HPEN hShadowPen = CreatePen(PS_SOLID, 1, RGB(128, 128, 128));  // ȸ�� ��
			SelectObject(pDIS->hDC, hShadowPen);
			POINT shadowPoints[] =
			{
				{ pDIS->rcItem.right, pDIS->rcItem.top + 2 },
				{ pDIS->rcItem.right, pDIS->rcItem.bottom },
				{ pDIS->rcItem.left + 2, pDIS->rcItem.bottom }
			};
			Polyline(pDIS->hDC, shadowPoints, 3);
			DeleteObject(hShadowPen);

			// �ܰ����� �׸��ϴ�.
			HPEN hBorderPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));  // ������ ��
			SelectObject(pDIS->hDC, hBorderPen);
			Rectangle(pDIS->hDC, pDIS->rcItem.left, pDIS->rcItem.top, pDIS->rcItem.right, pDIS->rcItem.bottom);
			DeleteObject(hBorderPen);

			// ��ư�� �ؽ�Ʈ�� �׸��ϴ�.
			SIZE size;
			wchar_t text[256];
			GetWindowText(pDIS->hwndItem, text, 256);
			GetTextExtentPoint32(pDIS->hDC, text, wcslen(text), &size);
			SetTextColor(pDIS->hDC, RGB(30, 28, 28));  // �ؽ�Ʈ ���� (�Ͼ��)
			TextOut(pDIS->hDC,
				(pDIS->rcItem.right - size.cx) / 2,
				(pDIS->rcItem.bottom - size.cy) / 2,
				text,
				wcslen(text));

			return TRUE;  // �޽��� ó�� �Ϸ�
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



	/* ������ Ŭ���� ����.*/
	WNDCLASS wc;
	ZeroMemory(&wc, sizeof(wc));	// ��� 0���� �ʱ�ȭ.

	// ������ Ŭ���� �� ����
	wc.hInstance = hInstance;
	wc.lpszClassName = TEXT("Computer Software");
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WindowProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.lpszClassName = L"ButtonWindowClass";

	// ������ Ŭ���� ���.
	if (RegisterClass(&wc) == 0)
	{
		MessageBox(NULL, L"RegisterClass failed!", L"Error", MB_ICONERROR);
		exit(-1);	//����
	}

	// Window viewport ���� ����
	RECT rect = { 0, 0, 800, 480 };
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, 0);
	int width = rect.right - rect.left;
	int height = rect.bottom - rect.top;

	// ������ ����
	hwnd = CreateWindow(
		wc.lpszClassName,
		TEXT("������"),
		WS_OVERLAPPEDWINDOW,
		0, 0,
		width, height,
		NULL, NULL,
		hInstance,
		NULL
	);


	// ���� �˻�.
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


	// â ���̱�.
	ShowWindow(hwnd, SW_SHOW); // â ����
	UpdateWindow(hwnd); // ������Ʈ�ؾ� ����. �� ������ ���ٰ� ���� ��.



	// �޽��� ó�� ����.
	MSG msg;
	ZeroMemory(&msg, sizeof(msg));



	while (msg.message != WM_QUIT)
	{
		// �޽��� ó��.
		if (GetMessage(&msg, hwnd, 0, 0))
		{

			// �޽��� �ؼ�����.
			TranslateMessage(&msg);
			// �޽����� ó���ؾ��� ���� ��������.
			DispatchMessage(&msg);

		}
	}
	UnregisterClass(wc.lpszClassName, wc.hInstance);

	//���� �޽��� ������
	return (int)msg.wParam;

}
