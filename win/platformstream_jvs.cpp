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
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

bool PlatformStream::jvs_open(Context* owner, deviceInfo* device, uint32_t width, uint32_t height,
    uint32_t fourCC, uint32_t fps)
{
    HINSTANCE hInstance = GetModuleHandle(NULL);
    static const char* className = "DUMMY_CLASS";

    WNDCLASSEX wx = {};
    wx.cbSize = sizeof(WNDCLASSEX);
    wx.lpfnWndProc = WindowProc;
    wx.hInstance = hInstance;
    wx.lpszClassName = className;

    if (!RegisterClassEx(&wx))
    {
        LOG(LOG_ERR, "Failed to register JVS native window class\n");
        return false;
    }

    m_hwnd = CreateWindowEx(0, className, "dummy_name", 0,
        CW_USEDEFAULT, CW_USEDEFAULT, width, height,
        NULL, NULL, hInstance, this);

    if (!m_hwnd)
    {
        LOG(LOG_ERR, "Failed to create JVS native window\n");
        return false;
    }

    m_channelId = atoi(&device->m_name.at(strlen("JVS_Channel")));

    RECT rect;
    if (!GetWindowRect(m_hwnd, &rect))
    {
        LOG(LOG_ERR, "Failed to get JVS native window rect\n");
        return false;
    }

    m_width = width;
    m_height = height;
    m_pRGB = NULL;

    JVS_RegisterNotify(m_hwnd, WM_SDK_NOTIFY);
    JVS_SetVideoPreview(m_channelId, m_hwnd, &rect, /*preview=*/true);

    if (!m_pRGB)
    {
        m_pRGB = new unsigned char[m_width * m_height * 4];
        m_frameBuffer.resize(m_width * m_height * 3);

        jvs_requestBitmap();
    }

    m_isOpen = true;
    return true;
}

void PlatformStream::jvs_requestBitmap()
{
    LOG(LOG_ERR, "jvs_requestBitmap\n");

    bool result = JVS_GetBitmap(m_channelId, m_pRGB);
    if (!result)
    {
        LOG(LOG_ERR, "Failed to request bitmap to JVS capture card channel %d\n",
            m_channelId);
    }
}

void PlatformStream::jvs_messagePump()
{
    MSG msg;
    while (PeekMessage(&msg, m_hwnd, 0, 0, PM_REMOVE))
    {
        LOG(LOG_ERR, "PeekMessage loop\n");
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

void PlatformStream::jvs_close()
{
    if (m_pRGB)
        delete[] m_pRGB;
}

LRESULT PlatformStream::jvs_onSDKNotify(WPARAM wParam, LPARAM lParam)
{
    LOG(LOG_ERR, "jvs_onSDKNotify\n");

    if (lParam == JVS_WM_BITMAP_OK)
    {
        int nChannel = wParam;
        if (nChannel < 0 || nChannel > 8)
            return FALSE;

        if (m_pRGB == NULL)
            return FALSE;

        m_bufferMutex.lock();

        const uint8_t* src = m_pRGB;
        uint8_t* dst = m_frameBuffer.data();
        for (uint32_t pixel = 0; pixel < m_width * m_height; pixel++)
        {
            uint8_t r = *src++;
            uint8_t g = *src++;
            uint8_t b = *src++;
            *src++;

            *dst++ = r;
            *dst++ = g;
            *dst++ = b;
        }

        m_newFrame = true;
        m_frames++;
        LOG(LOG_ERR, "jvs_onSDKNotify=> new frame\n");

        m_bufferMutex.unlock();

        //jvs_requestBitmap();
    }

    return TRUE;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
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
        pThis->jvs_onSDKNotify(wParam, lParam);
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
    fwrite(m_pRGB, nWidth * nHeight * nBits, 1, fout);

    fclose(fout);
}

#endif
