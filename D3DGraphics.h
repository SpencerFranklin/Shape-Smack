/*
TEAM FRANKLIN
*/
#pragma once

#include <d3d9.h>

struct Font {
	int charWidth;
	int charHeight;
	int nCharsPerRow;
	D3DCOLOR* surface;
};

void LoadFont(Font* font, D3DCOLOR* surface, const char* filename,
	int charWidth, int charHeight, int nCharsPerRow);

class D3DGraphics
{
public:
	D3DGraphics(HWND hWnd);
	~D3DGraphics();
	void PutPixel(int x, int y, int r, int g, int b);
	void PutPixel(int x, int y, D3DCOLOR c);
	void BeginFrame();
	void EndFrame();
	void DrawChar(char c, int x, int y, Font* font, D3DCOLOR color);
	void DrawSurface(int x, int y, int width, int height, D3DCOLOR key, const D3DCOLOR* surf);



private:
	IDirect3D9*			pDirect3D;
	IDirect3DDevice9*	pDevice;
	IDirect3DSurface9* pBackBuffer;
	D3DLOCKED_RECT rect;
};

