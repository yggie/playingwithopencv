package com.example.bryan.playingwithopencv;

import android.app.Activity;
import android.content.Intent;
import android.content.res.AssetManager;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.provider.MediaStore;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.widget.ImageView;
import android.widget.SeekBar;

import java.io.IOException;
import java.io.InputStream;

public class MainActivity extends AppCompatActivity {

    private static final int CAPTURE_IMAGE = 100;
    private ImageView imageView;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        this.imageView = (ImageView)findViewById(R.id.imageView);
        final SeekBar seekBar = (SeekBar)findViewById(R.id.seek_bar);

        final Bitmap bitmap = this.loadBitmap();
        final Bitmap resultBitmap = Bitmap.createBitmap(bitmap.getWidth(), bitmap.getHeight(), bitmap.getConfig());

        final int initialProgress = 50;
        this.applyFilter(bitmap, resultBitmap, initialProgress / 100.0f);

        this.imageView.setImageBitmap(resultBitmap);

        seekBar.setProgress(initialProgress);
        seekBar.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            @Override
            public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
                MainActivity.this.applyFilter(bitmap, resultBitmap, progress / 100.0f);
                MainActivity.this.imageView.setImageBitmap(resultBitmap);
            }

            @Override
            public void onStartTrackingTouch(SeekBar seekBar) {

            }

            @Override
            public void onStopTrackingTouch(SeekBar seekBar) {

            }
        });
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        if (requestCode == CAPTURE_IMAGE && resultCode == Activity.RESULT_OK) {
            final Bundle extras = data.getExtras();
            final Bitmap bitmap = (Bitmap)extras.get("data");
            imageView.setImageBitmap(bitmap);
        }
    }

    private  Bitmap loadBitmap() {
        return BitmapFactory.decodeResource(getResources(), R.drawable.test_image);
    }

    private native String stringFromJNI();
    private native Bitmap applyFilter(Bitmap source, Bitmap destination, float intensity);

    static {
        System.loadLibrary("hello-jni");
    }
}
