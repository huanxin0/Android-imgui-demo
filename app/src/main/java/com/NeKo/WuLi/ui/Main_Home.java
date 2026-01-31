package com.NeKo.WuLi.ui;

import android.content.ServiceConnection;
import android.os.RemoteException;
import com.NeKo.WuLi.ImGui.ImGuiJNI;
import com.NeKo.WuLi.ImGui.KeyboardViewService;
import com.NeKo.WuLi.ImGuiRoot;
import android.content.ComponentName;
import android.os.IBinder;
import com.NeKo.WuLi.AppConfig;
import android.content.Intent;
import com.NeKo.WuLi.ImGui.Root.ImGuiJNIRoot;
import com.topjohnwu.superuser.ipc.RootService;
import com.NeKo.WuLi.Main;
import com.NeKo.WuLi.ImGui.ImGuiViewService;
import com.NeKo.WuLi.ImGui.Root.ImGuiAidlService;
import android.util.Log;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import androidx.fragment.app.Fragment;

import com.NeKo.WuLi.R;
import com.google.android.material.button.MaterialButton;

public class Main_Home extends Fragment {
    
    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
        // 加载对应的布局文件
        View view = inflater.inflate(R.layout.u_home, container, false);
        
        MaterialButton login = view.findViewById(R.id.dl);
        
        login.setOnClickListener(v -> {
            KeyboardViewService.showFloat(AppConfig.context);
            if (AppConfig.app_Operation_mode_root) {
                RootService.bind(new Intent(requireContext(), ImGuiJNIRoot.class), new AIDLConnection());
                try {
                	ImGuiAidlService.GetIPC().setImGuiType(AppConfig.兼容模式, AppConfig.设置帧率);
                } catch(RemoteException err) {
                	err.printStackTrace();
                }
            } else {
                if (AppConfig.is测试) {
                    System.loadLibrary("Sky");
                } 
                if (!AppConfig.is测试) {
                    System.load(AppConfig.CxxPath);
                }
                ImGuiJNI.setImGuiType(AppConfig.兼容模式, AppConfig.设置帧率);
            }
            
           ImGuiViewService.showFloatWindow(requireActivity());
        });
        
        return view;
    }

    public static class AIDLConnection implements ServiceConnection {
        public static ImGuiRoot iTestService;
    
        @Override
        public void onServiceConnected(ComponentName name, IBinder service) {
            iTestService = ImGuiRoot.Stub.asInterface(service);
            ImGuiAidlService.InItIPC(iTestService);
            Log.d("Root", "链接成功");
        }

        @Override
        public void onServiceDisconnected(ComponentName componentName) {
            Log.d("Root", "链接失败");
        }  
	
    }
}