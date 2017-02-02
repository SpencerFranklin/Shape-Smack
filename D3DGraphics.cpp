/*
TEAM FRANKLIN
*/
#include "D3DGraphics.h"
#include "Bitmap.h"

D3DGraphics::D3DGraphics(HWND hWnd)
	: pBackBuffer(NULL)
{
	pDirect3D = Direct3DCreate9(D3D_SDK_VERSION);

	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.Windowed = TRUE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_ONE;
	d3dpp.Flags = D3DPRESENTFLAG_LOCKABLE_BACKBUFFER;

	pDirect3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_PUREDEVICE, &d3dpp, &pDevice);
}

D3DGraphics::~D3DGraphics()
{
	if (pDevice)
	{
		pDevice->Release();
		pDevice = NULL;
	}
	if (pDirect3D)
	{
		pDirect3D->Release();
		pDirect3D = NULL;
	}
}

void D3DGraphics::PutPixel(int x, int y, int r, int g, int b)
{



	((D3DCOLOR*)rect.pBits)[x + (rect.Pitch >> 2) * y] = D3DCOLOR_XRGB(r, g, b);

}
void D3DGraphics::PutPixel(int x, int y, D3DCOLOR c) {


	((D3DCOLOR*)rect.pBits)[x + (rect.Pitch >> 2) * y] = c;


}

void D3DGraphics::BeginFrame()
{
	pDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 0.0f, 0);
	pDevice->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &pBackBuffer);
	pBackBuffer->LockRect(&rect, NULL, NULL);
}

void D3DGraphics::EndFrame()
{
	pBackBuffer->UnlockRect();
	pBackBuffer->Release();
	pDevice->Present(NULL, NULL, NULL, NULL);
}

void LoadFont(Font* font, D3DCOLOR* surface, const char* filename,
	int charWidth, int charHeight, int nCharsPerRow) {

	LoadBmp(filename, surface);
	font->charHeight = charHeight;
	font->charWidth = charWidth;
	font->nCharsPerRow = nCharsPerRow;
	font->surface = surface;

}
void D3DGraphics::DrawChar(char c, int xoff, int yoff, Font* font, D3DCOLOR color) {

	const int sheetIndex = c - ' ';
	const int sCol = sheetIndex % font->nCharsPerRow;
	const int sRow = sheetIndex / font->nCharsPerRow;
	const int xStart = sCol * font->charWidth;
	const int yStart = sRow * font->charHeight;
	const int xEnd = xStart + font->charWidth;
	const int yEnd = yStart + font->charHeight;
	const int surfWidth = font->charWidth * font->nCharsPerRow;

	for (int y = yStart; y < yEnd; y++) {
		for (int x = xStart; x < xEnd; x++) {
			if (font->surface[x + y * surfWidth] == D3DCOLOR_XRGB(0, 0, 0)) {
				PutPixel(x + xoff - xStart, y + yoff - yStart, color);
			}

		}

	}

}

void D3DGraphics::DrawSurface(int xoff, int yoff, int width, int height, D3DCOLOR key, const D3DCOLOR* surf) {

	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			D3DCOLOR c = surf[x + y * width];
			if (c != key) {
				PutPixel(x + xoff, y + yoff, c);

			}
		}
	}

}