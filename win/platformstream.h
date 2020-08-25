/*

    OpenPnp-Capture: a video capture subsystem.

    Windows Stream class

    Created by Niels Moseley on 7/6/17.
    Copyright (c) 2017 Niels Moseley.

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all
    copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.
*/

#ifndef win_platformstream_h
#define win_platformstream_h

#include <windows.h>
#include <dshow.h>
#include <Vidcap.h>
#include <Ksmedia.h>

#include <stdint.h>
#include <vector>
#include <mutex>
#include "../common/logging.h"
#include "../common/stream.h"
#include "samplegrabber.h"

class Context;         // pre-declaration
class PlatformStream;  // pre-declaration

/** A class to handle callbacks from the video subsystem,
    A call is made for every frame.

    Note that the callback will be called by the DirectShow thread
    and should return as quickly as possible to avoid interference
    with the capturing process.
*/
class StreamCallbackHandler : public ISampleGrabberCB
{
public:
    StreamCallbackHandler(PlatformStream *stream) : m_stream(stream)
    {
        m_callbackCounter = 0;
    }

    ~StreamCallbackHandler()
    {
        LOG(LOG_INFO, "Callback counter = %d\n", m_callbackCounter);
    }

    /** callback handler used in this library */
    virtual HRESULT __stdcall SampleCB(double time, IMediaSample* sample) override;

    /** alternate callback handler (not used) */
    virtual HRESULT __stdcall BufferCB(double time, BYTE* buffer, long len) override
    {
        //m_callbackCounter++;
        return S_OK;
    }

    /** function implementation required by ISampleGrabberCB base class */
    virtual HRESULT __stdcall QueryInterface( REFIID iid, LPVOID *ppv )
    {
        if( iid == IID_ISampleGrabberCB || iid == IID_IUnknown )
        {
            *ppv = (void *) static_cast<ISampleGrabberCB*>( this );
            return S_OK;
        }
        return E_NOINTERFACE;
    }

    /** function implementation required by ISampleGrabberCB base class */
    virtual ULONG	__stdcall AddRef()
    {
        return 1;
    }

    /** function implementation required by ISampleGrabberCB base class */
    virtual ULONG	__stdcall Release()
    {
        return 2;
    }

    uint32_t getCallbackCounter() const
    {
        return m_callbackCounter;
    }

    void reset()
    {
        m_callbackCounter = 0;
    }


private:
    PlatformStream* m_stream;
    uint32_t        m_callbackCounter;
};

#ifdef JVSDK
enum class JVSCaptureThreadState
{
	Initialize,
	Error,
	Run,
	Exit
};

struct JVSCaptureState
{
	int channelId;
	PlatformStream* stream;
	bool isProcessed;
	bool isOpen;
};
#endif

/** The stream class handles the capturing of a single device */
class PlatformStream : public Stream
{
friend StreamCallbackHandler;

public:
    PlatformStream();
    virtual ~PlatformStream();

    /** Open a capture stream to a device and request a specific (internal) stream format. 
        When succesfully opened, capturing starts immediately.
    */
    virtual bool open(Context *owner, deviceInfo *device, uint32_t width, uint32_t height, 
        uint32_t fourCC, uint32_t fps) override;

    /** Close a capture stream */
    virtual void close() override;

    /** set the frame rate */
    virtual bool setFrameRate(uint32_t fps) override;

    /** Return the FOURCC media type of the stream */
    virtual uint32_t getFOURCC() override;

    /** get the limits of a camera/stream property (exposure, zoom etc) */
    virtual bool getPropertyLimits(uint32_t propID, int32_t *min, int32_t *max, int32_t *dValue) override;

    /** set property (exposure, zoom etc) of camera/stream */
    virtual bool setProperty(uint32_t propID, int32_t value) override;

    /** set automatic state of property (exposure, zoom etc) of camera/stream */
    virtual bool setAutoProperty(uint32_t propID, bool enabled) override;

    /** get property (exposure, zoom etc) of camera/stream */
    virtual bool getProperty(uint32_t propID, int32_t &outValue) override;
    
    /** get automatic state of property (exposure, zoom etc) of camera/stream */
    virtual bool getAutoProperty(uint32_t propID, bool &enabled) override;

#ifdef JVSDK
    bool jvs_open(Context* owner, deviceInfo* device, uint32_t width, uint32_t height,
        uint32_t fourCC, uint32_t fps);
    void jvs_close();
	void jvs_requestBitmap();
    void jvs_writeBitmap();

	bool m_isJVS;
	int m_channelId;

	// Static data

	static unsigned WINAPI jvs_captureThread(void* pParam);
    LRESULT jvs_onSDKNotify(WPARAM wParam, LPARAM lParam);

	static void jvs_messagePump();
	static bool jvs_initWindow();

	static HANDLE g_hCaptureThread;
	static unsigned int g_captureThreadId;
	static JVSCaptureThreadState g_captureThreadState;

	static ATOM g_atom;
	static HWND g_hwnd;
	static RECT g_rect;
	static int g_width, g_height;
	static unsigned char* g_pRGBA;

	static JVSCaptureState* g_currentCapture;
	static JVSCaptureState g_cameraCaptureStates[8];

	//static moodycamel::BlockingConcurrentQueue<JVSCaptureRequest> g_captureRequestsQueue;

#endif

#ifdef KSJAPI
    bool ksj_open(Context* owner, deviceInfo* device, uint32_t width, uint32_t height,
        uint32_t fourCC, uint32_t fps);
    void ksj_close();
	void ksj_messagePump();
    void ksj_requestBitmap();
	void ksj_captureBitmap();
	void ksj_writeBitmap();

    bool m_isKSJ;
    int m_deviceId;
#endif

#if defined(JVSDK) || defined(KSJAPI)
    bool hasNewFrame() override;
    bool captureFrame(uint8_t* RGBbufferPtr, uint32_t RGBbufferBytes) override;
#endif

protected:
    /** A re-implementation of Stream::submitBuffer with BGR to RGB conversion */
    virtual void submitBuffer(const uint8_t *ptr, size_t bytes) override;

    /** Add the Direct show filter graph to the object list so
        GraphEdt.exe can see it - for debugging purposes only.
        See: https://msdn.microsoft.com/en-us/library/windows/desktop/dd390650(v=vs.85).aspx    
    */
    HRESULT AddToRot(IUnknown *pUnkGraph, DWORD *pdwRegister);

    /** Remove the Direct show filter graph from the object list 
         - for debugging purposes only. */
    void RemoveFromRot(DWORD pdwRegister);

    /** Write filter graph to 'filtergraph.grf' file - for debugging purposes only.*/
    HRESULT SaveGraphFile(IGraphBuilder *pGraph);

    /** get DirectShow property + flags helper function */
    bool getDSProperty(uint32_t propID, long &value, long &flags);

    void dumpCameraProperties();

    IFilterGraph2*  m_graph;
    IMediaControl*  m_control;
    IBaseFilter*    m_sourceFilter;
    IBaseFilter*    m_sampleGrabberFilter;
    IBaseFilter*    m_nullRenderer;
    ISampleGrabber* m_sampleGrabber;
    ICaptureGraphBuilder2* m_capture;
    IAMCameraControl* m_camControl;
    IAMVideoProcAmp* m_videoProcAmp;

    /** Each time a new frame is available, the DirectShow subsystem
        will call the callback handler */
    StreamCallbackHandler *m_callbackHandler;

    VIDEOINFOHEADER m_videoInfo;            ///< video information of current captured stream

    DWORD dwRotRegister;    ///< for exposing the filtergraph to GraphEdt
};

#endif