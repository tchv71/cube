// ----------------------------------------------------------------------------
// wxGLAttributes: Visual/FBconfig attributes
// ----------------------------------------------------------------------------
// Gdk specific values
wxGLContextAttrs& wxGLContextAttrs::CoreProfile()
{
    SetNeedsARB();
    return *this;
}

wxGLContextAttrs& wxGLContextAttrs::MajorVersion(int val)
{
    return *this;
}

wxGLContextAttrs& wxGLContextAttrs::MinorVersion(int val)
{
    return *this;
}

wxGLContextAttrs& wxGLContextAttrs::CompatibilityProfile()
{
    SetNeedsARB();
    return *this;
}

wxGLContextAttrs& wxGLContextAttrs::ForwardCompatible()
{
    SetNeedsARB();
    return *this;
}

wxGLContextAttrs& wxGLContextAttrs::ES2()
{
    SetNeedsARB();
    return *this;
}

wxGLContextAttrs& wxGLContextAttrs::DebugCtx()
{
    SetNeedsARB();
    return *this;
}

wxGLContextAttrs& wxGLContextAttrs::Robust()
{
    SetNeedsARB();
    return *this;
}

wxGLContextAttrs& wxGLContextAttrs::NoResetNotify()
{
    SetNeedsARB();
    return *this;
}

wxGLContextAttrs& wxGLContextAttrs::LoseOnReset()
{
    SetNeedsARB();
    return *this;
}

wxGLContextAttrs& wxGLContextAttrs::ResetIsolation()
{
    SetNeedsARB();
    return *this;
}

wxGLContextAttrs& wxGLContextAttrs::ReleaseFlush(int val)
{
    SetNeedsARB();
    return *this;
}

wxGLContextAttrs& wxGLContextAttrs::PlatformDefaults()
{
    renderTypeRGBA = true;
    x11Direct = true;
    return *this;
}

void wxGLContextAttrs::EndList()
{
    AddAttribute(None);
}


wxGLAttributes& wxGLAttributes::RGBA()
{
    return *this;
}

wxGLAttributes& wxGLAttributes::BufferSize(int val)
{
    return *this;
}

wxGLAttributes& wxGLAttributes::Level(int val)
{
    return *this;
}

wxGLAttributes& wxGLAttributes::DoubleBuffer()
{
    return *this;
}

wxGLAttributes& wxGLAttributes::Stereo()
{
    return *this;
}

wxGLAttributes& wxGLAttributes::AuxBuffers(int val)
{
    return *this;
}

wxGLAttributes& wxGLAttributes::MinRGBA(int mRed, int mGreen, int mBlue, int mAlpha)
{
    return *this;
}

wxGLAttributes& wxGLAttributes::Depth(int val)
{
    return *this;
}

wxGLAttributes& wxGLAttributes::Stencil(int val)
{
    return *this;
}

wxGLAttributes& wxGLAttributes::MinAcumRGBA(int mRed, int mGreen, int mBlue, int mAlpha)
{
    return *this;
}

wxGLAttributes& wxGLAttributes::SampleBuffers(int val)
{
    return *this;
}

wxGLAttributes& wxGLAttributes::Samplers(int val)
{
    return *this;
}

wxGLAttributes& wxGLAttributes::FrameBuffersRGB()
{
    return *this;
}

void wxGLAttributes::EndList()
{
    AddAttribute(None);
}

wxGLAttributes& wxGLAttributes::PlatformDefaults()
{
    // No GLX specific values
    return *this;
}

wxGLAttributes& wxGLAttributes::Defaults()
{
    RGBA().DoubleBuffer().Depth(16).SampleBuffers(1).Samplers(4);
    return *this;
}

void wxGLAttributes::AddDefaultsForWXBefore31()
{
}

