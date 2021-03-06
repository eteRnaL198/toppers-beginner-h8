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
 *  @(#) $Id: sys_support.S,v 1.5 2003/12/11 07:00:10 honda Exp $
 */

#define _MACRO_ONLY
#include "h8.h"
#include "h8_3069f.h"

        .h8300h

        .text
        .align 2


/*
 *  IRQ0割込み
 */

/* IRQ0割込み許可 */

        .globl _irq0_handler_enable_int
        .globl _sw1_handler_enable_int
_irq0_handler_enable_int:
_sw1_handler_enable_int:
        mov.b   @H8IER, r0l
        bset    #H8IER_IRQ0E_BIT, r0l
        mov.b   r0l, @H8IER
        rts

/* IRQ0割込み禁止 */

        .globl _irq0_handler_disable_int
        .globl _sw1_handler_disable_int
_irq0_handler_disable_int:
_sw1_handler_disable_int:
        mov.b   @H8IER, r0l             /*  割込み禁止  */
        bclr    #H8IER_IRQ0E_BIT, r0l
        mov.b   r0l, @H8IER

        mov.b   @H8ISR, r0l             /*  割込み要求クリア  */
        bclr    #H8ISR_IRQ0F_BIT, r0l
        mov.b   r0l, @H8ISR
        rts


/*
 *  IRQ1割込み
 */

/* IRQ1割込み許可 */

        .globl _irq1_handler_enable_int
        .globl _sw2_handler_enable_int
_irq1_handler_enable_int:
_sw2_handler_enable_int:
        mov.b   @H8IER, r0l
        bset    #H8IER_IRQ1E_BIT, r0l
        mov.b   r0l, @H8IER
        rts

/* IRQ1割込み禁止 */

        .globl _irq1_handler_disable_int
        .globl _sw2_handler_disable_int
_irq1_handler_disable_int:
_sw2_handler_disable_int:
        mov.b   @H8IER, r0l             /*  割込み禁止  */
        bclr    #H8IER_IRQ1E_BIT, r0l
        mov.b   r0l, @H8IER

        mov.b   @H8ISR, r0l             /*  割込み要求クリア  */
        bclr    #H8ISR_IRQ1F_BIT, r0l
        mov.b   r0l, @H8ISR
        rts

