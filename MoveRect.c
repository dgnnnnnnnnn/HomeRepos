#ifdef UNICODE
#pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console")
#else
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
#endif
typedef int bool;
#define true 1
#define false 0

#include <windows.h>

POINT startPoint = { 0 };  // POINT ����ü ������ �ʱ�ȭ
POINT startPointR = { 0 };
POINT endPoint = { 0 };
int isMouseLButtonPressed = 0;
int isMouseRButtonPressed = 0;
RECT rectrect = { 0 };
RECT rectrect1 = { 0 };

const wchar_t* textCrash = L"Crash!!!";
int mx, my;
int emx, emy;
int mmx, mmy;

bool InRect(INT mmx, INT mmy, RECT rect1)
{
	if ((mmx > rect1.left && mmx < rect1.right) &&
		(mmy > rect1.top && mmy < rect1.bottom))
	{
		return 1; // ������ ��ħ
	}
	return 0; // �� RECT�� ��ġ�� ����
}






// �������� �̺�Ʈ�� ó���ϴ� �ݹ�(Callback) �Լ�.
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{

	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(hwnd, &ps);

	switch (uMsg)         // window message ������ ����
	{

	case WM_LBUTTONDOWN:  // ���콺 ���� Ŭ����
	{
		///** �簢�� �׸���

		startPoint.x = LOWORD(lParam);     // Ŭ���� ���콺 ��ġ�� x �� y ��ǥ�� ����
		startPoint.y = HIWORD(lParam);

		// ����� �簢���� Ÿ�� �簢���� �浹�� �˻�
	
		endPoint.x = startPoint.x;
		endPoint.y = startPoint.y;
		isMouseLButtonPressed = 1;

		InvalidateRect(hwnd, NULL, TRUE);


	}
	break;

	case WM_RBUTTONDOWN: // ���콺 ��Ŭ�� ����
	{
		mmx = mx = LOWORD(lParam);
		mmy = my = HIWORD(lParam);
		/*mx = LOWORD(lParam);
		my = HIWORD(lParam);*/
		emx = mmx;
		emy = mmy;

		/*
		endPoint.x = startPoint.x;
		endPoint.y = startPoint.y;*/

		if (InRect(mmx, mmy, rectrect)) {
			//�簢���� �������� �ִٸ�
			isMouseRButtonPressed = 1;
		}
		// InvalidateRect(hwnd, NULL, TRUE);

	}
	break;

	case WM_RBUTTONUP:
	{
		mmx = mmy = 0;
		isMouseRButtonPressed = 0;
		// InvalidateRgn(hwnd, NULL, TRUE);

	}
	break;

	case WM_MOUSEMOVE:
	{


		//rect.left = min(startPoint.x, endPoint.x);
		//rect.top = min(startPoint.y, endPoint.y);
		//rect.right = max(startPoint.x, endPoint.x);
		//rect.bottom = max(startPoint.y, endPoint.y);
	

			// if (isMouseRButtonPressed && InRect(startPoint, rectrect))
	//	if (isMouseRButtonPressed && InRect(startPoint, rectrect)) {
		if (isMouseRButtonPressed) {
			mx = LOWORD(lParam);
			my = HIWORD(lParam);

			int dx = mx - mmx;
			int dy = my - mmy;
			mmx = mx;
			mmy = my;   // ��������(?) ����

			//int dx = mx - startPoint.x;
			//int dy = my - startPoint.y;	

			if (dx != 0 || dy != 0) {
				

				rectrect.left += dx;
				rectrect.top += dy;
				rectrect.right += dx;
				rectrect.bottom += dy;

				startPoint.x = mx;
				startPoint.y = my;
				/*		startPoint.x = mx;
						startPoint.y = my;*/
				InvalidateRect(hwnd, NULL, TRUE);
			}

			
		
			
		}
		else if (isMouseLButtonPressed) {
			// ���콺 ��Ŭ�� �̺�Ʈ ó�� �߰�
			endPoint.x = LOWORD(lParam);
			endPoint.y = HIWORD(lParam);
			InvalidateRect(hwnd, NULL, TRUE);
		}
	}
	break;

	case WM_LBUTTONUP:
	{
		///** �簢�� �׸���
		//*/
		//HDC hdc = GetDC(hwnd);
		//RECT rect = { 50, 50, 150, 150 }; // ���� ��� ��ǥ (50, 50)���� ������ �ϴ� ��ǥ (150, 150)������ �簢��
		//

		//// �׸���
		//FillRect(hdc, &rect, (HBRUSH)(COLOR_WINDOW+1)); // �簢���� ���������� ä���
		//ReleaseDC(hwnd, hdc);

		//HDC hdc = GetDC(hwnd);
		//HBRUSH hBrush = CreateSolidBrush(RGB(255, 0, 255));
		//endPoint.x = LOWORD(lParam);
		//endPoint.y = HIWORD(lParam);

		isMouseLButtonPressed = 0;
	
		
	}
	break;
	case WM_PAINT:
	{
		
		 // �������� �׷��� ���ؽ�Ʈ�� ���, �׸��� �׸� �� �ְ� ��
		HBRUSH hBrush_eraser = CreateSolidBrush(RGB(255, 255, 255)); // ���찳

		RECT rect_eraser;
		GetClientRect(hwnd, &rect_eraser);
		FillRect(hdc, &rect_eraser, (HBRUSH)(COLOR_WINDOW + 1));


		if (isMouseLButtonPressed)
		{

	
			rectrect.left = min(startPoint.x, endPoint.x);
			rectrect.top = min(startPoint.y, endPoint.y);
			rectrect.right = max(startPoint.x, endPoint.x);
			rectrect.bottom = max(startPoint.y, endPoint.y);

			RECT rectrect = { startPoint.x, startPoint.y, endPoint.x, endPoint.y };
			Rectangle(hdc, startPoint.x, startPoint.y, endPoint.x, endPoint.y);

			HBRUSH hBrush = CreateSolidBrush(RGB(255, 0, 255)); // ��ũ �귯�� ����
			FillRect(hdc, &rectrect, hBrush);

			DeleteObject(hBrush);

			//MoveToEx(hdc, startPoint.x, startPoint.y, NULL);
			//LineTo(hdc, endPoint.x, endPoint.y);
			EndPaint(hwnd, &ps);
			break;
		}

		if (isMouseRButtonPressed) {
			// ���� ���¿� ���� ������ �׸� �� �ֽ��ϴ�.

			RECT rectrect1 = { rectrect.left, rectrect.top, rectrect.right, rectrect.bottom };
			Rectangle(hdc, rectrect1.left, rectrect1.top, rectrect1.right, rectrect1.bottom);

			HBRUSH hBrush = CreateSolidBrush(RGB(255, 0, 255));
			FillRect(hdc, &rectrect1, hBrush);

			DeleteObject(hBrush);
		}
		else {
			HBRUSH hBrush = CreateSolidBrush(RGB(56, 0, 0));
			FillRect(hdc, &rectrect1, hBrush);

		}


		EndPaint(hwnd, &ps);

		
	}
	break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_GETMINMAXINFO:  // Ȯ����� �Ұ�
	{
		MINMAXINFO* pMinMaxInfo = (MINMAXINFO*)lParam;
		pMinMaxInfo->ptMinTrackSize.x = 800; // �ּ� ���� ũ��
		pMinMaxInfo->ptMinTrackSize.y = 600; // �ּ� ���� ũ��
		pMinMaxInfo->ptMaxTrackSize.x = 800; // �ִ� ���� ũ��
		pMinMaxInfo->ptMaxTrackSize.y = 600; // �ִ� ���� ũ��
	}
	break;
	default:
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}

	return S_OK;
}



//bool RectIntersect(POINT startPoint, RECT rect1)
//{
//	if ((startPoint.x > rect1.left && startPoint.x < rect1.right) &&
//		(startPoint.y > rect1.top && startPoint.y < rect1.bottom))
//	{
//		return 1; // ������ ��ħ
//}
//	return 0; // �� RECT�� ��ġ�� ����
//}



#ifdef UNICODE
int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nShowCmd)
#else
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR pCmdLine, _In_ int nCmdShow)
#endif
{
	/* ������ Ŭ���� ����.*/
	WNDCLASS wc;
	ZeroMemory(&wc, sizeof(wc));	// ��� 0���� �ʱ�ȭ.

	// ������ Ŭ���� �� ����
	wc.hInstance = hInstance;
	wc.lpszClassName = TEXT("Computer Software");
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_CROSS);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WindowProc;

	// ������ Ŭ���� ���.
	if (RegisterClass(&wc) == 0)
	{
		MessageBox(NULL, L"RegisterClass failed!", L"Error", MB_ICONERROR);
		exit(-1);	//����
	}

	// Window viewport ���� ����
	RECT rect = { 150, 100, 800, 600 };
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, 0);
	int width = rect.right - rect.left;
	int height = rect.bottom - rect.top;

	// ������ ����
	HWND hwnd = CreateWindow(
		wc.lpszClassName,
		TEXT("[201907006 �����]"),
		WS_OVERLAPPEDWINDOW,
		0, 0,
		width, height,
		NULL, NULL,
		hInstance,
		NULL
	);

	// ���� �˻�.
	if (hwnd == NULL)
	{
		MessageBox(NULL, L"CreateWindow failed!", L"Error", MB_ICONERROR);
		exit(-1);
	}

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
			//if (PeekMessage(&msg, hwnd, 0, 0, PM_REMOVE)) // PM_REMOVE�� �ڸ��� �� �޼����� ���� ��Ұų�.�� �ǹ��ε� ����ٴ� ����.
		{

			// �޽��� �ؼ�����.
			TranslateMessage(&msg);
			// �޽����� ó���ؾ��� ���� ��������.
			DispatchMessage(&msg);

		}
		/*else
		{

		}*/
	}

	//���� �޽��� ������
	return (int)msg.wParam;

}


//bool InRect(POINT startPoint, RECT rect1)
//{
//	if ((startPoint.x > rect1.left && startPoint.x < rect1.right) &&
//		(startPoint.y > rect1.top && startPoint.y < rect1.bottom))
//	{
//		return 1; // ������ ��ħ
//	}
//	return 0; // �� RECT�� ��ġ�� ����
//}


//void my_paint(HDC hdc, BOOL* isMouseRButtonPressed, RECT* rectrect) {
//	HBRUSH hBrush;
//	if (*isMouseRButtonPressed) {
//		hBrush = CreateSolidBrush(RGB(255, 0, 255));
//		SelectObject(hdc, hBrush);
//	}
//	else {
//		SelectObject(hdc, GetStockObject(WHITE_BRUSH));
//	}
//	RECT rectrect1 = { rectrect->left, rectrect->top, rectrect->right, rectrect->bottom };
//	Rectangle(hdc, rectrect1.left, rectrect1.top, rectrect1.right, rectrect1.bottom);
//}
