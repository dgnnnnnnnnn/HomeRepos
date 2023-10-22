//#define DEBUG
#ifdef UNICODE
#ifdef DEBUG
#pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console")
#else
#pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:WINDOWS")
#endif // DEBUG
#else
#ifdef DEBUG
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
#else
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:WINDOWS")
#endif // DEBUG
#endif

typedef int bool;
#define true 1
#define false 0

#define TIMER_ID 1

#include <windows.h>

POINT startPoint = { 0 };         
POINT endPoint = { 0 };           
int isKeyPressed = 0;           // 시작-끝점 초기화, 특정 키가 눌렸는지 확인

RECT rect_user = { 5, 5, 10, 10 }; // 왼쪽 상단 좌표 (50, 50)에서 오른쪽 하단 좌표 (150, 150)까지의 사각형
RECT rect_target = { 50, 50, 150, 150 }; // 왼쪽 상단 좌표 (50, 50)에서 오른쪽 하단 좌표 (150, 150)까지의 사각형
// left, top, right, bottom

bool RectIntersect(const RECT* rect1, const RECT* rect2)
{
	if (rect1->left < rect2->right && rect1->right > rect2->left &&
		rect1->top < rect2->bottom && rect1->bottom > rect2->top)
	{
		return 1; // 두 RECT가 겹침
	}
	return 0; // 두 RECT가 겹치지 않음
}

// 윈도우의 이벤트를 처리하는 콜백(Callback) 함수.
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	HDC hdc = GetDC(hwnd);  // window 핸들 가져오기

	HBRUSH hBrush_user = CreateSolidBrush(RGB(0, 0, 255));       // indigo, pink, white
	HBRUSH hBrush_target = CreateSolidBrush(RGB(255, 0, 255));
	HBRUSH hBrush_eraser = CreateSolidBrush(RGB(255, 255, 255));
	const wchar_t* textDefault = L"Stopping";
	const wchar_t* textCrash = L"Crash!!!";
	const wchar_t* textCrash1 = L"asdsaCrash!!!";

	switch (uMsg)
	{
	case WM_KEYDOWN: // 키가 눌리면 호출 >> isKey를 1로 증가시키고
		isKeyPressed = 1;   
		if (wParam == VK_RIGHT)   // 만약 오른쪽 방향키라면
		{
			rect_user.left += 5;
			rect_user.right += 5;              // 옮기는 개념이 아닌 left-right만 바꿔 새로 그리는 개념
			InvalidateRect(hwnd, NULL, TRUE);  // 좌표를 조정하고 다시 그림
		}

		if (wParam == VK_LEFT)
		{
			rect_user.left -= 5;
			rect_user.right -= 5;
			InvalidateRect(hwnd, NULL, TRUE);
		}
		if (wParam == VK_UP)
		{
			rect_user.top -= 5;
			rect_user.bottom -= 5;
			InvalidateRect(hwnd, NULL, TRUE);
		}
		if (wParam == VK_DOWN)
		{
			rect_user.top += 5;
			rect_user.bottom += 5;
			InvalidateRect(hwnd, NULL, TRUE);
		}

		break;
	case WM_KEYUP:
		isKeyPressed = 0;
		break;
	case WM_PAINT:
	{
		if (isKeyPressed)   // 키가 눌리면(1) 수행
		{
			FillRect(hdc, &rect_target, hBrush_target);
			FillRect(hdc, &rect_user, hBrush_user);     // hdc의 rect_user 사각형을 hBrush_user로 채움
		}
		else
		{
			TextOut(hdc, 10, 10, textDefault, lstrlen(textDefault));   // printf와 비슷한 API용 출력문
			FillRect(hdc, &rect_target, hBrush_target);
			FillRect(hdc, &rect_user, hBrush_user); 
			// rect_user가 가려지지 않도록 나중에 선언해 덮어 쓰는 느낌으로 사용
		}

		// 사용자 사각형과 타겟 사각형의 충돌을 검사
		if (RectIntersect(&rect_user, &rect_target))
		{
			TextOut(hdc, 100, 10, textCrash, lstrlen(textCrash));
		}

	}
	break;
	case WM_CLOSE:
		DestroyWindow(hwnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}

	DeleteObject(hBrush_user);
	DeleteObject(hBrush_target);
	DeleteObject(hBrush_eraser);
	ReleaseDC(hwnd, hdc);

	return S_OK;
}
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
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
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

	// 메시지 처리.
	while (msg.message != WM_QUIT)
	{
		if (GetMessage(&msg, NULL, 0, 0))
		{
			// 메시지 해석해줘.
			TranslateMessage(&msg);
			// 메시지를 처리해야할 곳에 전달해줘.
			DispatchMessage(&msg);
		}
		//if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		//{
		//	// 메시지 해석해줘.
		//	TranslateMessage(&msg);
		//	// 메시지를 처리해야할 곳에 전달해줘.
		//	DispatchMessage(&msg);
		//} // PM_REMOVE의 자리는 이 메세지를 쓰고 어떡할거냐.의 의미인데 지운다는 것임.
		//else{}
	}

	UnregisterClass(wc.lpszClassName, wc.hInstance);

	//종료 메시지 보내기
	return (int)msg.wParam;

}