# UltraGpuImage

The ultimate 2D image rendering engine, inspired from [cats-oss/android-gpuimage](https://github.com/cats-oss/android-gpuimage), reinventing the wheel as my first OpenGL project after walking through the [Learn OpenGL](https://learnopengl.com/) tutorial.

_This project is still under development, stay tuned!_

## Architecture

UltraGpuImage has 3 layers:

+ Core OpenGL logic, including filter hierarchy, writing in C++ (cross-platform);
+ OpenGL context and window management, together with Renderer API, writing in platform-specific code (Java, ObjC, etc.);
+ View wrapper, providing the most easy-to-use API, writing in platform-specific code (Java, ObjC, etc.);

## Get started

TODO

### iOS project setup

Install [yonaskolb/XcodeGen](https://github.com/yonaskolb/XcodeGen) and CocoaPods, then run `setup_xcode_project.sh`, then open `UltraGpuImage.xcworkspace` with Xcode.

## Check points

+ [x] Android Bitmap rendering
+ [x] Android OES camera preview
+ [x] iOS UIImage rendering
+ [ ] iOS camera preview
+ [ ] more filters
+ [ ] support other platforms
