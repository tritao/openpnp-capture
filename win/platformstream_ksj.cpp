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

#if 0
LRESULT CALLBACK ksj_WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
#endif

bool PlatformStream::ksj_open(Context* owner, deviceInfo* device, uint32_t width, uint32_t height,
    uint32_t fourCC, uint32_t fps)
{
	m_deviceId = atoi(&device->m_name.at(strlen("KSJ_Device")));

	m_width = width;
	m_height = height;

#if 0
	HINSTANCE hInstance = GetModuleHandle(NULL);
	static const char* className = "DUMMY_CLASS";

	WNDCLASSEX wx = {};
	wx.cbSize = sizeof(WNDCLASSEX);
	wx.lpfnWndProc = ksj_WindowProc;
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

	KSJ_DEVICETYPE deviceType;
	int serials[128];
	unsigned short firmwareVersion[16];
	result = KSJ_DeviceGetInformation(m_deviceId, &deviceType, (int*)&serials, (unsigned short*)&firmwareVersion);

	result = KSJ_PreviewSetFieldOfView(m_deviceId, nColStart, nRowStart, nColSize, nRowSize,
		addressMode, addressMode);
	if (result != RET_SUCCESS)
	{
		LOG(LOG_WARNING, "KSJ_PreviewSetFieldOfView failed for device %d (result = %d)!\n",
			m_deviceId, result);
	}
#endif

    // Parameters copied from SMT app init_KJS_Camera().
    KSJ_SetParam(0, KSJ_RED, 0);
    KSJ_SetParam(0, KSJ_GREEN, 15);
    KSJ_SetParam(0, KSJ_BLUE, 15);
    KSJ_SetParam(0, KSJ_EXPOSURE_LINES, 500);
    KSJ_SetParam(0, KSJ_BRIGHTNESS, 60);
    KSJ_SetParam(0, KSJ_CONTRAST, 25);

	KSJ_WhiteBalanceSet(0, KSJ_SWB_AUTO_CONITNUOUS);

    m_frameBuffer.resize(m_width * m_height * 3);

#if 0
	result = KSJ_PreviewSetPos(m_deviceId, m_hwnd, 0, 0, width, height);
	if (result != RET_SUCCESS)
	{
		LOG(LOG_WARNING, "KSJ_PreviewSetPos failed for device %d (result = %d)!\n",
			m_deviceId, result);
	}

	result = KSJ_PreviewStartEx(m_deviceId, /*start=*/true, /*parallel=*/true);
	if (result != RET_SUCCESS)
	{
		LOG(LOG_WARNING, "KSJ_PreviewStartEx failed for device %d (result = %d)!\n",
			m_deviceId, result);
	}

	unsigned int threadID;
	m_hCaptureThread = (HANDLE)_beginthreadex(NULL, 0, &PlatformStream::ksj_captureX,
		this, 0, &threadID);
#endif

	m_newFrame = true;
    m_isOpen = true;

	return true;
}

void PlatformStream::ksj_captureBitmap()
{
	//LOG(LOG_INFO, "ksj_captureBitmap: Capturing bitmap.\n");

	m_bufferMutex.lock();

	int result = KSJ_CaptureRgbData(m_deviceId, m_frameBuffer.data());
	if (result != RET_SUCCESS)
	{
		LOG(LOG_WARNING, "KSJ_CaptureRgbData failed for device %d (result = %d)!\n",
			m_deviceId, result);
	}

	uint8_t* src = m_frameBuffer.data();
	for (uint32_t pixel = 0; pixel < m_width * m_height; pixel++)
	{
		uint8_t r = (*(src + 0))++;
		uint8_t g = (*(src + 1))++;
		uint8_t b = (*(src + 2))++;

		*src++ = b;
		*src++ = g;
		*src++ = r;
	}

	m_bufferMutex.unlock();

	m_newFrame = true;
	m_frames++;
}

void PlatformStream::ksj_close()
{
}

#define KSJ_WHITEBALANCE 100

static bool ConvertToKSJParam(CapPropertyID propID, KSJ_PARAM& param)
{
	switch (propID)
	{
		case CAPPROPID_EXPOSURE:
		{
			param = KSJ_EXPOSURE;
			return true;
		}
		case CAPPROPID_BRIGHTNESS:
		{
			param = KSJ_BRIGHTNESS;
			return true;
		}
		case CAPPROPID_CONTRAST:
		{
			param = KSJ_CONTRAST;
			return true;
		}
		case CAPPROPID_SATURATION:
		{
			param = KSJ_SATURATION;
			return true;
		}
		case CAPPROPID_GAMMA:
		{
			param = KSJ_GAMMA;
			return true;
		}
		case CAPPROPID_WHITEBALANCE:
		{
			param = (KSJ_PARAM)KSJ_WHITEBALANCE;
			return true;
		}
		case CAPPROPID_FOCUS:
		{
			param = (KSJ_PARAM)KSJ_RED;
			return true;
		}
		case CAPPROPID_GAIN:
		{
			param = (KSJ_PARAM)KSJ_GREEN;
			return true;
		}
		case CAPPROPID_ZOOM:
		{
			param = (KSJ_PARAM)KSJ_BLUE;
			return true;
		}
		case CAPPROPID_HUE:
		case CAPPROPID_SHARPNESS:
		case CAPPROPID_BACKLIGHTCOMP:
		case CAPPROPID_POWERLINEFREQ:
		default:
		{
			return false;
		}
	}
}

bool PlatformStream::ksj_getAutoProperty(uint32_t propID, bool& enabled)
{
	KSJ_PARAM param;
	if (!ConvertToKSJParam(propID, param))
	{
		enabled = false;
		return false;
	}

	if (param == KSJ_WHITEBALANCE)
	{
		KSJ_WB_MODE mode;
		int result = KSJ_WhiteBalanceGet(m_deviceId, &mode);
		if (result != RET_SUCCESS)
			return false;

		enabled = mode != KSJ_WB_DISABLE;
		return true;
	}

	enabled = true;
	return true;
}

bool PlatformStream::ksj_setAutoProperty(uint32_t propID, bool enabled)
{
	KSJ_PARAM param;
	if (!ConvertToKSJParam(propID, param))
	{
		enabled = false;
		return false;
	}

	if (param == KSJ_WHITEBALANCE)
	{
		KSJ_WB_MODE mode = enabled ? KSJ_SWB_AUTO_CONITNUOUS : KSJ_WB_DISABLE;
		int result = KSJ_WhiteBalanceSet(m_deviceId, mode);
		return result == RET_SUCCESS;
	}

	return true;
}

bool PlatformStream::ksj_getProperty(uint32_t propID, int32_t& outValue)
{
	KSJ_PARAM param;
	if (!ConvertToKSJParam(propID, param))
		return false;

	if (param == KSJ_WHITEBALANCE)
	{
		int nPhi;
		int result = KSJ_WhiteBalanceAutoGet(m_deviceId, &nPhi);

		outValue = nPhi;
		return result == RET_SUCCESS;

	}

	int result = KSJ_GetParam(m_deviceId, param, &outValue);
	return result == RET_SUCCESS;
}

bool PlatformStream::ksj_setProperty(uint32_t propID, int32_t value)
{
	KSJ_PARAM param;
	if (!ConvertToKSJParam(propID, param))
		return false;

	if (param == KSJ_WHITEBALANCE)
	{
		int result = KSJ_WhiteBalanceAutoSet(m_deviceId, value);
		return result == RET_SUCCESS;
	}

	int result = KSJ_SetParam(m_deviceId, param, value);
	return result == RET_SUCCESS;
}

bool PlatformStream::ksj_getPropertyLimits(CapPropertyID propID, int32_t* emin,
	int32_t* emax, int32_t* dValue)
{
	KSJ_PARAM param;
	if (!ConvertToKSJParam(propID, param))
		return false;

	if (param == KSJ_WHITEBALANCE)
	{
		return false;
	}

	int min, max;
	int result = KSJ_GetParamRange(m_deviceId, param, &min, &max);
	if (result != RET_SUCCESS)
		return false;

	*emin = min;
	*emax = max;

	int value;
	result = KSJ_GetParam(m_deviceId, param, &value);
	if (result != RET_SUCCESS)
		return false;

	*dValue = value;

	return true;
}

#if 0

unsigned WINAPI PlatformStream::ksj_captureX(void* pParam)
{
	PlatformStream* pStream = (PlatformStream*)pParam;
	pStream->ksj_captureBitmap();
	return 0;
}

void PlatformStream::ksj_messagePump()
{
	MSG msg;
	while (PeekMessage(&msg, m_hwnd, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

LRESULT CALLBACK ksj_WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
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
	}

	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
#endif

void PlatformStream::ksj_requestBitmap()
{

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
