1.  按键扫描改变，在 main 中扫描
2.  接收方式为 1 对 2，发送器区分左右站代码
3. 

2020-09-05

1. 修改 lcd12864
2. 修改 interact
3. 添加 hal_cpu_rtc
4. 添加 #include "hal_power.h"


2020-12-01
1. 添加电池检测功能   -> board.c (进行中)

2020-12-02
1. 修改 hal_InitKey()，board_init() 函数 （完成）
2. 修改 menu.c 文件 （进行中）

2020-12-06
1. 删除 device_base_config 函数
2. 修改 systemParamInit， systemParamSave 函数
3. 修改 wl_mode_init()， wl_set_param()； 添加全局变量 gwlCurrentParam；删除 getWLParam()，wlParamUpdataToMcu();
4. 修改 interface_display(), 添加 RF功率显示

2020-12-08
1. 计划：普通模式加入stop模式，RTC 1s 唤醒一次并显示时间

2021-2-5
修改 menu 文件 DeviceInfoCallBack 函数，DN显示为RF配置数据

2021-2-21
修改 lora 文件 wl_get_param() 为内部函数；
修改paramter 文件 systemParamInit() 函数 wlParam.speed = 0x2A;  // 串口波特率 ，空速 38400 2.4k

keys_option_handle()命令发送周期修改为 50ms
