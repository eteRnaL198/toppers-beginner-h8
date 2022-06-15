/*
 *  TOPPERS/JSP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Just Standard Profile Kernel
 * 
 *  Copyright (C) 2000-2004 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 *  Copyright (C) 2003-2004 by TOPPERS Project Educational Group.
 *  Copyright (C) 2004-2005 by Industrial Technology Institute,
 *                              Miyagi Prefectural Government, JAPAN
 * 
 *  上記著作権者は，以下の (1)〜(4) の条件か，Free Software Foundation 
 *  によって公表されている GNU General Public License の Version 2 に記
 *  述されている条件を満たす場合に限り，本ソフトウェア（本ソフトウェア
 *  を改変したものを含む．以下同じ）を使用・複製・改変・再配布（以下，
 *  利用と呼ぶ）することを無償で許諾する．
 *  (1) 本ソフトウェアをソースコードの形で利用する場合には，上記の著作
 *      権表示，この利用条件および下記の無保証規定が，そのままの形でソー
 *      スコード中に含まれていること．
 *  (2) 本ソフトウェアを，ライブラリ形式など，他のソフトウェア開発に使
 *      用できる形で再配布する場合には，再配布に伴うドキュメント（利用
 *      者マニュアルなど）に，上記の著作権表示，この利用条件および下記
 *      の無保証規定を掲載すること．
 *  (3) 本ソフトウェアを，機器に組み込むなど，他のソフトウェア開発に使
 *      用できない形で再配布する場合には，次のいずれかの条件を満たすこ
 *      と．
 *    (a) 再配布に伴うドキュメント（利用者マニュアルなど）に，上記の著
 *        作権表示，この利用条件および下記の無保証規定を掲載すること．
 *    (b) 再配布の形態を，別に定める方法によって，TOPPERSプロジェクトに
 *        報告すること．
 *  (4) 本ソフトウェアの利用により直接的または間接的に生じるいかなる損
 *      害からも，上記著作権者およびTOPPERSプロジェクトを免責すること．
 * 
 *  本ソフトウェアは，無保証で提供されているものである．上記著作権者お
 *  よびTOPPERSプロジェクトは，本ソフトウェアに関して，その適用可能性も
 *  含めて，いかなる保証も行わない．また，本ソフトウェアの利用により直
 *  接的または間接的に生じたいかなる損害に関しても，その責任を負わない．
 * 
 *  @(#) $Id: device.c,v 1.17 2003/12/20 08:11:59 hiro Exp $
 */

/* 
 *  AKI-H8/3069 I/Oボード・デバイスドライバ
 */

/* 
 *  液晶表示機能を追加
 * "akih8lcd_device.c", "akih8pot_device.c", "akih8pot_device.h"を参照
 *  by Yokoyama, 2007/10/18
 */

#ifdef JSP_KERNEL       /*  カーネルを使用する場合  */
#include "s_services.h"
#include "h8_3069f2.h"

#else /* JSP_KERNEL */  /*  カーネルを使用しない場合  */

#include "h8_3069f.h"
#include "typedef.h"
#include <sil.h>

#endif /* JSP_KERNEL */

#include "sil_akih8_3069f.h"
#include "device.h"

/*
 * LCDの信号
 */
#define	E_SIG			0x20			/* イネーブル信号 */
#define	RS				0x10			/* ＲＳ信号 */

/*
 *  LCDディレイカウント
 */
#ifndef DCNT_CMD
#define	DCNT_CMD		250
#endif
#ifndef DCNT_FNC
#define	DCNT_FNC		2000
#endif


/* register address */

/* I/O ports */

/*
 *  port4
 *  　P46：LED1
 *  　P47：LED2
 */
static const UB led_pattern[] = {H8P46DR, H8P47DR};

/*
 *  port5　DIP-SW
 *　　ポーリング設定時：入力ポート（スイッチのセンシング）
 *  　　P50：SW1
 *  　　P51：SW2
 *  　　P52：SW3
 *  　　P53：SW4
 *　　割込み設定時：ポート8への電圧供給（プルアップ）
 *  　　結線
 *  　　　　P50-P80-SW1-GND
 *  　　　　P51-P81-SW2-GND
 */
static const UB sw_pattern[] = {H8P50DR, H8P51DR, H8P52DR, H8P53DR};

static UB akih8_cled;


static void initial_p5_pull_up(void);
static void wait_time(int);
static void lcd_out8(UB c);
static void lcd_out4(UB d);
static void lcd_rs_bit(INT state);

/*
 *  ボードの初期化
 *  　
 */
void initial_board(int init_type) 
{
        initial_led();                  /*  LEDの初期化  */
	initial_lcd();                  /*  LCDの初期化, added by Yokoyama */

        switch(init_type) {
                case INIT_POLLING:      /*  ポーリング動作の初期化  */
                        initial_key();  
                        break;
                case INIT_INTERRUPT:    /*  割込み動作の初期化  */
                        initial_irq();
                        break;
        }
}


/*
 *  LEDの初期化
 *  　データバス幅が8ビットであることを仮定している。
 */
void initial_led(void) 
{
        /*  P4_6-7を出力  */
        sil_orb_ddr(IO_PORT4, H8P46DDR | H8P47DDR);
}


/*
 *  LEDの設定を行う
 *  　led:LED1またはLED2
 *  　req:ONまたはOFF
 */
void
set_led(int led, int req)
{
        UB p4 = sil_reb_mem((VP)H8P4DR);
        
        if (req == ON) {
                p4 |= led_pattern[led];
        } else {
                p4 &= ~led_pattern[led];
        }
        sil_wrb_mem((VP)H8P4DR, p4);
}


/*
 *  P5_0-1を入力かつプルアップに設定
 *　　　P5_0-1：入力かつプルアップ
 */
void
initial_p5_pull_up(void)
{
        /*  P5_0-1を入力に設定  */
        sil_anb_ddr(IO_PORT5, ~(H8P50DDR | H8P51DDR | H8P52DDR | H8P53DDR));
        
        /*  P5_0-1をプルアップに設定  */
        sil_orb_mem((VP)H8P5PCR, (H8P50PCR | H8P51PCR | H8P52PCR | H8P53PCR));
}

/*
 *  スイッチの初期化
 *  　P5_0-1：入力かつプルアップ
 */
void
initial_key(void)
{
        initial_p5_pull_up();
}


/*
 *  スイッチの状態を取り出す
 *  　sw:SW1またはSW2
 *  　戻り値:SWのON/OFF状態
 */
int
get_key(int sw)
{
        UB p5 = sil_reb_mem((VP)H8P5DR);
        
        p5 &= sw_pattern[sw];
        if (p5) {                       /*  負論理  */
                return OFF;
        } else {
                return ON;
        }
}


/*
 *  IRQ0,IRQ1の初期化
 *   IRQ0：P80
 *   IRQ1：P81
 *  　　結線
 *  　　　　P50-P80-SW1-GND
 *  　　　　P51-P81-SW2-GND
 *  　　　　
 *  　　　　ポート5のプルアップ設定によりポート8に電圧を供給する
 */
void
initial_irq(void)
{
        /*  P5_0-1：入力かつプルアップ */
        initial_p5_pull_up();

        /*  IRQ割込み禁止  */
        sil_anb_mem((VP)H8IER, ~(H8IER_IRQ0E | H8IER_IRQ1E));

        /*  
         *　割込みを禁止すると、一時的に入出力ポートになるので
         *　入出力方向を入力にする
         *　（割込みの入口処理でも割込み禁止にしている箇所があるので
         *　　そこでIRQ入力端子が出力ポートにならないようにする）
         */
        sil_anb_ddr(IO_PORT5, ~(H8P80DDR | H8P81DDR));

        /*  割込みレベルをレベル1に設定  */
        sil_orb_mem((VP)H8IPRA, (H8IPRA_IPRA7 | H8IPRA_IPRA6));

        /*  立ち下がりエッジ検出  */
        sil_orb_mem((VP)H8ISCR, (H8ISCR_IRQ0SC | H8ISCR_IRQ1SC));
        
        /*  割込み要求クリア  */
        sil_reb_mem((VP)H8ISR);
        sil_wrb_mem((VP)H8ISR, ~(H8ISR_IRQ0F | H8ISR_IRQ1F));

        /*  IRQ割込み許可  */
        sil_orb_mem((VP)H8IER, (H8IER_IRQ0E | H8IER_IRQ1E));

}


/*
 *  LCDを表示する
 */
static B *
display_line(B * p)
{
	int i;

	for(i = 0 ; i < 16 ; i++){
		lcd_rs_bit(ON);
		lcd_out4(*p++);
		lcd_rs_bit(OFF);
	}
	return p;
}

/*
 * LCDへの表示
 * 第１引数：表示行数、第２引数：表示バッファの先頭へのポインタ
 */
void
display_lcd(int no, char* p)  /* modified by Yokoyama */
{
	/* wai_sem(SEM_DISPLAY); */ /* deleted by Yokoyama */
	lcd_rs_bit(OFF);						/* カーソールホーム */
	lcd_out4(0x02);
	sil_dly_nse(DCNT_FNC);

	if(no != 1){							/* 1行目表示 */
		p = display_line(p);
	}

	if(no > 0){
		lcd_rs_bit(OFF);					/* ホーム2行目 */
		lcd_out4(0xc0);
		sil_dly_nse(DCNT_FNC);
		p = display_line(p);				/* 2行目表示 */
	}
	/* sig_sem(SEM_DISPLAY); */ /* deleted by Yokoyama */
}

/*
 * LCD 信号制御
 */
static void
lcd_bit(INT s, INT state)
{
        UB p4;   /* added by Yokoyama */

	if(state)
		akih8_cled |= s;
	else
		akih8_cled &= (~s);

        p4 = sil_reb_mem((VP)H8P4DR);     /* added by Yokoyama */
	p4 &= 0xc0;                       /* added by Yokoyama */
	p4 |= akih8_cled;                 /* added by Yokoyama */
        sil_wrb_mem((VP)H8P4DR, p4);      /* added by Yokoyama */
	/* sil_wrb_mem((VP)H8P4DR, akih8_cled); */ /* deleted by Yokoyama */
}

/*
 * LCD 8ビット出力
 */
void
lcd_out8(UB c)
{
        UB p4;   /* added by Yokoyama */

	lcd_bit(E_SIG, ON);
	akih8_cled = (akih8_cled & 0xc0) | (c & 0x3f);

        p4 = sil_reb_mem((VP)H8P4DR);     /* added by Yokoyama */
	p4 &= 0xc0;                       /* added by Yokoyama */
	p4 |= akih8_cled;                 /* added by Yokoyama */
        sil_wrb_mem((VP)H8P4DR, p4);      /* added by Yokoyama */
	/* sil_wrb_mem((VP)H8P4DR, akih8_cled); */ /* deleted by Yokoyama */
	sil_dly_nse(DCNT_CMD);
	lcd_bit(E_SIG, OFF);
	sil_dly_nse(DCNT_CMD);
}

/*
 * LCD 4ビット出力
 */
void
lcd_out4(UB d)
{
        UB p4;   /* added by Yokoyama */

	lcd_bit(E_SIG, ON);
	akih8_cled = (akih8_cled & 0xf0) | (d >> 4);

        p4 = sil_reb_mem((VP)H8P4DR);     /* added by Yokoyama */
	p4 &= 0xc0;                       /* added by Yokoyama */
	p4 |= akih8_cled;                 /* added by Yokoyama */
        sil_wrb_mem((VP)H8P4DR, p4);      /* added by Yokoyama */
	/* sil_wrb_mem((VP)H8P4DR, akih8_cled);*/ /* deleted by Yokoyama */
	sil_dly_nse(DCNT_CMD);
	lcd_bit(E_SIG, OFF);
	sil_dly_nse(DCNT_CMD);

	lcd_bit(E_SIG, ON);
	akih8_cled = (akih8_cled & 0xf0) | (d & 0x0f);

        p4 = sil_reb_mem((VP)H8P4DR);     /* added by Yokoyama */
	p4 &= 0xc0;                       /* added by Yokoyama */
	p4 |= akih8_cled;                 /* added by Yokoyama */
        sil_wrb_mem((VP)H8P4DR, p4);      /* added by Yokoyama */
	/* sil_wrb_mem((VP)H8P4DR, akih8_cled); */ /* deleted by Yokoyama */
	sil_dly_nse(DCNT_CMD);
	lcd_bit(E_SIG, OFF);
	sil_dly_nse(DCNT_CMD);
}

/*
 * LCD RS制御
 */
void
lcd_rs_bit(INT state)
{
	lcd_bit(RS, state);
}

/*
 * LCDの初期化
 */
/*
void
lcd_init(void)
*/

void
initial_lcd(void)  /* modified by Yokoyama */
{
	sil_wrb_mem((VP)H8P4DDR, 0xff);			/* ポート4出力設定		*/
					/* LCDのリセット */
	lcd_bit(RS, OFF);						/* LCDファンクションセット1 */
	lcd_out8(0x23);
	sil_dly_nse(DCNT_FNC);

	lcd_bit(RS, OFF);						/* LCDファンクションセット2 */
	lcd_out8(0x23);
	sil_dly_nse(DCNT_FNC);

	lcd_bit(RS, OFF);						/* LCDファンクションセット3 */
	lcd_out8(0x23);
	sil_dly_nse(DCNT_FNC);

	lcd_bit(RS, OFF);						/* ファンクションセット */
	lcd_out8(0x22);
	sil_dly_nse(DCNT_FNC);
					/* LCDの初期化 */
	lcd_bit(RS, OFF);						/* ファンクションセット */
	lcd_out4(0x28);
	sil_dly_nse(DCNT_FNC);

	lcd_bit(RS, OFF);						/* 表示ＯＮ */
	lcd_out4(0x0e);
	sil_dly_nse(4000);

	lcd_bit(RS, OFF);						/* エントリーモード */
	lcd_out4(0x0c);
	sil_dly_nse(DCNT_FNC);
}



