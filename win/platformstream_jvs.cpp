/*

    OpenPnp-Capture: a video capture subsystem.

    JVS Stream class

    Copyright (c) 2020 Joao Matos.
*/

#include "platformdeviceinfo.h"
#include "platformstream.h"
#include "platformcontext.h"
#include "scopedcomptr.h"

#ifdef JVSDK

#include <JVSDK.h>

#define WM_SDK_NOTIFY (WM_USER + 0x600)
LRESULT CALLBACK jvs_WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

HANDLE PlatformStream::g_hCaptureThread = 0;
unsigned int PlatformStream::g_captureThreadId = 0;
JVSCaptureThreadState PlatformStream::g_captureThreadState = JVSCaptureThreadState::Initialize;
JVSCaptureState* PlatformStream::g_currentCapture = 0;
JVSCaptureState PlatformStream::g_cameraCaptureStates[];

//moodycamel::BlockingConcurrentQueue<JVSCaptureRequest> PlatformStream::g_captureRequestsQueue;

ATOM PlatformStream::g_atom = 0;
HWND PlatformStream::g_hwnd = 0;
RECT PlatformStream::g_rect;

int PlatformStream::g_width = 0;
int PlatformStream::g_height = 0;
unsigned char* PlatformStream::g_pRGBA = 0;

bool PlatformStream::jvs_open(Context* owner, deviceInfo* device, uint32_t width, uint32_t height,
    uint32_t fourCC, uint32_t fps)
{
	m_width = width;
	m_height = height;
	m_channelId = atoi(&device->m_name.at(strlen("JVS_Channel")));
	m_isOpen = false;

	if (g_hCaptureThread == 0)
	{
		g_hCaptureThread = (HANDLE)_beginthreadex(NULL, 0, &PlatformStream::jvs_captureThread,
			this, 0, &g_captureThreadId);

		g_width = width;
		g_height = height;
	}

	JVS_OpenChannel(m_channelId);

	// Spin loop while the thread is initializing.
	while (g_captureThreadState == JVSCaptureThreadState::Initialize)
	{
	}

	if (g_captureThreadState == JVSCaptureThreadState::Error)
		return false;

	m_frameBuffer.resize(m_width * m_height * 3);

	g_cameraCaptureStates[m_channelId].channelId = m_channelId;
	g_cameraCaptureStates[m_channelId].stream = this;
	g_cameraCaptureStates[m_channelId].isOpen = true;
	m_isOpen = true;

	//jvs_requestBitmap();

    return true;
}

void PlatformStream::jvs_close()
{
	JVS_CloseChannel(m_channelId);

	g_cameraCaptureStates[m_channelId].isOpen = false;
	g_cameraCaptureStates[m_channelId].stream = 0;

	//PostThreadMessage(g_captureThreadId, WM_QUIT, 0, 0);
	//g_captureThreadState = JVSCaptureThreadState::Exit;
	//WaitForSingleObject(g_hCaptureThread, INFINITE);
}

void PlatformStream::jvs_requestBitmap()
{
	LOG(LOG_DEBUG, "PlatformStream::jvs_requestBitmap: %d\n", m_channelId);

#if 0
	JVSCaptureRequest request = {};

	JVSCaptureState& request = g_cameraCaptureStates[m_channelId];
	request.channelId = m_channelId;
	request.stream = this;
	request.isProcessed = false;
#endif

	//g_captureRequestsQueue.enqueue(request);
}

bool PlatformStream::jvs_initWindow()
{
	HINSTANCE hInstance = GetModuleHandle(NULL);
	static const char* className = "DUMMY_CLASS";

	WNDCLASSEX wx = {};
	wx.cbSize = sizeof(WNDCLASSEX);
	wx.lpfnWndProc = jvs_WindowProc;
	wx.hInstance = hInstance;
	wx.lpszClassName = className;

	g_atom = RegisterClassEx(&wx);

	if (!g_atom)
	{
		LOG(LOG_ERR, "Failed to register JVS native window class\n");
		return false;
	}

	g_hwnd = CreateWindowEx(0, className, "dummy_name", 0,
		CW_USEDEFAULT, CW_USEDEFAULT, g_width, g_height,
		NULL, NULL, hInstance, 0);

	if (!g_hwnd)
	{
		LOG(LOG_ERR, "Failed to create JVS native window\n");
		return false;
	}

	if (!GetWindowRect(g_hwnd, &g_rect))
	{
		LOG(LOG_ERR, "Failed to get JVS native window rect\n");
		return false;
	}

	return true;
}

unsigned WINAPI PlatformStream::jvs_captureThread(void* pParam)
{
	if (!jvs_initWindow())
	{
		g_captureThreadState = JVSCaptureThreadState::Error;
		return -1;
	}

	if (!g_pRGBA)
		g_pRGBA = new unsigned char[g_width * g_height * 4];

	g_captureThreadState = JVSCaptureThreadState::Run;

	while (g_captureThreadState != JVSCaptureThreadState::Exit)
	{
		//JVSCaptureRequest request;
		//g_captureRequestsQueue.wait_dequeue(request);

		for (int i = 0; i < 8; i++)
		{
			g_currentCapture = &g_cameraCaptureStates[i];
			if (!g_currentCapture->isOpen)
				continue;

			g_currentCapture->isProcessed = false;

			JVS_RegisterNotify(g_hwnd, WM_SDK_NOTIFY);
			JVS_SetVideoPreview(g_currentCapture->channelId, g_hwnd, &g_rect, /*preview=*/true);

			bool result = JVS_GetBitmap(g_currentCapture->channelId, g_pRGBA);
			//LOG(LOG_ERR, "JVS_GetBitmap %d\n", g_currentCapture->channelId);
			if (!result)
			{
				LOG(LOG_ERR, "Failed to request bitmap to JVS capture card channel %d\n",
					g_currentCapture->channelId);
			}

			// Wait until the current request is processed.
			while (!g_currentCapture->isProcessed)
				jvs_messagePump();
		}
	}

	if (g_pRGBA)
		delete[] g_pRGBA;

	return 0;
}

void PlatformStream::jvs_messagePump()
{
	MSG msg;
    while (PeekMessage(&msg, g_hwnd, 0, 0, PM_REMOVE))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

LRESULT PlatformStream::jvs_onSDKNotify(WPARAM wParam, LPARAM lParam)
{
    //LOG(LOG_DEBUG, "PlatformStream::jvs_onSDKNotify: %d\n", m_channelId);

    if (lParam == JVS_WM_BITMAP_OK)
    {
        int nChannel = wParam;
        if (nChannel < 0 || nChannel > 8)
            return FALSE;

        if (g_pRGBA == NULL)
            return FALSE;

        m_bufferMutex.lock();

        const uint8_t* src = g_pRGBA;
        uint8_t* dst = m_frameBuffer.data();
        for (uint32_t pixel = 0; pixel < m_width * m_height; pixel++)
        {
            uint8_t r = *src++;
            uint8_t g = *src++;
            uint8_t b = *src++;
            *src++;

            *dst++ = b;
            *dst++ = g;
            *dst++ = r;
        }

        m_newFrame = true;
        m_frames++;

        m_bufferMutex.unlock();

		g_currentCapture->isProcessed = true;
    }

    return TRUE;
}

LRESULT CALLBACK jvs_WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    PlatformStream* pThis;
    if (uMsg == WM_CREATE)
    {
        LPCREATESTRUCT lpcs = reinterpret_cast<LPCREATESTRUCT>(lParam);
        pThis = static_cast<PlatformStream*>(lpcs->lpCreateParams);
        SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pThis));
    }
    else
    {
        pThis = reinterpret_cast<PlatformStream*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
    }

    switch (uMsg)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    case WM_SDK_NOTIFY:
		PlatformStream::g_currentCapture->stream->jvs_onSDKNotify(wParam, lParam);
        return 0;
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

void PlatformStream::jvs_writeBitmap()
{
    int nWidth = m_width;
    int nHeight = m_height;
    int nBits = 4;
    ULONG ulSize = nWidth * nHeight * nBits;
    FILE* fout = fopen("bitmap.bmp", "wb");
    if (!fout)
        return;

    BITMAPFILEHEADER bmHeader;
    memset(&bmHeader, 0, sizeof(bmHeader));
    bmHeader.bfType = 0x4D42;
    bmHeader.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + nWidth * nHeight * nBits;
    bmHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

    BITMAPINFOHEADER bmInfo;
    memset(&bmInfo, 0, sizeof(bmInfo));
    bmInfo.biSize = sizeof(bmInfo);
    bmInfo.biWidth = nWidth;
    bmInfo.biHeight = nHeight;
    bmInfo.biPlanes = 1;
    bmInfo.biBitCount = nBits * 8;
    bmInfo.biCompression = 0;
    bmInfo.biSizeImage = nWidth * nHeight * nBits;
    bmInfo.biXPelsPerMeter = 300;
    bmInfo.biYPelsPerMeter = 300;

    fwrite(&bmHeader, sizeof(bmHeader), 1, fout);
    fwrite(&bmInfo, sizeof(bmInfo), 1, fout);
    fwrite(g_pRGBA, nWidth * nHeight * nBits, 1, fout);

    fclose(fout);
}

#endif
