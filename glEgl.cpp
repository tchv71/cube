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
    if ( val > 0 )
    {
        AddAttribute(EGL_CONTEXT_MAJOR_VERSION_KHR);
        AddAttribute(val);
        if ( val >= 3 )
            SetNeedsARB();
    }
    return *this;
}

wxEGLContextAttrs& wxEGLContextAttrs::MinorVersion(int val)
{
    if ( val >= 0 )
    {
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
    AddAttribute(0);
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
    AddAttribBits(EGL_SURFACE_TYPE, EGL_WINDOW_BIT);
    AddAttribBits(EGL_COLOR_BUFFER_TYPE, EGL_RGB_BUFFER);
    return *this;
}

wxEGLAttributes& wxEGLAttributes::BufferSize(int val)
{
    if ( val >= 0 )
    {
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
    if ( mRed >= 0)
    {
        AddAttribute(EGL_RED_SIZE);
        AddAttribute(mRed);
    }
    if ( mGreen >= 0)
    {
        AddAttribute(EGL_GREEN_SIZE);
        AddAttribute(mGreen);
    }
    if ( mBlue >= 0)
    {
        AddAttribute(EGL_BLUE_SIZE);
        AddAttribute(mBlue);
    }
    if ( mAlpha >= 0)
    {
        AddAttribute(EGL_ALPHA_SIZE);
        AddAttribute(mAlpha);
    }
    return *this;
}

wxEGLAttributes& wxEGLAttributes::Depth(int val)
{
    if ( val >= 0 )
    {
        AddAttribute(EGL_DEPTH_SIZE);
        AddAttribute(val);
    }
    return *this;
}

wxEGLAttributes& wxEGLAttributes::Stencil(int val)
{
    if ( val >= 0 )
    {
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
    AddAttribute(0);
}

wxEGLAttributes& wxEGLAttributes::PlatformDefaults()
{
    // No GLX specific values
    return *this;
}

wxEGLAttributes& wxEGLAttributes::Defaults()
{
    RGBA().DoubleBuffer().Depth(16).SampleBuffers(1).Samplers(4);
    return *this;
}

void wxEGLAttributes::AddDefaultsForWXBefore31()
{
    // ParseAttribList() will add EndList(), don't do it now
    DoubleBuffer();
    if ( wxGLCanvasEgl::GetGLXVersion() < 13 )
        RGBA().Depth(1).MinRGBA(1, 1, 1, 0);
    // For GLX >= 1.3 its defaults (GLX_RGBA_BIT and GLX_WINDOW_BIT) are OK
}


// ============================================================================
// wxGLContextEgl implementation
// ============================================================================

EGLDisplay wxGLContextEgl::GetEglDisplay (GdkWaylandDisplay *display_wayland)
{
  EGLDisplay dpy = NULL;
  struct wl_display* disp_wayland = gdk_wayland_display_get_wl_display(display_wayland);
  if (epoxy_has_egl_extension (NULL, "EGL_KHR_platform_base"))
    {
      PFNEGLGETPLATFORMDISPLAYPROC getPlatformDisplay = (PFNEGLGETPLATFORMDISPLAYPROC)eglGetProcAddress ("eglGetPlatformDisplay");

      if (getPlatformDisplay)
	dpy = getPlatformDisplay (EGL_PLATFORM_WAYLAND_EXT,
				  disp_wayland,
				  NULL);
      if (dpy)
	return dpy;
    }

  if (epoxy_has_egl_extension (NULL, "EGL_EXT_platform_base"))
    {
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

bool wxGLContextEgl::have_egl = true;
EGLDisplay wxGLContextEgl::egl_display;


gboolean wxGLContextEgl::InitGl (GdkDisplay *display)
{
  GdkWaylandDisplay *display_wayland = GDK_WAYLAND_DISPLAY (display);
  EGLint major, minor;
  EGLDisplay dpy;

  if (have_egl)
    return TRUE;

  dpy = GetEglDisplay (display_wayland);

  if (dpy == NULL)
    return FALSE;

  if (!eglInitialize (dpy, &major, &minor))
    return FALSE;

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
    int renderType = GLX_RGBA_TYPE;
    bool needsARB = false;

    if ( ctxAttrs )
    {
        contextAttribs = ctxAttrs->GetGLAttrs();
        x11Direct = ctxAttrs->x11Direct;
        renderType = ctxAttrs->renderTypeRGBA ? GLX_RGBA_TYPE : GLX_COLOR_INDEX_TYPE;
        needsARB = ctxAttrs->NeedsARB();
    }
    else if ( win->GetGLCTXAttrs().GetGLAttrs() )
    {
        // If OpenGL context parameters were set at wxGLCanvasNew ctor, get them now
        contextAttribs = win->GetGLCTXAttrs().GetGLAttrs();
        x11Direct = win->GetGLCTXAttrs().x11Direct;
        renderType = win->GetGLCTXAttrs().renderTypeRGBA ? GLX_RGBA_TYPE : GLX_COLOR_INDEX_TYPE;
        needsARB = win->GetGLCTXAttrs().NeedsARB();
    }
    // else use GPU driver defaults and x11Direct renderType ones

    m_isOk = false;
    
    GdkWindow* window = win->GTKGetDrawingWindow();
    GdkDisplay *display = gdk_window_get_display (window);
    if (!GDK_IS_WAYLAND_DISPLAY(display))
        return;
    if (!InitGl(display))
        return;
    EGLConfig config = win->GetEglConfig();
    
    Display* dpy = wxGetX11Display();
    XVisualInfo *vi = win->GetXVisualInfo();
    wxCHECK_RET( vi, "invalid visual for OpenGL" );

    // We need to create a temporary context to get the
    // glXCreateContextAttribsARB function
    GLXContext tempContext = glXCreateContext(dpy, vi, NULL, x11Direct);
    wxCHECK_RET(tempContext, "glXCreateContext failed" );

    PFNGLXCREATECONTEXTATTRIBSARBPROC wx_glXCreateContextAttribsARB
        = (PFNGLXCREATECONTEXTATTRIBSARBPROC)
        glXGetProcAddress((GLubyte *)"glXCreateContextAttribsARB");

    glXDestroyContext( dpy, tempContext );

    // The preferred way is using glXCreateContextAttribsARB, even for old context
    if ( !wx_glXCreateContextAttribsARB && needsARB ) // OpenGL 3 context creation
    {
        wxLogMessage(_("OpenGL 3.0 or later is not supported by the OpenGL driver."));
        return;
    }

    // Install a X error handler, so as to the app doesn't exit (without
    // even a warning) if GL >= 3.0 context creation fails
    //g_ctxErrorOccurred = false;
    //int (*oldHandler)(Display*, XErrorEvent*) = XSetErrorHandler(&CTXErrorHandler);

    if ( wx_glXCreateContextAttribsARB )
    {
        GLXFBConfig *fbc = NULL;// win->GetGLXFBConfig();
        wxCHECK_RET( fbc, "Invalid GLXFBConfig for OpenGL" );

        m_glContext = wx_glXCreateContextAttribsARB( dpy, fbc[0],
                                other ? other->m_glContext : None,
                                x11Direct, contextAttribs );

        // Some old hardware may accept the use of this ARB, but may fail.
        // In case of NULL attributes we'll try creating the context old-way.
        XSync( dpy, False );
        if ( g_ctxErrorOccurred && (!contextAttribs || !needsARB) )
        {
            g_ctxErrorOccurred = false; //Reset
            m_glContext = NULL;
        }
    }

    if ( !g_ctxErrorOccurred && !m_glContext )
    {
        // Old-way, without context atributes. Up to GL 2.1
/*        if ( wxGLCanvasNew::GetGLXVersion() >= 13 )
        {
            GLXFBConfig *fbc = win->GetGLXFBConfig();
            wxCHECK_RET( fbc, "Invalid GLXFBConfig for OpenGL" );

            m_glContext = glXCreateNewContext( dpy, fbc[0], renderType,
                                               other ? other->m_glContext : None,
                                               x11Direct );
        }
        else // GLX <= 1.2*/
        {
            m_glContext = glXCreateContext( dpy, vi,
                                            other ? other->m_glContext : None,
                                            x11Direct );
        }
    }

    // Sync to ensure any errors generated are processed.
    XSync( dpy, False );

    if ( g_ctxErrorOccurred || !m_glContext )
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

    if ( m_glContext == glXGetCurrentContext() )
        MakeCurrent(None, NULL);

    glXDestroyContext( wxGetX11Display(), m_glContext );
}

bool wxGLContextEgl::SetCurrent(const wxGLCanvasNew& win) const
{
    if ( !m_glContext )
        return false;

    return true;
//    const Window xid = win.GetXWindow();
//    wxCHECK2_MSG( xid, return false, wxT("window must be shown") );
//
//    return MakeCurrent(xid, m_glContext);
}

// wrapper around glXMakeContextCurrent/glXMakeCurrent depending on GLX
// version
/* static */
bool wxGLContextEgl::MakeCurrent(GLXDrawable drawable, GLXContext context)
{
//    if (wxGLCanvasNew::GetGLXVersion() >= 13)
//        return glXMakeContextCurrent( wxGetX11Display(), drawable, drawable, context);
//    else // GLX <= 1.2 doesn't have glXMakeContextCurrent()
//        return glXMakeCurrent( wxGetX11Display(), drawable, context);
}

// ============================================================================
// wxGLCanvasEgl implementation
// ============================================================================

// ----------------------------------------------------------------------------
// initialization methods and dtor
// ----------------------------------------------------------------------------

wxGLCanvasEgl::wxGLCanvasEgl()
{
    m_eglConfig = NULL;
}

bool wxGLCanvasEgl::InitVisual(const wxGLAttributes& dispAttrs)
{
    bool ret = InitXVisualInfo(dispAttrs, &m_fbc, &m_vi);
    if ( !ret )
    {
        wxFAIL_MSG("Failed to get a XVisualInfo for the requested attributes.");
    }
    return ret;
}

wxGLCanvasEgl::~wxGLCanvasEgl()
{
    if ( m_fbc && m_fbc != ms_glFBCInfo )
        XFree(m_fbc);

    if ( m_vi && m_vi != ms_glVisualInfo )
        XFree(m_vi);
}

// ----------------------------------------------------------------------------
// working with GL attributes
// ----------------------------------------------------------------------------



/* static */
bool wxGLCanvasEgl::IsGLXMultiSampleAvailable()
{
    static int s_isMultiSampleAvailable = -1;
    if ( s_isMultiSampleAvailable == -1 )
        s_isMultiSampleAvailable = IsExtensionSupported("GLX_ARB_multisample");

    return s_isMultiSampleAvailable != 0;
}


/* static */
bool wxGLCanvasEgl::InitXVisualInfo(const wxGLAttributes& dispAttrs,
                                    GLXFBConfig** pFBC,
                                    XVisualInfo** pXVisual)
{
    // GLX_XX attributes
    const int* attrsListGLX = dispAttrs.GetGLAttrs();
    if ( !attrsListGLX )
    {
        wxFAIL_MSG("wxEGLAttributes object is empty.");
        return false;
    }

    Display* dpy = wxGetX11Display();

    if ( GetGLXVersion() >= 13 )
    {
        int returned;
        *pFBC = glXChooseFBConfig(dpy, DefaultScreen(dpy), attrsListGLX, &returned);

        if ( *pFBC )
        {
            // Use the first good match
            *pXVisual = glXGetVisualFromFBConfig(wxGetX11Display(), **pFBC);
            if ( !*pXVisual )
            {
                XFree(*pFBC);
                *pFBC = NULL;
            }
        }
    }
    else // GLX <= 1.2
    {
        *pFBC = NULL;
        *pXVisual = glXChooseVisual(dpy, DefaultScreen(dpy),
                                    const_cast<int*>(attrsListGLX) );
    }

    return *pXVisual != NULL;
}

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

// ----------------------------------------------------------------------------
// default visual management
// ----------------------------------------------------------------------------

XVisualInfo *wxGLCanvasEgl::ms_glVisualInfo = NULL;
GLXFBConfig *wxGLCanvasEgl::ms_glFBCInfo = NULL;

/* static */
bool wxGLCanvasEgl::InitDefaultVisualInfo(const int *attribList)
{
    FreeDefaultVisualInfo();
    wxEGLAttributes dispAttrs;
    ParseAttribList(attribList, dispAttrs);

    return InitXVisualInfo(dispAttrs, &ms_glFBCInfo, &ms_glVisualInfo);
}

/* static */
void wxGLCanvasEgl::FreeDefaultVisualInfo()
{
    if ( ms_glFBCInfo )
    {
        XFree(ms_glFBCInfo);
        ms_glFBCInfo = NULL;
    }

    if ( ms_glVisualInfo )
    {
        XFree(ms_glVisualInfo);
        ms_glVisualInfo = NULL;
    }
}

// ----------------------------------------------------------------------------
// other GL methods
// ----------------------------------------------------------------------------

/* static */
int wxGLCanvasEgl::GetGLXVersion()
{
    static int s_glxVersion = 0;
    if ( s_glxVersion == 0 )
    {
        // check the GLX version
        int glxMajorVer, glxMinorVer;
        bool ok = glXQueryVersion(wxGetX11Display(), &glxMajorVer, &glxMinorVer);
        wxASSERT_MSG( ok, wxT("GLX version not found") );
        if (!ok)
            s_glxVersion = 10; // 1.0 by default
        else
            s_glxVersion = glxMajorVer*10 + glxMinorVer;
    }

    return s_glxVersion;
}

bool wxGLCanvasEgl::SwapBuffers()
{
    const Window xid = GetXWindow();
    wxCHECK2_MSG( xid, return false, wxT("window must be shown") );

    glXSwapBuffers(wxGetX11Display(), xid);
    return true;
}

bool wxGLCanvasEgl::IsShownOnScreen() const
{
    return GetXWindow() && wxGLCanvasBase::IsShownOnScreen();
}
#endif // wxUSE_GLCANVAS

