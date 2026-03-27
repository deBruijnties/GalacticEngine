#include "pch.h"
#include "FrameBuffer.h"
#include <glad/glad.h>
#include <iostream>

static void chooseFormat(FrameBufferFormat format,
    GLint& internalFormat, GLenum& dataFormat, GLenum& dataType)
{
    switch (format)
    {
    case FrameBufferFormat::RGBA8:
        internalFormat = GL_RGBA8;
        dataFormat = GL_RGBA;
        dataType = GL_UNSIGNED_BYTE;
        break;

    case FrameBufferFormat::RGBA16F:
        internalFormat = GL_RGBA16F;
        dataFormat = GL_RGBA;
        dataType = GL_FLOAT;
        break;
    case FrameBufferFormat::RGBA32F:
        internalFormat = GL_RGBA32F;
        dataFormat = GL_RGBA;
        dataType = GL_FLOAT;
        break;

    case FrameBufferFormat::RGB16F:
        internalFormat = GL_RGB16F;
        dataFormat = GL_RGB;
        dataType = GL_FLOAT;
        break;

    case FrameBufferFormat::R11F_G11F_B10F:
        internalFormat = GL_R11F_G11F_B10F;
        dataFormat = GL_RGB;
        dataType = GL_UNSIGNED_INT_10F_11F_11F_REV;
        break;

    case FrameBufferFormat::RG16F:
        internalFormat = GL_RG16F;
        dataFormat = GL_RG;
        dataType = GL_FLOAT;
        break;

    case FrameBufferFormat::R16F:
        internalFormat = GL_R16F;
        dataFormat = GL_RED;
        dataType = GL_FLOAT;
        break;
    case FrameBufferFormat::DEPTH_COMPONENT24:
        internalFormat = GL_DEPTH_COMPONENT24;
        dataFormat = GL_DEPTH_COMPONENT;
        dataType = GL_FLOAT;
        break;


    default:
        // fallback
        internalFormat = GL_RGBA8;
        dataFormat = GL_RGBA;
        dataType = GL_UNSIGNED_BYTE;
        break;
    }
}

//  Constructor: initialize members to safe defaults 
FrameBuffer::FrameBuffer()
    : fbo(0)
    , colorTex(0)
    , depthRbo(0)
    , width(0)
    , height(0)
    , format(FrameBufferFormat::RGBA8)
    , colorOnly(false)
    , prevDepthTestEnabled(GL_FALSE)
    , prevDepthWriteMask(GL_TRUE)
{
}

FrameBuffer::~FrameBuffer() {
    if (depthTex) { glDeleteTextures(1, &depthTex); depthTex = 0; }   // <- delete old depthTex
    if (depthRbo) { glDeleteRenderbuffers(1, &depthRbo); depthRbo = 0; }
    if (colorTex) { glDeleteTextures(1, &colorTex); colorTex = 0; }
    if (fbo) { glDeleteFramebuffers(1, &fbo); fbo = 0; }

}

void FrameBuffer::init(int w, int h, FrameBufferFormat fmt, bool colorOnlyMode /*= false*/)
{
    format = fmt;
    colorOnly = colorOnlyMode;
    resize(w, h);
}

void FrameBuffer::setColorOnly(bool on)
{
    if (colorOnly == on) return;
    colorOnly = on;
    // resize to re-create attachments accordingly with current size/format
    resize(width, height);
}

void FrameBuffer::bind() {
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    GLenum drawbuf = GL_COLOR_ATTACHMENT0;
    glDrawBuffers(1, &drawbuf);
    glViewport(0, 0, width, height);

    if (colorOnly) {
        glDisable(GL_DEPTH_TEST);   // FORCE depth off
        glDepthMask(GL_FALSE);      // FORCE depth writes off
    }
    else {
        // ensure depth is enabled when we want depth
        glEnable(GL_DEPTH_TEST);
        glDepthMask(GL_TRUE);
    }
}


void FrameBuffer::unbind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    GLenum drawbuf = GL_COLOR_ATTACHMENT0;
    glDrawBuffers(1, &drawbuf);
    glDrawBuffer(GL_BACK);

    if (colorOnly) {
        glEnable(GL_DEPTH_TEST);
        glDepthMask(GL_TRUE);
    }
    
}


void FrameBuffer::resize(int x, int y) {
    width = x;
    height = y;

    // delete previous GL objects (if any)
    if (depthTex) { glDeleteTextures(1, &depthTex); depthTex = 0; }   // <- delete old depthTex
    if (depthRbo) { glDeleteRenderbuffers(1, &depthRbo); depthRbo = 0; }
    if (colorTex) { glDeleteTextures(1, &colorTex); colorTex = 0; }
    if (fbo) { glDeleteFramebuffers(1, &fbo); fbo = 0; }

    // create fbo
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    // choose internal format / formats
    GLint internalFormat;
    GLenum dataFormat, dataType;
    chooseFormat(format, internalFormat, dataFormat, dataType);

    // create color texture
    glGenTextures(1, &colorTex);
    glBindTexture(GL_TEXTURE_2D, colorTex);

    // allocate storage
    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, dataFormat, dataType, nullptr);

    // filtering/wrap - common defaults
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // clamp to edge avoids bleeding for blur passes
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); // no mipmaps
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // attach to FBO
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorTex, 0);

    // depth/stencil renderbuffer: only create if NOT in color-only mode
    // create depth texture instead of renderbuffer
    if (!colorOnly)
    {
        glGenTextures(1, &depthTex);
        glBindTexture(GL_TEXTURE_2D, depthTex);

        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_DEPTH_COMPONENT32F,
            width, height,
            0,
            GL_DEPTH_COMPONENT,
            GL_FLOAT,
            nullptr
        );


        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_NONE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_NONE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);

        glFramebufferTexture2D(
            GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
            GL_TEXTURE_2D, depthTex, 0
        );
    }

    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (status != GL_FRAMEBUFFER_COMPLETE)
    {
        std::cout << "ERROR: Framebuffer incomplete! Status = 0x"
            << std::hex << status << std::dec << std::endl;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer::bindTex(int unit) {
    if (!colorTex) return;
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_2D, colorTex);
}

void FrameBuffer::bindDepthTex(int unit) {
    if (!depthTex) return;
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_2D, depthTex);
}
