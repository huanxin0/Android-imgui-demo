package com.NeKo.WuLi.ImGui.Root;

import com.NeKo.WuLi.ImGui.Root.ImGuiAidlService;
import com.NeKo.WuLi.ImGuiRoot;

public class ImGuiAidlService {
    private static ImGuiRoot ipc;

    public static boolean isConnect() {
        return ipc != null;
    }

    public static void InItIPC(ImGuiRoot ipc) {
        ImGuiAidlService.ipc = ipc;
    }

    public static ImGuiRoot GetIPC() {
        return ipc;
    }
}
