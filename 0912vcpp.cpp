#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string>
#include <windows.h>
#include <conio.h>
using namespace std;

int main() {



    for (int i = 0; i < 5; i++) {
        if (i == 0) {
            cout << "┌──────────────────────────────────┐\n";
            continue;
        }
        if (i == 2) {
            cout << "│    [week2] vcpp command input    │\n";
            cout << "│         201907006 정대건         │\n";
            continue;      // continue : 특정 사이클만 종료
        }
        if (i == 4) {
            cout << "└──────────────────────────────────┘\n";
            continue;
        }
        cout << "│                                  │\n";
    }
    // 스플래쉬 이미지 (처음과 마지막 사이클을 for문에 넣는게 효율적인지 아닐지 고민하다가 그냥 넣었습니다ㅠㅠ)

    // Disable line buffering on cin.

    while (true) {

        cout << "command>";
        string inp;
        getline(cin, inp);
        if ((inp[0] >= 'A' && inp[0] <= 'Z') || (inp[0] >= 'a' && inp[0] <= 'z')) {
            switch (inp[0]) {
            case 'q':
                cout << "qwer 화이팅.\n";
                break;
            case 'w':
                cout << "과제 너무 싫다.\n";
                break;
            case 'e':
                cout << "담주부턴 과제량 0배다.\n";
                break;
            case 'r':
                cout << "불행합니다.\n";
                break;
            default:
                cout << "예쁜이인가? 다시 입력해\n";
                break;
            }
        }
        else {
            inp[0] = _getch();
            if (inp[0] == 27) {
                exit(1);
            }


            // ESC 종료 못하겠습니다 ㅠㅠㅠㅠ

        }

    }
}

/*
    const int ESC = 27;
*/