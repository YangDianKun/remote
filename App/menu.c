/*************************************** Copyright (c)******************************************************
** File name            :   menu.c
** Latest modified Date :   2019-12-01
** Latest Version       :   0.1
** Descriptions         :   主文件，包含应用代码
**
**--------------------------------------------------------------------------------------------------------
** Created by           :   YangDianKun
** Created date         :   
** Version              :   1.0
** Descriptions         :   The original version
**
**--------------------------------------------------------------------------------------------------------
** Copyright            :  
** Author Email         :   1163101403@qq.com
**********************************************************************************************************/
#include "menu.h"
#include "board.h"
#include "main.h"
#include "paramter.h"
#include <stdio.h>


/* Private typedef -----------------------------------------------------------*/
typedef void (* tMenuFunc)(void);
typedef struct sMenuItem * tMenuItem;
typedef struct sMenu * tMenu;

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
u8 MenuItemIndex = 0, nMenuLevel = 0;
u8 ItemNumb[MAX_MENU_LEVELS];
tMenu psPrevMenu[MAX_MENU_LEVELS];
tMenu tMenuCurrent;

/* // 需要保存的系统参数变量 */
static u8 RFPowerParam;
static u8 RFAddrParam;
static u8 OptModeParam;
static u8 LRConfigParam;


/* // 菜单子项 */
struct sMenuItem
{
	u8* pszTitle;
	tMenu psSubMenu;
	tMenuFunc psSubEnterFunc;
};


/* // 菜单数据结构 */
struct sMenu
{
  u8* pszTitle;
  tMenuItem psItems;
  u8 ItemsIndex;
  u8 nItems;
};


/* function ----------------------------------------------------------------*/
static void DisplayMenu (void);

static void LRConfigCallBack(void);
static void RFPowerCallBack (void);
static void RFAddrCallBack (void);
static void OptModeCallBack (void);
static void ResumeFactoryCallBack(void);
static void SaveParamCallBack(void);
static void DeviceInfoCallBack(void);


/*------------------------------ Menu level 3 -------------------------------*/

/*------------------------------ Menu level 2 -------------------------------*/
struct sMenuItem OptModeMeunItems[] = {{"  模式: [省电]", NULL, OptModeCallBack},
									   {"  模式: [普通]", NULL, OptModeCallBack},
                                       {"  模式: [增强]", NULL, OptModeCallBack}};
struct sMenu OptModeMeun = {"    操作模式    ", OptModeMeunItems, 0, countof(OptModeMeunItems)};

struct sMenuItem LRConfigMeunItems[] = {{"   站号:  [左]", NULL, LRConfigCallBack},
									    {"   站号:  [右]", NULL, LRConfigCallBack}};
struct sMenu LRConfigMeun = {"    站号配置    ", LRConfigMeunItems, 0, countof(LRConfigMeunItems)};


struct sMenuItem RFPowerMeunItems[] = {{"  功率:[10dBm]", NULL, RFPowerCallBack},
									   {"  功率:[14dBm]", NULL, RFPowerCallBack},
                                       {"  功率:[17dBm]", NULL, RFPowerCallBack},
                                       {"  功率:[20dBm]", NULL, RFPowerCallBack}};
struct sMenu RFPowerMeun = {"    发射功率    ", RFPowerMeunItems, 0, countof(RFPowerMeunItems)};

struct sMenuItem RFAddrMeunItems[] = {"  [0000]", NULL, RFAddrCallBack};
struct sMenu RFAddrMeun = {"    设备地址    ", RFAddrMeunItems, 0, countof(RFAddrMeunItems)};

//struct sMenuItem ParamSaveMeunItems[] = {"......", NULL, NULL};
//struct sMenu ParamSaveMeun = {"     保  存     ", ParamSaveMeunItems, 0, countof(ParamSaveMeunItems)};
                              

/*------------------------------ Menu level 1 -------------------------------*/
struct sMenuItem MainMenuItems[] = {
  {"  1.工作模式->", &OptModeMeun, NULL},
  {"  2.站号配置->", &LRConfigMeun, NULL},
  {"  3.发射功率->", &RFPowerMeun, NULL},
  {"  4.设备地址->", &RFAddrMeun, NULL},
  {"  5.恢复出厂->", NULL, ResumeFactoryCallBack},
  {"  6.设备信息->", NULL, DeviceInfoCallBack},
  {"  7.保    存->", NULL, SaveParamCallBack}};

struct sMenu MainMenu = {"     设  置     ", MainMenuItems, 0, countof(MainMenuItems)};


/*******************************************************************************
* Function Name  : menu_init
* Description    : initialization the menu.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/	
int8_t MenuInit (void)
{
	nMenuLevel = 0;
	MenuItemIndex = 0;
	tMenuCurrent = &MainMenu;
	psPrevMenu[nMenuLevel] = tMenuCurrent;
	ItemNumb[nMenuLevel] = MenuItemIndex;
	
	return 0;
}

static void SystemParamLoadToMenu(void)
{
	OptModeParam = USER_DATA->systemMode;
	RFPowerParam = gwlCurrentParam.option;
	LRConfigParam = USER_DATA->devID;
	
	if (USER_DATA->systemMode == SYS_LOW_POWER_MODE){
		OptModeMeun.ItemsIndex = 0;
	}else if (USER_DATA->systemMode == SYS_NORMAL_MODE){
		OptModeMeun.ItemsIndex = 1;
	}else {
		OptModeMeun.ItemsIndex = 2;
	}
	
	if (USER_DATA->devID == LEFT_DEV_ID){
		LRConfigMeun.ItemsIndex = 0;
	}else {
		LRConfigMeun.ItemsIndex = 1;
	}
	
	switch (gwlCurrentParam.option & 0x03)
	{
		case 0: 
			RFPowerMeun.ItemsIndex = 3; // 20dBm
			break; 
		case 1:
			RFPowerMeun.ItemsIndex = 2; // 17dBm			
			break; 
		case 2: 
			RFPowerMeun.ItemsIndex = 1; // 14dBm
			break; 
		case 3: 
			RFPowerMeun.ItemsIndex = 0; // 10dBm
			break; 
		default:break;
	}
}

void EnterMenu(void)
{
	u8 cnt = 0;
	
	lcd_str_5x8(4, 8, 0, 0, "Please wait");
	while (!IsKey3Down()) 
	{
		lcd_str_gb2312(6, (4 + cnt * 8), 0, 0, ".");
		delay_ms(200);
		if (++cnt >= 15)
			break;
	}
	
	if (cnt >= 15)
	{
		MenuInit();
		OptionMenu();
	}
}

void OptionMenu(void)
{
	SystemParamLoadToMenu();
	lcd_clear();
	DisplayMenu();
	
	while(1)
	{
		delay_ms(10);
		if (!IsKey6Down()) // 菜单下翻
		{
			while(!IsKey6Down());
			if (tMenuCurrent->ItemsIndex == tMenuCurrent->nItems - 1)
				tMenuCurrent->ItemsIndex = 0;
			else
				tMenuCurrent->ItemsIndex++;
			
			MenuItemIndex = tMenuCurrent->ItemsIndex;
			//lcd_str_gb2312_line(2, 0, 0, tMenuCurrent->psItems[MenuItemIndex].pszTitle);
			DisplayMenu();
		}
		else if (!IsKey5Down()) // 菜单上翻
		{
			while(!IsKey5Down());
			if (tMenuCurrent->ItemsIndex == 0)
				tMenuCurrent->ItemsIndex = tMenuCurrent->nItems - 1;
			else
				tMenuCurrent->ItemsIndex--;
			
			MenuItemIndex = tMenuCurrent->ItemsIndex;
			//lcd_str_gb2312_line(2, 0, 0, tMenuCurrent->psItems[MenuItemIndex].pszTitle);
			DisplayMenu();
		}
		else if (!IsKey2Down()) // 进入子菜单
		{
			while(!IsKey2Down());
			/* // 执行当前菜单选项回调函数 */
			if (tMenuCurrent->psItems[MenuItemIndex].psSubEnterFunc != NULL)  
			{
				tMenuCurrent->psItems[MenuItemIndex].psSubEnterFunc();
			}
			/* // 如果有下级菜单，则切换到下级菜单 */
			if (tMenuCurrent->psItems[MenuItemIndex].psSubMenu != NULL)  
			{
				//ItemNumb[nMenuLevel] = MenuItemIndex;  // 记录当前菜单选项索引
				psPrevMenu[nMenuLevel] = tMenuCurrent; // 记录当前菜单指针
				nMenuLevel++;  // 菜单级数加 1
				
				tMenuCurrent = tMenuCurrent->psItems[MenuItemIndex].psSubMenu; // 当前菜单指针指向下级菜单指针
				MenuItemIndex = tMenuCurrent->ItemsIndex; 
				
				//lcd_clear();  // 清屏
				DisplayMenu();

			}

		}
		else if (!IsKey4Down()) // 退出子菜单
		{
			while(!IsKey4Down());
			/* // 如果菜单级数不为零，则回退到上级菜单 */
			if (nMenuLevel != 0)
			{
				nMenuLevel--; // 菜单级数减 1
				tMenuCurrent = psPrevMenu[nMenuLevel]; // 当前菜单指针指向上级菜单
				//MenuItemIndex = ItemNumb[nMenuLevel];  // 记录当前菜单选项索引
                MenuItemIndex = tMenuCurrent->ItemsIndex;				
				
				//lcd_clear();  // 清屏
				DisplayMenu();
				
			}
			else
			{
				lcd_clear();
				break; // 退出菜单设置
			}
		}
//		else if (!IsKey3Down())
//		{
//			while(!IsKey3Down());
//			lcd_clear();
//			break; // 退出菜单设置
//		}
	
	}
}

//
static void DisplayMenu (void)
{
	lcd_str_gb2312_line(0, 0, LCD_DOWN_FRAM, tMenuCurrent->pszTitle);
	//lcd_str_gb2312_line(2, 0, 0, tMenuCurrent->psItems[MenuItemIndex].pszTitle);
	lcd_str_gb2312(3, 0, 0, 0, tMenuCurrent->psItems[MenuItemIndex].pszTitle);
	lcd_str_gb2312(6,0,1,0,"确认");
	lcd_str_gb2312(6,96,1,0,"返回");
}


//LRConfigMeunItems
static void LRConfigCallBack(void)
{

	switch(MenuItemIndex)
	{
		case 0: LRConfigParam = LEFT_DEV_ID; break;
		case 1: LRConfigParam = RIGHT_DEV_ID; break;
		default: break;
	}
	
	lcd_clear();
	lcd_str_gb2312(3, 0, 0, 0, "  参数已确认");
	delay_ms(1000);
	DisplayMenu();
}

// RFPowerMeunItems
static void RFPowerCallBack (void)
{
	switch(MenuItemIndex)
	{
		case 0: RFPowerParam = 0x7F; break;
		case 1: RFPowerParam = 0x7E; break;
		case 2: RFPowerParam = 0x7D; break;
		case 3: RFPowerParam = 0x7C; break;
		default: break;
	}
	lcd_clear();
	lcd_str_gb2312(3, 0, 0, 0, "  参数已确认");
	delay_ms(1000);
	DisplayMenu();
}

// RFAddrCallBack
static void RFAddrCallBack (void)
{
	RFAddrParam = 0;
	//lcd_clear();
}

// OptModeMeunItems
static void OptModeCallBack (void)
{
	switch(MenuItemIndex)
	{
		case 0: OptModeParam = SYS_LOW_POWER_MODE; break;
		case 1: OptModeParam = SYS_NORMAL_MODE; break;
		case 2: OptModeParam = SYS_HIG_POWER_MODE; break;
		default: break;
	}
	lcd_clear();
	lcd_str_gb2312(3, 0, 0, 0, "  参数已确认");
	delay_ms(1000);
	DisplayMenu();
}

// Resume factory setting
static void ResumeFactoryCallBack(void)
{}

// save parameter
static void SaveParamCallBack(void)
{
	UserData_type userData;
	int8_t resultA = 0;
	int8_t resultB = 0;
	
	// 判断保存在单片机 flash 内参数是否修改；是保存，否无动作
	if (USER_DATA->devID != LRConfigParam || USER_DATA->systemMode != OptModeParam)
	{
		userData.devID = LRConfigParam;
		userData.systemMode = OptModeParam;
		if (systemParamSave(&userData) < 0)
			resultA -= 1; // 失败
		else
			resultA += 1;
	}
	
	// 判断 RF 参数是否修改；是保存，否无动作
	if (gwlCurrentParam.option != RFPowerParam)
	{
		gwlCurrentParam.option = RFPowerParam;
		if (wl_set_param(&gwlCurrentParam) < 0)
			resultB -= 1; // 失败
		else
			resultB += 1;
	}
	
	// 判断保存结果
	if (resultA == 0 && resultB == 0) 
	{
		// 没有保存动作
	}
	else if (resultA < 0 || resultB < 0) // 保存失败
	{
		lcd_clear();
		lcd_str_gb2312(3, 0, 0, 0, "  保存失败");
		delay_ms(1000);
		DisplayMenu();
	}
	else // 保存成功
	{
		lcd_clear();
		lcd_str_gb2312(3, 0, 0, 0, "  保存成功");
		delay_ms(1000);
		board_soft_reset();    // 单片机软复位
	}
	
}
	
// DeviceInfoCallBack
static void DeviceInfoCallBack(void)
{
	u8 rf_str[13];
	u8 *pwl;
	u8 i;
	
	lcd_clear();
	lcd_str_5x8(0, 0, 0, 0, (uint8_t *)DEVICE_MODLE);
	lcd_str_5x8(2, 0, 0, 0, (uint8_t *)FIRMWARE_VERSION);
	lcd_str_5x8(4, 0, 0, 0, (uint8_t *)DEVICE_NUMBER);
	
	pwl = &gwlCurrentParam.addr_h;
	for(i = 0; i < sizeof(gwlCurrentParam) - 1; i++)
	{
		sprintf((char *)(&rf_str[i * 2]), "%X", *pwl++);
	}
	lcd_str_5x8(6, 6, 0, 0, "RF :");
	lcd_str_5x8(6, 34, 0, 0, rf_str);
	
	while(1)
	{
		delay_ms(20);
		if (!IsKey4Down())
		{
			while(!IsKey4Down());
			break;
		}
	}
	lcd_clear();
	DisplayMenu();
}

