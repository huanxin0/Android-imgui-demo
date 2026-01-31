package com.NeKo.WuLi.ImGui;

import android.content.Context;
import android.view.SurfaceHolder;
import android.graphics.PixelFormat;
import android.util.Log;
import android.os.RemoteException;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import com.NeKo.WuLi.AppConfig;
import com.NeKo.WuLi.ImGui.ImGuiJNI;
import com.NeKo.WuLi.ImGui.Root.ImGuiAidlService;

public class ImGuiSurfaceView extends SurfaceView implements SurfaceHolder.Callback {
	
    public ImGuiSurfaceView(Context context) {
        super(context);
        setZOrderOnTop(true);
        getHolder().setFormat(PixelFormat.TRANSPARENT);
        getHolder().addCallback(this);
    }

    @Override
    public void surfaceCreated(final SurfaceHolder holder) {
        Log.d("ImGuiSurfaceView","surfaceCreated");
		holder.setType(SurfaceHolder.SURFACE_TYPE_GPU);
        try {
            if (AppConfig.app_Operation_mode_root) {
                ImGuiAidlService.GetIPC().setSurface(holder.getSurface());
            } else {
                ImGuiJNI.setSurface(holder.getSurface());
            }
        } catch (RemoteException e) {
            e.printStackTrace();
        }
    }

    @Override
    public void surfaceChanged(SurfaceHolder holder, int format, int width, int height) {
		try {
            if (AppConfig.app_Operation_mode_root) {
			    ImGuiAidlService.GetIPC().start(width,height);
            } else {
                ImGuiJNI.start(width,height);
            }
		} catch (RemoteException e) {
			e.printStackTrace();
		}
    }

    @Override
    public void surfaceDestroyed(SurfaceHolder holder) {

    }
}