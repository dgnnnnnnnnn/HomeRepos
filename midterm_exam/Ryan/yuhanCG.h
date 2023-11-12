// yuhanCG.h
#pragma once

#include <windows.h>


/* 마우스좌표는 기존에 point형으로 받도록 만들어뒀었어서 int형 대신 했습니다 죄송합니다 ㅠㅠㅠ */
void DrawRyan(HWND hwnd, HDC hdc, POINT startPoint, POINT endPoint);

void DrawBonobono(HWND hwnd, HDC hdc, int blink);

