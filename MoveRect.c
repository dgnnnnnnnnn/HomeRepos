#ifdef UNICODE
#pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console")
#else
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
#endif
typedef int bool;
#define true 1
#define false 0

#include <windows.h>

POINT startPoint = { 0 };  // POINT 구조체 변수를 초기화
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
		return 1; // 도형과 겹침
	}
	return 0; // 두 RECT가 겹치지 않음
}






// 윈도우의 이벤트를 처리하는 콜백(Callback) 함수.
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{

	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(hwnd, &ps);

	switch (uMsg)         // window message 종류별 수행
	{

	case WM_LBUTTONDOWN:  // 마우스 왼쪽 클릭시
	{
		///** 사각형 그리기

		startPoint.x = LOWORD(lParam);     // 클릭한 마우스 위치의 x 및 y 좌표가 저장
		startPoint.y = HIWORD(lParam);

		// 사용자 사각형과 타겟 사각형의 충돌을 검사
	
		endPoint.x = startPoint.x;
		endPoint.y = startPoint.y;
		isMouseLButtonPressed = 1;

		InvalidateRect(hwnd, NULL, TRUE);


	}
	break;

	case WM_RBUTTONDOWN: // 마우스 우클릭 시작
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
			//사각형의 영역내에 있다면
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
			mmy = my;   // 오차조정(?) 느낌

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
			// 마우스 좌클릭 이벤트 처리 추가
			endPoint.x = LOWORD(lParam);
			endPoint.y = HIWORD(lParam);
			InvalidateRect(hwnd, NULL, TRUE);
		}
	}
	break;

	case WM_LBUTTONUP:
	{
		///** 사각형 그리기
		//*/
		//HDC hdc = GetDC(hwnd);
		//RECT rect = { 50, 50, 150, 150 }; // 왼쪽 상단 좌표 (50, 50)에서 오른쪽 하단 좌표 (150, 150)까지의 사각형
		//

		//// 그리기
		//FillRect(hdc, &rect, (HBRUSH)(COLOR_WINDOW+1)); // 사각형을 빨간색으로 채우기
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
		
		 // 윈도우의 그래픽 컨텍스트를 얻고, 그림을 그릴 수 있게 함
		HBRUSH hBrush_eraser = CreateSolidBrush(RGB(255, 255, 255)); // 지우개

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

			HBRUSH hBrush = CreateSolidBrush(RGB(255, 0, 255)); // 핑크 브러시 생성
			FillRect(hdc, &rectrect, hBrush);

			DeleteObject(hBrush);

			//MoveToEx(hdc, startPoint.x, startPoint.y, NULL);
			//LineTo(hdc, endPoint.x, endPoint.y);
			EndPaint(hwnd, &ps);
			break;
		}

		if (isMouseRButtonPressed) {
			// 현재 상태에 따라 도형을 그릴 수 있습니다.

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
	case WM_GETMINMAXINFO:  // 확대축소 불가
	{
		MINMAXINFO* pMinMaxInfo = (MINMAXINFO*)lParam;
		pMinMaxInfo->ptMinTrackSize.x = 800; // 최소 가로 크기
		pMinMaxInfo->ptMinTrackSize.y = 600; // 최소 세로 크기
		pMinMaxInfo->ptMaxTrackSize.x = 800; // 최대 가로 크기
		pMinMaxInfo->ptMaxTrackSize.y = 600; // 최대 세로 크기
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
//		return 1; // 도형과 겹침
//}
//	return 0; // 두 RECT가 겹치지 않음
//}



#ifdef UNICODE
int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nShowCmd)
#else
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR pCmdLine, _In_ int nCmdShow)
#endif
{
	/* 윈도우 클래스 선언.*/
	WNDCLASS wc;
	ZeroMemory(&wc, sizeof(wc));	// 모두 0으로 초기화.

	// 윈도우 클래스 값 설정
	wc.hInstance = hInstance;
	wc.lpszClassName = TEXT("Computer Software");
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_CROSS);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WindowProc;

	// 윈도우 클래스 등록.
	if (RegisterClass(&wc) == 0)
	{
		MessageBox(NULL, L"RegisterClass failed!", L"Error", MB_ICONERROR);
		exit(-1);	//예외
	}

	// Window viewport 영역 조정
	RECT rect = { 150, 100, 800, 600 };
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, 0);
	int width = rect.right - rect.left;
	int height = rect.bottom - rect.top;

	// 윈도우 생성
	HWND hwnd = CreateWindow(
		wc.lpszClassName,
		TEXT("[201907006 정대건]"),
		WS_OVERLAPPEDWINDOW,
		0, 0,
		width, height,
		NULL, NULL,
		hInstance,
		NULL
	);

	// 오류 검사.
	if (hwnd == NULL)
	{
		MessageBox(NULL, L"CreateWindow failed!", L"Error", MB_ICONERROR);
		exit(-1);
	}

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
			//if (PeekMessage(&msg, hwnd, 0, 0, PM_REMOVE)) // PM_REMOVE의 자리는 이 메세지를 쓰고 어떡할거냐.의 의미인데 지운다는 것임.
		{

			// 메시지 해석해줘.
			TranslateMessage(&msg);
			// 메시지를 처리해야할 곳에 전달해줘.
			DispatchMessage(&msg);

		}
		/*else
		{

		}*/
	}

	//종료 메시지 보내기
	return (int)msg.wParam;

}


//bool InRect(POINT startPoint, RECT rect1)
//{
//	if ((startPoint.x > rect1.left && startPoint.x < rect1.right) &&
//		(startPoint.y > rect1.top && startPoint.y < rect1.bottom))
//	{
//		return 1; // 도형과 겹침
//	}
//	return 0; // 두 RECT가 겹치지 않음
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
