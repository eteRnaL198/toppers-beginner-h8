/*
 *  TOPPERS/JSP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Just Standard Profile Kernel
 * 
 *  Copyright (C) 2000-2004 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
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
 *  @(#) $Id: sil_akih8_3069f.h,v 1.7 2003/07/01 13:32:20 hiro Exp $
 */

/*
 *   システムインタフェースレイヤ（追加部分）
 *   　I/Oポートのデータ・ディレクション・レジスタへのアクセス
 *
 *　　H8のDDRは書き込み専用であり、そのままでは所望のビットだけを
 *　　変更することができない。（bset,bclr命令でも回避不可）
 *　　そのため、本実装では、メモリ上にテンポラリを用意して、DDRの
 *　　現在値を保持する方法を採っている。
 *　　
 *　　ポート7は入力専用のため、省略している。
 *
 *      　　　注意
 *      　　　　ポート1,2,5はアドレスバスとして使用されている
 *      　　　　ポート3はデータバスとして使用されている
 *      　　　　ポート7は入力専用なので入出力方向の設定の必要はない
 *      　　　　ポート8はCS信号に使用されている
 *      　　　　          Chip Select
 *      　　　　ポートBはUCAS信号に使用されている
 *      　　　　          Column Address Strobe
 */

#ifdef JSP_KERNEL
#include "s_services.h"
#include "h8_3069f2.h"

#else /* JSP_KERNEL */

#include "h8_3069f.h"
#include "typedef.h"
#include <sil.h>

#endif /* JSP_KERNEL */


#include "sil_akih8_3069f.h"

/*
 *　メモリ上に配置するDDRのテンポラリ領域
 *　　
 *　　ポート7は入力専用のため、省略している。
 */
#define DUMMY   (UB)0
static UB ddr_tmp[] = {
        0xff, 							/*  ポート1  */
        0xff, 							/*  ポート2  */
        0xff, 							/*  ポート3  */
        DUMMY, 							/*  ポート4  */
        H8P5DDR_A16, 					/*  ポート5  */
        DUMMY,  						/*  ポート6  */
        (UB)(H8P8DDR_CS1|H8P8DDR_CS2),  /*  ポート8  */
        DUMMY,  						/*  ポート9  */
        DUMMY,  						/*  ポートA  */
        H8PBDDR_UCAS 					/*  ポートB  */
};

static const VP ddr_adr[] = {
        (VP)H8P1DDR,
        (VP)H8P2DDR,
        (VP)H8P3DDR,
        (VP)H8P4DDR,
        (VP)H8P5DDR,
        (VP)H8P6DDR,
        (VP)H8P8DDR,
        (VP)H8P9DDR,
        (VP)H8PADDR,
        (VP)H8PBDDR
};


/*
 *      DDRの読み出し
 */
UB sil_reb_ddr(UINT port)
{
        return ddr_tmp[port];
}

/*
 *      DDRの書き込み
 */
void sil_wrb_ddr(UINT port, UB data)
{
        ddr_tmp[port] = data;
        sil_wrb_mem(ddr_adr[port], data);
}

/*
 *      DDRのAND演算
 */
void sil_anb_ddr(UINT port, UB data)
{
        UB ddr;

        ddr = sil_reb_ddr(port);
        ddr &= data;
        sil_wrb_ddr(port, ddr);
}

/*
 *      DDRのOR演算
 */
void sil_orb_ddr(UINT port, UB data)
{
        UB ddr;

        ddr = sil_reb_ddr(port);
        ddr |= data;
        sil_wrb_ddr(port, ddr);
}

void sil_dly_nse(UINT dlytim)
{
        while(dlytim--);
}
