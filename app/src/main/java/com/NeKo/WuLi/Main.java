package com.NeKo.WuLi;

import android.Manifest;
import android.content.pm.PackageManager;
import android.os.Bundle;
import android.widget.FrameLayout;
import androidx.appcompat.app.AppCompatActivity;

import androidx.fragment.app.Fragment;
import androidx.fragment.app.FragmentManager;
import androidx.fragment.app.FragmentTransaction;
import android.widget.Toast;
import android.view.WindowManager;

import com.NeKo.WuLi.AppConfig;
import com.NeKo.WuLi.Tools;
import com.NeKo.WuLi.ui.Main_Home;
import com.NeKo.WuLi.ui.Main_Info;
import com.NeKo.WuLi.ui.Main_Settings;
import com.google.android.material.navigationrail.NavigationRailView;

public class Main extends AppCompatActivity {
    
    private NavigationRailView navigationRail;
    private FrameLayout fragmentContainer;
    
    private Main_Home mhome;
    private Main_Settings msettings;
    private Main_Info minfo;
    private Fragment currentFragment;
    
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        
        getWindow().setFlags(
            WindowManager.LayoutParams.FLAG_FULLSCREEN,
            WindowManager.LayoutParams.FLAG_FULLSCREEN
        );
        
        if (getSupportActionBar() != null) {
            getSupportActionBar().hide();
        }
        
        setContentView(R.layout.activity_main);
        this.存储权限();
        Tools.申请悬浮窗权限(this);
        
        AppConfig.context = this;
        AppConfig.防录屏 = (boolean)Tools.读取(this, "防录屏", false);
        AppConfig.兼容模式 = (boolean)Tools.读取(this, "兼容模式", false);
        AppConfig.设置帧率 = (int)Tools.读取(this, "选择帧率", 60);
        AppConfig.触摸更新频率 = (long)Tools.读取(this, "触摸更新频率", 24L);
        
        navigationRail = findViewById(R.id.navigation_rail);
        fragmentContainer = findViewById(R.id.fragment_container);
        
        // 初始化 Fragment
        mhome = new Main_Home();
        msettings = new Main_Settings();
        minfo = new Main_Info();
        
        // 默认显示主页
        if (savedInstanceState == null) {
            switchFragment(mhome, "home");
            navigationRail.setSelectedItemId(R.id.navigation_home);
        }

        // 设置导航监听器
        navigationRail.setOnItemSelectedListener(item -> {
            int itemId = item.getItemId();
            
            if (itemId == R.id.navigation_home) {
                switchFragment(mhome, "home");
                return true;
            } else if (itemId == R.id.navigation_settings) {
                switchFragment(msettings, "settings");
                return true;
            } else if (itemId == R.id.navigation_about) {
                switchFragment(minfo, "about");
                return true;
            }
            return false;
        });
        
        // 可选：设置项目重新选择的监听（点击已选中的项目时）
        navigationRail.setOnItemReselectedListener(item -> {
            // 可以在这里处理重新点击已选中项目的逻辑
            // 例如：滚动到顶部、刷新数据等
        });
        
        Tools.申请ROOT();
        if (Tools.检测ROOT()) {
            AppConfig.app_Operation_mode_root = true;
        }
        
        Renewal.检查应用更新(this, Main.this);
        
    }
    
    public void 存储权限(){
        boolean isGranted = true;
        if (android.os.Build.VERSION.SDK_INT >= 23) {
            if (this.checkSelfPermission(Manifest.permission.WRITE_EXTERNAL_STORAGE) != PackageManager.PERMISSION_GRANTED) {
                isGranted = false;
            }
            if (this.checkSelfPermission(Manifest.permission.READ_EXTERNAL_STORAGE) != PackageManager.PERMISSION_GRANTED) {
                isGranted = false;
            }
            if (!isGranted) {
                this.requestPermissions(
                    new String[]{Manifest.permission.ACCESS_COARSE_LOCATION, Manifest.permission
                        .ACCESS_FINE_LOCATION,
                        Manifest.permission.READ_EXTERNAL_STORAGE,
                        Manifest.permission.WRITE_EXTERNAL_STORAGE},
                    102);
            }
		}
	}

    private void switchFragment(Fragment fragment, String tag) {
        // 如果当前已经是这个 Fragment，则不切换
        if (currentFragment != null && currentFragment.getClass().equals(fragment.getClass())) {
            return;
        }

        FragmentManager fragmentManager = getSupportFragmentManager();
        FragmentTransaction transaction = fragmentManager.beginTransaction();
        
        // 隐藏当前 Fragment
        if (currentFragment != null) {
            transaction.hide(currentFragment);
        }
        
        // 如果 Fragment 已经添加过，则显示；否则添加
        Fragment targetFragment = fragmentManager.findFragmentByTag(tag);
        if (targetFragment != null) {
            transaction.show(targetFragment);
        } else {
            transaction.add(R.id.fragment_container, fragment, tag);
        }
        
        transaction.commit();
        currentFragment = fragment;
    }
}