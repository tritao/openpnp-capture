/*

    OpenPnp-Capture: a video capture subsystem.

    KSJ Stream class

    Copyright (c) 2020 Joao Matos.
*/

#include "platformdeviceinfo.h"
#include "platformstream.h"
#include "platformcontext.h"
#include "scopedcomptr.h"

#ifdef KSJAPI

#include <KSJApi.h>

bool PlatformStream::ksj_open(Context* owner, deviceInfo* device, uint32_t width, uint32_t height,
    uint32_t fourCC, uint32_t fps)
{
    m_deviceId = atoi(&device->m_name.at(strlen("KSJ_Device")));

    // Parameters copied from SMT app init_KJS_Camera().
    int nColStart = 324;
    int nRowStart = 0;
    int nColSize = 1944;
    int nRowSize = 1944;
    KSJ_ADDRESSMODE addressMode = KSJ_SKIPNONE;

    int result = KSJ_CaptureSetFieldOfView(m_deviceId, nColStart, nRowStart, nColSize, nRowSize,
        addressMode, addressMode);
    if (result != RET_SUCCESS)
    {
        LOG(LOG_WARNING, "KSJ_CaptureSetFieldOfView failed for device %d (result = %d)!\n",
            m_deviceId, result);
    }

    /*
    result = KSJ_PreviewSetFieldOfView(m_deviceId, nColStart, nRowStart, nColSize, nRowSize,
        addressMode, addressMode);
    if (result != RET_SUCCESS)
    {
        LOG(LOG_WARNING, "KSJ_PreviewSetFieldOfView failed for device %d (result = %d)!\n",
            m_deviceId, result);
    }
    */

    // Parameters copied from SMT app init_KJS_Camera().
    KSJ_SetParam(0, KSJ_RED, 0);
    KSJ_SetParam(0, KSJ_GREEN, 15);
    KSJ_SetParam(0, KSJ_BLUE, 15);
    KSJ_SetParam(0, KSJ_EXPOSURE_LINES, 500);
    KSJ_SetParam(0, KSJ_BRIGHTNESS, 60);
    KSJ_SetParam(0, KSJ_CONTRAST, 25);

    m_frameBuffer.resize(m_width * m_height * 3);
    ksj_requestBitmap();

    m_isOpen = true;
    return true;
}

void PlatformStream::ksj_close()
{
}

void PlatformStream::ksj_requestBitmap()
{
    LOG(LOG_ERR, "jvs_requestBitmap\n");

    m_bufferMutex.lock();

    int result = KSJ_CaptureRgbData(m_deviceId, m_frameBuffer.data());
    if (result != RET_SUCCESS)
    {
        LOG(LOG_WARNING, "KSJ_CaptureRgbData failed for device %d (result = %d)!\n",
            m_deviceId, result);
    }

    m_bufferMutex.unlock();

    m_newFrame = true;
    m_frames++;
}

void PlatformStream::ksj_writeBitmap()
{
    int nWidth = m_width;
    int nHeight = m_height;
    int nBits = 3;
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
    fwrite(m_frameBuffer.data(), nWidth * nHeight * nBits, 1, fout);

    fclose(fout);
}

#endif
