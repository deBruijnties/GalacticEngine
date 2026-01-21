#pragma once

enum class FrameBufferFormat {
    RGBA8,
    RGBA16F,
    RGB16F,
    RGBA32F,
    R11F_G11F_B10F,
    RG16F,
    R16F,
    DEPTH_COMPONENT24
};

class FrameBuffer {
public:
    int width = 0, height = 0;
    unsigned int fbo = 0;
    unsigned int colorTex = 0;
    unsigned int depthTex = 0;

    unsigned int depthRbo = 0;

    FrameBuffer();
    ~FrameBuffer();

    // initialize framebuffer
    void init(int w, int h, FrameBufferFormat fmt = FrameBufferFormat::RGBA8, bool colorOnlyMode = false);

    // enable/disable color-only mode (recreates attachments)
    void setColorOnly(bool on);

    void bind();
    void unbind();
    void resize(int x, int y);

    void bindTex(int unit);

    void bindDepthTex(int unit);

private:
    FrameBufferFormat format = FrameBufferFormat::RGBA8;

    bool colorOnly = false;

    // store previous GL depth states for restoring after color-only binds
    unsigned int prevDepthTestEnabled = 0;
    unsigned int prevDepthWriteMask = 1;
};
