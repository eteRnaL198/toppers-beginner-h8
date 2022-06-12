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
volatile Fish fish1_data = {31, -1, 1};
volatile Fish *fish1 = &fish1_data;
volatile UB start_sw_past, start_sw_current;
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

	start_sw_past = get_key(START_SW);
	up_sw_past = get_key(UP_SW);
	land_sw_past = get_key(LAND_SW);
}

void fish_task() {
	int isEat = move_fish(fish1);
	if(isEat) eat();
	draw_fish(fish1);
	ext_tsk();
}

void angler_task() {
	land();
	ext_tsk();
}



/* ----------------------------
	ハンドラ
 ---------------------------- */
void fish_handler() {
	if(screen == PLAY) iact_tsk(FISH_TASK);
}

void angler_handler() {
	if(screen == PLAY) {
		draw_rod();
		set_led(TIMER_LED, OFF);
	}
}

void screen_handler() {
	display_lcd(2, lcdbuf);
	clear_display();
}

void switch_handler() {
	// スイッチ1: 画面制御
	start_sw_current = get_key(START_SW);
	if(start_sw_current != start_sw_past) {
		if(start_sw_current == ON) {
			syslog_0(LOG_NOTICE, "Start the game");
			screen = PLAY;
		} else {
			screen = MENU;
			syslog_0(LOG_NOTICE, "Pause the game");
		}
	}
	start_sw_past = start_sw_current;

  // スイッチ2: 釣り上げる
	up_sw_current = get_key(UP_SW);
	if(screen == PLAY && up_sw_current != up_sw_past && up_sw_current == ON) {
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
int move_fish(Fish *fish) {
	int isEat = 0;
	if(fish->x == BAIT_X+16) {
		isEat = 1;
		fish->x = 31;
	} else {
		fish->x += fish->direction;
	}
	set_led(POW_LED, OFF);
	return isEat;
}

void eat() {
	set_led(POW_LED, ON);
	VP_INT *tmp;
	ER result = trcv_dtq(LAND_DTQ, tmp, 500);
	if(result == E_TMOUT) {
		draw_msg("Miss..");
	}
	set_led(POW_LED, OFF);
}

void land() {
	ER result = tsnd_dtq(LAND_DTQ, (VP_INT)1, 0);
	if(result == E_OK) {
		draw_msg("Catch! +$5");
		set_led(TIMER_LED, ON);
	} else if(result == E_TMOUT) {
		draw_msg("Too fast!");
	}
}

void draw_fish(Fish *fish) {
	if(fish->direction == -1) {
		// <゜))<
		lcdbuf[fish->x] = 0x3c;
		lcdbuf[fish->x+1] = 0xdf;
		lcdbuf[fish->x+2] = 0x29;
		lcdbuf[fish->x+3] = 0x29;
		lcdbuf[fish->x+4] = 0x3c;
	} else {
		// >((゜>
		lcdbuf[fish->x-4] = 0x3e; 
		lcdbuf[fish->x-3] = 0x28;
		lcdbuf[fish->x-2] = 0x28;
		// lcdbuf[fish->x-1] = 0xf8; // x
		lcdbuf[fish->x-1] = 0xdf; // ゜
		lcdbuf[fish->x] = 0x3e;
		// TODO 上の行にはみ出さないように
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
[] 魚挙動
[] too fastで逃げる
[] 釣り竿上げた感出す

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
	- メッセージの管理
		- 1対1のメッセージはデータで送る
		- 1対多 or 多対多のメッセージは共有変数にして、セマフォで排他制御する
	- 周期ハンドラで呼び出し
	- 可読性を高めるために
		- サブルーチンを用いてスッキリ
		- サブルーチンからの呼び出しは最低限にした
	
*/
