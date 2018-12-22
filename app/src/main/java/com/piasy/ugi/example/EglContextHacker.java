package com.piasy.ugi.example;

import android.opengl.EGLContext;
import java.lang.reflect.Field;
import org.webrtc.EglBase;

/**
 * Created by Piasy{github.com/Piasy} on 2018/12/22.
 */
public class EglContextHacker {
    public static EGLContext getContextFromEglBase(EglBase eglBase) {
        EglBase.Context context = eglBase.getEglBaseContext();
        try {
            Field f = context.getClass().getDeclaredField("egl14Context");
            f.setAccessible(true);
            return (EGLContext) f.get(context);
        } catch (NoSuchFieldException | IllegalAccessException e) {
            e.printStackTrace();
        }
        return null;
    }
}
