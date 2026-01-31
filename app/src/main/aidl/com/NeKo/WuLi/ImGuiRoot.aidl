package com.NeKo.WuLi;

interface ImGuiRoot {
    void setSurface(in Surface surface);
    String start(int ScreenX, int ScreenY);
    void MotionEventClick(boolean down, float PosX, float PosY);
    void setPid(int pid);
    void setKey(String key);
	void setUUid(String UUID);
    void setImGuiType(boolean type, int fps);
    
    int[] getWindowsRect();
    int[] getWindowsId();
    
    void UpdateInputText(String text);
    void DeleteInputText();
    void showKeyboard(boolean show);
}