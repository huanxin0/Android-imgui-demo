package com.NeKo.WuLi.ImGui;

import android.content.Context;
import android.graphics.PixelFormat;
import android.os.RemoteException;
import android.util.Log;
import android.view.Surface;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.graphics.SurfaceTexture;
import android.view.TextureView;
import androidx.annotation.NonNull;
import com.NeKo.WuLi.AppConfig;
import com.NeKo.WuLi.ImGui.ImGuiJNI;
import com.NeKo.WuLi.ImGui.Root.ImGuiAidlService;

public class ImGuiTextureView extends TextureView implements TextureView.SurfaceTextureListener {
	
    public ImGuiTextureView(Context context) {
        super(context);
        this.setOpaque(true);
        setSurfaceTextureListener(this);
    }
   
    @Override
    public void onSurfaceTextureAvailable(@NonNull SurfaceTexture surface, int width, int height) {
        try {
            if (AppConfig.app_Operation_mode_root) {
                ImGuiAidlService.GetIPC().start(width, height);
                ImGuiAidlService.GetIPC().setSurface(new Surface(surface));
            } else {
                ImGuiJNI.start(width, height);
                ImGuiJNI.setSurface(new Surface(surface));            
            }
       } catch (RemoteException e) {
            e.printStackTrace();
        }
        Log.d("ImGuiTextureView", "surfaceCreated_");
    }

    @Override
    public void onSurfaceTextureSizeChanged(@NonNull SurfaceTexture surface, int width, int height) {
    }

    @Override
    public boolean onSurfaceTextureDestroyed(@NonNull SurfaceTexture surface) {
        Log.e("NDK-java", "onSurfaceTextureDestroyed"); //结束
        return false;
    }

    @Override
    public void onSurfaceTextureUpdated(@NonNull SurfaceTexture surface) {
        //Log.e("NDK-java", "onSurfaceTextureUpdated");
    }
    
    
}



