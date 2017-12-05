package echo.textndk;

import java.util.ArrayList;

/**
 * 作者：EchoJ on 2017/11/29 14:28 <br>
 * 邮箱：echojiangyq@gmail.com <br>
 * 描述：
 */

public class NativeItfs {
    static {
        System.loadLibrary("test-jni-lib");
    }

    public static native ArrayList<Poi> testArgs(int a, byte[] bytes, String name, ArrayList<Poi> poi);
}
