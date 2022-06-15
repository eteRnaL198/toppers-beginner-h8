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
 *      システムインタフェースレイヤ（追加部分）
 */

#ifndef _SIL_AKIH8_3069F_H_
#define _SIL_AKIH8_3069F_H_

/*
 *　I/Oポートのデータ・ディレクション・レジスタDDRへのアクセス
 *　
 *　　H8のDDRは書き込み専用であり、そのままでは所望のビットだけを
 *　　変更することができない。（bset,bclr命令でも回避不可）
 *　　そのため、本実装では、メモリ上にテンポラリを用意して、DDRの
 *　　現在値を保持する方法を採っている。
 */
extern UB sil_reb_ddr(UINT port);
extern void sil_wrb_ddr(UINT port, UB data);
extern void sil_anb_ddr(UINT port, UB data);
extern void sil_orb_ddr(UINT port, UB data);
extern void sil_dly_nse(UINT dlytim);

/*
 *　DDRの番号定義
 *　
 *　　配列のインデックスに用いる。
 *　　ポート7は入力専用のため、省略している。
 */
enum IO_PORT {
	IO_PORT1,
	IO_PORT2,
	IO_PORT3,
	IO_PORT4,
	IO_PORT5,
	IO_PORT6,
	IO_PORT8,
	IO_PORT9,
	IO_PORTA,
	IO_PORTB
};

/*
 *  8ビット単位のAND/OR 読出し／書込み
 */
Inline void
sil_anb_mem(VP mem, VB data)
{
        VB tmp = sil_reb_mem(mem);
        tmp &= data;
        sil_wrb_mem(mem, tmp);
}

Inline void
sil_orb_mem(VP mem, VB data)
{
        VB tmp = sil_reb_mem(mem);
        tmp |= data;
        sil_wrb_mem(mem, tmp);
}


#endif /* _SIL_AKIH8_3069F_H_ */
