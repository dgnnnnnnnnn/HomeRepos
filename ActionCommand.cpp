#include <iostream>
#include <conio.h>
#include <vector>
#include <chrono>
#include <string>
#include <regex>
using namespace std;

#define UP 72
#define DOWN 80
#define LEFT 75
#define RIGHT 77


// inputkey(char) >> command(string) >> inputBuffer(array)
int main() {
    // char형은 키보드 방향키같은 특문을 입력받지 못함
    vector<string> inputBuffer;
    vector<string> finalBuffer;
    string command, checkAct;
    char inputKey;

    // 현재 시간을 가져오는 함수
    auto startTime = std::chrono::high_resolution_clock::now();

    while (true) {

        if (_kbhit()) {  // 1. while문안에서 키보드 눌렸을 시 if문이 실행된다.
            inputKey = _getch(); // 키 입력을 읽음

          
           

            // 출력용 command에 inputKey쌓기
            if (inputKey == -32) {
                inputKey = _getch();

                if (inputKey == UP) {
                    inputBuffer.push_back("↑"); // 위쪽 화살표 키 (↑)
                }
                else if (inputKey == DOWN) {
                    inputBuffer.push_back("↓"); // 아래쪽 화살표 키 (↓)
                }
                else if (inputKey == LEFT) {
                    inputBuffer.push_back("←"); // 왼쪽 화살표 키 (←)
                }
                else if (inputKey == RIGHT) {
                    inputBuffer.push_back("→"); // 오른쪽 화살표 키 (→)
                }



            }
            else {
                // char >> string 형변환 후 inputBuffer에 삽입
                string convertKey(1, inputKey);
                inputBuffer.push_back(convertKey);
            }


            // 이후 화살표 키 처리 로직 추가
            if (inputKey == 27) { // ESC 키를 누르면 프로그램 종료
                std::cout << "프로그램을 종료합니다." << std::endl;
                break;
            }
            // 화면을 지우고 현재 입력된 커맨드를 표시
            


            // 현재 시간을 기록, 프로그램 시작 후 경과한 시간을 밀리초 단위로 계산
            auto currentTime = std::chrono::high_resolution_clock::now();
            auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - startTime).count();


            if (elapsedTime >= 300) {
                finalBuffer.push_back(inputBuffer.back());

                
                //for (auto iter : finalBuffer) {
                //    command += iter;
                //}
                for (int i = 0; i < finalBuffer.size(); i++) {
                    command += finalBuffer[i];
                }

                /* >> 콘솔화면 클리어 
                   >> command> 문구 출력 
                   >> finalBuffer 값을 쌓은 command 출력
                   >> 정규식으로 패턴 검사(일치하면 문구출력)
                   >> command 초기화                         */
                system("cls");
                cout << "command> " << command;
                  

                // 정규식 3개
                regex patternOne(".{0,3}↓→a$");
                regex patternTwo(".{0,3}yenn$");
                regex patternThree(".{0,3}↑u1$");

                if (regex_search(command, patternOne)) {

                    // 클리어 후 기술 문구 출력
                    system("cls");
                    cout << "아도겐! =o";
                }
                else if(regex_search(command, patternTwo)) {
                    system("cls");
                    cout << "저격성공!";
                }
                else if (regex_search(command, patternThree)) {
                    system("cls");
                    cout << "타이가아 숏토!";
                }
                command = "";
                
                // 현재 시간으로 재설정
                startTime = std::chrono::high_resolution_clock::now();
            }

        }

    }
    return 0;
}


/*
* 키보드를 입력한 순간부터 0.3초마다(300ms) 동안 입력을 쌓은 후에
↓→a 순으로 입력이 됐다면
"아도겐! =o" 이라고 출력한다.
총 3개의 기술을 만들어보시오.

("asdf↓→a" 처럼 이상한 단어가 섞여있어도 인식시킬것")
아무때나 esc를 누르면 종료하도록 할것.

1. 시간제한, 입력을 문자열에 쌓고
2. 검사
*/
