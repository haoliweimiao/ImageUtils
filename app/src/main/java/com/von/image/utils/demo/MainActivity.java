package com.von.image.utils.demo;

import android.annotation.SuppressLint;
import android.graphics.Bitmap;
import android.os.Bundle;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;
import androidx.databinding.DataBindingUtil;

import com.von.image.utils.demo.databinding.ActivityMainBinding;
import com.von.image.utils.lib.NativeImageUtils;

import java.io.InputStream;

public class MainActivity extends AppCompatActivity {

    private static final String TAG = "ImageUtils";
    private ActivityMainBinding mBinding;
    private NV21ToBitmapUtil nv21ToBitmap;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        mBinding = DataBindingUtil.setContentView(this, R.layout.activity_main);
        nv21ToBitmap = new NV21ToBitmapUtil(this);
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        getMenuInflater().inflate(R.menu.menu, menu);
        return super.onCreateOptionsMenu(menu);
    }

    @SuppressLint("NonConstantResourceId")
    @Override
    public boolean onOptionsItemSelected(@NonNull MenuItem item) {
        switch (item.getItemId()) {
            case R.id.menu_show_nv21:
                showNV21Image();
                break;
            case R.id.menu_show_yuy2:
                showYuy2Image();
                break;
            default:
                break;
        }
        return super.onOptionsItemSelected(item);
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        mBinding = null;
    }

    /**
     * show yuy2 image
     */
    private void showYuy2Image() {
        byte[] imageData = null;
        try {
            InputStream is = getAssets().open("image_w720_h1280.yuy2");
            int size = is.available();
            imageData = new byte[size];
            is.read(imageData);
            is.close();
        } catch (Exception ex) {
            ex.printStackTrace();
        }

        if (imageData != null) {
            int width = 1280;
            int height = 720;

            byte[] nv21Data = new byte[width * height * 3 / 2];
            NativeImageUtils.yuy2ToNV21(imageData, nv21Data, width, height, 90, false);
            Bitmap bitmap = nv21ToBitmap.nv21ToBitmap(nv21Data, height, width);
            if (bitmap != null) {
                runOnUiThread(() -> {
                    mBinding.imgContent.setImageBitmap(bitmap);
                });
            }
        }
    }

    /**
     * show nv21 image
     */
    private void showNV21Image() {
        byte[] imageData = null;
        try {
            InputStream is = getAssets().open("image_w720_h1280.nv21");
            int size = is.available();
            imageData = new byte[size];
            is.read(imageData);
            is.close();
        } catch (Exception ex) {
            ex.printStackTrace();
        }

        if (imageData != null) {
            int width = 720;
            int height = 1280;
            int length = width * height * 3 / 2;
            byte[] i420Cache = new byte[length];

            long allStartTime = System.currentTimeMillis();
            Bitmap bitmap = null;
            long startTime = System.currentTimeMillis();

//            {
//                NativeImageUtils.nv21ToI420(imageData, i420Cache, width, height);// ok
//                Log.i(TAG, "nativeNV21ToI420 time : " + (System.currentTimeMillis() - startTime));
//
//                byte[] i420MirrorCache = new byte[length];
//                startTime = System.currentTimeMillis();
//                NativeImageUtils.i420Mirror(i420Cache, i420MirrorCache, width, height);
//                Log.i(TAG, "nativeMirrorI420 time : " + (System.currentTimeMillis() - startTime));
//
//                // 90 270 OK
//                byte[] i420RotateCache = new byte[length];
//                startTime = System.currentTimeMillis();
//                NativeImageUtils.i420Rotate(i420MirrorCache, i420RotateCache, width, height, 270);
//                Log.i(TAG, "nativeRotateI420 time : " + (System.currentTimeMillis() - startTime));
//
//                startTime = System.currentTimeMillis();
//                NativeImageUtils.i420ToNV21(i420RotateCache, imageData, height, width);
//                Log.i(TAG, "nativeI420ToNV21 time : " + (System.currentTimeMillis() - startTime));
//                Log.i(TAG, "all time : " + (System.currentTimeMillis() - allStartTime));
//
//                bitmap = nv21ToBitmap.nv21ToBitmap(imageData, height, width);
//
//                if (bitmap != null) {
//                    Bitmap finalBitmap = bitmap;
//                    runOnUiThread(() -> {
//                        mBinding.imgContent.setImageBitmap(finalBitmap);
//                    });
//                }
//            }


            // 与上述步骤执行顺序一致
            {
                NativeImageUtils.nv21Transform(imageData, i420Cache, width, height, 270, true);
                Log.i(TAG, "nativeNV21Rotate time : " + (System.currentTimeMillis() - startTime));


                bitmap = nv21ToBitmap.nv21ToBitmap(i420Cache, height, width);

                if (bitmap != null) {
                    Bitmap finalBitmap = bitmap;
                    runOnUiThread(() -> {
                        mBinding.imgContent.setImageBitmap(finalBitmap);
                    });
                }
            }
        }
    }
}