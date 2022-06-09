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
 *  @(#) $Id: timer2.c,v 1.0 2003/11/08 08:12:24 roi Exp $
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

#include <t_services.h>
#include "kernel_id.h"
#include "device.h"
#include "timer2.h"

/* 数字を表す文字コード, LCD表示のために追加*/
static char const raddec[] = "0123456789";

/* LCD表示用バッファ, LCD表示のために追加 */
/* 1行は16文字表示可能、2行分のバッファ */
static char lcdbuf[32];

volatile int timer_extd = 1;
volatile int is_timer_act = 0;

/*
 *  タイマ用タスク
 */
void timer_task(VP_INT exinf) {
	SYSTIM base_time, current_time, act_time;
	TMO tmout = 0;
	UB pow_led = OFF;
	UB timer_led = OFF;
	int blink_count = -1;
	int t_s, t_qs;

	syslog_1(LOG_NOTICE, "Sample1 timer task starts (exinf = %d).", exinf);
	get_tim(&base_time);	/* 現在時間の取り出し */
	get_tim(&act_time);	/* 現在時間の取り出し */
	for (;;) {
		tslp_tsk(tmout);	/* TICK待ち */
		t_s = act_time / T_1SEC; // 1sに変換
		t_qs = act_time / T_TICK; // 1/4sに変換

		/* 奇数秒で電源LEDをオン */
		if (t_s & 0x01) {
			pow_led = ON;
		} else {
			pow_led = OFF;
		}
		set_led(POW_LED, pow_led);

		// タイマLEDを10秒毎に250m秒で点滅させる
		if (t_s != 0 && t_s % 10 == 0 && t_qs % 2 == 0) {
			timer_led = ON;
		} else {
			timer_led = OFF;
		}

		// 1分後にタイマLEDを30回点滅させる
		if (t_s != 0 && t_s % (60*timer_extd) == 0 && t_qs % 4 == 0) {
			blink_count = 30;
		}
		if (blink_count > 0 && (t_qs % 2 == 0) && is_timer_act) {
			timer_led = ON;
			blink_count--;
		} else if(timer_led != ON) { // 10秒毎の点滅を妨げないように
			timer_led = OFF;
		}

		syslog_4(LOG_NOTICE, "time: %d, tick: %d, timer_led:%d, cnt:%d", t_s, t_qs, timer_led, blink_count);
		set_led(TIMER_LED, timer_led);
		display_time((UW) act_time);	/* act_timeのLCD表示 */

		if(is_timer_act) {
			act_time += T_TICK;
		}
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
void entry_task(VP_INT exinf) {
	UB start_sw, sw;
	UB time_led = OFF;
	UB up_sw_past, up_sw_current;

	syslog_1(LOG_NOTICE, "Sample entry task starts (exinf = %d).", exinf);
	initial_key();		/* キーの初期化 */
	initial_led();		/* LEDの初期化 */
	initial_lcd();		/* LCDの初期化 */
	start_sw = get_key(START_SW);	/* 現在のタイマ起動SWの取り込み */
	up_sw_past = get_key(UP_SW);	/* 現在のタイマ延長SWの取り込み */

	for (;;) {
		tslp_tsk(250);
		sw = get_key(START_SW);
		if (start_sw != sw) {
			syslog_1(LOG_NOTICE, "Change START_SW = 0x%x.", (int)sw);
			if (sw == ON) {
				syslog_0(LOG_NOTICE, "Start timer");
				act_tsk(TIMER_TASK);	/* タイマータスクの起動 */
				is_timer_act = 1;
			} else {
				syslog_0(LOG_NOTICE, "Stop timer");
				is_timer_act = 0;
			}
			start_sw = sw;
		}


		// タイマー延長スイッチがオンになったらタイマーを60秒延長する
		up_sw_current = get_key(UP_SW);
		if(up_sw_past != up_sw_current) {
			if(up_sw_current == ON) {
				syslog_0(LOG_NOTICE, "Extend timer 60 seconds");
				timer_extd++;
			}
			up_sw_past = up_sw_current;
		}
	}
}


/*
 * 時間を表示する
 * arg1:時間
 */
void display_time(UW time)
{
	int i;

	/* バッファにはスペースの文字コードを入れておく */
	for (i = 0; i < 32; i++) {
		lcdbuf[i] = ' ';
	}

	/* １行目は左側から"Time"と表示する */
	lcdbuf[0] = 'T';
	lcdbuf[1] = 'i';
	lcdbuf[2] = 'm';
	lcdbuf[3] = 'e';
	lcdbuf[4] = 'r';

	// lcdbuf[0] = 'R';
	// lcdbuf[1] = 'e';
	// lcdbuf[2] = 'm';
	// lcdbuf[3] = 'a';
	// lcdbuf[4] = 'i';
	// lcdbuf[5] = 'n';
	// lcdbuf[6] = 'i';
	// lcdbuf[7] = 'n';
	// lcdbuf[8] = 'g';

	// lcdbuf[10] = 'T';
	// lcdbuf[11] = 'i';
	// lcdbuf[12] = 'm';
	// lcdbuf[13] = 'e';

	// lcdbuf[31] = 's';

	// 残り時間を表示する
	syslog_0(LOG_NOTICE, "display time");
	int alert_time = 0;
	while ((time / T_1SEC) > alert_time) {
		alert_time += 60*timer_extd;
	}
	int remaining_time = alert_time - (time / T_1SEC);
	i = 30;
	do {
		lcdbuf[i] = raddec[remaining_time % 1000];
		remaining_time /= 1000;
		i--;
	} while (time != 0);


	/* バッファに記憶された文字をLCD表示（２行表示） */
	display_lcd(2, lcdbuf);
}
