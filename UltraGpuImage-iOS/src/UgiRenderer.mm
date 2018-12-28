//
/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2018 Piasy
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
//


#import "UgiRenderer.h"

#import <GLKit/GLKit.h>

#import "UgiLogging.h"
#import "UgiTransformation+Native.h"

#include "ugi_renderer.h"
#include "filters/ugi_preprocess_filter.h"

#define TAG "UgiRenderer"

@implementation UgiRenderer {
    NSString* _name;

    dispatch_queue_t _renderQueue;

    EAGLContext* _glContext;
    GLuint _fbo;
    GLuint _rbo;

    Ugi::Renderer* _renderer;
    id<UgiFilter> _filter;
}

- (instancetype)init {
    return [self initWithName:@""];
}

- (instancetype)initWithName:(NSString*)name {
    if (self = [super init]) {
        _name = name;

        NSString* queueName =
            [NSString stringWithFormat:@"UgiRenderer-%@", _name];
        _renderQueue = dispatch_queue_create(queueName.UTF8String, NULL);

        __block EAGLContext* context = nil;
        // TODO: do we need this dispatch?
        dispatch_sync(_renderQueue, ^{
            context =
                [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES3];
        });
        if (!context) {
            [self logError:@"Failed to create EAGLContext"];
            _renderQueue = nil;
            return nil;
        }
        _glContext = context;

        _transformation = [[UgiTransformation alloc] init];
        _renderer = new Ugi::Renderer(*(_transformation.transformation));
    }

    [self logInfo:@"new UgiRenderer success"];

    return self;
}

- (void)dealloc {
    _renderQueue = nil;
    if (_renderer) {
        delete _renderer;
        _renderer = nullptr;
    }
}

- (void)onSurfaceCreated:(CAEAGLLayer*)eaglLayer {
    __weak UgiRenderer* weakSelf = self;
    [self runOnRenderThread:^{
        UgiRenderer* strongSelf = weakSelf;
        if (!strongSelf) {
            return;
        }
        eaglLayer.opaque = YES;
        eaglLayer.drawableProperties = @{
            kEAGLDrawablePropertyRetainedBacking : @(NO),
            kEAGLDrawablePropertyColorFormat : kEAGLColorFormatRGBA8
        };

        glGenFramebuffers(1, &strongSelf->_fbo);
        glGenRenderbuffers(1, &strongSelf->_rbo);
        glBindFramebuffer(GL_FRAMEBUFFER, strongSelf->_fbo);
        glBindRenderbuffer(GL_RENDERBUFFER, strongSelf->_rbo);
        dispatch_sync(dispatch_get_main_queue(), ^{
            [strongSelf->_glContext renderbufferStorage:GL_RENDERBUFFER
                                           fromDrawable:eaglLayer];
        });

        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                                  GL_RENDERBUFFER, strongSelf->_rbo);

        strongSelf->_renderer->OnSurfaceCreated();
    }];
}

- (void)onSurfaceDestroyed {
    __weak UgiRenderer* weakSelf = self;
    [self runOnRenderThread:^{
        UgiRenderer* strongSelf = weakSelf;
        if (!strongSelf) {
            return;
        }

        if (strongSelf->_fbo) {
            glDeleteFramebuffers(1, &(strongSelf->_fbo));
            strongSelf->_fbo = 0;
        }
        if (strongSelf->_rbo) {
            glDeleteRenderbuffers(1, &(strongSelf->_rbo));
            strongSelf->_rbo = 0;
        }
    }];
}

- (void)renderRgb:(GLuint)textureId {
    __weak UgiRenderer* weakSelf = self;
    [self runOnRenderThread:^{
        UgiRenderer* strongSelf = weakSelf;
        if (!strongSelf) {
            return;
        }
        strongSelf->_renderer->RenderTexture(Ugi::kTextureTypeRgb, textureId);
        [strongSelf->_glContext presentRenderbuffer:GL_RENDERBUFFER];
    }];
}

- (void)runOnRenderThread:(void (^)(void))task {
    __weak UgiRenderer* weakSelf = self;
    dispatch_async(_renderQueue, ^{
        UgiRenderer* strongSelf = weakSelf;
        if (!strongSelf) {
            return;
        }
        [EAGLContext setCurrentContext:strongSelf->_glContext];
        task();
        [EAGLContext setCurrentContext:nil];
    });
}

- (void)notifyTransformationUpdated {
    __weak UgiRenderer* weakSelf = self;
    [self runOnRenderThread:^{
        UgiRenderer* strongSelf = weakSelf;
        if (!strongSelf) {
            return;
        }
        strongSelf->_renderer->UpdateTransformation(
            *(strongSelf->_transformation.transformation));
    }];
}

- (void)setFilter:(id<UgiFilter>)filter {
    _filter = filter;
    Ugi::Filter* nativeFilter = (Ugi::Filter*) [filter nativeFilter];
    if (_renderer && nativeFilter) {
        _renderer->SetFilter(nativeFilter);
    }
}

- (void)logInfo:(NSString*)fmt, ... {
    va_list args;
    va_start(args, fmt);
    NSString* log = [NSString stringWithFormat:fmt, args];
    va_end(args);

    [UgiLogging info:@"%s %@(%p) %@", TAG, _name, self, log];
}

- (void)logError:(NSString*)fmt, ... {
    va_list args;
    va_start(args, fmt);
    NSString* log = [NSString stringWithFormat:fmt, args];
    va_end(args);

    [UgiLogging error:@"%s %@(%p) %@", TAG, _name, self, log];
}

@end
