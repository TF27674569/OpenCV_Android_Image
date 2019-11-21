package com.ndk.sample;

import android.graphics.Bitmap;

/**
 * create by TIAN FENG on 2019/11/20
 */
public class NDKBitmapUtils {

    static {
        System.loadLibrary("native-lib");
    }

    public static final int VERTICAL = 0;
    public static final int HORIZONTAL = 1;


    /**
     * 实现逆世界效果
     * 默认VERTICAL 竖向逆世界，非VERTICAL就是水平逆世界
     */
    public static final Bitmap againstWorld(Bitmap bitmap) {
        return againstWorld(bitmap, VERTICAL);
    }

    public static final native Bitmap againstWorld(Bitmap bitmap, int orientation);

    /**
     * 浮雕效果
     *
     * @param bitmap 原图像
     * @return 浮雕效果图像
     */
    public static final native Bitmap anaglyph(Bitmap bitmap);

    /**
     * 实现马赛克效果
     *
     * @param bitmap 原图像
     * @return 马赛克图片
     */
    public static final native Bitmap mosaic(Bitmap bitmap);

    /**
     * 实现图片毛玻璃效果
     *
     * @param bitmap 原图像
     * @return 毛玻璃效果
     */
    public static final native Bitmap groundGlass(Bitmap bitmap);

    /**
     * 实现图像油画效果
     *
     * @param bitmap
     * @return 油画效果图像
     */
    public static final native Bitmap oilPainting(Bitmap bitmap);

    /**
     * 灰度图像处理效果
     *
     * @param bitmap 原图像
     * @return 优化后的灰度图像
     */
    public static final native Bitmap garyOptimize(Bitmap bitmap);

}
