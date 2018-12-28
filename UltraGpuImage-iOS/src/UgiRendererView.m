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


#import "UgiRendererView.h"

#include "ugi_gl.h"

#import "UgiRenderer.h"
#import "UgiLogging.h"

@implementation UgiRendererView {
    UgiRenderMode _renderMode;
    UgiRenderer* _renderer;

    GLuint _pictureTextureId;
    int _pictureWidth;
    int _pictureHeight;
}

- (instancetype)initWithFrame:(CGRect)frame andName:(NSString*)name {
    if (self = [super initWithFrame:frame]) {
        _pictureTextureId = -1;
    }
    return self;
}

+ (Class)layerClass {
    return [CAEAGLLayer class];
}

- (void)initialize:(NSString*)name renderMode:(UgiRenderMode)renderMode {
    if (!self.layer) {
        [UgiLogging error:@"UgiRendererView initialize error: no layer"];
        return;
    }
    _renderMode = renderMode;
    _renderer = [[UgiRenderer alloc] initWithName:name];
    if (!_renderer) {
        return;
    }

    [_renderer onSurfaceCreated:(CAEAGLLayer*)self.layer];
}

- (UgiTransformation*)transformation {
    return _renderer.transformation;
}

- (void)notifyTransformationUpdated {
    [_renderer notifyTransformationUpdated];
    if (_renderMode == kUgiRenderModePicture && _pictureTextureId != -1) {
        [_renderer renderRgb:_pictureTextureId];
    }
}

- (void)setFilter:(id<UgiFilter>)filter {
    [_renderer setFilter:filter];
    [self notifyFilterUpdated];
}

- (void)notifyFilterUpdated {
    if (_renderMode == kUgiRenderModePicture && _pictureTextureId != -1) {
        [_renderer renderRgb:_pictureTextureId];
    }
}

- (void)renderPicture:(UIImage*)picture {
    [self renderPicture:picture needFlipVertically:true];
}

- (void)renderPicture:(UIImage*)picture
    needFlipVertically:(bool)needFlipVertically {
    __weak UgiRendererView* weakSelf = self;
    [_renderer runOnRenderThread:^{
        UgiRendererView* strongSelf = weakSelf;
        if (!strongSelf) {
            return;
        }

        CGImageRef image;
        if (needFlipVertically) {
            // credit: https://stackoverflow.com/a/24799281/3077508
            UIGraphicsBeginImageContextWithOptions(picture.size, NO,
                                                   picture.scale);
            CGContextDrawImage(
                UIGraphicsGetCurrentContext(),
                CGRectMake(0, 0, picture.size.width, picture.size.height),
                picture.CGImage);
            UIImage* flippedPicture =
                UIGraphicsGetImageFromCurrentImageContext();
            UIGraphicsEndImageContext();
            image = flippedPicture.CGImage;
        } else {
            image = picture.CGImage;
        }
        strongSelf->_pictureWidth = (int)CGImageGetWidth(image);
        strongSelf->_pictureHeight = (int)CGImageGetHeight(image);

        CGSize size = strongSelf.layer.bounds.size;

        [strongSelf->_renderer.transformation
            updateInput:strongSelf->_pictureWidth
                 height:strongSelf->_pictureHeight];
        [strongSelf->_renderer.transformation updateOutput:(int)size.width
                                                    height:(int)size.height];
        [strongSelf->_renderer notifyTransformationUpdated];

        strongSelf->_pictureTextureId = [UgiRendererView loadTexture:image];
        [strongSelf->_renderer renderRgb:strongSelf->_pictureTextureId];
    }];
}

+ (GLuint)loadTexture:(CGImageRef)image {
    CGFloat width = CGImageGetWidth(image);
    CGFloat height = CGImageGetHeight(image);
    CFDataRef dataFromImageDataProvider =
        CGDataProviderCopyData(CGImageGetDataProvider(image));
    GLubyte* imageData = (GLubyte*)CFDataGetBytePtr(dataFromImageDataProvider);
    GLuint textureId = 0;
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    // TODO: GL_BGRA or GL_RGBA
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, (int)width, (int)height, 0, GL_BGRA,
                 GL_UNSIGNED_BYTE, imageData);
    return textureId;
}

@end
