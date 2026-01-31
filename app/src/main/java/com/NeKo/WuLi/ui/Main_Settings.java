package com.NeKo.WuLi.ui;

import android.content.Context;
import android.os.Bundle;
import android.view.Gravity;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.PopupMenu;
import android.widget.Toast;
import androidx.fragment.app.Fragment;

import com.NeKo.WuLi.AppConfig;
import com.NeKo.WuLi.R;
import com.NeKo.WuLi.Tools;
import com.google.android.material.button.MaterialButton;
import com.google.android.material.materialswitch.MaterialSwitch;
import java.math.BigInteger;

public class Main_Settings extends Fragment {
    
    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
        Context context = requireContext();
        View view = inflater.inflate(R.layout.u_setings, container, false);
        
        MaterialSwitch 防录屏 = view.findViewById(R.id.fanglu);
        MaterialSwitch 兼容模式 = view.findViewById(R.id.jianron);
        MaterialButton 选择帧率 = view.findViewById(R.id.fpsxuan);
        MaterialButton 触摸更新频率 = view.findViewById(R.id.tgxxz);
        
        防录屏.setChecked((boolean)Tools.读取(context, "防录屏", false));
        兼容模式.setChecked((boolean)Tools.读取(context, "兼容模式", false));
        选择帧率.setText(String.valueOf(Tools.读取(context, "选择帧率", 60))+"FPS");
        触摸更新频率.setText(String.valueOf(Tools.读取(context, "触摸更新频率", 24L)));
        
        防录屏.setOnCheckedChangeListener((v,c) -> {
            AppConfig.防录屏 = c;
            Tools.存储(context, "防录屏", c);
        });
        
        兼容模式.setOnCheckedChangeListener((v,c) -> {
            AppConfig.兼容模式 = c;
            Tools.存储(context, "兼容模式", c);
        });
        
        选择帧率.setOnClickListener(v -> {
            PopupMenu pop = new PopupMenu(context, v, Gravity.TOP);
            pop.getMenu().add("60FPS");
            pop.getMenu().add("90FPS");
            pop.getMenu().add("120FPS");
            
            pop.setOnMenuItemClickListener(item -> {
                
                
                if (item.getTitle().equals("60FPS")) {
                    Tools.存储(context, "选择帧率", 60);
                } else if (item.getTitle().equals("90FPS")) {
                    Tools.存储(context, "选择帧率", 90);
                } else {
                    Tools.存储(context, "选择帧率", 120);
                } 
                选择帧率.setText(item.getTitle())   ;
                return true;
            });
            
            pop.show();
        });
        
        触摸更新频率.setOnClickListener(v -> {
            PopupMenu pop = new PopupMenu(context, v, Gravity.TOP);
            
            pop.getMenu().add("10");
            pop.getMenu().add("24");
            pop.getMenu().add("45");
            pop.getMenu().add("60");
            
            pop.setOnMenuItemClickListener(item -> {
                Tools.存储(context, "触摸更新频率", Long.parseLong(item.getTitle().toString()));
                触摸更新频率.setText(String.valueOf(item.getTitle()));
                return true;
            });
            pop.show();
        });
        
        return view;
    }
}
