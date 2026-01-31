package com.NeKo.WuLi.ImGui.Root;

import androidx.annotation.NonNull;
import android.content.Intent;
import android.os.IBinder;
import android.os.Process;
import android.os.RemoteException;
import android.view.MotionEvent;
import android.view.Surface;
import com.NeKo.WuLi.AppConfig;
import com.NeKo.WuLi.ImGui.ImGuiJNI;
import com.NeKo.WuLi.ImGuiRoot;
import com.topjohnwu.superuser.ipc.RootService;

import android.content.Context;
import java.io.File;
import android.widget.Toast;

public class ImGuiJNIRoot extends RootService{
    static {
            if (Process.myUid() == 0) { // 检查是否以 root 权限运行
                
                if (AppConfig.is测试) {
                    System.loadLibrary("Sky");
                } 
                if (!AppConfig.is测试) {
                    System.load(AppConfig.CxxPath);
                }
            }
      }

	
	@Override
	public IBinder onBind(@NonNull Intent intent) {
		return new ImGuiRoot.Stub() {
			
            public void setSurface(Surface surface) throws RemoteException {
                ImGuiJNI.setSurface(surface);
            }
                        
            public String start(int ScreenX, int ScreenY) throws RemoteException {
                return ImGuiJNI.start(ScreenX, ScreenY);
            }

            public void MotionEventClick(boolean down, float PosX, float PosY) throws RemoteException {
                ImGuiJNI.MotionEventClick(down, PosX, PosY);
            }
            
            public void setPid(int pid) throws RemoteException {
                ImGuiJNI.setPid(pid);
            }
		    
			public void setKey(String key) {
				ImGuiJNI.setKey(key);
			}
		
			public void setUUid(String UUID) {
				ImGuiJNI.setUUid(UUID);
			}
            
            public void setImGuiType(boolean type, int fps) {
                ImGuiJNI.setImGuiType(type, fps);
            }
            
            public int[] getWindowsRect() {
                return ImGuiJNI.getWindowsRect();
            }
            
            public int[] getWindowsId() {
                return ImGuiJNI.getWindowsId();
            }
            
            public void UpdateInputText(String text) {
                ImGuiJNI.UpdateInputText(text);
            }
            
            public void DeleteInputText() {
                ImGuiJNI.DeleteInputText();
            }
            
            public void showKeyboard(boolean show) {
                ImGuiJNI.showKeyboard(show);
            }
		};
	}
}
