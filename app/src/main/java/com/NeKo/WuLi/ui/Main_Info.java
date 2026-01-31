package com.NeKo.WuLi.ui;

import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import androidx.fragment.app.Fragment;

import com.NeKo.WuLi.R;

public class Main_Info extends Fragment {
    
    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {
        // 加载对应的布局文件
        return inflater.inflate(R.layout.u_info, container, false);
    }
}
