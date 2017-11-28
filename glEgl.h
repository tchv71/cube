///////////////////////////////////////////////////////////////////////////////
// Name:        wx/unix/glx11.h
// Purpose:     class common for all X11-based wxGLCanvasNew implementations
// Author:      Vadim Zeitlin
// Created:     2007-04-15
// Copyright:   (c) 2007 Vadim Zeitlin <vadim@wxwindows.org>
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

#ifndef _WX_UNIX_EGL_H_
#define _WX_UNIX_EGL_H_

#include <EGL/egl.h>
#include <gtk-3.0/gdk/gdkwayland.h>
#include <wayland-egl.h>


//typedef wxEGLAttributes wxGLAttributes;

class wxGLCanvasNew;

// ----------------------------------------------------------------------------
// wxEGLContextAttrs: OpenGL rendering context attributes
// ----------------------------------------------------------------------------

class WXDLLIMPEXP_GL wxEGLContextAttrs : public wxGLContextAttrs
{
public:
    // Setters, allowing chained calls
    wxEGLContextAttrs& CoreProfile();
    wxEGLContextAttrs& MajorVersion(int val);
    wxEGLContextAttrs& MinorVersion(int val);
    wxEGLContextAttrs& OGLVersion(int vmayor, int vminor)
        { return MajorVersion(vmayor).MinorVersion(vminor); }
    wxEGLContextAttrs& CompatibilityProfile();
    wxEGLContextAttrs& ForwardCompatible();
    wxEGLContextAttrs& ES2();
    wxEGLContextAttrs& DebugCtx();
    wxEGLContextAttrs& Robust();
    wxEGLContextAttrs& NoResetNotify();
    wxEGLContextAttrs& LoseOnReset();
    wxEGLContextAttrs& ResetIsolation();
    wxEGLContextAttrs& ReleaseFlush(int val = 1); //'int' allows future values
    wxEGLContextAttrs& PlatformDefaults();
    void EndList(); // No more values can be chained

    // Currently only used for X11 context creation
    bool x11Direct; // X11 direct render
    bool renderTypeRGBA;
};

// ----------------------------------------------------------------------------
// wxEGLAttributes: canvas configuration
// ----------------------------------------------------------------------------

class WXDLLIMPEXP_GL wxEGLAttributes : public wxGLAttributes
{
public:
    // Setters, allowing chained calls
    wxEGLAttributes& RGBA();
    wxEGLAttributes& BufferSize(int val);
    wxEGLAttributes& Level(int val);
    wxEGLAttributes& DoubleBuffer();
    wxEGLAttributes& Stereo();
    wxEGLAttributes& AuxBuffers(int val);
    wxEGLAttributes& MinRGBA(int mRed, int mGreen, int mBlue, int mAlpha);
    wxEGLAttributes& Depth(int val);
    wxEGLAttributes& Stencil(int val);
    wxEGLAttributes& MinAcumRGBA(int mRed, int mGreen, int mBlue, int mAlpha);
    wxEGLAttributes& PlatformDefaults();
    wxEGLAttributes& Defaults();
    wxEGLAttributes& SampleBuffers(int val);
    wxEGLAttributes& Samplers(int val);
    wxEGLAttributes& FrameBuffersRGB();
    void EndList(); // No more values can be chained

    // This function is undocumented and can not be chained on purpose!
    // To keep backwards compatibility with versions before wx3.1 we add here
    // the default values used in those versions for the case of NULL list.
    void AddDefaultsForWXBefore31();
};


// ----------------------------------------------------------------------------
// wxGLContextEgl
// ----------------------------------------------------------------------------
class WXDLLIMPEXP_GL wxGLContextEgl : public wxGLContextBase
{
public:
    wxGLContextEgl(wxGLCanvasNew *win,
                const wxGLContextEgl *other = NULL,
                const wxEGLContextAttrs *ctxAttrs = NULL);
    virtual ~wxGLContextEgl();

    virtual bool SetCurrent(const wxGLCanvasNew& win) const/* wxOVERRIDE*/;
    virtual bool SetCurrent(const wxGLCanvas& win) const {}/* wxOVERRIDE*/;
    static EGLDisplay GetEglDisplay(GdkWindow* window) ;

private:
    // attach context to the drawable or unset it (if NULL)
    //static bool MakeCurrent(GLXDrawable drawable, GLXContext context);

    EGLContext m_glContext;

    wxDECLARE_CLASS(wxGLContextEgl);
private:
    static EGLDisplay CreateEglDisplay (GdkWaylandDisplay *display_wayland);
    static gboolean InitGl (GdkDisplay *display);
    static bool have_egl;
    static EGLDisplay egl_display;
};

// ----------------------------------------------------------------------------
// wxGLCanvasEgl
// ----------------------------------------------------------------------------


class WXDLLIMPEXP_GL wxGLCanvasEgl : public wxGLCanvasBase
{
public:
    // initialization and dtor
    // -----------------------

    // default ctor doesn't do anything, InitVisual() must be called
    wxGLCanvasEgl();

    // initializes GLXFBConfig and XVisualInfo corresponding to the given attributes
    bool InitVisual(const wxGLAttributes& dispAttrs, GdkWindow* window);

    // frees XVisualInfo info
    virtual ~wxGLCanvasEgl();


    // implement wxGLCanvasBase methods
    // --------------------------------

    virtual bool SwapBuffers() wxOVERRIDE;



    // override some wxWindow methods
    // ------------------------------

    // return true only if the window is realized: OpenGL context can't be
    // created until we are
    virtual bool IsShownOnScreen() const wxOVERRIDE;


    // implementation only from now on
    // -------------------------------

    EGLConfig GetEglConfig() { return egl_config; } 
    bool SetCurrentContext(EGLContext ctx) const;
private:
    void EnsureEglWindowAndSurface();
    EGLConfig egl_config;
    EGLSurface egl_surface;
    struct wl_egl_window* egl_window; 
};

// ----------------------------------------------------------------------------
// wxGLApp
// ----------------------------------------------------------------------------

// this is used in wx/glcanvas.h, prevent it from defining a generic wxGLApp
#define wxGL_APP_DEFINED
#if 0
class WXDLLIMPEXP_GL wxGLApp : public wxGLAppBase
{
public:
    wxGLApp() : wxGLAppBase() { }

    // implement wxGLAppBase method
    virtual bool InitGLVisual(const int *attribList) wxOVERRIDE
    {
        return wxGLCanvasEgl::InitDefaultVisualInfo(attribList);
    }

    // This method is not currently used by the library itself, but remains for
    // backwards compatibility and also because wxGTK has it we could start
    // using it for the same purpose in wxX11 too some day.
    virtual void* GetXVisualInfo() wxOVERRIDE
    {
        return wxGLCanvasEgl::GetDefaultXVisualInfo();
    }

    // and override this wxApp method to clean up
    virtual int OnExit() wxOVERRIDE
    {
        wxGLCanvasEgl::FreeDefaultVisualInfo();

        return wxGLAppBase::OnExit();
    }

private:
    wxDECLARE_DYNAMIC_CLASS(wxGLApp);
};
#endif

#endif // _WX_UNIX_GLX11_H_

