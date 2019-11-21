package com.ndk.sample;


import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.ImageView;
import android.widget.Toast;

import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;


public class MainActivity extends AppCompatActivity {

    private static final String TAG = "MainActivity";


    final int resId = R.drawable.image1;

    private ImageView imageView;

    ExecutorService executorService = Executors.newSingleThreadExecutor();


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        imageView = findViewById(R.id.imageView);
    }


    public void default_(View view) {
        imageView.setImageResource(resId);
    }

    public void againstWorld_v(View view) {
        Bitmap bitmap = NDKBitmapUtils.againstWorld(BitmapFactory.decodeResource(getResources(), resId));
        imageView.setImageBitmap(bitmap);
    }

    public void againstWorld_h(View view) {
        Bitmap bitmap = NDKBitmapUtils.againstWorld(BitmapFactory.decodeResource(getResources(), resId), NDKBitmapUtils.HORIZONTAL);
        imageView.setImageBitmap(bitmap);
    }

    public void anaglyph(View view) {
        Bitmap bitmap = NDKBitmapUtils.anaglyph(BitmapFactory.decodeResource(getResources(), resId));
        imageView.setImageBitmap(bitmap);
    }

    public void mosaic(View view) {
        Bitmap bitmap = NDKBitmapUtils.mosaic(BitmapFactory.decodeResource(getResources(), resId));
        imageView.setImageBitmap(bitmap);
    }

    public void groundGlass(View view) {
        Bitmap bitmap = NDKBitmapUtils.groundGlass(BitmapFactory.decodeResource(getResources(), resId));
        imageView.setImageBitmap(bitmap);
    }

    public void oilPainting(View view) {
        Toast.makeText(this, "油画较耗时，请稍等", Toast.LENGTH_SHORT).show();

        executorService.execute(new Runnable() {
            @Override
            public void run() {
                final Bitmap bitmap = NDKBitmapUtils.oilPainting(BitmapFactory.decodeResource(getResources(), R.drawable.image1));

                imageView.post(new Runnable() {
                    @Override
                    public void run() {
                        imageView.setImageBitmap(bitmap);
                        Toast.makeText(MainActivity.this, "油画效果，请欣赏", Toast.LENGTH_SHORT).show();
                    }
                });
            }
        });

    }


    public void gray(View view) {
        Bitmap bitmap = NDKBitmapUtils.garyOptimize(BitmapFactory.decodeResource(getResources(), R.drawable.image1));
        imageView.setImageBitmap(bitmap);
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        if (!executorService.isShutdown()) {
            executorService.shutdown();
        }
    }

}
