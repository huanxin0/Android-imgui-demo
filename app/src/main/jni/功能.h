

void 消磁() {
    clearResultList();//清空之前的搜索结果
    MemorySearch("1.0F", TYPE_FLOAT); 
    ImproveOffset("645759872.0F", TYPE_FLOAT, 0x74);  
    ImproveOffset("-303.0565490722656F", TYPE_FLOAT, 0x7c);  
    ImproveOffset("-1.0F", TYPE_FLOAT, -0x14);  
    ImproveOffset("-1.0F", TYPE_FLOAT, -0x18); 
    ImproveOffset("0.5F", TYPE_FLOAT, -0x60); 
    ImproveOffset("100F", TYPE_FLOAT, -0x8);
    printResultListToFile("/storage/emulated/0/测试.log");
    clearResultList();//清空之前的搜索结果
}