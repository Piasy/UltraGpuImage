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


#import "ViewController.h"

#import <SDAutoLayout/SDAutoLayout.h>
#import <UltraGpuImage_iOS/UltraGpuImage_iOS.h>

@implementation ViewController {
    UgiRendererView* _rendererView;
    UgiTransformation* _transformation;
}

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view, typically from a nib.

    _rendererView = [[UgiRendererView alloc] init];
    [self.view addSubview:_rendererView];
    _rendererView.sd_layout.widthRatioToView(self.view, 1)
        .heightRatioToView(self.view, 1);

    [_rendererView initialize:@"test" renderMode:kUgiRenderModePicture];
    _transformation = [_rendererView transformation];

    UgiFilterGroup* filterGroup = [[UgiFilterGroup alloc] init];
    [filterGroup addFilter:[[UgiPreprocessFilter alloc] init]];
    [_rendererView setFilter:filterGroup];

    [_rendererView renderPicture:[UIImage imageNamed:@"awesomeface"]];
}


@end
