/*
 *  TOPPERS/JSP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Just Standard Profile Kernel
 * 
 *  Copyright (C) 2000-2002 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 *  Copyright (C) 2003-2004 by TOPPERS Project Educational Group.
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
 *  @(#) $Id: timer2lcd.h,v 1.1 2003/12/21 21:43:24 roi Exp $
 *  "lcd" added by Yokoyama
 */

/* 
 *  キッチンタイマのヘッダーファイル
 */
#include <t_services.h>

/*
 *  タスク優先度の設定
 */
#define DEFAULT_MAIN_PRIORITY      8
#define FISH_PRIORITY              9
#define ANGLER_PRIORITY            9

/*
 *  ターゲットに依存する可能性のある定数の定義
 *  スタックサイズ
 */

#ifndef STACK_SIZE
#define STACK_SIZE  386     /* タスクのスタックサイズ(トップ・カプセル参照) */
#endif /* STACK_SIZE */

/*
 *  タイマープロセスの時間設定
 */
#define T_TICK              250     /* 実行周期 250ms */
#define T_1SEC              1000    /* タイマー上の１秒 */


#define BAIT_X  4

#define START 0
#define PLAY 1
#define MENU 2
#define OVER 3

/*
 *  型宣言
 */
#ifndef _MACRO_ONLY

typedef struct {
  int x;
  int direction;
  int level;
} Fish;

extern void init_task();
extern void fish_task();
extern void angler_task();

extern void fish_handler();
extern void angler_handler();
extern void screen_handler();
extern void switch_handler();

int move_fish(Fish*);
void eat();
void land();
void blink_led(UB);
void draw_fish(Fish*);
void draw_rod();
void draw_msg(char*);
void clear_display();


#endif /* _MACRO_ONLY */
