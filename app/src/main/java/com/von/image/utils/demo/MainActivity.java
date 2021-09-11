package com.von.image.utils.demo;

import android.os.Bundle;

import androidx.appcompat.app.AppCompatActivity;
import androidx.databinding.DataBindingUtil;

import com.von.image.utils.demo.databinding.ActivityMainBinding;
import com.von.image.utils.lib.NativeImageUtils;

public class MainActivity extends AppCompatActivity {

    private ActivityMainBinding mBinding;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        mBinding = DataBindingUtil.setContentView(this, R.layout.activity_main);
        mBinding.tvContent.setText(NativeImageUtils.testSwig("hello world"));
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        mBinding = null;
    }
}