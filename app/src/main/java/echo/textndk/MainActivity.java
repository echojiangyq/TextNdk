package echo.textndk;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.widget.TextView;

import com.alibaba.fastjson.JSON;

import java.util.ArrayList;

public class MainActivity extends AppCompatActivity {

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        // Example of a call to a native method
        TextView tv = (TextView) findViewById(R.id.sample_text);
        tv.setText(stringFromJNI());

        testJni();
    }

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    public native String stringFromJNI();
    
    public static void testJni(){
        final ArrayList<Poi> pois = new ArrayList<>();
        
        for (int i=0; i<3; i++){
            final Poi poi = new Poi();
            poi.age = i;
            poi.name = "echo" + i;
            poi.isHot = i % 2==0;
            poi.data = new byte[3];
            for (int j=0; j<3; j++){
                poi.data[j] = (byte) (j + i);
            }
            pois.add(poi);     
        }
        Log.d("java-jni", JSON.toJSONString(pois, true));
        Log.d("java-jni", "==============================================================================");
        final ArrayList<Poi> poisRet = NativeItfs.testArgs(10086, new byte[]{101, 102, 103}, "hello echo", pois);
        Log.d("java-jni", JSON.toJSONString(poisRet, true));
    }
}
