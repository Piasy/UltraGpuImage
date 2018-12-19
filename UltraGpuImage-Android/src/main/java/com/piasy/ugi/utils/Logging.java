package com.piasy.ugi.utils;

import android.util.Log;

/**
 * Created by Piasy{github.com/Piasy} on 2018/12/17.
 */
public class Logging {
    private static Loggable sLoggable;
    private static boolean sLogToConsole;

    public static void setLoggable(final Loggable loggable) {
        sLoggable = loggable;
    }

    public static void setLogToConsole(final boolean logToConsole) {
        sLogToConsole = logToConsole;
    }

    public static void info(String tag, String message) {
        if (sLoggable != null) {
            sLoggable.info(tag, message);
        }
        if (sLogToConsole) {
            Log.i(tag, message);
        }
    }

    public static void error(String tag, String message) {
        error(tag, message, null);
    }

    public static void error(String tag, String message, Throwable e) {
        if (sLoggable != null) {
            sLoggable.error(tag, message, e);
        }
        if (sLogToConsole) {
            Log.e(tag, message, e);
        }
    }

    public interface Loggable {
        void info(String tag, String message);

        void error(String tag, String message, Throwable e);
    }
}
