///////////////////////////////////////////////////////////////////////////////
// Name:        src/unix/glx11.cpp
// Purpose:     code common to all X11-based wxGLCanvasNew implementations
// Author:      Vadim Zeitlin
// Created:     2007-04-15
// Copyright:   (c) 2007 Vadim Zeitlin <vadim@wxwindows.org>
// Licence:     wxWindows licence
///////////////////////////////////////////////////////////////////////////////

// ============================================================================
// declarations
// ============================================================================

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

// for compilers that support precompilation, includes "wx.h".
#include "wx/wxprec.h"

#if wxUSE_GLCANVAS
#ifndef WX_PRECOMP
#include "wx/log.h"
#endif //WX_PRECOMP

#include <epoxy/egl.h>
#include "wx/glcanvas.h"
#include "glcanvas.h"
#include "glEgl.h"
#include <EGL/egl.h>
#include <EGL/eglext.h>

// ----------------------------------------------------------------------------
// wxEGLContextAttrs: OpenGL rendering context attributes
// ----------------------------------------------------------------------------
// GLX specific values

wxEGLContextAttrs& wxEGLContextAttrs::CoreProfile()
{
    AddAttribBits(EGL_CONTEXT_OPENGL_PROFILE_MASK_KHR,
                  EGL_CONTEXT_OPENGL_CORE_PROFILE_BIT_KHR);
    SetNeedsARB();
    return *this;
}

wxEGLContextAttrs& wxEGLContextAttrs::MajorVersion(int val)
{
    if ( val > 0 ) {
        AddAttribute(EGL_CONTEXT_MAJOR_VERSION_KHR);
        AddAttribute(val);
        if ( val >= 3 )
            SetNeedsARB();
    }
    return *this;
}

wxEGLContextAttrs& wxEGLContextAttrs::MinorVersion(int val)
{
    if ( val >= 0 ) {
        AddAttribute(EGL_CONTEXT_MINOR_VERSION_KHR);
        AddAttribute(val);
    }
    return *this;
}

wxEGLContextAttrs& wxEGLContextAttrs::CompatibilityProfile()
{
    AddAttribBits(EGL_CONTEXT_OPENGL_PROFILE_MASK_KHR,
                  EGL_CONTEXT_OPENGL_COMPATIBILITY_PROFILE_BIT_KHR);
    SetNeedsARB();
    return *this;
}

wxEGLContextAttrs& wxEGLContextAttrs::ForwardCompatible()
{
    AddAttribBits(EGL_CONTEXT_FLAGS_KHR,
                  EGL_CONTEXT_OPENGL_FORWARD_COMPATIBLE_BIT_KHR);
    SetNeedsARB();
    return *this;
}

wxEGLContextAttrs& wxEGLContextAttrs::ES2()
{
//    AddAttribBits(GLX_CONTEXT_PROFILE_MASK_ARB,
//                  GLX_CONTEXT_ES2_PROFILE_BIT_EXT);
    SetNeedsARB();
    return *this;
}

wxEGLContextAttrs& wxEGLContextAttrs::DebugCtx()
{
    AddAttribBits(EGL_CONTEXT_FLAGS_KHR,
                  EGL_CONTEXT_OPENGL_DEBUG_BIT_KHR);
    SetNeedsARB();
    return *this;
}

wxEGLContextAttrs& wxEGLContextAttrs::Robust()
{
    AddAttribBits(EGL_CONTEXT_FLAGS_KHR,
                  EGL_CONTEXT_OPENGL_ROBUST_ACCESS_BIT_KHR);
    SetNeedsARB();
    return *this;
}

wxEGLContextAttrs& wxEGLContextAttrs::NoResetNotify()
{
    AddAttribBits(EGL_CONTEXT_FLAGS_KHR,
                  EGL_NO_RESET_NOTIFICATION_KHR);
    SetNeedsARB();
    return *this;
}

wxEGLContextAttrs& wxEGLContextAttrs::LoseOnReset()
{
    AddAttribBits(EGL_CONTEXT_FLAGS_KHR,
                  EGL_LOSE_CONTEXT_ON_RESET_KHR);
    SetNeedsARB();
    return *this;
}

wxEGLContextAttrs& wxEGLContextAttrs::ResetIsolation()
{
    AddAttribBits(EGL_CONTEXT_FLAGS_KHR,
                  EGL_CONTEXT_OPENGL_RESET_NOTIFICATION_STRATEGY_KHR);
    SetNeedsARB();
    return *this;
}

wxEGLContextAttrs& wxEGLContextAttrs::ReleaseFlush(int val)
{
//    AddAttribute(GLX_CONTEXT_RELEASE_BEHAVIOR_ARB);
//    if ( val == 1 )
//        AddAttribute(GLX_CONTEXT_RELEASE_BEHAVIOR_FLUSH_ARB);
//    else
//        AddAttribute(GLX_CONTEXT_RELEASE_BEHAVIOR_NONE_ARB);
    SetNeedsARB();
    return *this;
}

wxEGLContextAttrs& wxEGLContextAttrs::PlatformDefaults()
{
    renderTypeRGBA = true;
    x11Direct = true;
    return *this;
}

void wxEGLContextAttrs::EndList()
{
    AddAttribute(EGL_NONE);
}

// ----------------------------------------------------------------------------
// wxEGLAttributes: Visual/FBconfig attributes
// ----------------------------------------------------------------------------
// GLX specific values

//   Different versions of GLX API use rather different attributes lists, see
//   the following URLs:
//
//   - <= 1.2: http://www.opengl.org/sdk/docs/man/xhtml/glXChooseVisual.xml
//   - >= 1.3: http://www.opengl.org/sdk/docs/man/xhtml/glXChooseFBConfig.xml
//
//   Notice in particular that
//   - GLX_RGBA is boolean attribute in the old version of the API but a
//     value of GLX_RENDER_TYPE in the new one
//   - Boolean attributes such as GLX_DOUBLEBUFFER don't take values in the
//     old version but must be followed by True or False in the new one.

wxEGLAttributes& wxEGLAttributes::RGBA()
{
//    if ( wxGLCanvasEgl::GetGLXVersion() >= 13 )
//        AddAttribBits(GLX_RENDER_TYPE, GLX_RGBA_BIT);
//    else
//        AddAttribute(GLX_RGBA);
    AddAttribute(EGL_SURFACE_TYPE);
    AddAttribute(EGL_WINDOW_BIT);
    AddAttribute(EGL_COLOR_BUFFER_TYPE);
    AddAttribute(EGL_RGB_BUFFER);
    return *this;
}

wxEGLAttributes& wxEGLAttributes::BufferSize(int val)
{
    if ( val >= 0 ) {
        AddAttribute(EGL_BUFFER_SIZE);
        AddAttribute(val);
    }
    return *this;
}

wxEGLAttributes& wxEGLAttributes::Level(int val)
{
    AddAttribute(EGL_LEVEL);
    AddAttribute(val);
    return *this;
}

wxEGLAttributes& wxEGLAttributes::DoubleBuffer()
{
//    AddAttribute(GLX_DOUBLEBUFFER);
//    if ( wxGLCanvasEgl::GetGLXVersion() >= 13 )
//        AddAttribute(True);
    return *this;
}

wxEGLAttributes& wxEGLAttributes::Stereo()
{
//    AddAttribute(GLX_STEREO);
//    if ( wxGLCanvasEgl::GetGLXVersion() >= 13 )
//        AddAttribute(True);
    return *this;
}

wxEGLAttributes& wxEGLAttributes::AuxBuffers(int val)
{
//    if ( val >= 0 )
//    {
//        AddAttribute(GLX_AUX_BUFFERS);
//        AddAttribute(val);
//    }
    return *this;
}

wxEGLAttributes& wxEGLAttributes::MinRGBA(int mRed, int mGreen, int mBlue, int mAlpha)
{
    if ( mRed >= 0) {
        AddAttribute(EGL_RED_SIZE);
        AddAttribute(mRed);
    }
    if ( mGreen >= 0) {
        AddAttribute(EGL_GREEN_SIZE);
        AddAttribute(mGreen);
    }
    if ( mBlue >= 0) {
        AddAttribute(EGL_BLUE_SIZE);
        AddAttribute(mBlue);
    }
    if ( mAlpha >= 0) {
        AddAttribute(EGL_ALPHA_SIZE);
        AddAttribute(mAlpha);
    }
    return *this;
}

wxEGLAttributes& wxEGLAttributes::Depth(int val)
{
    if ( val >= 0 ) {
        AddAttribute(EGL_DEPTH_SIZE);
        AddAttribute(val);
    }
    return *this;
}

wxEGLAttributes& wxEGLAttributes::Stencil(int val)
{
    if ( val >= 0 ) {
        AddAttribute(EGL_STENCIL_SIZE);
        AddAttribute(val);
    }
    return *this;
}

wxEGLAttributes& wxEGLAttributes::MinAcumRGBA(int mRed, int mGreen, int mBlue, int mAlpha)
{
//    if ( mRed >= 0)
//    {
//        AddAttribute(GLX_ACCUM_RED_SIZE);
//        AddAttribute(mRed);
//    }
//    if ( mGreen >= 0)
//    {
//        AddAttribute(GLX_ACCUM_GREEN_SIZE);
//        AddAttribute(mGreen);
//    }
//    if ( mBlue >= 0)
//    {
//        AddAttribute(GLX_ACCUM_BLUE_SIZE);
//        AddAttribute(mBlue);
//    }
//    if ( mAlpha >= 0)
//    {
//        AddAttribute(GLX_ACCUM_ALPHA_SIZE);
//        AddAttribute(mAlpha);
//    }
    return *this;
}

wxEGLAttributes& wxEGLAttributes::SampleBuffers(int val)
{
    AddAttribute(EGL_SAMPLE_BUFFERS);
    AddAttribute(val);
    return *this;
}

wxEGLAttributes& wxEGLAttributes::Samplers(int val)
{
    AddAttribute(EGL_SAMPLES);
    AddAttribute(val);
    return *this;
}

wxEGLAttributes& wxEGLAttributes::FrameBuffersRGB()
{
//    AddAttribute(GLX_FRAMEBUFFER_SRGB_CAPABLE_ARB);
//    AddAttribute(True);
    return *this;
}

void wxEGLAttributes::EndList()
{
    AddAttribute(EGL_NONE);
}

wxEGLAttributes& wxEGLAttributes::PlatformDefaults()
{
    // No GLX specific values
    return *this;
}

wxEGLAttributes& wxEGLAttributes::Defaults()
{
    //RGBA().DoubleBuffer().Depth(16).SampleBuffers(1).Samplers(4);
    RGBA().MinRGBA(8,8,8,-1);
    EndList();
    return *this;
}

void wxEGLAttributes::AddDefaultsForWXBefore31()
{
    // ParseAttribList() will add EndList(), don't do it now
    DoubleBuffer();
    RGBA().Depth(1).MinRGBA(1, 1, 1, 0);
    // For GLX >= 1.3 its defaults (GLX_RGBA_BIT and GLX_WINDOW_BIT) are OK
}


// ============================================================================
// wxGLContextEgl implementation
// ============================================================================

EGLDisplay wxGLContextEgl::CreateEglDisplay (GdkWaylandDisplay *display_wayland)
{
    EGLDisplay dpy = NULL;
    struct wl_display* disp_wayland = gdk_wayland_display_get_wl_display(display_wayland);
    if (epoxy_has_egl_extension (NULL, "EGL_KHR_platform_base")) {
        PFNEGLGETPLATFORMDISPLAYPROC getPlatformDisplay = (PFNEGLGETPLATFORMDISPLAYPROC)eglGetProcAddress ("eglGetPlatformDisplay");

        if (getPlatformDisplay)
            dpy = getPlatformDisplay (EGL_PLATFORM_WAYLAND_EXT,
                                      disp_wayland,
                                      NULL);
        if (dpy)
            return dpy;
    }

    if (epoxy_has_egl_extension (NULL, "EGL_EXT_platform_base")) {
        PFNEGLGETPLATFORMDISPLAYEXTPROC getPlatformDisplay =
            (PFNEGLGETPLATFORMDISPLAYEXTPROC) eglGetProcAddress ("eglGetPlatformDisplayEXT");

        if (getPlatformDisplay)
            dpy = getPlatformDisplay (EGL_PLATFORM_WAYLAND_EXT,
                                      disp_wayland,
                                      NULL);
        if (dpy)
            return dpy;
    }

    return eglGetDisplay ((EGLNativeDisplayType) disp_wayland);
}

bool wxGLContextEgl::have_egl = false;
EGLDisplay wxGLContextEgl::egl_display;


gboolean wxGLContextEgl::InitGl (GdkDisplay *display)
{
    GdkWaylandDisplay *display_wayland = GDK_WAYLAND_DISPLAY (display);
    EGLint major, minor;
    EGLDisplay dpy;

    if (have_egl)
        return TRUE;

    if (!display_wayland)
    {
	display_wayland = GDK_WAYLAND_DISPLAY (gdk_window_get_display(gdk_get_default_root_window()));
    }
    dpy = CreateEglDisplay (display_wayland);

    if (dpy == NULL)
	return FALSE;
    if (!eglInitialize (dpy, &major, &minor))
    {
	EGLint err = eglGetError();
        return FALSE;
    }

    if (!eglBindAPI (EGL_OPENGL_API))
        return FALSE;

    egl_display = dpy;
    //display_wayland->egl_major_version = major;
    //display_wayland->egl_minor_version = minor;

    have_egl = true;

    /*  display_wayland->have_egl_khr_create_context =
        epoxy_has_egl_extension (dpy, "EGL_KHR_create_context");

      display_wayland->have_egl_buffer_age =
        epoxy_has_egl_extension (dpy, "EGL_EXT_buffer_age");

      display_wayland->have_egl_swap_buffers_with_damage =
        epoxy_has_egl_extension (dpy, "EGL_EXT_swap_buffers_with_damage");

      display_wayland->have_egl_surfaceless_context =
        epoxy_has_egl_extension (dpy, "EGL_KHR_surfaceless_context");

      GDK_NOTE (OPENGL,
                g_message ("EGL API version %d.%d found\n"
                           " - Vendor: %s\n"
                           " - Version: %s\n"
                           " - Client APIs: %s\n"
                           " - Extensions:\n"
                           "\t%s",
                           display_wayland->egl_major_version,
                           display_wayland->egl_minor_version,
                           eglQueryString (dpy, EGL_VENDOR),
                           eglQueryString (dpy, EGL_VERSION),
                           eglQueryString (dpy, EGL_CLIENT_APIS),
                           eglQueryString (dpy, EGL_EXTENSIONS)));
    */
    return TRUE;
}

static bool g_ctxErrorOccurred = false;

wxIMPLEMENT_CLASS(wxGLContextEgl, wxObject);

wxGLContextEgl::wxGLContextEgl(wxGLCanvasNew *win,
                               const wxGLContextEgl *other,
                               const wxEGLContextAttrs *ctxAttrs)
    : m_glContext(NULL)
{
    const int* contextAttribs = NULL;
    Bool x11Direct = True;
    //int renderType = GLX_RGBA_TYPE;
    bool needsARB = false;

    if ( ctxAttrs ) {
        contextAttribs = ctxAttrs->GetGLAttrs();
        x11Direct = ctxAttrs->x11Direct;
        //renderType = ctxAttrs->renderTypeRGBA ? GLX_RGBA_TYPE : GLX_COLOR_INDEX_TYPE;
        needsARB = ctxAttrs->NeedsARB();
    } else if ( win->GetGLCTXAttrs().GetGLAttrs() ) {
        // If OpenGL context parameters were set at wxGLCanvasNew ctor, get them now
        contextAttribs = win->GetGLCTXAttrs().GetGLAttrs();
        x11Direct = win->GetGLCTXAttrs().x11Direct;
        //renderType = win->GetGLCTXAttrs().renderTypeRGBA ? GLX_RGBA_TYPE : GLX_COLOR_INDEX_TYPE;
        needsARB = win->GetGLCTXAttrs().NeedsARB();
    }
    // else use GPU driver defaults and x11Direct renderType ones

    m_isOk = false;

    GdkWindow* window = win->GTKGetDrawingWindow();
    gboolean bRes = gdk_window_has_native(window);
    GdkDisplay *display = gdk_window_get_display (window);
    if (!GDK_IS_WAYLAND_DISPLAY(display))
        return;
    if (!InitGl(display))
        return;
    EGLConfig egl_config = win->GetEglConfig();


    eglBindAPI(EGL_OPENGL_API);
    m_glContext =  eglCreateContext (egl_display,
                                     egl_config,
                                     other != NULL ? other->m_glContext
                                     : EGL_NO_CONTEXT,
                                     contextAttribs);

    if (!m_glContext )
        wxLogMessage(_("Couldn't create OpenGL context"));
    else
        m_isOk = true;
    // Restore old error handler
    //XSetErrorHandler( oldHandler );
}

wxGLContextEgl::~wxGLContextEgl()
{
    if ( !m_glContext )
        return;

    if (eglGetCurrentContext () == m_glContext)
        eglMakeCurrent(egl_display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);

    eglDestroyContext (egl_display, m_glContext);
}

bool wxGLContextEgl::SetCurrent(const wxGLCanvasNew& win) const
{
    if ( !m_glContext )
        return false;
    return win.SetCurrentContext(m_glContext);
}

EGLDisplay wxGLContextEgl::GetEglDisplay(GdkWindow* window)
{
    GdkDisplay* display = gdk_window_get_display(window);
    InitGl(display);
    return egl_display;
}

// ============================================================================
// wxGLCanvasEgl implementation
// ============================================================================

// ----------------------------------------------------------------------------
// initialization methods and dtor
// ----------------------------------------------------------------------------

wxGLCanvasEgl::wxGLCanvasEgl() :
    egl_config(NULL),
    egl_surface(NULL),
    egl_window(NULL)
{
}

bool wxGLCanvasEgl::InitVisual(const wxGLAttributes& dispAttrs, GdkWindow* window)
{
    if (egl_config)
	return true;
    int count = 0;
    const int* attrs = dispAttrs.GetGLAttrs();
    EGLDisplay dpy = wxGLContextEgl::GetEglDisplay(window);
    if (!eglChooseConfig (dpy, attrs, NULL, 0, &count) || count < 1) {
	EGLint err = eglGetError();
        wxFAIL_MSG("No available configurations for the given pixel format");
        return false;
    }
    EGLConfig configs[count];
    if (!eglChooseConfig (dpy, attrs, configs, count, &count) || count < 1) {
        wxFAIL_MSG("No available configurations for the given pixel format");
        return false;
    }

    /* Pick first valid configuration i guess? */
    egl_config = configs[0];
    return true;
}

wxGLCanvasEgl::~wxGLCanvasEgl()
{
}

// ----------------------------------------------------------------------------
// working with GL attributes
// ----------------------------------------------------------------------------



#if 0
/* static */
bool wxGLCanvasBase::IsDisplaySupported(const wxGLAttributes& dispAttrs)
{
    GLXFBConfig *fbc = NULL;
    XVisualInfo *vi = NULL;

    bool isSupported = wxGLCanvasEgl::InitXVisualInfo(dispAttrs, &fbc, &vi);

    if ( fbc )
        XFree(fbc);
    if ( vi )
        XFree(vi);

    return isSupported;
}

/* static */
bool wxGLCanvasBase::IsDisplaySupported(const int *attribList)
{
    wxEGLAttributes dispAttrs;
    ParseAttribList(attribList, dispAttrs);

    return IsDisplaySupported(dispAttrs);
}

/* static */
bool wxGLCanvasBase::IsExtensionSupported(const char *extension)
{
    Display * const dpy = wxGetX11Display();

    return IsExtensionInList(glXQueryExtensionsString(dpy, DefaultScreen(dpy)),
                             extension);
}
#endif


bool wxGLCanvasEgl::SwapBuffers()
{
    EnsureEglWindowAndSurface();
    EGLDisplay dpy = wxGLContextEgl::GetEglDisplay(GTKGetDrawingWindow());
    eglSwapInterval(dpy,0);
    EGLBoolean bRes = eglSwapBuffers(dpy,egl_surface);
    EGLint err = eglGetError();
    return bRes;
}

bool wxGLCanvasEgl::IsShownOnScreen() const
{
    return wxGLCanvasBase::IsShownOnScreen();
}

void wxGLCanvasEgl::EnsureEglWindowAndSurface()
{
    if (!egl_window) {
        GdkWindow* window = GTKGetDrawingWindow();
        int s = gdk_window_get_scale_factor(window);
        int w = gdk_window_get_width(window)*s;
        int h = gdk_window_get_height(window)*s;
#if 0
        GdkDisplay* display = gdk_window_get_display(window);
        struct wl_compositor* compositor =  gdk_wayland_display_get_wl_compositor(display);
        struct wl_surface* wl_surf = wl_compositor_create_surface(compositor);
        wl_region *region = wl_compositor_create_region(compositor);
        wl_region_add(region,0,0,w,h);
        wl_surface_set_opaque_region(wl_surf, region);
#else
        struct wl_surface* wl_surf = gdk_wayland_window_get_wl_surface(window);
#if 0
        GdkDisplay* display = gdk_window_get_display(window);
        struct wl_compositor* compositor =  gdk_wayland_display_get_wl_compositor(display);
        wl_region *region = wl_compositor_create_region(compositor);
        wl_region_add(region,0,0,w,h);
        //wl_surface_set_opaque_region(wl_surf, region);
#endif
#endif
        egl_window = wl_egl_window_create(wl_surf,w,h);
        //wl_egl_window_resize(egl_window,w,h,0,0);
        wl_surface_set_buffer_scale(wl_surf, s);
    }

    if (!egl_surface) {
        egl_surface = eglCreateWindowSurface (wxGLContextEgl::GetEglDisplay(GTKGetDrawingWindow()), egl_config, (EGLNativeWindowType)egl_window, NULL);
	EGLint err = eglGetError();
	int i=0;
    }
}

bool wxGLCanvasEgl::SetCurrentContext(EGLContext ctx) const
{
    const_cast<wxGLCanvasEgl*>(this)->EnsureEglWindowAndSurface();
    return eglMakeCurrent(wxGLContextEgl::GetEglDisplay(GTKGetDrawingWindow()),egl_surface,egl_surface,ctx);
}
#endif // wxUSE_GLCANVAS
