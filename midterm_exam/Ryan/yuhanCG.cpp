#include "yuhanCG.h"
#include <cmath>


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

void DrawBox(HWND hwnd, HDC hdc, POINT startPointBox, POINT endPointBox) {
	// 작은 좌표를 startPoint로 수정
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

	// Define a brush with a color
	HBRUSH hBrush = CreateSolidBrush(RGB(255, 150, 138)); //  cobalt color
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

INT DrawCircle(HWND hwnd, HDC hdc, POINT startPoint, POINT endPoint, INT radius, BOOL L_BOOL) {
	/* 원을 채우려면, Ellipse() 함수를 사용할 때 채우기를 원하는 브러시를 디바이스 컨텍스트 (DC)에 선택 */
	HBRUSH hBrushCircle = CreateSolidBrush(RGB(255, 216, 190)); // 귤색 브러시 생성
	HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, hBrushCircle);

	HPEN hTransparentPen = CreatePen(PS_NULL, 0, RGB(0, 0, 0)); // 투명한 펜 생성
	HPEN hOldPen = (HPEN)SelectObject(hdc, hTransparentPen); // 투명한 펜을 디바이스 컨텍스트에 선택
	SelectObject(hdc, hBrushCircle);

	// L_BOOL : 좌클릭 그리기 모드만 반지름을 재설정하므로 bool형 변수 사용
	if (L_BOOL) {
		radius = sqrt(pow(endPoint.x - startPoint.x, 2)
			+ pow(endPoint.y - startPoint.y, 2));
	}
	Ellipse(hdc, startPoint.x - radius, startPoint.y - radius,
		startPoint.x + radius, startPoint.y + radius);

	SelectObject(hdc, hOldPen);
	SelectObject(hdc, hOldBrush);
	DeleteObject(hTransparentPen);
	DeleteObject(hBrushCircle);

	return radius;
}

void DrawBonobono(HWND hwnd, HDC hdc, int blink) {

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

	if (blink == 1) {
		MoveToEx(hdc, 322, 213, NULL);
		LineTo(hdc, 338, 229);
		MoveToEx(hdc, 319, 240, NULL);
		LineTo(hdc, 338, 227);

		MoveToEx(hdc, 468, 239, NULL);
		LineTo(hdc, 485, 222);
		MoveToEx(hdc, 468, 239, NULL);
		LineTo(hdc, 485, 252);
	}
	else if (blink == 0) {

		Ellipse(hdc, 311, 216, 323, 235);
		Ellipse(hdc, 480, 216, 492, 235);

		Ellipse(hdc, 314, 222, 320, 228);
		Ellipse(hdc, 483, 222, 489, 228);
	}
	SelectObject(hdc, OldBrush3);
	DeleteObject(Bono3);


}

void DrawRyan(HWND hwnd, HDC hdc, POINT startPointRyan, POINT endPointRyan)
{

	// 다양한 드래그 방향에 따른 동기화를 위해 POINT 좌표값 조정
	POINT startPoint = { min(startPointRyan.x, endPointRyan.x),  min(startPointRyan.y, endPointRyan.y) };
	POINT endPoint = { max(startPointRyan.x, endPointRyan.x),  max(startPointRyan.y, endPointRyan.y) };


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

	MoveToEx(hdc, leftEyebrowStart.x, leftEyebrowStart.y + 3, NULL);
	LineTo(hdc, leftEyebrowEnd.x, leftEyebrowEnd.y + 2);

	MoveToEx(hdc, rightEyebrowStart.x, rightEyebrowStart.y + 3, NULL);
	LineTo(hdc, rightEyebrowEnd.x, rightEyebrowEnd.y + 2);

	// 점선 그리기
	int minX = startPoint.x;
	int minY = startPoint.y;
	int maxX = endPoint.x;
	int maxY = endPoint.y;

	// 왼쪽 귀의 외곽 좌표를 계산
	minX = min(minX, startPoint.x + DEFAULT_EAR1_X * scaleX);
	minY = min(minY, startPoint.y + DEFAULT_EAR1_Y * scaleY);
	maxX = max(maxX, startPoint.x + (DEFAULT_EAR1_X + DEFAULT_EAR1_WIDTH) * scaleX);
	maxY = max(maxY, startPoint.y + (DEFAULT_EAR1_Y + DEFAULT_EAR1_HEIGHT) * scaleY);

	// 오른쪽 귀의 외곽 좌표를 계산
	minX = min(minX, startPoint.x + DEFAULT_EAR2_X * scaleX);
	minY = min(minY, startPoint.y + DEFAULT_EAR2_Y * scaleY);
	maxX = max(maxX, startPoint.x + (DEFAULT_EAR2_X + DEFAULT_EAR2_WIDTH) * scaleX);
	maxY = max(maxY, startPoint.y + (DEFAULT_EAR2_Y + DEFAULT_EAR2_HEIGHT) * scaleY);

	// 점선 테두리를 그리기 위한 펜 설정
	HPEN hPenDotted = CreatePen(PS_DOT, 1, RGB(0, 0, 0));
	HPEN hOldPen = (HPEN)SelectObject(hdc, hPenDotted);
	HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, GetStockObject(NULL_BRUSH));

	// 수정된 경계를 기반으로 점선 테두리 그리기
	Rectangle(hdc, minX, minY, maxX, maxY);

	// 원래의 펜과 브러시로 복구
	SelectObject(hdc, hOldPen);
	SelectObject(hdc, hOldBrush);
	DeleteObject(hPenDotted);


}

void DrawCube(HWND hwnd, HDC hdc, POINT startPointCube, POINT endPointCube, POINT* cubePoints, BOOL REZISE_BOOL) {
	// 드래그 방향을 기준으로 평행 투영에 대한 오프셋 계산 
	int offset_x, offset_y;


	if (!REZISE_BOOL) {
		// 0~3 INDEX는 전면 사각형을 그리므로 드래그 방향에 관계 없이 항상 동일
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
	}




	// Define a brush with a color
	HBRUSH hBrushOne = CreateSolidBrush(RGB(124, 143, 178)); // Serenity ver.1 color
	SelectObject(hdc, hBrushOne);

	// Fill the front,top,right face
	POINT frontFace[4] = { cubePoints[0], cubePoints[1], cubePoints[2], cubePoints[3] };
	Polygon(hdc, frontFace, 4);
	DeleteObject(hBrushOne); // Don't forget to delete the brush

	HBRUSH hBrushTwo = CreateSolidBrush(RGB(145, 168, 210)); // Serenity ver.2 color
	SelectObject(hdc, hBrushTwo);
	POINT topFace[4] = { cubePoints[0], cubePoints[1], cubePoints[5], cubePoints[4] };
	Polygon(hdc, topFace, 4);

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
	DeleteObject(hBrushTwo); // Don't forget to delete the brush

}





