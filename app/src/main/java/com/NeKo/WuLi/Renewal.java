package com.NeKo.WuLi;

import android.content.Intent;
import android.net.Uri;
import android.util.Log;
import androidx.appcompat.app.AlertDialog;
import static cn.hutool.core.util.ClassLoaderUtil.getClassLoader;

import android.app.Activity;
import android.content.Context;
import android.widget.Toast;

import com.google.android.material.dialog.MaterialAlertDialogBuilder;
import com.NeKo.WuLi.HotUpdate.ClassLoaderHookHelper;
import com.NeKo.WuLi.HotUpdate.NuomHttpUtil;

import java.io.File;
import java.lang.reflect.InvocationTargetException;

import cn.hutool.core.util.ZipUtil;
import cn.hutool.http.HttpUtil;
import cn.hutool.json.JSONObject;
import cn.hutool.json.JSONUtil;

public class Renewal {
    private static boolean Server_updeta = false;
    private static String UpdetePath = "";
    
    public static void 检查应用更新(Context context, Activity activity) {
        UpdetePath = context.getFilesDir() + AppConfig.AppPath;
        AppConfig.CxxPath = UpdetePath + AppConfig.AppCxxName;
        
        new Thread(() -> {
            NuomHttpUtil.httpResult result = NuomHttpUtil.getRequest(AppConfig.AppServerApi + "/api_v1.php");
            if(result.code != 200) {
                activity.runOnUiThread(() -> Toast.makeText(context, "获取应用信息失败", Toast.LENGTH_SHORT).show());
            } else {
                JSONObject json;
                try {
                	json = JSONUtil.parseObj(result.body);
                    String 内容 = json.getStr("公告");
                    String 版本 = json.getStr("版本");
                    
                    String 更新公告 = json.getStr("更新公告");
                    String 更新链接 = json.getStr("更新链接");
                    
                    if(Tools.取自身版本号(context).equals(版本)) {
                    	activity.runOnUiThread(() -> new MaterialAlertDialogBuilder(context)
                            .setTitle("公告")
                            .setMessage(内容)
                            .setNegativeButton("确定", (dialog, which) -> {
                        
                            })
                            .show());
                            
                        result = NuomHttpUtil.getRequest(AppConfig.AppServerApi+"/api_v2.php");
                        if (result.code == 200) { //不等于200说明连接失败了
                            
                            JSONObject jsonObject;
                            try {
                                jsonObject = JSONUtil.parseObj(result.body); //解析json
                            } catch (Exception e) {
                                activity.runOnUiThread(() -> Toast.makeText(context, "配置解析失败", Toast.LENGTH_SHORT).show());
                                return;
                            }

                            String 是否更新 = jsonObject.getStr("补丁开关");
                            String du_md5 = jsonObject.getStr("补丁MD5"); //最新版的MD5
                            String di_md5 = Tools.取Md5(UpdetePath + "补丁.zip"); // 本地文件的MD5

                            Log.e("最新补丁", du_md5);
                            Log.e("本地补丁", di_md5);
            
                            if (是否更新.equals("更新")) {
                                Server_updeta = true;
                            }
            
                            if (!du_md5.equals(di_md5)) {
                                
                                Tools.归递删除(new File(context.getFilesDir() + AppConfig.AppPath));
                                activity.runOnUiThread(() -> Toast.makeText(context, "更新补丁", Toast.LENGTH_SHORT).show());
                                //下载补丁，20s超时
                                try {
                                    long fileSize = HttpUtil.downloadFile(jsonObject.getStr("补丁地址"), new File(UpdetePath + "补丁.zip"), 20000);
                                    // 下载成功后解压
                                    ZipUtil.unzip(UpdetePath + "补丁.zip");
                                } catch (Exception e) {
                                    System.out.println("下载失败，原因" + e.toString());
                                }
                                
                                Tools.归递只读(new File(UpdetePath));
                
                                activity.runOnUiThread(() -> new MaterialAlertDialogBuilder(context).setTitle("补丁更新公告")
                                .setMessage(jsonObject.getStr("补丁公告"))
                                .setNegativeButton("确定", (dialog, which) -> {
                        
                                })
                                .show());
                            }

                            // 注释掉这行代表关闭dex热更新
                            if (Server_updeta) {
                                try {
                                    //更新dex
                                    ClassLoaderHookHelper.hookV23(getClassLoader(), new File(UpdetePath + AppConfig.AppDexName), context.getCacheDir());
                                } catch (IllegalAccessException e) {
                                    throw new RuntimeException(e);
                                } catch (InvocationTargetException e) {
                                    throw new RuntimeException(e);
                                }
                            }
                                
                        }
                        
                        return;
                    } else {
                        activity.runOnUiThread(() ->{
                            MaterialAlertDialogBuilder 更新 = new MaterialAlertDialogBuilder(context)
                                .setTitle("发现新版本")
                                .setMessage(更新公告)
                                .setNegativeButton("更新", (dialog, which) -> {
                                    Intent gx = new Intent(Intent.ACTION_VIEW, Uri.parse(更新链接));
                                    context.startActivity(gx);
                                });
                            
                            AlertDialog 更新弹窗 = 更新.create();
                            更新弹窗.setCanceledOnTouchOutside(false);
                            更新弹窗.setCancelable(false);
                            更新弹窗.show();
                        });
                    }
                } catch(Exception err) {
                	err.printStackTrace();
                }
            }
        }).start();
    }
}
