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
int isKeyPressed = 0;           // ����-���� �ʱ�ȭ, Ư�� Ű�� ���ȴ��� Ȯ��

RECT rect_user = { 5, 5, 10, 10 }; // ���� ��� ��ǥ (50, 50)���� ������ �ϴ� ��ǥ (150, 150)������ �簢��
RECT rect_target = { 50, 50, 150, 150 }; // ���� ��� ��ǥ (50, 50)���� ������ �ϴ� ��ǥ (150, 150)������ �簢��
// left, top, right, bottom

bool RectIntersect(const RECT* rect1, const RECT* rect2)
{
	if (rect1->left < rect2->right && rect1->right > rect2->left &&
		rect1->top < rect2->bottom && rect1->bottom > rect2->top)
	{
		return 1; // �� RECT�� ��ħ
	}
	return 0; // �� RECT�� ��ġ�� ����
}

// �������� �̺�Ʈ�� ó���ϴ� �ݹ�(Callback) �Լ�.
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	HDC hdc = GetDC(hwnd);  // window �ڵ� ��������

	HBRUSH hBrush_user = CreateSolidBrush(RGB(0, 0, 255));       // indigo, pink, white
	HBRUSH hBrush_target = CreateSolidBrush(RGB(255, 0, 255));
	HBRUSH hBrush_eraser = CreateSolidBrush(RGB(255, 255, 255));
	const wchar_t* textDefault = L"Stopping";
	const wchar_t* textCrash = L"Crash!!!";
	const wchar_t* textCrash1 = L"asdsaCrash!!!";

	switch (uMsg)
	{
	case WM_KEYDOWN: // Ű�� ������ ȣ�� >> isKey�� 1�� ������Ű��
		isKeyPressed = 1;   
		if (wParam == VK_RIGHT)   // ���� ������ ����Ű���
		{
			rect_user.left += 5;
			rect_user.right += 5;              // �ű�� ������ �ƴ� left-right�� �ٲ� ���� �׸��� ����
			InvalidateRect(hwnd, NULL, TRUE);  // ��ǥ�� �����ϰ� �ٽ� �׸�
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
		if (isKeyPressed)   // Ű�� ������(1) ����
		{
			FillRect(hdc, &rect_target, hBrush_target);
			FillRect(hdc, &rect_user, hBrush_user);     // hdc�� rect_user �簢���� hBrush_user�� ä��
		}
		else
		{
			TextOut(hdc, 10, 10, textDefault, lstrlen(textDefault));   // printf�� ����� API�� ��¹�
			FillRect(hdc, &rect_target, hBrush_target);
			FillRect(hdc, &rect_user, hBrush_user); 
			// rect_user�� �������� �ʵ��� ���߿� ������ ���� ���� �������� ���
		}

		// ����� �簢���� Ÿ�� �簢���� �浹�� �˻�
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
	/* ������ Ŭ���� ����.*/
	WNDCLASS wc;
	ZeroMemory(&wc, sizeof(wc));	// ��� 0���� �ʱ�ȭ.

	// ������ Ŭ���� �� ����
	wc.hInstance = hInstance;
	wc.lpszClassName = TEXT("Computer Software");
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_CROSS);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
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

	// �޽��� ó��.
	while (msg.message != WM_QUIT)
	{
		if (GetMessage(&msg, NULL, 0, 0))
		{
			// �޽��� �ؼ�����.
			TranslateMessage(&msg);
			// �޽����� ó���ؾ��� ���� ��������.
			DispatchMessage(&msg);
		}
		//if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		//{
		//	// �޽��� �ؼ�����.
		//	TranslateMessage(&msg);
		//	// �޽����� ó���ؾ��� ���� ��������.
		//	DispatchMessage(&msg);
		//} // PM_REMOVE�� �ڸ��� �� �޼����� ���� ��Ұų�.�� �ǹ��ε� ����ٴ� ����.
		//else{}
	}

	UnregisterClass(wc.lpszClassName, wc.hInstance);

	//���� �޽��� ������
	return (int)msg.wParam;

}