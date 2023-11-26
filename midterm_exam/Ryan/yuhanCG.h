// yuhanCG.h
#pragma once

#include <windows.h>

void DrawBox(HWND hwnd, HDC hdc, POINT startPointBox, POINT endPointBox);

INT DrawCircle(HWND hwnd, HDC hdc, POINT startPoint, POINT endPoint, INT radius, BOOL L_BOOL);

void DrawBonobono(HWND hwnd, HDC hdc, int blink);

void DrawRyan(HWND hwnd, HDC hdc, POINT startPoint, POINT endPoint);

void DrawCube(HWND hwnd, HDC hdc, POINT startPointCube, POINT endPointCube, POINT cubePoints[], BOOL REZISE_BOOL);



