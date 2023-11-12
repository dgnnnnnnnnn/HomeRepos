#include "yuhanCG.h"


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

void DrawRyan(HWND hwnd, HDC hdc, POINT startPointRyan, POINT endPointRyan)
{

	// �پ��� �巡�� ���⿡ ���� ����ȭ�� ���� POINT ��ǥ�� ����
	POINT startPoint = { min(startPointRyan.x, endPointRyan.x),  min(startPointRyan.y, endPointRyan.y) };
	POINT endPoint = { max(startPointRyan.x, endPointRyan.x),  max(startPointRyan.y, endPointRyan.y) };


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

	// ���� �׸���
	int minX = startPoint.x;
	int minY = startPoint.y;
	int maxX = endPoint.x;
	int maxY = endPoint.y;

	// ���� ���� �ܰ� ��ǥ�� ���
	minX = min(minX, startPoint.x + DEFAULT_EAR1_X * scaleX);
	minY = min(minY, startPoint.y + DEFAULT_EAR1_Y * scaleY);
	maxX = max(maxX, startPoint.x + (DEFAULT_EAR1_X + DEFAULT_EAR1_WIDTH) * scaleX);
	maxY = max(maxY, startPoint.y + (DEFAULT_EAR1_Y + DEFAULT_EAR1_HEIGHT) * scaleY);

	// ������ ���� �ܰ� ��ǥ�� ���
	minX = min(minX, startPoint.x + DEFAULT_EAR2_X * scaleX);
	minY = min(minY, startPoint.y + DEFAULT_EAR2_Y * scaleY);
	maxX = max(maxX, startPoint.x + (DEFAULT_EAR2_X + DEFAULT_EAR2_WIDTH) * scaleX);
	maxY = max(maxY, startPoint.y + (DEFAULT_EAR2_Y + DEFAULT_EAR2_HEIGHT) * scaleY);

	// ���� �׵θ��� �׸��� ���� �� ����
	HPEN hPenDotted = CreatePen(PS_DOT, 1, RGB(0, 0, 0));
	HPEN hOldPen = (HPEN)SelectObject(hdc, hPenDotted);
	HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, GetStockObject(NULL_BRUSH));

	// ������ ��踦 ������� ���� �׵θ� �׸���
	Rectangle(hdc, minX, minY, maxX, maxY);

	// ������ ��� �귯�÷� ����
	SelectObject(hdc, hOldPen);
	SelectObject(hdc, hOldBrush);
	DeleteObject(hPenDotted);


}

void DrawBonobono(HWND hwnd, HDC hdc, int blink) {

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



