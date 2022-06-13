/*
 *  TOPPERS/JSP Rose-RT Link Programs
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Just Standard Profile Kernel
 * 
 *  Copyright (C) 2000-2004 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 *  Copyright (C) 2003-2004 by Platform Development Center
 *                              RIOCH COMPANY,LTD. JAPAN
 *  Copyright (C) 2004-2005 by Industrial Technology Institute,
 *                              Miyagi Prefectural Government, JAPAN
 * 
 *  上記著作権者は，Free Software Foundation によって公表されている 
 *  GNU General Public License の Version 2 に記述されている条件か，以
 *  下の(1)〜(4)の条件を満たす場合に限り，本ソフトウェア（本ソフトウェ
 *  アを改変したものを含む．以下同じ）を使用・複製・改変・再配布（以下，
 *  利用と呼ぶ）することを無償で許諾する．
 *  (1) 本ソフトウェアをソースコードの形で利用する場合には，上記の著作
 *      権表示，この利用条件および下記の無保証規定が，そのままの形でソー
 *      スコード中に含まれていること．
 *  (2) 本ソフトウェアを再利用可能なバイナリコード（リロケータブルオブ
 *      ジェクトファイルやライブラリなど）の形で利用する場合には，利用
 *      に伴うドキュメント（利用者マニュアルなど）に，上記の著作権表示，
 *      この利用条件および下記の無保証規定を掲載すること．
 *  (3) 本ソフトウェアを再利用不可能なバイナリコードの形または機器に組
 *      み込んだ形で利用する場合には，次のいずれかの条件を満たすこと．
 *    (a) 利用に伴うドキュメント（利用者マニュアルなど）に，上記の著作
 *        権表示，この利用条件および下記の無保証規定を掲載すること．
 *    (b) 利用の形態を，別に定める方法によって，上記著作権者に報告する
 *        こと．
 *  (4) 本ソフトウェアの利用により直接的または間接的に生じるいかなる損
 *      害からも，上記著作権者を免責すること．
 * 
 *  本ソフトウェアは，無保証で提供されているものである．上記著作権者は，
 *  本ソフトウェアに関して，その適用可能性も含めて，いかなる保証も行わ
 *  ない．また，本ソフトウェアの利用により直接的または間接的に生じたい
 *  かなる損害に関しても，その責任を負わない．
 * 
 *  @(#) $Id: timer2lcd.c,v 1.0 2003/11/08 08:12:24 roi Exp $
 *  "lcd" added by Yokoyama
 */

/* 
 *  TOPPERS初級教育用のサンプルプログラム
 *  　タイマ機能はまだ実装されていない。
 *  　timer1とまったく同じ内容である。
 *  　編集後にtimer1と比較をする。
 *  
 *  entry_taskではタイマ起動SWのオン、オフにあわせてタイマLEDを点灯、消灯．
 *  timer_taskは電源LEDを１秒間隔に点滅する．
 */

/* 
 *  液晶（LCD）に時間を表示する機能を追加
 * "akih8lcd_device.c", "akih8pot_device.c", "akih8pot_device.h"を参照
 *  by Yokoyama, 2007/10/18
 */

#include <t_services.h>
#include "kernel_id.h"
#include "device.h"
#include "fishing.h"


// LCD表示用
static char const raddec[] = "0123456789"; /* 数字を表す文字コード*/
static char lcdbuf[32] = ""; /* 1行は16文字表示可能、2行分のバッファ */

volatile int screen = START;
volatile int need_land = 0;
volatile Fish fish1_data = {32, -1, 1};
volatile Fish *fish1 = &fish1_data;
volatile Fish fish2_data = {14, 1, 2};
volatile Fish *fish2 = &fish2_data;
volatile Fish **act_fish = &fish1;
volatile UB up_sw_past, up_sw_current;
volatile UB land_sw_past, land_sw_current;


/* ----------------------------
	タスク
 ---------------------------- */
void init_task() {
	initial_key();		/* キーの初期化 */
	initial_led();		/* LEDの初期化 */
	initial_lcd();		/* LCDの初期化 */

	clear_display();

	up_sw_past = get_key(UP_SW);
	land_sw_past = get_key(LAND_SW);
}

void fish_task() {
	int need_eat = move_fish(*act_fish);
	if(need_eat) {
		eat();
		init_fish(*act_fish);
	}
	draw_fish(*act_fish);
	set_flg(COMPLETE_FLG, 0x01);
	ext_tsk();
}

void angler_task() {
	draw_rod();
	set_led(TIMER_LED, OFF);
	if(need_land) {
		land();
		init_fish(*act_fish);
		need_land = 0;
	}
	set_flg(COMPLETE_FLG, 0x02);
	ext_tsk();
}

void display_task() {
	int tmp;
	wai_flg(COMPLETE_FLG, 0x03, TWF_ANDW, &tmp);
	display_lcd(2, lcdbuf);
	clear_display();
}



/* ----------------------------
	ハンドラ
 ---------------------------- */
void screen_handler() {
	iact_tsk(DISPLAY_TASK);
	if(screen == PLAY) {
		iact_tsk(FISH_TASK);
		iact_tsk(ANGLER_TASK);
	}
}

void switch_handler() {
	// スイッチ1: 画面制御
	if(get_key(START_SW) == ON) {
		screen = PLAY;
	} else {
		screen = MENU;
	}

  // スイッチ2: 釣り上げる
	up_sw_current = get_key(UP_SW);
	if(screen == PLAY && up_sw_current != up_sw_past && up_sw_current == ON) {
		need_land = 1;
		iact_tsk(ANGLER_TASK);
	}
	up_sw_past = up_sw_current;

	// land_sw_current = get_key(LAND_SW);
	// if(land_sw_current != land_sw_past && land_sw_current == ON) {
	// 	iact_tsk(ANGLER_TASK);
	// }
	// land_sw_past = land_sw_current;

}



/* ----------------------------
	サブルーチン
 ---------------------------- */
void init_fish(Fish *fish) {
	fish->x = 14;
	fish->direction = 1;
	fish = &fish2;
}

int move_fish(Fish *fish) {
	int isEat = 0;
	if(fish->x == BAIT_X+16) isEat = 1;
	else fish->x += fish->direction; // TODO 足すかどうかはランダム
	set_led(POW_LED, OFF);
	return isEat;
}

void eat() {
	set_led(POW_LED, ON);
	VP_INT *tmp;
	ER result = trcv_dtq(LAND_DTQ, tmp, 500);
	if(result == E_TMOUT) {
		draw_msg("Miss");
	}
	set_led(POW_LED, OFF);
}

void land() {
	ER result = tsnd_dtq(LAND_DTQ, (VP_INT)1, 0);
	if(result == E_OK) {
		draw_msg("Catch!");
		set_led(TIMER_LED, ON);
	} else if(result == E_TMOUT) {
		draw_msg("Too early");
	}
}

void draw_fish(Fish *fish) {
	if(fish->direction == -1) {
		// <゜))<
		if(fish->x >= 16) lcdbuf[fish->x] = 0x3c;
		if(fish->x >= 15) lcdbuf[fish->x+1] = 0xdf;
		if(fish->x >= 14) lcdbuf[fish->x+2] = 0x29;
		if(fish->x >= 13) lcdbuf[fish->x+3] = 0x29;
		if(fish->x >= 12) lcdbuf[fish->x+4] = 0x3c;
	} else if(fish->direction == 1) {
		// >((゜>
		if(fish->x >= 20) lcdbuf[fish->x-4] = 0x3e; 
		if(fish->x >= 19) lcdbuf[fish->x-3] = 0x28;
		if(fish->x >= 18) lcdbuf[fish->x-2] = 0x28;
		if(fish->x >= 17) lcdbuf[fish->x-1] = 0xdf;
		if(fish->x >= 16) lcdbuf[fish->x] = 0x3e;
	}
}

void draw_rod() {
	lcdbuf[BAIT_X-4] = 0x5f;
	lcdbuf[BAIT_X-3] = 0x5f;
	lcdbuf[BAIT_X-2] = 0x5f;
	lcdbuf[BAIT_X-1] = 0x2f;
	lcdbuf[BAIT_X] = 'J'; // 餌
}

void draw_msg(char *str) {
	int n = 0;
	while(str[n] != '\0') n++;
	int i = 15;
	while(n > 0) {
		lcdbuf[i] = str[n-1];
		n--;
		i--;
	}
}

void clear_display() {
	int i;
	for(i=0; i<32; i++) {
		lcdbuf[i] = ' ';
	}
}



/* メモ

// TODO
[] 画面遷移
[] お金
[] $0でゲームオーバー
[] 餌レベル
[] 魚挙動 TODO 足すかどうかはランダム

- スイッチ
	- 1 画面操作
		- ホーム→プレイ
		- その後 メニュー/プレイ
	- 2 釣り竿トグル 通常/高級
	- 3 
	- 4 釣り上げる

- タスク
	- 画面管理
		- イベントフラグで対応する画面表示
		- 画面
			- 釣る
			- 魚GET・逃げる
			- 設定画面
				-	餌の種類変える
				- お金見れる
	- 魚
		- 名前・座標・レベル
		- 動く
		- 食べる
		- 逃げる
	- 釣り人(餌)
		- 餌
			- 安い/高い (j/J)
			- 変更
			- 
			- 釣り上げる
		- お金
			- 餌代払う
			- 0になったらgame over


周期ハンドラを作成してタスクを呼び出す
- 魚ハンドラ → 魚タスク → 食べる → 逃げる/釣られる
- スイッチハンドラ → 釣り人タスク → 釣る

- 餌を上げた時
	- 上げた瞬間データを送信(タイムアウト 0s)
		- データキュー領域のサイズは0で同期させる
		- 送信できた→釣れる
		- できなかった→早すぎた
	- 魚サイド
		- 餌を食べたらデータを受信(タイムアウト ～1s)
		- 受信できた→釣られる
		- できなかった→遅すぎた


- 餌代を毎回減らす
- 餌の値段を上げると良い魚が来るが、難易度は上がる
- スイッチ操作で餌の入れ替え
	- 一旦魚は逃げる
	- お金の操作
魚は餌に近づくようにする
	早く上げると逃げる
魚を何種類か作成して、それぞれの魚の難易度・報酬を設定する

- LED
	- 電源LED
		- 魚が餌を食べたら点灯
	- タイマーLED
		- 連れたら点灯


- 工夫
	- データキュー
		- 魚が餌を食べたら釣られるのを一定時間待つようにした
	- イベントフラグ
		- バッファに内容を書き込んでから最後に表示するようにした
		- 書き込むより先に表示しないように
	- 周期ハンドラでタスクを呼び出した
	- 可読性を高めるために
		- サブルーチンを用いてスッキリ
		- サブルーチンからの呼び出しは最低限にした
	
*/
