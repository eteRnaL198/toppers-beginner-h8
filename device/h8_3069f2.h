/*
 *  TOPPERS/JSP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Just Standard Profile Kernel
 * 
 *  Copyright (C) 2000-2004 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 *  Copyright (C) 2004-2005 by Industrial Technology Institute,
 *                              Miyagi Prefectural Government, JAPAN
 *  Copyright (C) 2001-2004 by Dep. of Computer Science and Engineering
 *                   Tomakomai National College of Technology, JAPAN
 *  Copyright (C) 2001-2004 by Kunihiko Ohnaka
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
 *  @(#) $Id: h8_3069f.h,v 1.5 2004/09/03 15:39:08 honda Exp $
 */

#ifndef _H8_3069F2_H_
#define _H8_3069F2_H_

/*
 *  H8/3069F 用定義（追加分）
 */

/*
 *  内蔵メモリの定義
 */

#define H8IN_ROM_BASE		0x000000
#define H8IN_ROM_SIZE		0x080000
#define H8IN_RAM_BASE		0xffbf10
#define H8IN_RAM_SIZE		0x004000

/*
 *  I/Oポート
 */
/*  ポート4  */
#define H8P46DDR		0x40
#define H8P47DDR		0x80

#define H8P46DR			0x40
#define H8P47DR			0x80

/*  ポート5  */
#define H8P50DDR		0x01
#define H8P51DDR		0x02
#define H8P52DDR		0x04
#define H8P53DDR		0x08

#define H8P50DR			0x01
#define H8P51DR			0x02
#define H8P52DR			0x04
#define H8P53DR			0x08

#define H8P50PCR		0x01
#define H8P51PCR		0x02
#define H8P52PCR		0x04
#define H8P53PCR		0x08


/*  ポート8  */
#define H8P80DR			0x01
#define H8P81DR			0x02

#define H8P80DDR		0x01
#define H8P81DDR		0x02

/*
 *  割込み関連
 */
/*  IRQセンス・コントロール・レジスタ  */
//#define H8ISCR			0xfee014
#define H8ISCR_IRQ0SC	0x01
#define H8ISCR_IRQ1SC	0x02

/*  IRQイネーブル・レジスタ  */
//#define H8IER			0xfee015
#define H8IER_IRQ0E		0x01
#define H8IER_IRQ1E		0x02

/*  インターラプト・ステータス・レジスタ  */
//#define H8ISR			0xfee016
#define H8ISR_IRQ0F		0x01
#define H8ISR_IRQ1F		0x02

#define H8ISR_IRQ0F_BIT		0x00
#define H8ISR_IRQ1F_BIT		0x01

/*  インターラプト・プライオリティ・レジスタ  */
//#define H8IPRA			0xfee018
#define H8IPRA_IPRA7		0x80
#define H8IPRA_IPRA6		0x40

#endif /* _H8_3069F2_H_ */
