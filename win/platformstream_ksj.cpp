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
    if (!m_pRGB)
    {
        m_pRGB = new unsigned char[m_width * m_height * 4];
        m_frameBuffer.resize(m_width * m_height * 3);

        //jvs_requestBitmap();
    }

    m_isOpen = true;
    return true;
}

void PlatformStream::ksj_close()
{
    //if (m_pRGB)
        //delete[] m_pRGB;
}

void PlatformStream::ksj_requestBitmap()
{
    LOG(LOG_ERR, "jvs_requestBitmap\n");
    /*
    bool result = JVS_GetBitmap(m_channelId, m_pRGB);
    if (!result)
    {
        LOG(LOG_ERR, "Failed to request bitmap to JVS capture card channel %d\n",
            m_channelId);
    }
    */
}

void PlatformStream::ksj_messagePump()
{

}

void PlatformStream::ksj_writeBitmap()
{
    /*
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
    */
}

#endif
