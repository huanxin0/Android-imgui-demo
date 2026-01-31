package com.NeKo.WuLi.ImGui;
import android.view.Surface;

public class ImGuiJNI {
    public static native void setSurface(Surface surface);
    public static native String start(int ScreenX, int ScreenY);
    public static native void MotionEventClick(boolean down, float PosX, float PosY);
    
    public static native void setPid(int pid);
	public static native void setKey(String key);
	public static native void setUUid(String UUID);
    public static native void setImGuiType(boolean type, int fps);

    public static native int[] getWindowsRect();
    public static native int[] getWindowsId();

    public static native void UpdateInputText(String text);
    public static native void DeleteInputText();
    public static native void showKeyboard(boolean show);
}
