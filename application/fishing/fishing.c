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

volatile int is_running = 0;

// LCD表示用
static char const raddec[] = "0123456789"; /* 数字を表す文字コード*/
static char lcdbuf[32]; /* 1行は16文字表示可能、2行分のバッファ */

/*
 *  タイマ用タスク
 */
void timer_task(VP_INT exinf)
{
	SYSTIM base_time, current_time;
	TMO tmout = 0;
	UB pow_led = OFF;
	int t_s, t_qs;

	syslog_1(LOG_NOTICE, "Sample1 timer task starts (exinf = %d).", exinf);
	get_tim(&base_time);	/* 現在時間の取り出し */

	Fish fish1_data = {31};
	Fish *fish1 = &fish1_data;

	for (;;) {
		tslp_tsk(tmout);	/* TICK待ち */
		t_s = current_time / T_1SEC; // 1sに変換
		t_qs = current_time / T_TICK; // 1/4sに変換

		/* 奇数秒で電源LEDオン */
		if (t_s & 0x01) {
			// pow_led = ON;
		} else {
			pow_led = OFF;
		}
		set_led(POW_LED, pow_led);

		move_fish(t_s, fish1);

		base_time += T_TICK;
		get_tim(&current_time);
		tmout = base_time - current_time;
		if (tmout < 0)
			tmout = 0;
	}
}

/*
 *  メインタスク
 *  (スイッチプロセス)
 */
void entry_task(VP_INT exinf)
{
	UB start_sw, sw;
	UB time_led = OFF;

	syslog_1(LOG_NOTICE, "Sample entry task starts (exinf = %d).", exinf);
	initial_key();		/* キーの初期化 */
	initial_led();		/* LEDの初期化 */
	initial_lcd();		/* LCDの初期化 */
	clear_buf();

	start_sw = get_key(START_SW);	/* 現在のタイマ起動SWの取り込み */
	act_tsk(TIMER_TASK);	/* タイマータスクの起動 */

	for (;;) {
		tslp_tsk(500);
		sw = get_key(START_SW);
		if (start_sw != sw) {
			syslog_1(LOG_NOTICE, "Change START_SW = 0x%x.",(int)sw);
			if (sw == ON) {
				// time_led = ON;
				is_running = 1;
			} else {
				time_led = OFF;
				is_running = 0;
			}
			start_sw = sw;
		}
		set_led(TIMER_LED, time_led);	/* タイマLEDの設定 */
	}
}

/* 以下、LCD表示のために追加 */

/*
 * 時間を表示する
 * arg1:時間
 * arg2:魚へのポインタ
 */
void move_fish(int t_s, Fish *fish) {
	UB pow_led = OFF;
	if(is_running) {
		clear_buf();
		int bait_x = 5;
		lcdbuf[bait_x] = 'J'; // 餌
		if(fish->x+16 == bait_x) {
			pow_led = ON;
		} else {
			pow_led = OFF;
			fish->x--;
		}
		set_led(POW_LED, pow_led);
		draw_fish(fish);
	}

	/* バッファに記憶された文字をLCD表示（２行表示） */
	display_lcd(2, lcdbuf);
}

void clear_buf() {
	/* バッファにはスペースの文字コードを入れておく */
	int i;
	for (i = 0; i < 32; i++) {
		lcdbuf[i] = ' ';
	}
}

void draw_fish(Fish *fish) {
	// <゜)<
	lcdbuf[fish->x] = 0x3c;
	lcdbuf[fish->x+1] = 0xdf;
	lcdbuf[fish->x+2] = 0x29;
	lcdbuf[fish->x+3] = 0x3c;

	// >(@>
	// lcdbuf[25] = 0x3e; 
	// lcdbuf[26] = 0x28;
	// lcdbuf[27] = 0x40; 
	// lcdbuf[28] = 0x3e;
}


// タスク
// 魚
	// 名前・座標・報酬・難易度
	// 動かす・食べる
	// 逃がす
// 餌
	// 安い・高い
	// 変更
	// 上げる
// お金
	// 減らす・増やす

// TODO 周期ハンドラを作成してタスクを呼び出す
	// 周期的に魚を動かす
// TODO 餌を上げるとメールを送信する
	// 魚が餌を食べてなかったら逃げる
//TODO タイムアウト付きのイベントフラグを立てる
	// 制限時間内にスイッチが押される(イベント発生)か判定する

// 画面
	// 釣る
	// 魚GET・逃げる

// TODO 餌代を毎回減らす
// TODO 餌の値段を上げると良い魚が来るが、難易度は上がる
// TODO スイッチ操作で餌の入れ替え
	// 	一旦魚は逃げる
	//  お金の操作
// TODO 魚は餌に近づくようにする
	// 早く上げると逃げる
// TODO 魚を何種類か作成して、それぞれの魚の難易度・報酬を設定する
