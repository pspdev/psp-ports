#ifndef __codegen_h__
#define __codegen_h__


/* GPR register set */

#define  R_zero 0
#define  R_at   1
#define  R_v0   2
#define  R_v1   3
#define  R_a0   4
#define  R_a1   5
#define  R_a2   6
#define  R_a3   7

#define  R_a4   8
#define  R_a5   9
#define  R_v6   10
#define  R_v7   11
#define  R_t0   12
#define  R_t1   13
#define  R_t2   14
#define  R_t3   15

#define  R_s0   16
#define  R_s1   17
#define  R_s2   18
#define  R_s3   19
#define  R_s4   20
#define  R_s5   21
#define  R_s6   22
#define  R_s7   23

#define  R_t8   24
#define  R_t9   25
#define  R_k0   26
#define  R_k1   27
#define  R_gp   28
#define  R_sp   29
#define  R_s8   30
#define  R_ra   31


/**
   The VFPU contains 32 registers (128bits each, 4x32bits).

   VFPU Registers can get accessed as Matrices, Vectors or single words.
   All registers are overlayed and enumerated in 3 digits (Matrix/Column/Row):

	M000 | C000   C010   C020   C030	M100 | C110   C110   C120   C130
	-----+--------------------------	-----+--------------------------
	R000 | S000   S010   S020   S030	R100 | S100   S110   S120   S130
	R001 | S001   S011   S021   S031	R101 | S101   S111   S121   S131
	R002 | S002   S012   S022   S032	R102 | S102   S112   S122   S132
	R003 | S003   S013   S023   S033	R103 | S103   S113   S123   S133

  same for matrices starting at M200 - M700.
  Subvectors can get addressed as singles/pairs/triplets/quads.
  Submatrices can get addressed 2x2 pairs, 3x3 triplets or 4x4 quads.

  So Q_C010 specifies the Quad Column starting at S010, T_C011 the triple Column starting at S011.
*/


/* VFPU registers, Quadword addressing */
#define  Q_C000  0	/* First digit specifies matrix, second the Column */
#define  Q_C010  1
#define  Q_C020  2
#define  Q_C030  3
#define  Q_C100  4
#define  Q_C110  5
#define  Q_C120  6
#define  Q_C130  7

#define  Q_C200  8
#define  Q_C210  9
#define  Q_C220  10
#define  Q_C230  11
#define  Q_C300  12
#define  Q_C310  13
#define  Q_C320  14
#define  Q_C330  15

#define  Q_C400  16
#define  Q_C410  17
#define  Q_C420  18
#define  Q_C430  19
#define  Q_C500  20
#define  Q_C510  21
#define  Q_C520  22
#define  Q_C530  23

#define  Q_C600  24
#define  Q_C610  25
#define  Q_C620  26
#define  Q_C630  27
#define  Q_C700  28
#define  Q_C710  29
#define  Q_C720  30
#define  Q_C730  31

#define  Q_R000  32	/* First Digit specifies Matrix, third the Row */
#define  Q_R001  33
#define  Q_R002  34
#define  Q_R003  35
#define  Q_R100  36
#define  Q_R101  37
#define  Q_R102  38
#define  Q_R103  39

#define  Q_R200  40
#define  Q_R201  41
#define  Q_R202  42
#define  Q_R203  43
#define  Q_R300  44
#define  Q_R301  45
#define  Q_R302  46
#define  Q_R303  47

#define  Q_R400  48
#define  Q_R401  49
#define  Q_R402  50
#define  Q_R403  51
#define  Q_R500  52
#define  Q_R501  53
#define  Q_R502  54
#define  Q_R503  55

#define  Q_R600  56
#define  Q_R601  57
#define  Q_R602  58
#define  Q_R603  59
#define  Q_R700  60
#define  Q_R701  61
#define  Q_R702  62
#define  Q_R703  63


/* VFPU registers, Triple addressing */
#define  T_C000  0x00    /* Triples in Columns, First Digit specifies Matrix, second Column, third Row */
#define  T_C010  0x01
#define  T_C020  0x02
#define  T_C030  0x03
#define  T_C100  0x04
#define  T_C110  0x05
#define  T_C120  0x06
#define  T_C130  0x07

#define  T_C200  0x08
#define  T_C210  0x09
#define  T_C220  0x0a
#define  T_C230  0x0b
#define  T_C300  0x0c
#define  T_C310  0x0d
#define  T_C320  0x0e
#define  T_C330  0x0f

#define  T_C400  0x10
#define  T_C410  0x11
#define  T_C420  0x12
#define  T_C430  0x13
#define  T_C500  0x14
#define  T_C510  0x15
#define  T_C520  0x16
#define  T_C530  0x17

#define  T_C600  0x18
#define  T_C610  0x19
#define  T_C620  0x1a
#define  T_C630  0x1b
#define  T_C700  0x1c
#define  T_C710  0x1d
#define  T_C720  0x1e
#define  T_C730  0x1f

#define  T_R000  0x20    /* Pairs in Rows, First Digit specifies Matrix, second Column, third Row */
#define  T_R001  0x21
#define  T_R002  0x22
#define  T_R003  0x23
#define  T_R100  0x24
#define  T_R101  0x25
#define  T_R102  0x26
#define  T_R103  0x27

#define  T_R200  0x28
#define  T_R201  0x29
#define  T_R202  0x2a
#define  T_R203  0x2b
#define  T_R300  0x2c
#define  T_R301  0x2d
#define  T_R302  0x2e
#define  T_R303  0x2f

#define  T_R400  0x30
#define  T_R401  0x31
#define  T_R402  0x32
#define  T_R403  0x33
#define  T_R500  0x34
#define  T_R501  0x35
#define  T_R502  0x36
#define  T_R503  0x37

#define  T_R600  0x38
#define  T_R601  0x39
#define  T_R602  0x3a
#define  T_R603  0x3b
#define  T_R700  0x3c
#define  T_R701  0x3d
#define  T_R702  0x3e
#define  T_R703  0x3f

#define  T_C001  0x40    /* Triples in Columns, First Digit specifies Matrix, second Column, third Row */
#define  T_C011  0x41
#define  T_C021  0x42
#define  T_C031  0x43
#define  T_C101  0x44
#define  T_C111  0x45
#define  T_C121  0x46
#define  T_C131  0x47

#define  T_C201  0x48
#define  T_C211  0x49
#define  T_C221  0x4a
#define  T_C231  0x4b
#define  T_C301  0x4c
#define  T_C311  0x4d
#define  T_C321  0x4e
#define  T_C331  0x4f

#define  T_C401  0x50
#define  T_C411  0x51
#define  T_C421  0x52
#define  T_C431  0x53
#define  T_C501  0x54
#define  T_C511  0x55
#define  T_C521  0x56
#define  T_C531  0x57

#define  T_C601  0x58
#define  T_C611  0x59
#define  T_C621  0x5a
#define  T_C631  0x5b
#define  T_C701  0x5c
#define  T_C711  0x5d
#define  T_C721  0x5e
#define  T_C731  0x5f

#define  T_R010  0x60    /* Pairs in Rows, First Digit specifies Matrix, second Column, third Row */
#define  T_R011  0x61
#define  T_R012  0x62
#define  T_R013  0x63
#define  T_R110  0x64
#define  T_R111  0x65
#define  T_R112  0x66
#define  T_R113  0x67

#define  T_R210  0x68
#define  T_R211  0x69
#define  T_R212  0x6a
#define  T_R213  0x6b
#define  T_R310  0x6c
#define  T_R311  0x6d
#define  T_R312  0x6e
#define  T_R313  0x6f

#define  T_R410  0x70
#define  T_R411  0x71
#define  T_R412  0x72
#define  T_R413  0x73
#define  T_R510  0x74
#define  T_R511  0x75
#define  T_R512  0x76
#define  T_R513  0x77

#define  T_R610  0x78
#define  T_R611  0x79
#define  T_R612  0x7a
#define  T_R613  0x7b
#define  T_R710  0x7c
#define  T_R711  0x7d
#define  T_R712  0x7e
#define  T_R713  0x7f


/* VFPU registers, Pair addressing */
#define  P_C000  0x00    /* Pairs in Columns, First Digit specifies Matrix, second Column, third Row */
#define  P_C010  0x01
#define  P_C020  0x02
#define  P_C030  0x03
#define  P_C100  0x04
#define  P_C110  0x05
#define  P_C120  0x06
#define  P_C130  0x07

#define  P_C200  0x08
#define  P_C210  0x09
#define  P_C220  0x0a
#define  P_C230  0x0b
#define  P_C300  0x0c
#define  P_C310  0x0d
#define  P_C320  0x0e
#define  P_C330  0x0f

#define  P_C400  0x10
#define  P_C410  0x11
#define  P_C420  0x12
#define  P_C430  0x13
#define  P_C500  0x14
#define  P_C510  0x15
#define  P_C520  0x16
#define  P_C530  0x17

#define  P_C600  0x18
#define  P_C610  0x19
#define  P_C620  0x1a
#define  P_C630  0x1b
#define  P_C700  0x1c
#define  P_C710  0x1d
#define  P_C720  0x1e
#define  P_C730  0x1f

#define  P_R000  0x20    /* Pairs in Rows, First Digit specifies Matrix, second Column, third Row */
#define  P_R001  0x21
#define  P_R002  0x22
#define  P_R003  0x23
#define  P_R100  0x24
#define  P_R101  0x25
#define  P_R102  0x26
#define  P_R103  0x27

#define  P_R200  0x28
#define  P_R201  0x29
#define  P_R202  0x2a
#define  P_R203  0x2b
#define  P_R300  0x2c
#define  P_R301  0x2d
#define  P_R302  0x2e
#define  P_R303  0x2f

#define  P_R400  0x30
#define  P_R401  0x31
#define  P_R402  0x32
#define  P_R403  0x33
#define  P_R500  0x34
#define  P_R501  0x35
#define  P_R502  0x36
#define  P_R503  0x37

#define  P_R600  0x38
#define  P_R601  0x39
#define  P_R602  0x3a
#define  P_R603  0x3b
#define  P_R700  0x3c
#define  P_R701  0x3d
#define  P_R702  0x3e
#define  P_R703  0x3f

#define  P_C002  0x40    /* Pairs in Columns, First Digit specifies Matrix, second Column, third Row */
#define  P_C012  0x41
#define  P_C022  0x42
#define  P_C032  0x43
#define  P_C102  0x44
#define  P_C112  0x45
#define  P_C122  0x46
#define  P_C132  0x47

#define  P_C202  0x48
#define  P_C212  0x49
#define  P_C222  0x4a
#define  P_C232  0x4b
#define  P_C302  0x4c
#define  P_C312  0x4d
#define  P_C322  0x4e
#define  P_C332  0x4f

#define  P_C402  0x50
#define  P_C412  0x51
#define  P_C422  0x52
#define  P_C432  0x53
#define  P_C502  0x54
#define  P_C512  0x55
#define  P_C522  0x56
#define  P_C532  0x57

#define  P_C602  0x58
#define  P_C612  0x59
#define  P_C622  0x5a
#define  P_C632  0x5b
#define  P_C702  0x5c
#define  P_C712  0x5d
#define  P_C722  0x5e
#define  P_C732  0x5f

#define  P_R020  0x60    /* Pairs in Rows, First Digit specifies Matrix, second Column, third Row */
#define  P_R021  0x61
#define  P_R022  0x62
#define  P_R023  0x63
#define  P_R120  0x64
#define  P_R121  0x65
#define  P_R122  0x66
#define  P_R123  0x67

#define  P_R220  0x68
#define  P_R221  0x69
#define  P_R222  0x6a
#define  P_R223  0x6b
#define  P_R320  0x6c
#define  P_R321  0x6d
#define  P_R322  0x6e
#define  P_R323  0x6f

#define  P_R420  0x70
#define  P_R421  0x71
#define  P_R422  0x72
#define  P_R423  0x73
#define  P_R520  0x74
#define  P_R521  0x75
#define  P_R522  0x76
#define  P_R523  0x77

#define  P_R620  0x78
#define  P_R621  0x79
#define  P_R622  0x7a
#define  P_R623  0x7b
#define  P_R720  0x7c
#define  P_R721  0x7d
#define  P_R722  0x7e
#define  P_R723  0x7f


/* VFPU registers, Single addressing */
#define  S_S000  0x00    /* First Digit specifies Matrix, second Column, third Row */
#define  S_S010  0x01
#define  S_S020  0x02
#define  S_S030  0x03
#define  S_S100  0x04
#define  S_S110  0x05
#define  S_S120  0x06
#define  S_S130  0x07

#define  S_S200  0x08
#define  S_S210  0x09
#define  S_S220  0x0a
#define  S_S230  0x0b
#define  S_S300  0x0c
#define  S_S310  0x0d
#define  S_S320  0x0e
#define  S_S330  0x0f

#define  S_S400  0x10
#define  S_S410  0x11
#define  S_S420  0x12
#define  S_S430  0x13
#define  S_S500  0x14
#define  S_S510  0x15
#define  S_S520  0x16
#define  S_S530  0x17

#define  S_S600  0x18
#define  S_S610  0x19
#define  S_S620  0x1a
#define  S_S630  0x1b
#define  S_S700  0x1c
#define  S_S710  0x1d
#define  S_S720  0x1e
#define  S_S730  0x1f

#define  S_S001  0x20
#define  S_S011  0x21
#define  S_S021  0x22
#define  S_S031  0x23
#define  S_S101  0x24
#define  S_S111  0x25
#define  S_S121  0x26
#define  S_S131  0x27

#define  S_S201  0x28
#define  S_S211  0x29
#define  S_S221  0x2a
#define  S_S231  0x2b
#define  S_S301  0x2c
#define  S_S311  0x2d
#define  S_S321  0x2e
#define  S_S331  0x2f

#define  S_S401  0x30
#define  S_S411  0x31
#define  S_S421  0x32
#define  S_S431  0x33
#define  S_S501  0x34
#define  S_S511  0x35
#define  S_S521  0x36
#define  S_S531  0x37

#define  S_S601  0x38
#define  S_S611  0x39
#define  S_S621  0x3a
#define  S_S631  0x3b
#define  S_S701  0x3c
#define  S_S711  0x3d
#define  S_S721  0x3e
#define  S_S731  0x3f

#define  S_S002  0x40
#define  S_S012  0x41
#define  S_S022  0x42
#define  S_S032  0x43
#define  S_S102  0x44
#define  S_S112  0x45
#define  S_S122  0x46
#define  S_S132  0x47

#define  S_S202  0x48
#define  S_S212  0x49
#define  S_S222  0x4a
#define  S_S232  0x4b
#define  S_S302  0x4c
#define  S_S312  0x4d
#define  S_S322  0x4e
#define  S_S332  0x4f

#define  S_S402  0x50
#define  S_S412  0x51
#define  S_S422  0x52
#define  S_S432  0x53
#define  S_S502  0x54
#define  S_S512  0x55
#define  S_S522  0x56
#define  S_S532  0x57

#define  S_S602  0x58
#define  S_S612  0x59
#define  S_S622  0x5a
#define  S_S632  0x5b
#define  S_S702  0x5c
#define  S_S712  0x5d
#define  S_S722  0x5e
#define  S_S732  0x5f

#define  S_S003  0x60
#define  S_S013  0x61
#define  S_S023  0x62
#define  S_S033  0x63
#define  S_S103  0x64
#define  S_S113  0x65
#define  S_S123  0x66
#define  S_S133  0x67

#define  S_S203  0x68
#define  S_S213  0x69
#define  S_S223  0x6a
#define  S_S233  0x6b
#define  S_S303  0x6c
#define  S_S313  0x6d
#define  S_S323  0x6e
#define  S_S333  0x6f

#define  S_S403  0x70
#define  S_S413  0x71
#define  S_S423  0x72
#define  S_S433  0x73
#define  S_S503  0x74
#define  S_S513  0x75
#define  S_S523  0x76
#define  S_S533  0x77

#define  S_S603  0x78
#define  S_S613  0x79
#define  S_S623  0x7a
#define  S_S633  0x7b
#define  S_S703  0x7c
#define  S_S713  0x7d
#define  S_S723  0x7e
#define  S_S733  0x7f


/* VFPU registers, 4x4 Matrix (Quad) addressing */
#define  Q_M000  0	/* First digit specifies Matrix */
#define  Q_M100  4
#define  Q_M200  8
#define  Q_M300  12
#define  Q_M400  16
#define  Q_M500  20
#define  Q_M600  24
#define  Q_M700  28

#define  Q_E000  32
#define  Q_E100  36
#define  Q_E200  40
#define  Q_E300  44
#define  Q_E400  48
#define  Q_E500  52
#define  Q_E600  56
#define  Q_E700  60


/*********** MIPS instruction set *************/

/*
+-------------+------------+---------+---------------------------------------+
|31         26|25        21|20     16|15                                   0 |
+-------------+------------+---------+---------------------------------------+
| opcode 0x8c |  base[4-0] | rt[4-0] |             offset[15-0]              |
+-------------+------------+---------+---------------------------------------+

  LoadWord Relative to Address in General Purpose Register

    lw %rt, offset(%base)

	%rt:	GPR Destination Register (0...31)
	%base:	GPR, specifies Source Address Base
	offset:	signed Offset added to Source Address Base

    %rt <- word_at_address (offset + %base)
*/
#define lw(rt,offset,base) \
	(0x8c000000 | ((base) << 21) | ((rt) << 16) | ((offset) & 0xffff))


/*
+-------------+------------+---------+---------------------------------------+
|31         26|25        21|20     16|15                                   0 |
+-------------+------------+---------+---------------------------------------+
| opcode 0xac |  base[4-0] | rt[4-0] |             offset[15-0]              |
+-------------+------------+---------+---------------------------------------+

  StoreWord Relative to Address in General Purpose Register

    sw %rt, offset(%base)

	%rt:	GPR Register to Store (0...31)
	%base:	GPR, specifies Source Address Base
	offset:	signed Offset added to Source Address Base

    word_at_address (offset + %base) <- %rt
*/
#define sw(rt,offset,base) \
	(0xac000000 | ((base) << 21) | ((rt) << 16) | ((offset) & 0xffff))


/*
+-------------+------------+---------+---------------------------------------+
|31         26|25        21|20     16|15                                   0 |
+-------------+------------+---------+---------------------------------------+
| opcode 0x42 |   rs[4-0]  | rt[4-0] |              immediate                |
+-------------+------------+---------+---------------------------------------+

  Add Immediate Unsigned Word

    addiu %rt, %rs, immediate

	%rt:		GPR Destination Register (0...31)
	%rs:		GPR Source Register (0...31)
	immediate:	value added to Source Register

    %rt <- %rs + sign_extended(immediate)
*/
#define addiu(rt,rs,immediate) \
	(0x24000000 | ((rs) << 21) | ((rt) << 16) | ((immediate) & 0xffff))


/*********** VFPU instruction set *************/

/*
+-------------+-----------+---------+----------------------------+-----+-----+
|31         26|25       21|20     16|15                        2 |  1  |  0  |
+-------------+-----------+---------+----------------------------+-----+-----+
| opcode 0xd8 | base[4-0] | rd[4-0] |         offset[15-2]       |  0  |rd[5]|
+-------------+-----------+---------+----------------------------+-----+-----+

  LoadVector.Single/Quadword Relative to Address in General Purpose Register
  Final Address needs to be 16-byte aligned.

    lv.s %vfpu_rd, offset(%base)
    lv.q %vfpu_rd, offset(%base)

	%vfpu_rd:	VFPU Vector Destination Register ([s|q]reg)
	%base:		GPR, specifies Source Address Base
	offset:		signed Offset added to Source Address Base

    vfpu_regs[%vfpu_rd] <- vector_at_address (offset + %gpr)
*/
#define lv_s(vfpu_rd,offset,base)				\
	(0xc8000000 |						\
	 ((base) << 21) |					\
	 (((vfpu_rd) & 0x1f) << 16) | ((vfpu_rd) >> 5) |	\
	 ((offset) & 0xfffc))

#define lv_q(vfpu_rd,offset,base)				\
	(0xd8000000 |						\
	 ((base) << 21) |					\
	 (((vfpu_rd) & 0x1f) << 16) | ((vfpu_rd) >> 5) |	\
	 ((offset) & 0xfffc))


/*
+-------------+-----------+---------+----------------------------+-----+-----+
|31         26|25       21|20     16|15                        2 |  1  |  0  |
+-------------+-----------+---------+----------------------------+-----+-----+
| opcode 0xd4 | base[4-0] | rd[4-0] |         offset[15-2]       |  0  |rd[5]|
| opcode 0xd4 | base[4-0] | rd[4-0] |         offset[15-2]       |  1  |rd[5]|
+-------------+-----------+---------+----------------------------+-----+-----+

  LoadVectorLeft/Right.Quadword Relative to Address in General Purpose Register
  Load unaligned Quadwords. lvl.q contains address of lower part, lvr.q the high one.

    lvl.q %vfpu_rd, offset(%base)
    lvr.q %vfpu_rd, offset(%base)

	%vfpu_rd:	VFPU Vector Destination Register ([s|q]reg)
	%base:		GPR, specifies Source Address Base
	offset:		signed Offset added to Source Address Base

    vfpu_regs[%vfpu_rd] <- vector_at_address (offset + %gpr)
*/
#define lvl_q(vfpu_rd,offset,base)				\
	(0xd4000000 |						\
	 ((base) << 21) |					\
	 (((vfpu_rd) & 0x1f) << 16) | ((vfpu_rd) >> 5) |	\
	 ((offset) & 0xfffc))

#define lvr_q(vfpu_rd,offset,base)				\
	(0xd4000002 |						\
	 ((base) << 21) |					\
	 (((vfpu_rd) & 0x1f) << 16) | ((vfpu_rd) >> 5) |	\
	 ((offset) & 0xfffc))


/*
+-------------+-----------+---------+----------------------------+-----+-----+
|31         26|25       21|20     16|15                        2 |  1  |  0  |
+-------------+-----------+---------+----------------------------+-----+-----+
| opcode 0xf8 | base[4-0] | rs[4-0] |         offset[15-2]       | c_p |rs[5]|
+-------------+-----------+---------+----------------------------+-----+-----+

  StoreVector.Quadword Relative to Address in General Purpose Register
  Final Address needs to be 16-byte aligned.

    sv.s %vfpu_rs, offset(%base), cache_policy
    sv.q %vfpu_rs, offset(%base), cache_policy

	%vfpu_rs:	VFPU Vector Register ([s|q]reg)
	%base:		specifies Source Address Base
	offset:		signed Offset added to Source Address Base
	cache_policy:	0 = write-through, 1 = write-back

    vector_at_address (offset + %gpr) <- vfpu_regs[%vfpu_rs]
*/
#define sv_s(vfpu_rs,offset,base,cache_policy)			\
	(0xe8000000 |						\
	 ((base) << 21) |					\
	 (((vfpu_rs) & 0x1f) << 16) | ((vfpu_rs) >> 5) |	\
	 ((offset) & 0xfffc) |					\
	 ((cache_policy) << 1))

#define sv_q(vfpu_rs,offset,base,cache_policy)			\
	(0xf8000000 |						\
	 ((base) << 21) |					\
	 (((vfpu_rs) & 0x1f) << 16) | ((vfpu_rs) >> 5) |	\
	 ((offset) & 0xfffc) |					\
	 ((cache_policy) << 1))


/*
+-------------+-----------+---------+----------------------------+-----+-----+
|31         26|25       21|20     16|15                        2 |  1  |  0  |
+-------------+-----------+---------+----------------------------+-----+-----+
| opcode 0xf4 | base[4-0] | rs[4-0] |         offset[15-2]       |  0  |rs[5]|
| opcode 0xf4 | base[4-0] | rs[4-0] |         offset[15-2]       |  1  |rs[5]|
+-------------+-----------+---------+----------------------------+-----+-----+

  StoreVectorLeft/Right.Quadword Relative to Address in General Purpose Register
  Store unaligned Quadwords. svl.q contains address of lower part, svr.q the high one.

    svl.q %vfpu_rs, offset(%base), cache_policy
    svr.q %vfpu_rs, offset(%base), cache_policy

	%vfpu_rs:	VFPU Vector Register ([s|q]reg)
	%base:		specifies Source Address Base
	offset:		signed Offset added to Source Address Base

    vector_at_address (offset + %gpr) <- vfpu_regs[%vfpu_rs]
*/
#define svl_q(vfpu_rs,offset,base)				\
	(0xf4000000 |						\
	 ((base) << 21) |					\
	 (((vfpu_rs) & 0x1f) << 16) | ((vfpu_rs) >> 5) |	\
	 ((offset) & 0xfffc))

#define svr_q(vfpu_rs,offset,base)				\
	(0xf4000002 |						\
	 ((base) << 21) |					\
	 (((vfpu_rs) & 0x1f) << 16) | ((vfpu_rs) >> 5) |	\
	 ((offset) & 0xfffc))


/*
+------------------------------------------+--------------+---+--------------+
|31                                     15 | 14         8 | 7 | 6          0 |
+------------------------------------------+--------------+---+--------------+
|              opcode 0xd0000000           | vfpu_rs[6-0] | 1 | vfpu_rd[6-0] |
+------------------------------------------+--------------+---+--------------+

  Move Vector

    vmov.s %vfpu_rd, %vfpu_rs    ; Move Single
    vmov.p %vfpu_rd, %vfpu_rs    ; Move Pair
    vmov.t %vfpu_rd, %vfpu_rs    ; Move Triple
    vmov.q %vfpu_rd, %vfpu_rs    ; Move Quad

	%vfpu_rd:	VFPU Vector Destination Register ([s|p|t|q]reg 0..127)
	%vfpu_rs:	VFPU Vector Source Register ([s|p|t|q]reg 0..127)

    vfpu_regs[%vfpu_rd] <- vfpu_regs[%vfpu_rs]
*/
#define vmov_s(vfpu_rd,vfpu_rs)  (0xd0000000 | ((vfpu_rs) << 8) | (vfpu_rd))
#define vmov_p(vfpu_rd,vfpu_rs)  (0xd0000080 | ((vfpu_rs) << 8) | (vfpu_rd))
#define vmov_t(vfpu_rd,vfpu_rs)  (0xd0008000 | ((vfpu_rs) << 8) | (vfpu_rd))
#define vmov_q(vfpu_rd,vfpu_rs)  (0xd0008080 | ((vfpu_rs) << 8) | (vfpu_rd))


/*
+------------------------------------+---------------------------------------+
|31              23|22             16|15                                   0 |
+------------------+-----------------+---------------------------------------+
|    opcode 0xdf   |  vfpu_rd[6-0]   |              immediate                |
+------------------+-----------------+---------------------------------------+

  Set Single Vector Component to Immediate Integer

    vimm.s %vfpu_rd, immediate   ; Set Vector Component to immediate Integer

	immediate:	Integer, converted to Float before loading into sreg
	%vfpu_rd:	VFPU Vector Destination Register (sreg 0..127)

    vfpu_regs[%vfpu_rd] <- (float) immediate
*/
#define viim_s(vfpu_rd,immediate)  (0xdf000000 | ((vfpu_rd) << 16) | ((immediate) & 0xffff))


/*
+-------------------------------------------------------------+--------------+
|31                                                         7 | 6         0  |
+-------------------------------------------------------------+--------------+
|              opcode 0xd0060000                              | vfpu_rd[6-0] |
+-------------------------------------------------------------+--------------+

  SetVectorZero.Single/Pair/Triple/Quad

    vzero.s %vfpu_rd	; Set 1 Vector Component to 0.0f
    vzero.p %vfpu_rd	; Set 2 Vector Components to 0.0f
    vzero.t %vfpu_rd	; Set 3 Vector Components to 0.0f
    vzero.q %vfpu_rd	; Set 4 Vector Components to 0.0f

	%vfpu_rd:	VFPU Vector Destination Register ([s|p|t|q]reg 0..127)

    vfpu_regs[%vfpu_rd] <- 0.0f
*/
#define vzero_s(vfpu_rd)  (0xd0060000 | (vfpu_rd))
#define vzero_p(vfpu_rd)  (0xd0060080 | (vfpu_rd))
#define vzero_t(vfpu_rd)  (0xd0068000 | (vfpu_rd))
#define vzero_q(vfpu_rd)  (0xd0068080 | (vfpu_rd))


/*
+-------------------------------------------------------------+--------------+
|31                                                         7 | 6         0  |
+-------------------------------------------------------------+--------------+
|              opcode 0xd0070000                              | vfpu_rd[6-0] |
+-------------------------------------------------------------+--------------+

  SetVectorOne.Single/Pair/Triple/Quad

    vone.s %vfpu_rd	; Set 1 Vector Component to 1.0f
    vone.p %vfpu_rd	; Set 2 Vector Components to 1.0f
    vone.t %vfpu_rd	; Set 3 Vector Components to 1.0f
    vone.q %vfpu_rd	; Set 4 Vector Components to 1.0f

	%vfpu_rd:	VFPU Vector Destination Register ([s|p|t|q]reg 0..127)

    vfpu_regs[%vfpu_rd] <- 0.0f
*/
#define vone_s(vfpu_rd)  (0xd0070000 | (vfpu_rd))
#define vone_p(vfpu_rd)  (0xd0070080 | (vfpu_rd))
#define vone_t(vfpu_rd)  (0xd0078000 | (vfpu_rd))
#define vone_q(vfpu_rd)  (0xd0078080 | (vfpu_rd))


/*
+------------------------+------------------+----+--------+---+--------------+ 
|31                   21 | 20            16 | 15 | 14   8 | 7 | 6         0  | 
+------------------------+------------------+----+--------+---+--------------+ 
| opcode 0xd06 (s)       | constant (0-31)  |  0 |   0    | 0 | vfpu_rd[6-0] | 
| opcode 0xd06 (p)       | constant (0-31)  |  0 |   0    | 1 | vfpu_rd[6-0] | 
| opcode 0xd06 (t)       | constant (0-31)  |  1 |   0    | 0 | vfpu_rd[6-0] | 
| opcode 0xd06 (q)       | constant (0-31)  |  1 |   0    | 1 | vfpu_rd[6-0] | 
+------------------------+------------------+----+--------+---+--------------+ 

   StoreConstant.Single/Pair/Triple/Quad 

   vcst.s %vfpu_rd, %a ; store constant into single 
   vcst.p %vfpu_rd, %a ; store constant into pair 
   vcst.t %vfpu_rd, %a ; store constant into triple 
   vcst.q %vfpu_rd, %a ; store constant into quad 

      %vfpu_rd:   VFPU Vector Destination Register ([s|p|t|q]reg 0..127) 
      %a:         VFPU Constant ID     Value 
                  ================     ========================================== 
                  0  = ZERO            0 
                  1  = HUGE            340282346638528859811704183484516925440.0 
                  2  = sqrt(2)         1.41421 
                  3  = 1.0 / sqrt(2.0) 0.70711 
                  4  = 2.0 / sqrt(PI)  1.12838 
                  5  = 2.0 / PI        0.63662 
                  6  = 1.0 / PI        0.31831 
                  7  = PI/4.0          0.78540 
                  8  = PI/2.0          1.57080 
                  9  = PI              3.14159 
                  10 = E               2,71828 
                  11 = log2(E)         1.44270 
                  12 = log10(E)        0.43429 
                  13 = log2(2.0)       0.69315 
                  14 = log2(10.0)      2.30259 
                  15 = 2 * PI          6.28319 
                  16 = PI / 6.0        0.52360 
                  17 = log10(2)        0.30103 
                  18 = log2(10.0)      3.32193 
                  19 = sqrt(3.0) / 2.0 0.86603 
                  20-31 = n/a          0 

   vfpu_regs[%vfpu_rd] <- constants[%a]   ; one of the VFPU_XXX constants below
*/
#define vcst_s(vfpu_rd, a) (0xd0600000 | ((a) << 16) | (vfpu_rd)) 
#define vcst_p(vfpu_rd, a) (0xd0600080 | ((a) << 16) | (vfpu_rd)) 
#define vcst_t(vfpu_rd, a) (0xd0608000 | ((a) << 16) | (vfpu_rd)) 
#define vcst_q(vfpu_rd, a) (0xd0608080 | ((a) << 16) | (vfpu_rd)) 

#define  VFPU_ZERO     0
#define  VFPU_HUGE     1
#define  VFPU_SQRT2    2
#define  VFPU_SQRT1_2  3
#define  VFPU_2_SQRTPI 4
#define  VFPU_2_PI     5
#define  VFPU_1_PI     6
#define  VFPU_PI_4     7
#define  VFPU_PI_2     8
#define  VFPU_PI       9
#define  VFPU_E        10
#define  VFPU_LOG2E    11
#define  VFPU_LOG10E   12
#define  VFPU_LN2      13
#define  VFPU_LN10     14
#define  VFPU_2PI      15
#define  VFPU_PI_6     16
#define  VFPU_LOG10TWO 17
#define  VFPU_LOG2TEN  18
#define  VFPU_SQRT3_2  19


/*
+----------------------+--------------+----+--------------+---+--------------+
|31                 23 | 22        16 | 15 | 14         8 | 7 | 6         0  |
+----------------------+--------------+----+--------------+---+--------------+
|  opcode 0x60000000   | vfpu_rt[6-0] |    | vfpu_rs[6-0] |   | vfpu_rd[6-0] |
+----------------------+--------------+----+--------------+---+--------------+

  VectorAdd.Single/Pair/Triple/Quad

    vadd.s %vfpu_rd, %vfpu_rs, %vfpu_rt   ; Add Single
    vadd.p %vfpu_rd, %vfpu_rs, %vfpu_rt   ; Add Pair
    vadd.t %vfpu_rd, %vfpu_rs, %vfpu_rt   ; Add Triple
    vadd.q %vfpu_rd, %vfpu_rs, %vfpu_rt   ; Add Quad

	%vfpu_rt:	VFPU Vector Source Register ([s|p|t|q]reg 0..127)
	%vfpu_rs:	VFPU Vector Source Register ([s|p|t|q]reg 0..127)
	%vfpu_rd:	VFPU Vector Destination Register ([s|p|t|q]reg 0..127)

    vfpu_regs[%vfpu_rd] <- vfpu_regs[%vfpu_rs] + vfpu_regs[%vfpu_rt]
*/
#define vadd_s(vfpu_rd,vfpu_rs,vfpu_rt)  (0x60000000 | ((vfpu_rt) << 16) | ((vfpu_rs) << 8) | (vfpu_rd))
#define vadd_p(vfpu_rd,vfpu_rs,vfpu_rt)  (0x60000080 | ((vfpu_rt) << 16) | ((vfpu_rs) << 8) | (vfpu_rd))
#define vadd_t(vfpu_rd,vfpu_rs,vfpu_rt)  (0x60008000 | ((vfpu_rt) << 16) | ((vfpu_rs) << 8) | (vfpu_rd))
#define vadd_q(vfpu_rd,vfpu_rs,vfpu_rt)  (0x60008080 | ((vfpu_rt) << 16) | ((vfpu_rs) << 8) | (vfpu_rd))


/* 
+----------------------+--------------+----+--------------+---+--------------+ 
|31                 23 | 22        16 | 15 | 14         8 | 7 | 6         0  | 
+----------------------+--------------+----+--------------+---+--------------+ 
|  opcode 0x608 (s)    | vfpu_rt[6-0] | 0  | vfpu_rs[6-0] | 0 | vfpu_rd[6-0] | 
|  opcode 0x608 (p)    | vfpu_rt[6-0] | 0  | vfpu_rs[6-0] | 1 | vfpu_rd[6-0] | 
|  opcode 0x608 (t)    | vfpu_rt[6-0] | 1  | vfpu_rs[6-0] | 0 | vfpu_rd[6-0] | 
|  opcode 0x608 (q)    | vfpu_rt[6-0] | 1  | vfpu_rs[6-0] | 1 | vfpu_rd[6-0] | 
+----------------------+--------------+----+--------------+---+--------------+ 

   VectorSub.Single/Pair/Triple/Quad 

    vsub.s %vfpu_rd, %vfpu_rs, %vfpu_rt   ; Sub Single 
    vsub.p %vfpu_rd, %vfpu_rs, %vfpu_rt   ; Sub Pair 
    vsub.t %vfpu_rd, %vfpu_rs, %vfpu_rt   ; Sub Triple 
    vsub.q %vfpu_rd, %vfpu_rs, %vfpu_rt   ; Sub Quad 

        %vfpu_rt:   VFPU Vector Source Register ([s|p|t|q]reg 0..127) 
        %vfpu_rs:   VFPU Vector Source Register ([s|p|t|q]reg 0..127) 
        %vfpu_rd:   VFPU Vector Destination Register ([s|p|t|q]reg 0..127) 

    vfpu_regs[%vfpu_rd] <- vfpu_regs[%vfpu_rs] - vfpu_regs[%vfpu_rt] 
*/ 
#define vsub_s(vfpu_rd,vfpu_rs,vfpu_rt)  (0x60800000 | ((vfpu_rt) << 16) | ((vfpu_rs) << 8) | (vfpu_rd)) 
#define vsub_p(vfpu_rd,vfpu_rs,vfpu_rt)  (0x60800080 | ((vfpu_rt) << 16) | ((vfpu_rs) << 8) | (vfpu_rd)) 
#define vsub_t(vfpu_rd,vfpu_rs,vfpu_rt)  (0x60808000 | ((vfpu_rt) << 16) | ((vfpu_rs) << 8) | (vfpu_rd)) 
#define vsub_q(vfpu_rd,vfpu_rs,vfpu_rt)  (0x60808080 | ((vfpu_rt) << 16) | ((vfpu_rs) << 8) | (vfpu_rd)) 


/* 
+----------------------+--------------+----+--------------+---+--------------+ 
|31                 23 | 22        16 | 15 | 14         8 | 7 | 6         0  | 
+----------------------+--------------+----+--------------+---+--------------+ 
|  opcode 0x638 (s)    | vfpu_rt[6-0] | 0  | vfpu_rs[6-0] | 0 | vfpu_rd[6-0] | 
|  opcode 0x638 (p)    | vfpu_rt[6-0] | 0  | vfpu_rs[6-0] | 1 | vfpu_rd[6-0] | 
|  opcode 0x638 (t)    | vfpu_rt[6-0] | 1  | vfpu_rs[6-0] | 0 | vfpu_rd[6-0] | 
|  opcode 0x638 (q)    | vfpu_rt[6-0] | 1  | vfpu_rs[6-0] | 1 | vfpu_rd[6-0] | 
+----------------------+--------------+----+--------------+---+--------------+ 

   VectorDiv.Single/Pair/Triple/Quad 

    vdiv.s %vfpu_rd, %vfpu_rs, %vfpu_rt   ; Sub Single 
    vdiv.p %vfpu_rd, %vfpu_rs, %vfpu_rt   ; Sub Pair 
    vdiv.t %vfpu_rd, %vfpu_rs, %vfpu_rt   ; Sub Triple 
    vdiv.q %vfpu_rd, %vfpu_rs, %vfpu_rt   ; Sub Quad 

        %vfpu_rt:   VFPU Vector Source Register ([s|p|t|q]reg 0..127) 
        %vfpu_rs:   VFPU Vector Source Register ([s|p|t|q]reg 0..127) 
        %vfpu_rd:   VFPU Vector Destination Register ([s|p|t|q]reg 0..127) 

    vfpu_regs[%vfpu_rd] <- vfpu_regs[%vfpu_rs] / vfpu_regs[%vfpu_rt] 
*/ 
#define vdiv_s(vfpu_rd,vfpu_rs,vfpu_rt)  (0x63800000 | ((vfpu_rt) << 16) | ((vfpu_rs) << 8) | (vfpu_rd)) 
#define vdiv_p(vfpu_rd,vfpu_rs,vfpu_rt)  (0x63800080 | ((vfpu_rt) << 16) | ((vfpu_rs) << 8) | (vfpu_rd)) 
#define vdiv_t(vfpu_rd,vfpu_rs,vfpu_rt)  (0x63808000 | ((vfpu_rt) << 16) | ((vfpu_rs) << 8) | (vfpu_rd)) 
#define vdiv_q(vfpu_rd,vfpu_rs,vfpu_rt)  (0x63808080 | ((vfpu_rt) << 16) | ((vfpu_rs) << 8) | (vfpu_rd)) 


/* 
+----------------------+--------------+----+--------------+---+--------------+ 
|31                 23 | 22        16 | 15 | 14         8 | 7 | 6         0  | 
+----------------------+--------------+----+--------------+---+--------------+ 
|  opcode 0x640 (s)    | vfpu_rt[6-0] | 0  | vfpu_rs[6-0] | 0 | vfpu_rd[6-0] | 
|  opcode 0x640 (p)    | vfpu_rt[6-0] | 0  | vfpu_rs[6-0] | 1 | vfpu_rd[6-0] | 
|  opcode 0x640 (t)    | vfpu_rt[6-0] | 1  | vfpu_rs[6-0] | 0 | vfpu_rd[6-0] | 
|  opcode 0x640 (q)    | vfpu_rt[6-0] | 1  | vfpu_rs[6-0] | 1 | vfpu_rd[6-0] | 
+----------------------+--------------+----+--------------+---+--------------+ 

   VectorMul.Single/Pair/Triple/Quad 

    vmul.s %vfpu_rd, %vfpu_rs, %vfpu_rt   ; Sub Single 
    vmul.p %vfpu_rd, %vfpu_rs, %vfpu_rt   ; Sub Pair 
    vmul.t %vfpu_rd, %vfpu_rs, %vfpu_rt   ; Sub Triple 
    vmul.q %vfpu_rd, %vfpu_rs, %vfpu_rt   ; Sub Quad 

        %vfpu_rt:   VFPU Vector Source Register ([s|p|t|q]reg 0..127) 
        %vfpu_rs:   VFPU Vector Source Register ([s|p|t|q]reg 0..127) 
        %vfpu_rd:   VFPU Vector Destination Register ([s|p|t|q]reg 0..127) 

    vfpu_regs[%vfpu_rd] <- vfpu_regs[%vfpu_rs] * vfpu_regs[%vfpu_rt] 
*/ 
#define vmul_s(vfpu_rd,vfpu_rs,vfpu_rt)  (0x64000000 | ((vfpu_rt) << 16) | ((vfpu_rs) << 8) | (vfpu_rd)) 
#define vmul_p(vfpu_rd,vfpu_rs,vfpu_rt)  (0x64000080 | ((vfpu_rt) << 16) | ((vfpu_rs) << 8) | (vfpu_rd)) 
#define vmul_t(vfpu_rd,vfpu_rs,vfpu_rt)  (0x64008000 | ((vfpu_rt) << 16) | ((vfpu_rs) << 8) | (vfpu_rd)) 
#define vmul_q(vfpu_rd,vfpu_rs,vfpu_rt)  (0x64008080 | ((vfpu_rt) << 16) | ((vfpu_rs) << 8) | (vfpu_rd)) 


/*
+----------------------+--------------+----+--------------+---+--------------+
|31                 23 | 22        16 | 15 | 14         8 | 7 | 6         0  |
+----------------------+--------------+----+--------------+---+--------------+
|  opcode 0x65008080   | vfpu_rt[6-0] |    | vfpu_rs[6-0] |   | vfpu_rd[6-0] |
+----------------------+--------------+----+--------------+---+--------------+

  VectorScale.Pair/Triple/Quad

    vscl.p %vfpu_rd, %vfpu_rs, %vfpu_rt   ; Scale Pair by %vfpu_rt
    vscl.t %vfpu_rd, %vfpu_rs, %vfpu_rt   ; Scale Triple by %vfpu_rt
    vscl.q %vfpu_rd, %vfpu_rs, %vfpu_rt   ; Scale Quad by %vfpu_rt

	%vfpu_rt:	VFPU Scalar Source Register (sreg 0..127)
	%vfpu_rs:	VFPU Vector Source Register ([p|t|q]reg 0..127)
	%vfpu_rd:	VFPU Vector Destination Register ([s|p|t|q]reg 0..127)

    vfpu_regs[%vfpu_rd] <- vfpu_regs[%vfpu_rs] * vfpu_reg[%vfpu_rt]
*/
#define vscl_p(vfpu_rd,vfpu_rs,vfpu_rt)  (0x65000080 | ((vfpu_rt) << 16) | ((vfpu_rs) << 8) | (vfpu_rd))
#define vscl_t(vfpu_rd,vfpu_rs,vfpu_rt)  (0x65008000 | ((vfpu_rt) << 16) | ((vfpu_rs) << 8) | (vfpu_rd))
#define vscl_q(vfpu_rd,vfpu_rs,vfpu_rt)  (0x65008080 | ((vfpu_rt) << 16) | ((vfpu_rs) << 8) | (vfpu_rd))


/* 
+-----------------------------------------+--+--------------+-+--------------+ 
|31                                    16 |15| 14         8 |7| 6         0  | 
+-----------------------------------------+--+--------------+-+--------------+ 
| opcode 0xd0100000 (s)                   | 0| vfpu_rs[6-0] |0| vfpu_rd[6-0] | 
| opcode 0xd0100080 (p)                   | 0| vfpu_rs[6-0] |1| vfpu_rd[6-0] | 
| opcode 0xd0108000 (t)                   | 1| vfpu_rs[6-0] |0| vfpu_rd[6-0] | 
| opcode 0xd0108080 (q)                   | 1| vfpu_rs[6-0] |1| vfpu_rd[6-0] | 
+-----------------------------------------+--+--------------+-+--------------+ 

  Reciprocal.Single/Pair/Triple/Quad 

    vrcp.s  %vfpu_rd, %vfpu_rs   ; calculate reciprocal (1/z) on single 
    vrcp.p  %vfpu_rd, %vfpu_rs   ; calculate reciprocal (1/z) on pair 
    vrcp.t  %vfpu_rd, %vfpu_rs   ; calculate reciprocal (1/z) on triple 
    vrcp.q  %vfpu_rd, %vfpu_rs   ; calculate reciprocal (1/z) on quad 

	%vfpu_rd:   VFPU Vector Target Register ([s|p|t|q]reg 0..127) 
	%vfpu_rs:   VFPU Vector Source Register ([s|p|t|q]reg 0..127) 

   vfpu_regs[%vfpu_rd] <- 1.0 / vfpu_regs[%vfpu_rs] 
*/ 

#define vrcp_s(vfpu_rd, vfpu_rs) (0xd0100000 | (vfpu_rs << 8) | (vfpu_rd)) 
#define vrcp_p(vfpu_rd, vfpu_rs) (0xd0100080 | (vfpu_rs << 8) | (vfpu_rd)) 
#define vrcp_t(vfpu_rd, vfpu_rs) (0xd0108000 | (vfpu_rs << 8) | (vfpu_rd)) 
#define vrcp_q(vfpu_rd, vfpu_rs) (0xd0108080 | (vfpu_rs << 8) | (vfpu_rd)) 


/* 
+-----------------------------------------+--+--------------+-+--------------+ 
|31                                    16 |15| 14         8 |7| 6         0  | 
+-----------------------------------------+--+--------------+-+--------------+ 
| opcode 0xd0140000 (s)                   | 0| vfpu_rs[6-0] |0| vfpu_rd[6-0] | 
| opcode 0xd0140080 (p)                   | 0| vfpu_rs[6-0] |1| vfpu_rd[6-0] | 
| opcode 0xd0148000 (t)                   | 1| vfpu_rs[6-0] |0| vfpu_rd[6-0] | 
| opcode 0xd0148080 (q)                   | 1| vfpu_rs[6-0] |1| vfpu_rd[6-0] | 
+-----------------------------------------+--+--------------+-+--------------+ 

  Exp2.Single/Pair/Triple/Quad (calculate 2 raised to the specified real number) 

    vexp2.s  %vfpu_rd, %vfpu_rs   ; calculate 2^y 
    vexp2.p  %vfpu_rd, %vfpu_rs   ; calculate 2^y 
    vexp2.t  %vfpu_rd, %vfpu_rs   ; calculate 2^y 
    vexp2.q  %vfpu_rd, %vfpu_rs   ; calculate 2^y 

	%vfpu_rd:   VFPU Vector Target Register ([s|p|t|q]reg 0..127) 
	%vfpu_rs:   VFPU Vector Source Register ([s|p|t|q]reg 0..127) 

   vfpu_regs[%vfpu_rd] <- 2^(vfpu_regs[%vfpu_rs]) 
*/ 

#define vexp2_s(vfpu_rd, vfpu_rs) (0xd0140000 | (vfpu_rs << 8) | (vfpu_rd)) 
#define vexp2_p(vfpu_rd, vfpu_rs) (0xd0140080 | (vfpu_rs << 8) | (vfpu_rd)) 
#define vexp2_t(vfpu_rd, vfpu_rs) (0xd0148000 | (vfpu_rs << 8) | (vfpu_rd)) 
#define vexp2_q(vfpu_rd, vfpu_rs) (0xd0148080 | (vfpu_rs << 8) | (vfpu_rd)) 


/* 
+-----------------------------------------+--+--------------+-+--------------+ 
|31                                    16 |15| 14         8 |7| 6         0  | 
+-----------------------------------------+--+--------------+-+--------------+ 
| opcode 0xd0150000 (s)                   | 0| vfpu_rs[6-0] |0| vfpu_rd[6-0] | 
| opcode 0xd0150080 (p)                   | 0| vfpu_rs[6-0] |1| vfpu_rd[6-0] | 
| opcode 0xd0158000 (t)                   | 1| vfpu_rs[6-0] |0| vfpu_rd[6-0] | 
| opcode 0xd0158080 (q)                   | 1| vfpu_rs[6-0] |1| vfpu_rd[6-0] | 
+-----------------------------------------+--+--------------+-+--------------+ 

  Log2.Single/Pair/Triple/Quad (calculate logarithm base 2 of the specified real number) 

    vlog2.s  %vfpu_rd, %vfpu_rs 
    vlog2.p  %vfpu_rd, %vfpu_rs 
    vlog2.t  %vfpu_rd, %vfpu_rs 
    vlog2.q  %vfpu_rd, %vfpu_rs 

	%vfpu_rd:   VFPU Vector Target Register ([s|p|t|q]reg 0..127) 
	%vfpu_rs:   VFPU Vector Source Register ([s|p|t|q]reg 0..127) 

    vfpu_regs[%vfpu_rd] <- log2(vfpu_regs[%vfpu_rs]) 
*/ 

#define vlog2_s(vfpu_rd, vfpu_rs) (0xd0150000 | (vfpu_rs << 8) | (vfpu_rd)) 
#define vlog2_p(vfpu_rd, vfpu_rs) (0xd0150080 | (vfpu_rs << 8) | (vfpu_rd)) 
#define vlog2_t(vfpu_rd, vfpu_rs) (0xd0158000 | (vfpu_rs << 8) | (vfpu_rd)) 
#define vlog2_q(vfpu_rd, vfpu_rs) (0xd0158080 | (vfpu_rs << 8) | (vfpu_rd)) 

/* 
+-----------------------------------------+--+--------------+-+--------------+ 
|31                                    16 |15| 14         8 |7| 6         0  | 
+-----------------------------------------+--+--------------+-+--------------+ 
| opcode 0xd0160000 (s)                   | 0| vfpu_rs[6-0] |0| vfpu_rd[6-0] | 
| opcode 0xd0160080 (p)                   | 0| vfpu_rs[6-0] |1| vfpu_rd[6-0] | 
| opcode 0xd0168000 (t)                   | 1| vfpu_rs[6-0] |0| vfpu_rd[6-0] | 
| opcode 0xd0168080 (q)                   | 1| vfpu_rs[6-0] |1| vfpu_rd[6-0] | 
+-----------------------------------------+--+--------------+-+--------------+ 

  SquareRoot.Single/Pair/Triple/Quad 

    vsqrt.s  %vfpu_rd, %vfpu_rs   ; calculate square root 
    vsqrt.p  %vfpu_rd, %vfpu_rs   ; calculate square root 
    vsqrt.t  %vfpu_rd, %vfpu_rs   ; calculate square root 
    vsqrt.q  %vfpu_rd, %vfpu_rs   ; calculate square root 

	%vfpu_rd:   VFPU Vector Target Register ([s|p|t|q]reg 0..127) 
	%vfpu_rs:   VFPU Vector Source Register ([s|p|t|q]reg 0..127) 

    vfpu_regs[%vfpu_rd] <- sqrt(vfpu_regs[%vfpu_rs]) 
*/ 

#define vsqrt_s(vfpu_rd, vfpu_rs) (0xd0160000 | (vfpu_rs << 8) | (vfpu_rd)) 
#define vsqrt_p(vfpu_rd, vfpu_rs) (0xd0160080 | (vfpu_rs << 8) | (vfpu_rd)) 
#define vsqrt_t(vfpu_rd, vfpu_rs) (0xd0168000 | (vfpu_rs << 8) | (vfpu_rd)) 
#define vsqrt_q(vfpu_rd, vfpu_rs) (0xd0168080 | (vfpu_rs << 8) | (vfpu_rd)) 


/* 
+-----------------------------------------+--+--------------+-+--------------+ 
|31                                    16 |15| 14         8 |7| 6         0  | 
+-----------------------------------------+--+--------------+-+--------------+ 
| opcode 0xd0110000 (s)                   | 0| vfpu_rs[6-0] |0| vfpu_rd[6-0] | 
| opcode 0xd0110080 (p)                   | 0| vfpu_rs[6-0] |1| vfpu_rd[6-0] | 
| opcode 0xd0118000 (t)                   | 1| vfpu_rs[6-0] |0| vfpu_rd[6-0] | 
| opcode 0xd0118080 (q)                   | 1| vfpu_rs[6-0] |1| vfpu_rd[6-0] | 
+-----------------------------------------+--+--------------+-+--------------+ 

  ReciprocalSquareRoot.Single/Pair/Triple/Quad 

    vrsq.s  %vfpu_rd, %vfpu_rs   ; calculate reciprocal sqrt (1/sqrt(x)) on single 
    vrsq.p  %vfpu_rd, %vfpu_rs   ; calculate reciprocal sqrt (1/sqrt(x)) on pair 
    vrsq.t  %vfpu_rd, %vfpu_rs   ; calculate reciprocal sqrt (1/sqrt(x)) on triple 
    vrsq.q  %vfpu_rd, %vfpu_rs   ; calculate reciprocal sqrt (1/sqrt(x)) on quad 

	%vfpu_rd:   VFPU Vector Target Register ([s|p|t|q]reg 0..127) 
	%vfpu_rs:   VFPU Vector Source Register ([s|p|t|q]reg 0..127) 

    vfpu_regs[%vfpu_rd] <- 1.0 / sqrt(vfpu_regs[%vfpu_rs]) 
*/ 
#define vrsq_s(vfpu_rd, vfpu_rs) (0xd0110000 | (vfpu_rs << 8) | (vfpu_rd)) 
#define vrsq_p(vfpu_rd, vfpu_rs) (0xd0110080 | (vfpu_rs << 8) | (vfpu_rd)) 
#define vrsq_t(vfpu_rd, vfpu_rs) (0xd0118000 | (vfpu_rs << 8) | (vfpu_rd)) 
#define vrsq_q(vfpu_rd, vfpu_rs) (0xd0118080 | (vfpu_rs << 8) | (vfpu_rd)) 


/* 
+-----------------------------------------+--+--------------+-+--------------+ 
|31                                    16 |15| 14         8 |7| 6         0  | 
+-----------------------------------------+--+--------------+-+--------------+ 
| opcode 0xd0120000 (s)                   | 0| vfpu_rs[6-0] |0| vfpu_rd[6-0] | 
| opcode 0xd0120080 (p)                   | 0| vfpu_rs[6-0] |1| vfpu_rd[6-0] | 
| opcode 0xd0128000 (t)                   | 1| vfpu_rs[6-0] |0| vfpu_rd[6-0] | 
| opcode 0xd0128080 (q)                   | 1| vfpu_rs[6-0] |1| vfpu_rd[6-0] | 
+-----------------------------------------+--+--------------+-+--------------+ 

  Sinus.Single/Pair/Triple/Quad 

    vsin.s  %vfpu_rd, %vfpu_rs   ; calculate sin on single 
    vsin.p  %vfpu_rd, %vfpu_rs   ; calculate sin on pair 
    vsin.t  %vfpu_rd, %vfpu_rs   ; calculate sin on triple 
    vsin.q  %vfpu_rd, %vfpu_rs   ; calculate sin on quad 

	%vfpu_rd:   VFPU Vector Target Register ([s|p|t|q]reg 0..127) 
	%vfpu_rs:   VFPU Vector Source Register ([s|p|t|q]reg 0..127) 

    vfpu_regs[%vfpu_rd] <- sin(vfpu_regs[%vfpu_rs]) 

  NOTE: the argument period range is scaled from 0.0 to 2.0 for numerical precisision.
        Multiply input values by 2.0/pi to get into 0 ... 2pi range.
        Multiply input values by 1.0/90.0 to get into 0deg ... 360deg range.
*/
#define vsin_s(vfpu_rd, vfpu_rs) (0xd0120000 | (vfpu_rs << 8) | (vfpu_rd)) 
#define vsin_p(vfpu_rd, vfpu_rs) (0xd0120080 | (vfpu_rs << 8) | (vfpu_rd)) 
#define vsin_t(vfpu_rd, vfpu_rs) (0xd0128000 | (vfpu_rs << 8) | (vfpu_rd)) 
#define vsin_q(vfpu_rd, vfpu_rs) (0xd0128080 | (vfpu_rs << 8) | (vfpu_rd)) 


/* 
+-----------------------------------------+--+--------------+-+--------------+ 
|31                                    16 |15| 14         8 |7| 6         0  | 
+-----------------------------------------+--+--------------+-+--------------+ 
| opcode 0xd0130000 (s)                   | 0| vfpu_rs[6-0] |0| vfpu_rd[6-0] | 
| opcode 0xd0130080 (p)                   | 0| vfpu_rs[6-0] |1| vfpu_rd[6-0] | 
| opcode 0xd0138000 (t)                   | 1| vfpu_rs[6-0] |0| vfpu_rd[6-0] | 
| opcode 0xd0138080 (q)                   | 1| vfpu_rs[6-0] |1| vfpu_rd[6-0] | 
+-----------------------------------------+--+--------------+-+--------------+ 

  Cosine.Single/Pair/Triple/Quad 

    vcos.s  %vfpu_rd, %vfpu_rs   ; calculate cos on single 
    vcos.p  %vfpu_rd, %vfpu_rs   ; calculate cos on pair 
    vcos.t  %vfpu_rd, %vfpu_rs   ; calculate cos on triple 
    vcos.q  %vfpu_rd, %vfpu_rs   ; calculate cos on quad 

	%vfpu_rd:   VFPU Vector Target Register ([s|p|t|q]reg 0..127) 
	%vfpu_rs:   VFPU Vector Source Register ([s|p|t|q]reg 0..127) 

    vfpu_regs[%vfpu_rd] <- cos(vfpu_regs[%vfpu_rs]) 

  NOTE: the argument period range is scaled from 0.0 to 2.0 for numerical precisision.
        Multiply input values by 2.0/pi to get into 0 ... 2pi range.
        Multiply input values by 1.0/90.0 to get into 0deg ... 360deg range.
*/ 

#define vcos_s(vfpu_rd, vfpu_rs) (0xd0130000 | (vfpu_rs << 8) | (vfpu_rd)) 
#define vcos_p(vfpu_rd, vfpu_rs) (0xd0130080 | (vfpu_rs << 8) | (vfpu_rd)) 
#define vcos_t(vfpu_rd, vfpu_rs) (0xd0138000 | (vfpu_rs << 8) | (vfpu_rd)) 
#define vcos_q(vfpu_rd, vfpu_rs) (0xd0138080 | (vfpu_rs << 8) | (vfpu_rd)) 

/* 
+-----------------------------------------+--+--------------+-+--------------+ 
|31                                    16 |15| 14         8 |7| 6         0  | 
+-----------------------------------------+--+--------------+-+--------------+ 
| opcode 0xd0170000 (s)                   | 0| vfpu_rs[6-0] |0| vfpu_rd[6-0] | 
| opcode 0xd0170080 (p)                   | 0| vfpu_rs[6-0] |1| vfpu_rd[6-0] | 
| opcode 0xd0178000 (t)                   | 1| vfpu_rs[6-0] |0| vfpu_rd[6-0] | 
| opcode 0xd0178080 (q)                   | 1| vfpu_rs[6-0] |1| vfpu_rd[6-0] | 
+-----------------------------------------+--+--------------+-+--------------+ 

  ArcSin.Single/Pair/Triple/Quad 

    vasin.s  %vfpu_rd, %vfpu_rs   ; calculate arcsin 
    vasin.p  %vfpu_rd, %vfpu_rs   ; calculate arcsin 
    vasin.t  %vfpu_rd, %vfpu_rs   ; calculate arcsin 
    vasin.q  %vfpu_rd, %vfpu_rs   ; calculate arcsin 

	%vfpu_rd:   VFPU Vector Target Register ([s|p|t|q]reg 0..127) 
	%vfpu_rs:   VFPU Vector Source Register ([s|p|t|q]reg 0..127) 

    vfpu_regs[%vfpu_rd] <- arcsin(vfpu_regs[%vfpu_rs]) 

  NOTE: the argument period range is scaled from 0.0 to 2.0 for numerical precisision.
        Multiply input values by 2.0/pi to get into 0 ... 2pi range.
        Multiply input values by 1.0/90.0 to get into 0deg ... 360deg range.
*/ 

#define vasin_s(vfpu_rd, vfpu_rs) (0xd0170000 | (vfpu_rs << 8) | (vfpu_rd)) 
#define vasin_p(vfpu_rd, vfpu_rs) (0xd0170080 | (vfpu_rs << 8) | (vfpu_rd)) 
#define vasin_t(vfpu_rd, vfpu_rs) (0xd0178000 | (vfpu_rs << 8) | (vfpu_rd)) 
#define vasin_q(vfpu_rd, vfpu_rs) (0xd0178080 | (vfpu_rs << 8) | (vfpu_rd)) 


/* 
+-----------------------------------------+--+--------------+-+--------------+ 
|31                                    16 |15| 14         8 |7| 6         0  | 
+-----------------------------------------+--+--------------+-+--------------+ 
| opcode 0xd01a0000 (s)                   | 0| vfpu_rs[6-0] |0| vfpu_rd[6-0] | 
| opcode 0xd01a0080 (p)                   | 0| vfpu_rs[6-0] |1| vfpu_rd[6-0] | 
| opcode 0xd01a8000 (t)                   | 1| vfpu_rs[6-0] |0| vfpu_rd[6-0] | 
| opcode 0xd01a8080 (q)                   | 1| vfpu_rs[6-0] |1| vfpu_rd[6-0] | 
+-----------------------------------------+--+--------------+-+--------------+ 

  NegativeSin.Single/Pair/Triple/Quad 

    vnsin.s  %vfpu_rd, %vfpu_rs   ; calculate negative sin 
    vnsin.p  %vfpu_rd, %vfpu_rs   ; calculate negative sin 
    vnsin.t  %vfpu_rd, %vfpu_rs   ; calculate negative sin 
    vnsin.q  %vfpu_rd, %vfpu_rs   ; calculate negative sin 

	%vfpu_rd:   VFPU Vector Target Register ([s|p|t|q]reg 0..127) 
	%vfpu_rs:   VFPU Vector Source Register ([s|p|t|q]reg 0..127) 

    vfpu_regs[%vfpu_rd] <- -sin(vfpu_regs[%vfpu_rs]) 
*/
#define vnsin_s(vfpu_rd, vfpu_rs) (0xd01a0000 | (vfpu_rs << 8) | (vfpu_rd)) 
#define vnsin_p(vfpu_rd, vfpu_rs) (0xd01a0080 | (vfpu_rs << 8) | (vfpu_rd)) 
#define vnsin_t(vfpu_rd, vfpu_rs) (0xd01a8000 | (vfpu_rs << 8) | (vfpu_rd)) 
#define vnsin_q(vfpu_rd, vfpu_rs) (0xd01a8080 | (vfpu_rs << 8) | (vfpu_rd)) 


/* 
+-----------------------------------------+--+--------------+-+--------------+ 
|31                                    16 |15| 14         8 |7| 6         0  | 
+-----------------------------------------+--+--------------+-+--------------+ 
| opcode 0xd0180000 (s)                   | 0| vfpu_rs[6-0] |0| vfpu_rd[6-0] | 
| opcode 0xd0180080 (p)                   | 0| vfpu_rs[6-0] |1| vfpu_rd[6-0] | 
| opcode 0xd0188000 (t)                   | 1| vfpu_rs[6-0] |0| vfpu_rd[6-0] | 
| opcode 0xd0188080 (q)                   | 1| vfpu_rs[6-0] |1| vfpu_rd[6-0] | 
+-----------------------------------------+--+--------------+-+--------------+ 

  NegativeReciprocal.Single/Pair/Triple/Quad 

    vnrcp.s  %vfpu_rd, %vfpu_rs   ; calculate negative reciprocal 
    vnrcp.p  %vfpu_rd, %vfpu_rs   ; calculate negative reciprocal 
    vnrcp.t  %vfpu_rd, %vfpu_rs   ; calculate negative reciprocal 
    vnrcp.q  %vfpu_rd, %vfpu_rs   ; calculate negative reciprocal 

	%vfpu_rd:   VFPU Vector Target Register ([s|p|t|q]reg 0..127) 
	%vfpu_rs:   VFPU Vector Source Register ([s|p|t|q]reg 0..127) 

    vfpu_regs[%vfpu_rd] <- -1.0 / vfpu_regs[%vfpu_rs] 
*/ 

#define vnrcp_s(vfpu_rd, vfpu_rs) (0xd0180000 | (vfpu_rs << 8) | (vfpu_rd)) 
#define vnrcp_p(vfpu_rd, vfpu_rs) (0xd0180080 | (vfpu_rs << 8) | (vfpu_rd)) 
#define vnrcp_t(vfpu_rd, vfpu_rs) (0xd0188000 | (vfpu_rs << 8) | (vfpu_rd)) 
#define vnrcp_q(vfpu_rd, vfpu_rs) (0xd0188080 | (vfpu_rs << 8) | (vfpu_rd)) 


/* 
+-----------------------------------------+--+--------------+-+--------------+ 
|31                                    16 |15| 14         8 |7| 6         0  | 
+-----------------------------------------+--+--------------+-+--------------+ 
| opcode 0xd01c0000 (s)                   | 0| vfpu_rs[6-0] |0| vfpu_rd[6-0] | 
| opcode 0xd01c0080 (p)                   | 0| vfpu_rs[6-0] |1| vfpu_rd[6-0] | 
| opcode 0xd01c8000 (t)                   | 1| vfpu_rs[6-0] |0| vfpu_rd[6-0] | 
| opcode 0xd01c8080 (q)                   | 1| vfpu_rs[6-0] |1| vfpu_rd[6-0] | 
+-----------------------------------------+--+--------------+-+--------------+ 

  ReciprocalExp2.Single/Pair/Triple/Quad 

    vrexp2.s  %vfpu_rd, %vfpu_rs   ; calculate 1/(2^y) 
    vrexp2.p  %vfpu_rd, %vfpu_rs   ; calculate 1/(2^y) 
    vrexp2.t  %vfpu_rd, %vfpu_rs   ; calculate 1/(2^y) 
    vrexp2.q  %vfpu_rd, %vfpu_rs   ; calculate 1/(2^y) 

	%vfpu_rd:   VFPU Vector Target Register ([s|p|t|q]reg 0..127) 
	%vfpu_rs:   VFPU Vector Source Register ([s|p|t|q]reg 0..127) 

    vfpu_regs[%vfpu_rd] <- 1/exp2(vfpu_regs[%vfpu_rs]) 
*/ 
#define vrexp2_s(vfpu_rd, vfpu_rs) (0xd01c0000 | (vfpu_rs << 8) | (vfpu_rd)) 
#define vrexp2_p(vfpu_rd, vfpu_rs) (0xd01c0080 | (vfpu_rs << 8) | (vfpu_rd)) 
#define vrexp2_t(vfpu_rd, vfpu_rs) (0xd01c8000 | (vfpu_rs << 8) | (vfpu_rd)) 
#define vrexp2_q(vfpu_rd, vfpu_rs) (0xd01c8080 | (vfpu_rs << 8) | (vfpu_rd)) 


/* 
+----------------------+--------------+----+--------------+---+--------------+ 
|31                 23 | 22        16 | 15 | 14         8 | 7 | 6         0  | 
+----------------------+--------------+----+--------------+---+--------------+ 
|  opcode 0x648 (p)    | vfpu_rt[6-0] | 0  | vfpu_rs[6-0] | 1 | vfpu_rd[6-0] | 
|  opcode 0x648 (t)    | vfpu_rt[6-0] | 1  | vfpu_rs[6-0] | 0 | vfpu_rd[6-0] | 
|  opcode 0x648 (q)    | vfpu_rt[6-0] | 1  | vfpu_rs[6-0] | 1 | vfpu_rd[6-0] | 
+----------------------+--------------+----+--------------+---+--------------+ 

   VectorDotProduct.Pair/Triple/Quad 

    vdot.p %vfpu_rd, %vfpu_rs, %vfpu_rt   ; Dot Product Pair 
    vdot.t %vfpu_rd, %vfpu_rs, %vfpu_rt   ; Dot Product Triple 
    vdot.q %vfpu_rd, %vfpu_rs, %vfpu_rt   ; Dot Product Quad 

        %vfpu_rt:   VFPU Vector Source Register ([s|p|t|q]reg 0..127) 
        %vfpu_rs:   VFPU Vector Source Register ([s|p|t|q]reg 0..127) 
        %vfpu_rd:   VFPU Vector Destination Register ([s|p|t|q]reg 0..127) 

    vfpu_regs[%vfpu_rd] <- dotproduct(vfpu_regs[%vfpu_rs], vfpu_regs[%vfpu_rt]) 
*/ 
#define vdot_p(vfpu_rd,vfpu_rs,vfpu_rt)  (0x64800080 | ((vfpu_rt) << 16) | ((vfpu_rs) << 8) | (vfpu_rd)) 
#define vdot_t(vfpu_rd,vfpu_rs,vfpu_rt)  (0x64808000 | ((vfpu_rt) << 16) | ((vfpu_rs) << 8) | (vfpu_rd)) 
#define vdot_q(vfpu_rd,vfpu_rs,vfpu_rt)  (0x64808080 | ((vfpu_rt) << 16) | ((vfpu_rs) << 8) | (vfpu_rd)) 


/* 
+-------------------------------------+----+--------------+---+--------------+ 
|31                                16 | 15 | 14         8 | 7 | 6         0  | 
+-------------------------------------+----+--------------+---+--------------+ 
|        opcode 0xd046 (p)            | 0  | vfpu_rs[6-0] | 1 | vfpu_rd[6-0] | 
|        opcode 0xd046 (t)            | 1  | vfpu_rs[6-0] | 0 | vfpu_rd[6-0] | 
|        opcode 0xd046 (q)            | 1  | vfpu_rs[6-0] | 1 | vfpu_rd[6-0] | 
+-------------------------------------+----+--------------+---+--------------+ 

  ???.Pair/Triple/Quad  --  Accumulate Components of Vector into Single Float

    vfad.p %vfpu_rd, %vfpu_rs  ; Accumulate Components of Pair 
    vfad.t %vfpu_rd, %vfpu_rs  ; Accumulate Components of Triple 
    vfad.q %vfpu_rd, %vfpu_rs  ; Accumulate Components of Quad 

        %vfpu_rs:   VFPU Vector Source Register ([p|t|q]reg 0..127) 
        %vfpu_rd:   VFPU Vector Destination Register (sreg 0..127) 

    vfpu_regs[%vfpu_rd] <- Sum_Of_Components(vfpu_regs[%vfpu_rs]) 
*/ 
#define vfad_p(vfpu_rd,vfpu_rs)  (0xd0460080 | ((vfpu_rs) << 8) | (vfpu_rd)) 
#define vfad_t(vfpu_rd,vfpu_rs)  (0xd0468000 | ((vfpu_rs) << 8) | (vfpu_rd)) 
#define vfad_q(vfpu_rd,vfpu_rs)  (0xd0468080 | ((vfpu_rs) << 8) | (vfpu_rd)) 


/* 
+-------------------------------------+----+--------------+---+--------------+ 
|31                                16 | 15 | 14         8 | 7 | 6         0  | 
+-------------------------------------+----+--------------+---+--------------+ 
|        opcode 0xd047 (p)            | 0  | vfpu_rs[6-0] | 1 | vfpu_rd[6-0] | 
|        opcode 0xd047 (t)            | 1  | vfpu_rs[6-0] | 0 | vfpu_rd[6-0] | 
|        opcode 0xd047 (q)            | 1  | vfpu_rs[6-0] | 1 | vfpu_rd[6-0] | 
+-------------------------------------+----+--------------+---+--------------+ 

  VectorAverage.Pair/Triple/Quad  --  Average Components of Vector into Single Float

    vavg.p %vfpu_rd, %vfpu_rs  ; Accumulate Components of Pair 
    vavg.t %vfpu_rd, %vfpu_rs  ; Accumulate Components of Triple 
    vavg.q %vfpu_rd, %vfpu_rs  ; Accumulate Components of Quad 

        %vfpu_rs:   VFPU Vector Source Register ([p|t|q]reg 0..127) 
        %vfpu_rd:   VFPU Vector Destination Register (sreg 0..127) 

    vfpu_regs[%vfpu_rd] <- Average_Of_Components(vfpu_regs[%vfpu_rs]) 
*/ 
#define vavg_p(vfpu_rd,vfpu_rs)  (0xd0470080 | ((vfpu_rs) << 8) | (vfpu_rd)) 
#define vavg_t(vfpu_rd,vfpu_rs)  (0xd0478000 | ((vfpu_rs) << 8) | (vfpu_rd)) 
#define vavg_q(vfpu_rd,vfpu_rs)  (0xd0478080 | ((vfpu_rs) << 8) | (vfpu_rd)) 


/* 
+----------------------+--------------+----+--------------+---+--------------+ 
|31                 23 | 22        16 | 15 | 14         8 | 7 | 6         0  | 
+----------------------+--------------+----+--------------+---+--------------+ 
|  opcode 0x660 (p)    | vfpu_rt[6-0] | 0  | vfpu_rs[6-0] | 1 | vfpu_rd[6-0] | 
|  opcode 0x660 (t)    | vfpu_rt[6-0] | 1  | vfpu_rs[6-0] | 0 | vfpu_rd[6-0] | 
|  opcode 0x660 (q)    | vfpu_rt[6-0] | 1  | vfpu_rs[6-0] | 1 | vfpu_rd[6-0] | 
+----------------------+--------------+----+--------------+---+--------------+ 

   VectorHomogenousDotProduct.Pair/Triple/Quad 

    vhdp.p %vfpu_rd, %vfpu_rs, %vfpu_rt   ; Dot Product Pair 
    vhdp.t %vfpu_rd, %vfpu_rs, %vfpu_rt   ; Dot Product Triple 
    vhdp.q %vfpu_rd, %vfpu_rs, %vfpu_rt   ; Dot Product Quad 

        %vfpu_rt:   VFPU Vector Source Register ([s|p|t|q]reg 0..127) 
        %vfpu_rs:   VFPU Vector Source Register ([s|p|t|q]reg 0..127) 
        %vfpu_rd:   VFPU Vector Destination Register ([s|p|t|q]reg 0..127) 

    vfpu_regs[%vfpu_rd] <- homogenousdotproduct(vfpu_regs[%vfpu_rs], vfpu_regs[%vfpu_rt]) 
*/ 
#define vhdp_p(vfpu_rd,vfpu_rs,vfpu_rt)  (0x66000080 | ((vfpu_rt) << 16) | ((vfpu_rs) << 8) | (vfpu_rd)) 
#define vhdp_t(vfpu_rd,vfpu_rs,vfpu_rt)  (0x66008000 | ((vfpu_rt) << 16) | ((vfpu_rs) << 8) | (vfpu_rd)) 
#define vhdp_q(vfpu_rd,vfpu_rs,vfpu_rt)  (0x66008080 | ((vfpu_rt) << 16) | ((vfpu_rs) << 8) | (vfpu_rd)) 


/* 
+-------------------------------------+----+--------------+---+--------------+ 
|31                                16 | 15 | 14         8 | 7 | 6          0 | 
+-------------------------------------+----+--------------+---+--------------+ 
| opcode 0xd0010000 (s)               |  0 | vfpu_rs[6-0] | 0 | vfpu_rd[6-0] | 
| opcode 0xd0010080 (p)               |  0 | vfpu_rs[6-0] | 1 | vfpu_rd[6-0] | 
| opcode 0xd0018000 (t)               |  1 | vfpu_rs[6-0] | 0 | vfpu_rd[6-0] | 
| opcode 0xd0018080 (q)               |  1 | vfpu_rs[6-0] | 1 | vfpu_rd[6-0] | 
+-------------------------------------+----+--------------+---+--------------+ 

  AbsoluteValue.Single/Pair/Triple/Quad 

    vabs.s %vfpu_rd, %vfpu_rs    ; Absolute Value Single 
    vabs.p %vfpu_rd, %vfpu_rs    ; Absolute Value Pair 
    vabs.t %vfpu_rd, %vfpu_rs    ; Absolute Value Triple 
    vabs.q %vfpu_rd, %vfpu_rs    ; Absolute Value Quad 

        %vfpu_rd:   VFPU Vector Destination Register (m[p|t|q]reg 0..127) 
        %vfpu_rs:   VFPU Vector Source Register (m[p|t|q]reg 0..127) 

    vfpu_regs[%vfpu_rd] <- abs(vfpu_regs[%vfpu_rs]) 
*/ 
#define vabs_s(vfpu_rd,vfpu_rs)  (0xd0010000 | ((vfpu_rs) << 8) | (vfpu_rd)) 
#define vabs_p(vfpu_rd,vfpu_rs)  (0xd0010080 | ((vfpu_rs) << 8) | (vfpu_rd)) 
#define vabs_t(vfpu_rd,vfpu_rs)  (0xd0018000 | ((vfpu_rs) << 8) | (vfpu_rd)) 
#define vabs_q(vfpu_rd,vfpu_rs)  (0xd0018080 | ((vfpu_rs) << 8) | (vfpu_rd)) 


/* 
+-------------------------------------+----+--------------+---+--------------+ 
|31                                16 | 15 | 14         8 | 7 | 6          0 | 
+-------------------------------------+----+--------------+---+--------------+ 
| opcode 0xd002 (s)                   |  0 | vfpu_rs[6-0] | 0 | vfpu_rd[6-0] | 
| opcode 0xd002 (p)                   |  0 | vfpu_rs[6-0] | 1 | vfpu_rd[6-0] | 
| opcode 0xd002 (t)                   |  1 | vfpu_rs[6-0] | 0 | vfpu_rd[6-0] | 
| opcode 0xd002 (q)                   |  1 | vfpu_rs[6-0] | 1 | vfpu_rd[6-0] | 
+-------------------------------------+----+--------------+---+--------------+ 

  Negate.Single/Pair/Triple/Quad 

    vneg.s %vfpu_rd, %vfpu_rs    ; Negate Single 
    vneg.p %vfpu_rd, %vfpu_rs    ; Negate Pair 
    vneg.t %vfpu_rd, %vfpu_rs    ; Negate Triple 
    vneg.q %vfpu_rd, %vfpu_rs    ; Negate Quad 

        %vfpu_rd:   VFPU Vector Destination Register (m[p|t|q]reg 0..127) 
        %vfpu_rs:   VFPU Vector Source Register (m[p|t|q]reg 0..127) 

    vfpu_regs[%vfpu_rd] <- -vfpu_regs[%vfpu_rs] 
*/
#define vneg_s(vfpu_rd,vfpu_rs)  (0xd0020000 | ((vfpu_rs) << 8) | (vfpu_rd)) 
#define vneg_p(vfpu_rd,vfpu_rs)  (0xd0020080 | ((vfpu_rs) << 8) | (vfpu_rd)) 
#define vneg_t(vfpu_rd,vfpu_rs)  (0xd0028000 | ((vfpu_rs) << 8) | (vfpu_rd)) 
#define vneg_q(vfpu_rd,vfpu_rs)  (0xd0028080 | ((vfpu_rs) << 8) | (vfpu_rd)) 


/* 
+-------------------------------------+----+--------------+---+--------------+ 
|31                                16 | 15 | 14         8 | 7 | 6          0 | 
+-------------------------------------+----+--------------+---+--------------+ 
| opcode 0xd04a (s)                   |  0 | vfpu_rs[6-0] | 0 | vfpu_rd[6-0] | 
| opcode 0xd04a (p)                   |  0 | vfpu_rs[6-0] | 1 | vfpu_rd[6-0] | 
| opcode 0xd04a (t)                   |  1 | vfpu_rs[6-0] | 0 | vfpu_rd[6-0] | 
| opcode 0xd04a (q)                   |  1 | vfpu_rs[6-0] | 1 | vfpu_rd[6-0] | 
+-------------------------------------+----+--------------+---+--------------+ 

  Sign.Single/Pair/Triple/Quad 

    vsgn.s %vfpu_rd, %vfpu_rs    ; Get Sign Single 
    vsgn.p %vfpu_rd, %vfpu_rs    ; Get Sign Pair 
    vsgn.t %vfpu_rd, %vfpu_rs    ; Get Sign Triple 
    vsgn.q %vfpu_rd, %vfpu_rs    ; Get Sign Quad 

        %vfpu_rd:   VFPU Vector Destination Register (m[p|t|q]reg 0..127) 
        %vfpu_rs:   VFPU Vector Source Register (m[p|t|q]reg 0..127) 

    vfpu_regs[%vfpu_rd] <- sign(vfpu_regs[%vfpu_rs]) 

    this will set rd values to 1 or -1, depending on sign of input values 
*/ 
#define vsgn_s(vfpu_rd,vfpu_rs)  (0xd04a0000 | ((vfpu_rs) << 8) | (vfpu_rd)) 
#define vsgn_p(vfpu_rd,vfpu_rs)  (0xd04a0080 | ((vfpu_rs) << 8) | (vfpu_rd)) 
#define vsgn_t(vfpu_rd,vfpu_rs)  (0xd04a8000 | ((vfpu_rs) << 8) | (vfpu_rd)) 
#define vsgn_q(vfpu_rd,vfpu_rs)  (0xd04a8080 | ((vfpu_rs) << 8) | (vfpu_rd)) 


/*
+----------------------+--------------+----+--------------+---+--------------+
|31                 23 | 22        16 | 15 | 14         8 | 7 | 6         0  |
+----------------------+--------------+----+--------------+---+--------------+
| opcode 0x6d0 (s)     | vfpu_rt[6-0] |  0 | vfpu_rs[6-0] | 0 | vfpu_rd[6-0] |
| opcode 0x6d0 (p)     | vfpu_rt[6-0] |  0 | vfpu_rs[6-0] | 1 | vfpu_rd[6-0] |
| opcode 0x6d0 (t)     | vfpu_rt[6-0] |  1 | vfpu_rs[6-0] | 0 | vfpu_rd[6-0] |
| opcode 0x6d0 (q)     | vfpu_rt[6-0] |  1 | vfpu_rs[6-0] | 1 | vfpu_rd[6-0] |
+----------------------+--------------+----+--------------+---+--------------+

  VectorMin.Single/Pair/Triple/Quad

    vmin.s %vfpu_rd, %vfpu_rs, %vfpu_rt ; Get Minimum Value Single 
    vmin.p %vfpu_rd, %vfpu_rs, %vfpu_rt ; Get Minimum Value Pair 
    vmin.t %vfpu_rd, %vfpu_rs, %vfpu_rt ; Get Minimum Value Triple 
    vmin.q %vfpu_rd, %vfpu_rs, %vfpu_rt ; Get Minimum Value Quad 

        %vfpu_rt:   VFPU Vector Source Register (sreg 0..127) 
        %vfpu_rs:   VFPU Vector Source Register ([p|t|q]reg 0..127) 
        %vfpu_rd:   VFPU Vector Destination Register ([s|p|t|q]reg 0..127) 

    vfpu_regs[%vfpu_rd] <- min(vfpu_regs[%vfpu_rs], vfpu_reg[%vfpu_rt]) 
*/ 
#define vmin_s(vfpu_rd,vfpu_rs,vfpu_rt)  (0x6D000000 | ((vfpu_rt) << 16) | ((vfpu_rs) << 8) | (vfpu_rd)) 
#define vmin_p(vfpu_rd,vfpu_rs,vfpu_rt)  (0x6D000080 | ((vfpu_rt) << 16) | ((vfpu_rs) << 8) | (vfpu_rd)) 
#define vmin_t(vfpu_rd,vfpu_rs,vfpu_rt)  (0x6D008000 | ((vfpu_rt) << 16) | ((vfpu_rs) << 8) | (vfpu_rd)) 
#define vmin_q(vfpu_rd,vfpu_rs,vfpu_rt)  (0x6D008080 | ((vfpu_rt) << 16) | ((vfpu_rs) << 8) | (vfpu_rd)) 


/* 
+----------------------+--------------+----+--------------+---+--------------+ 
|31                 23 | 22        16 | 15 | 14         8 | 7 | 6         0  | 
+----------------------+--------------+----+--------------+---+--------------+ 
| opcode 0x6d8 (s)     | vfpu_rt[6-0] |  0 | vfpu_rs[6-0] | 0 | vfpu_rd[6-0] | 
| opcode 0x6d8 (p)     | vfpu_rt[6-0] |  0 | vfpu_rs[6-0] | 1 | vfpu_rd[6-0] | 
| opcode 0x6d8 (t)     | vfpu_rt[6-0] |  1 | vfpu_rs[6-0] | 0 | vfpu_rd[6-0] | 
| opcode 0x6d8 (q)     | vfpu_rt[6-0] |  1 | vfpu_rs[6-0] | 1 | vfpu_rd[6-0] | 
+----------------------+--------------+----+--------------+---+--------------+ 

  VectorMax.Single/Pair/Triple/Quad 

    vmax.s %vfpu_rd, %vfpu_rs, %vfpu_rt ; Get Maximum Value Single 
    vmax.p %vfpu_rd, %vfpu_rs, %vfpu_rt ; Get Maximum Value Pair 
    vmax.t %vfpu_rd, %vfpu_rs, %vfpu_rt ; Get Maximum Value Triple 
    vmax.q %vfpu_rd, %vfpu_rs, %vfpu_rt ; Get Maximum Value Quad 

        %vfpu_rt:   VFPU Vector Source Register (sreg 0..127) 
        %vfpu_rs:   VFPU Vector Source Register ([p|t|q]reg 0..127) 
        %vfpu_rd:   VFPU Vector Destination Register ([s|p|t|q]reg 0..127) 

    vfpu_regs[%vfpu_rd] <- max(vfpu_regs[%vfpu_rs], vfpu_reg[%vfpu_rt]) 
*/ 
#define vmax_s(vfpu_rd,vfpu_rs,vfpu_rt)  (0x6D800000 | ((vfpu_rt) << 16) | ((vfpu_rs) << 8) | (vfpu_rd)) 
#define vmax_p(vfpu_rd,vfpu_rs,vfpu_rt)  (0x6D800080 | ((vfpu_rt) << 16) | ((vfpu_rs) << 8) | (vfpu_rd)) 
#define vmax_t(vfpu_rd,vfpu_rs,vfpu_rt)  (0x6D808000 | ((vfpu_rt) << 16) | ((vfpu_rs) << 8) | (vfpu_rd)) 
#define vmax_q(vfpu_rd,vfpu_rs,vfpu_rt)  (0x6D808080 | ((vfpu_rt) << 16) | ((vfpu_rs) << 8) | (vfpu_rd)) 


/* 
+-------------------------------------+----+--------------+---+--------------+ 
|31                                16 | 15 | 14         8 | 7 | 6          0 | 
+-------------------------------------+----+--------------+---+--------------+ 
| opcode 0xd0040000 (s)               |  0 | vfpu_rs[6-0] | 0 | vfpu_rd[6-0] | 
| opcode 0xd0040080 (p)               |  0 | vfpu_rs[6-0] | 1 | vfpu_rd[6-0] | 
| opcode 0xd0048000 (t)               |  1 | vfpu_rs[6-0] | 0 | vfpu_rd[6-0] | 
| opcode 0xd0048080 (q)               |  1 | vfpu_rs[6-0] | 1 | vfpu_rd[6-0] | 
+-------------------------------------+----+--------------+---+--------------+ 

  SaturateValue0.Single/Pair/Triple/Quad 

    vsat0.s %vfpu_rd, %vfpu_rs    ; Clamp Value Single in Range 0.0...1.0
    vsat0.p %vfpu_rd, %vfpu_rs    ; Clamp Value Pair in Range 0.0...1.0
    vsat0.t %vfpu_rd, %vfpu_rs    ; Clamp Value Triple in Range 0.0...1.0
    vsat0.q %vfpu_rd, %vfpu_rs    ; Clamp Value Quad in Range 0.0...1.0

        %vfpu_rd:   VFPU Vector Destination Register (m[p|t|q]reg 0..127) 
        %vfpu_rs:   VFPU Vector Source Register (m[p|t|q]reg 0..127) 

    vfpu_regs[%vfpu_rd] <- vfpu_regs[%vfpu_rs] < 0.0 ? 0.0 : vfpu_regs[%vfpu_rs] > 1.0 ? 1.0 : vfpu_regs[%vfpu_rs]
*/ 
#define vsat0_s(vfpu_rd,vfpu_rs)  (0xd0040000 | ((vfpu_rs) << 8) | (vfpu_rd)) 
#define vsat0_p(vfpu_rd,vfpu_rs)  (0xd0040080 | ((vfpu_rs) << 8) | (vfpu_rd)) 
#define vsat0_t(vfpu_rd,vfpu_rs)  (0xd0048000 | ((vfpu_rs) << 8) | (vfpu_rd)) 
#define vsat0_q(vfpu_rd,vfpu_rs)  (0xd0048080 | ((vfpu_rs) << 8) | (vfpu_rd)) 


/* 
+-------------------------------------+----+--------------+---+--------------+ 
|31                                16 | 15 | 14         8 | 7 | 6          0 | 
+-------------------------------------+----+--------------+---+--------------+ 
| opcode 0xd0050000 (s)               |  0 | vfpu_rs[6-0] | 0 | vfpu_rd[6-0] | 
| opcode 0xd0050080 (p)               |  0 | vfpu_rs[6-0] | 1 | vfpu_rd[6-0] | 
| opcode 0xd0058000 (t)               |  1 | vfpu_rs[6-0] | 0 | vfpu_rd[6-0] | 
| opcode 0xd0058080 (q)               |  1 | vfpu_rs[6-0] | 1 | vfpu_rd[6-0] | 
+-------------------------------------+----+--------------+---+--------------+ 

  SaturateValue1.Single/Pair/Triple/Quad 

    vsat1.s %vfpu_rd, %vfpu_rs    ; Clamp Value Single in Range -1.0...1.0
    vsat1.p %vfpu_rd, %vfpu_rs    ; Clamp Value Pair in Range -1.0...1.0
    vsat1.t %vfpu_rd, %vfpu_rs    ; Clamp Value Triple in Range -1.0...1.0
    vsat1.q %vfpu_rd, %vfpu_rs    ; Clamp Value Quad in Range -1.0...1.0

        %vfpu_rd:   VFPU Vector Destination Register (m[p|t|q]reg 0..127) 
        %vfpu_rs:   VFPU Vector Source Register (m[p|t|q]reg 0..127) 

    vfpu_regs[%vfpu_rd] <- vfpu_regs[%vfpu_rs] < -1.0 ? -1.0 : vfpu_regs[%vfpu_rs] > 1.0 ? 1.0 : vfpu_regs[%vfpu_rs]
*/ 
#define vsat1_s(vfpu_rd,vfpu_rs)  (0xd0050000 | ((vfpu_rs) << 8) | (vfpu_rd)) 
#define vsat1_p(vfpu_rd,vfpu_rs)  (0xd0050080 | ((vfpu_rs) << 8) | (vfpu_rd)) 
#define vsat1_t(vfpu_rd,vfpu_rs)  (0xd0058000 | ((vfpu_rs) << 8) | (vfpu_rd)) 
#define vsat1_q(vfpu_rd,vfpu_rs)  (0xd0058080 | ((vfpu_rs) << 8) | (vfpu_rd)) 


/*
+------------------------------------+--------------------+---+--------------+
|31                               16 | 15 | 14          8 | 7 | 6          0 |
+------------------------------------+--------------------+---+--------------+
|    opcode 0xd03a0000               |    | vfpu_rs[6-0]  |   | vfpu_rd[6-0] |
+------------------------------------+--------------------+---+--------------+

  Convert Unsigned Short to Integer

    vus2i.s %vfpu_rd, %vfpu_rs   ; convert [sreg] vfpu_rs -> [preg] vfpu_rd
    vus2i.p %vfpu_rd, %vfpu_rs   ; convert [preg] vfpu_rs -> [qreg] vfpu_rd 

	%vfpu_rs:	VFPU Vector Source Register ([s|p]reg 0..127)
	%vfpu_rd:	VFPU Vector Destination Register ([p|q]reg 0..127)

  vus2i.s:
    vfpu_regs[%vfpu_rd_p[0]] <- (int) low_16(vfpu_regs[%vfpu_rs]) / 2
    vfpu_regs[%vfpu_rd_p[1]] <- (int) high_16(vfpu_regs[%vfpu_rs]) / 2

  vus2i.p:
    vfpu_regs[%vfpu_rd_q[0]] <- (int) low_16(vfpu_regs[%vfpu_rs_p[0]]) / 2
    vfpu_regs[%vfpu_rd_q[1]] <- (int) high_16(vfpu_regs[%vfpu_rs_p[0]]) / 2
    vfpu_regs[%vfpu_rd_q[2]] <- (int) low_16(vfpu_regs[%vfpu_rs_p[1]]) / 2
    vfpu_regs[%vfpu_rd_q[3]] <- (int) high_16(vfpu_regs[%vfpu_rs_p[1]]) / 2
*/
#define vus2i_s(vfpu_rd,vfpu_rs)  (0xd03a0000 | ((vfpu_rd) << 16) | (vfpu_rd))
#define vus2i_p(vfpu_rd,vfpu_rs)  (0xd03a0080 | ((vfpu_rd) << 16) | (vfpu_rd))


/*
+------------------------------------+--------------------+---+--------------+
|31                               16 | 15 | 14          8 | 7 | 6          0 |
+------------------------------------+--------------------+---+--------------+
|    opcode 0xd03b0000               |    | vfpu_rs[6-0]  |   | vfpu_rd[6-0] |
+------------------------------------+--------------------+---+--------------+

  Convert Short to Integer

    vs2i.s %vfpu_rd, %vfpu_rs   ; convert [sreg] vfpu_rs -> [preg] vfpu_rd
    vs2i.p %vfpu_rd, %vfpu_rs   ; convert [preg] vfpu_rs -> [qreg] vfpu_rd 

	%vfpu_rs:	VFPU Vector Source Register ([s|p]reg 0..127)
	%vfpu_rd:	VFPU Vector Destination Register ([p|q]reg 0..127)

  vs2i.s:
    vfpu_regs[%vfpu_rd_p[0]] <- (int) low_16(vfpu_regs[%vfpu_rs])
    vfpu_regs[%vfpu_rd_p[1]] <- (int) high_16(vfpu_regs[%vfpu_rs])

  vs2i.p:
    vfpu_regs[%vfpu_rd_q[0]] <- (int) low_16(vfpu_regs[%vfpu_rs_p[0]])
    vfpu_regs[%vfpu_rd_q[1]] <- (int) high_16(vfpu_regs[%vfpu_rs_p[0]])
    vfpu_regs[%vfpu_rd_q[2]] <- (int) low_16(vfpu_regs[%vfpu_rs_p[1]])
    vfpu_regs[%vfpu_rd_q[3]] <- (int) high_16(vfpu_regs[%vfpu_rs_p[1]])
*/
#define vs2i_s(vfpu_rd,vfpu_rs)  (0xd03b0000 | ((vfpu_rd) << 16) | (vfpu_rd))
#define vs2i_p(vfpu_rd,vfpu_rs)  (0xd03b0080 | ((vfpu_rd) << 16) | (vfpu_rd))


/*
+----------------------+-------------+----+---------------+---+--------------+
|31                 21 | 20       16 | 15 | 14          8 | 7 | 6          0 |
+----------------------+-------------+----+---------------+---+--------------+
|  opcode 0xd2200000   |  scale[4-1] |    | vfpu_rs[6-0]  |   | vfpu_rd[6-0] |
+----------------------+-------------+----+---------------+---+--------------+

  Float to Int, Truncated

    vf2iz.s %vfpu_rd, %vfpu_rs, scale   ; Truncate and Convert Float to Integer (Single)
    vf2iz.p %vfpu_rd, %vfpu_rs, scale   ; Truncate and Convert Float to Integer (Pair)
    vf2iz.t %vfpu_rd, %vfpu_rs, scale   ; Truncate and Convert Float to Integer (Triple)
    vf2iz.q %vfpu_rd, %vfpu_rs, scale   ; Truncate and Convert Float to Integer (Quad)

	%vfpu_rs:	VFPU Vector Source Register ([s|p|t|q]reg 0..127)
	%vfpu_rd:	VFPU Vector Destination Register ([s|p|t|q]reg 0..127)
	scale:		Multiply by (2^scale) before converting to Float

    vfpu_regs[%vfpu_rd] <- (int) (2^scale * vfpu_regs[%vfpu_rs])
*/
#define vf2iz_s(vfpu_rd,vfpu_rs,scale)  (0xd2200000 | ((scale) << 16) | ((vfpu_rs) << 8) | (vfpu_rd))
#define vf2iz_p(vfpu_rd,vfpu_rs,scale)  (0xd2200080 | ((scale) << 16) | ((vfpu_rs) << 8) | (vfpu_rd))
#define vf2iz_t(vfpu_rd,vfpu_rs,scale)  (0xd2208000 | ((scale) << 16) | ((vfpu_rs) << 8) | (vfpu_rd))
#define vf2iz_q(vfpu_rd,vfpu_rs,scale)  (0xd2208080 | ((scale) << 16) | ((vfpu_rs) << 8) | (vfpu_rd))


/*
+------------------------------------------+--------------+---+--------------+
|31                                     15 | 14         8 | 7 | 6          0 |
+------------------------------------------+--------------+---+--------------+
|              opcode 0xd0000000           | vfpu_rs[6-0] | 1 | vfpu_rd[6-0] |
+------------------------------------------+--------------+---+--------------+

  Copy/Move Matrix to new Register

    vmmov.p %vfpu_rd, %vfpu_rs    ; Move 2x2
    vmmov.t %vfpu_rd, %vfpu_rs    ; Move 3x3
    vmmov.q %vfpu_rd, %vfpu_rs    ; Move 4x4

	%vfpu_rd:	VFPU Matrix Destination Register (m[p|t|q]reg 0..127)
	%vfpu_rs:	VFPU Matrix Source Register (m[p|t|q]reg 0..127)

    vfpu_mtx[%vfpu_rd] <- vfpu_mtx[%vfpu_rs]
*/
#define vmmov_p(vfpu_rd,vfpu_rs)  (0xf3800080 | ((vfpu_rs) << 8) | (vfpu_rd))
#define vmmov_t(vfpu_rd,vfpu_rs)  (0xf3808000 | ((vfpu_rs) << 8) | (vfpu_rd))
#define vmmov_q(vfpu_rd,vfpu_rs)  (0xf3808080 | ((vfpu_rs) << 8) | (vfpu_rd))


/*
+-------------------------------------------------------------+--------------+
|31                                                         7 | 6         0  |
+-------------------------------------------------------------+--------------+
|              opcode 0xf3868080                              | vfpu_rd[6-0] |
+-------------------------------------------------------------+--------------+

  SetMatrixZero.Single/Pair/Triple/Quad

    vmzero.p %vfpu_rd	; Set 2x2 Submatrix to 0.0f
    vmzero.t %vfpu_rd	; Set 3x3 Submatrix to 0.0f
    vmzero.q %vfpu_rd	; Set 4x4 Matrix to 0.0f

	%vfpu_rd:	VFPU Matrix Destination Register ([p|t|q]reg 0..127)

    vfpu_mtx[%vfpu_rd] <- 0.0f
*/
#define vmzero_p(vfpu_rd)  (0xf3860080 | (vfpu_rd))
#define vmzero_t(vfpu_rd)  (0xf3868000 | (vfpu_rd))
#define vmzero_q(vfpu_rd)  (0xf3868080 | (vfpu_rd))


/*
+-------------------------------------------------------------+--------------+
|31                                                         7 | 6         0  |
+-------------------------------------------------------------+--------------+
|              opcode 0xf3870080                              | vfpu_rd[6-0] |
+-------------------------------------------------------------+--------------+

  SetMatrixOne.Single/Pair/Triple/Quad

    vmone.p %vfpu_rd	; Set 2x2 Submatrix to 1.0f
    vmone.t %vfpu_rd	; Set 3x3 Submatrix to 1.0f
    vmone.q %vfpu_rd	; Set 4x4 Matrix to 1.0f

	%vfpu_rd:	VFPU Matrix Destination Register ([p|t|q]reg 0..127)

    vfpu_mtx[%vfpu_rd] <- 1.0f
*/
#define vmone_p(vfpu_rd)  (0xf3870080 | (vfpu_rd))
#define vmone_t(vfpu_rd)  (0xf3878000 | (vfpu_rd))
#define vmone_q(vfpu_rd)  (0xf3878080 | (vfpu_rd))


/*
+-------------------------------------------------------------+--------------+
|31                                                         7 | 6         0  |
+-------------------------------------------------------------+--------------+
|              opcode 0xf3838080                              | vfpu_rd[6-0] |
+-------------------------------------------------------------+--------------+

    vmidt.p %vfpu_rd	; Set 2x2 Submatrix to Identity
    vmidt.t %vfpu_rd	; Set 3x3 Submatrix to Identity
    vmidt.q %vfpu_rd	; Set 4x4 Matrix to Identity

	%vfpu_rd:	VFPU Matrix Destination Register ([s|p|t|q]reg 0..127)

    vfpu_mtx[%vfpu_rd] <- identity matrix
*/
#define vmidt_p(vfpu_rd)  (0xf3830080 | (vfpu_rd))
#define vmidt_t(vfpu_rd)  (0xf3838000 | (vfpu_rd))
#define vmidt_q(vfpu_rd)  (0xf3838080 | (vfpu_rd))


/* 
+-------------------------------------------------------------+--------------+ 
|31                                   16 | 15 | 14     8  | 7 | 6         0  | 
+-------------------------------------------------------------+--------------+ 
| opcode 0xd003 (p)                      |  0 |      0    | 1 | vfpu_rd[6-0] | 
| opcode 0xd003 (t)                      |  1 |      0    | 0 | vfpu_rd[6-0] | 
| opcode 0xd003 (q)                      |  1 |      0    | 1 | vfpu_rd[6-0] | 
+-------------------------------------------------------------+--------------+ 
    
   VectorLoadIdentity.Pair/Triple/Quad 

    vidt.p %vfpu_rd   ; Set 2x1 Vector in 2x2 Matrix to Identity 
    vidt.t %vfpu_rd   ; Set 3x1 Vector in 3x3 Matrix to Identity 
    vidt.q %vfpu_rd   ; Set 4x1 Vector in 4x4 Matrix to Identity 

        %vfpu_rd:   VFPU Vector Destination Register ([s|p|t|q]reg 0..127) 

    vfpu_regs[%vfpu_rd] <- identity vector (the matching row/column from the identity matrix for this index)
*/
#define vidt_p(vfpu_rd)  (0xd0030080 | (vfpu_rd)) 
#define vidt_t(vfpu_rd)  (0xd0038000 | (vfpu_rd)) 
#define vidt_q(vfpu_rd)  (0xd0038080 | (vfpu_rd)) 


/*
+----------------------+--------------+----+--------------+---+--------------+
|31                 23 | 22        16 | 15 | 14         8 | 7 | 6         0  |
+----------------------+--------------+----+--------------+---+--------------+
|  opcode 0x65008080   | vfpu_rt[6-0] |    | vfpu_rs[6-0] |   | vfpu_rd[6-0] |
+----------------------+--------------+----+--------------+---+--------------+

  MatrixScale.Pair/Triple/Quad, multiply all components by scale factor

    vmscl.p %vfpu_rd, %vfpu_rs, %vfpu_rt   ; Scale 2x2 Matrix by %vfpu_rt
    vmscl.t %vfpu_rd, %vfpu_rs, %vfpu_rt   ; Scale 3x3 Matrix by %vfpu_rt
    vmscl.q %vfpu_rd, %vfpu_rs, %vfpu_rt   ; Scale 4x4 Matrix by %vfpu_rt

	%vfpu_rt:	VFPU Vector Source Register, Scale (sreg 0..127)
	%vfpu_rs:	VFPU Vector Source Register, Matrix ([p|t|q]reg 0..127)
	%vfpu_rd:	VFPU Vector Destination Register, Matrix ([s|p|t|q]reg 0..127)

    vfpu_mtx[%vfpu_rd] <- vfpu_mtx[%vfpu_rs] * vfpu_reg[%vfpu_rt]
*/
#define vmscl_p(vfpu_rd,vfpu_rs,vfpu_rt)  (0xf2000080 | ((vfpu_rt) << 16) | ((vfpu_rs) << 8) | (vfpu_rd))
#define vmscl_t(vfpu_rd,vfpu_rs,vfpu_rt)  (0xf2008000 | ((vfpu_rt) << 16) | ((vfpu_rs) << 8) | (vfpu_rd))
#define vmscl_q(vfpu_rd,vfpu_rs,vfpu_rt)  (0xf2008080 | ((vfpu_rt) << 16) | ((vfpu_rs) << 8) | (vfpu_rd))


/*
+--------------------------+--------------+--+--------------+-+--------------+
|31                     23 | 22        16 |15| 14         8 |7| 6         0  |
+--------------------------+--------------+--+--------------+-+--------------+
| opcode 0xf0000080 (p)    | vfpu_rt[6-0] | 0| vfpu_rs[6-0] |1| vfpu_rd[6-0] |
| opcode 0xf0008000 (t)    | vfpu_rt[6-0] | 1| vfpu_rs[6-0] |0| vfpu_rd[6-0] |
| opcode 0xf0008080 (q)    | vfpu_rt[6-0] | 1| vfpu_rs[6-0] |1| vfpu_rd[6-0] |
+--------------------------+--------------+--+--------------+-+--------------+

  MatrixMultiply.Pair/Triple/Quad

    vmmul.p %vfpu_rd, %vfpu_rs, %vfpu_rt   ; multiply 2 2x2 Submatrices
    vmmul.t %vfpu_rd, %vfpu_rs, %vfpu_rt   ; multiply 2 3x3 Submatrices
    vmmul.q %vfpu_rd, %vfpu_rs, %vfpu_rt   ; multiply 2 4x4 Matrices

	%vfpu_rd:	VFPU Matrix Destination Register ([p|t|q]reg 0..127)
	%vfpu_rs:	VFPU Matrix Source Register ([p|t|q]reg 0..127)
	%vfpu_rt:	VFPU Matrix Source Register ([p|t|q]reg 0..127)

    vfpu_mtx[%vfpu_rd] <- vfpu_mtx[%vfpu_rt] * vfpu_mtx[%vfpu_rs]
*/
#define vmmul_p(vfpu_rd, vfpu_rs, vfpu_rt) (0xf0000080 | ((vfpu_rt) << 16) | (((vfpu_rs) ^ 0x20) << 8) | (vfpu_rd)) 
#define vmmul_t(vfpu_rd, vfpu_rs, vfpu_rt) (0xf0008000 | ((vfpu_rt) << 16) | (((vfpu_rs) ^ 0x20) << 8) | (vfpu_rd)) 
#define vmmul_q(vfpu_rd, vfpu_rs, vfpu_rt) (0xf0008080 | ((vfpu_rt) << 16) | (((vfpu_rs) ^ 0x20) << 8) | (vfpu_rd)) 


/* 
+----------------------+--------------+----+--------------+---+--------------+ 
|31                 23 | 22        16 | 15 | 14         8 | 7 | 6         0  | 
+----------------------+--------------+----+--------------+---+--------------+ 
| opcode 0xf08  (p)    | vfpu_rt[6-0] |  0 | vfpu_rs[6-0] | 1 | vfpu_rd[6-0] | 
| opcode 0xf10  (t)    | vfpu_rt[6-0] |  1 | vfpu_rs[6-0] | 0 | vfpu_rd[6-0] | 
| opcode 0xf18  (q)    | vfpu_rt[6-0] |  1 | vfpu_rs[6-0] | 1 | vfpu_rd[6-0] | 
+----------------------+--------------+----+--------------+---+--------------+ 

  VectorTransform.Pair/Triple/Quad (Matrix-Vector product)

    vtfm2.p %vfpu_rd, %vfpu_rs, %vfpu_rt ; Transform pair vector by 2x2 matrix 
    vtfm3.t %vfpu_rd, %vfpu_rs, %vfpu_rt ; Transform triple vector by 3x3 matrix 
    vtfm4.q %vfpu_rd, %vfpu_rs, %vfpu_rt ; Transform quad vector by 4x4 matrix 

	%vfpu_rt:   VFPU Vector Source Register ([p|t|q]reg 0..127) 
	%vfpu_rs:   VFPU Matrix Source Register ([p|t|q]matrix 0..127) 
	%vfpu_rd:   VFPU Vector Destination Register ([p|t|q]reg 0..127) 

    vfpu_regs[%vfpu_rd] <- transform(vfpu_matrix[%vfpu_rs], vfpu_vector[%vfpu_rt]) 
*/ 
#define vtfm2_p(vfpu_rd,vfpu_rs,vfpu_rt)  (0xF0800080 | ((vfpu_rt) << 16) | ((vfpu_rs) << 8) | (vfpu_rd)) 
#define vtfm3_t(vfpu_rd,vfpu_rs,vfpu_rt)  (0xF1008000 | ((vfpu_rt) << 16) | ((vfpu_rs) << 8) | (vfpu_rd)) 
#define vtfm4_q(vfpu_rd,vfpu_rs,vfpu_rt)  (0xF1808080 | ((vfpu_rt) << 16) | ((vfpu_rs) << 8) | (vfpu_rd)) 


/* 
+----------------------+--------------+----+--------------+---+--------------+ 
|31                 23 | 22        16 | 15 | 14         8 | 7 | 6         0  | 
+----------------------+--------------+----+--------------+---+--------------+ 
| opcode 0xf08 (p)     | vfpu_rt[6-0] |  0 | vfpu_rs[6-0] | 0 | vfpu_rd[6-0] | 
| opcode 0xf10 (t)     | vfpu_rt[6-0] |  0 | vfpu_rs[6-0] | 1 | vfpu_rd[6-0] | 
| opcode 0xf18 (q)     | vfpu_rt[6-0] |  1 | vfpu_rs[6-0] | 0 | vfpu_rd[6-0] | 
+----------------------+--------------+----+--------------+---+--------------+ 

  VectorHomogeneousTransform.Pair/Triple/Quad 

    vhtfm2.p %vfpu_rd, %vfpu_rs, %vfpu_rt ; Homogeneous transform pair vector by 2x2 matrix 
    vhtfm3.t %vfpu_rd, %vfpu_rs, %vfpu_rt ; Homogeneous transform triple vector by 3x3 matrix 
    vhtfm4.q %vfpu_rd, %vfpu_rs, %vfpu_rt ; Homogeneous transform quad vector by 4x4 matrix 

        %vfpu_rt:   VFPU Vector Source Register ([p|t|q]reg 0..127) 
        %vfpu_rs:   VFPU Matrix Source Register ([p|t|q]matrix 0..127) 
        %vfpu_rd:   VFPU Vector Destination Register ([p|t|q]reg 0..127) 

    vfpu_regs[%vfpu_rd] <- homeogenoustransform(vfpu_matrix[%vfpu_rs], vfpu_vector[%vfpu_rt]) 
*/ 
#define vhtfm2_p(vfpu_rd,vfpu_rs,vfpu_rt)  (0xF0800000 | ((vfpu_rt) << 16) | ((vfpu_rs) << 8) | (vfpu_rd)) 
#define vhtfm3_t(vfpu_rd,vfpu_rs,vfpu_rt)  (0xF1000080 | ((vfpu_rt) << 16) | ((vfpu_rs) << 8) | (vfpu_rd)) 
#define vhtfm4_q(vfpu_rd,vfpu_rs,vfpu_rt)  (0xF1808000 | ((vfpu_rt) << 16) | ((vfpu_rs) << 8) | (vfpu_rd))


/*
+------------------------------------------+--------------+---+--------------+
|31                                     15 | 14         8 | 7 | 6          0 |
+------------------------------------------+--------------+---+--------------+
|              opcode 0xd0598080           | vfpu_rs[6-0] | 1 | vfpu_rd[6-0] |
+------------------------------------------+--------------+---+--------------+

    vt4444.q %vfpu_rd, %vfpu_rs    ; ?????? color conversion ????????????

	%vfpu_rd:	VFPU Vector Destination Register (qreg 0..127)
	%vfpu_rs:	VFPU Vector Source Register (qreg 0..127)

    ???????????????????
*/
#define vt4444_q(vfpu_rd,vfpu_rs)  (0xd0598080 | ((vfpu_rs) << 8) | (vfpu_rd))


/*
+------------------------------------------+--------------+---+--------------+
|31                                     15 | 14         8 | 7 | 6          0 |
+------------------------------------------+--------------+---+--------------+
|              opcode 0xd05a8080           | vfpu_rs[6-0] | 1 | vfpu_rd[6-0] |
+------------------------------------------+--------------+---+--------------+

    vt5551.q %vfpu_rd, %vfpu_rs    ; ?????? color conversion ????????????

	%vfpu_rd:	VFPU Vector Destination Register (qreg 0..127)
	%vfpu_rs:	VFPU Vector Source Register (qreg 0..127)

    ???????????????????
*/
#define vt5551_q(vfpu_rd,vfpu_rs)  (0xd05a8080 | ((vfpu_rs) << 8) | (vfpu_rd))


/*
+------------------------------------------+--------------+---+--------------+
|31                                     15 | 14         8 | 7 | 6          0 |
+------------------------------------------+--------------+---+--------------+
|              opcode 0xd05b8080           | vfpu_rs[6-0] | 1 | vfpu_rd[6-0] |
+------------------------------------------+--------------+---+--------------+

    vt5650.q %vfpu_rd, %vfpu_rs    ; ?????? color conversion ????????????

	%vfpu_rd:	VFPU Vector Destination Register (qreg 0..127)
	%vfpu_rs:	VFPU Vector Source Register (qreg 0..127)

    ???????????????????
*/
#define vt5650_q(vfpu_rd,vfpu_rs)  (0xd05b8080 | ((vfpu_rs) << 8) | (vfpu_rd))



/* helpers for direct __asm__ use: */
#define _cgen_stringify(x)  #x
#define cgen_stringify(x)   _cgen_stringify(x)
#define cgen_asm(x)         ".loc 1 " cgen_stringify(__LINE__) " 0\n\t.word " cgen_stringify(x) "\n\t"

#endif


#if 0   /* list of unimplemented insns, from svn.pspdev.org/binutils-psp/opcodes/mips-opc.c */

{"break",   "B",        0x0000000d, 0xfc00003f, TRAP,                   0,              AL      },
{"cache",   "k,o(b)",   0xbc000000, 0xfc000000, RD_b,                   0,              I3|I32|T3|AL},
{"clo",     "d,s",      0x00000017, 0xfc1f07ff, WR_d|RD_s,              0,              AL      },
{"clz",     "d,s",      0x00000016, 0xfc1f07ff, WR_d|RD_s,              0,              AL      },
{"max",     "d,v,t",    0x0000002c, 0xfc0007ff, WR_d|RD_s|RD_t,         0,              AL      },
{"min",     "d,v,t",    0x0000002d, 0xfc0007ff, WR_d|RD_s|RD_t,         0,              AL      },
{"eret",    "",         0x42000018, 0xffffffff, 0,                      0,              I3|I32|AL       },
{"ext",     "t,r,+A,+C", 0x7c000000, 0xfc00003f, WR_t|RD_s,             0,              I33|AL  },
{"ins",     "t,r,+A,+B", 0x7c000004, 0xfc00003f, WR_t|RD_s,             0,              I33|AL  },
{"ldl",     "t,o(b)",   0x68000000, 0xfc000000, LDD|WR_t|RD_b,          0,              I3|AL   },
{"madd",    "s,t",      0x0000001c, 0xfc00ffff, RD_s|RD_t|WR_HILO|IS_M,      0,         AL      },
{"maddu",   "s,t",      0x0000001d, 0xfc00ffff, RD_s|RD_t|WR_HILO|IS_M,      0,         AL      },
{"movn",    "d,v,t",    0x0000000b, 0xfc0007ff, WR_d|RD_s|RD_t,         0,              I4|I32|AL       },
{"movz",    "d,v,t",    0x0000000a, 0xfc0007ff, WR_d|RD_s|RD_t,         0,              I4|I32|AL       },
{"msub",    "s,t",      0x0000002e, 0xfc00ffff, RD_s|RD_t|WR_HILO|IS_M, 0,              AL      },
{"msubu",   "s,t",      0x0000002f, 0xfc00ffff, RD_s|RD_t|WR_HILO|IS_M, 0,              AL      },
{"ror",     "d,w,<",    0x00200002, 0xffe0003f, WR_d|RD_t,              0,              N5|I33|AL       },
{"rorv",    "d,t,s",    0x00000046, 0xfc0007ff, RD_t|RD_s|WR_d,         0,              N5|I33|AL       },
{"rotrv",   "d,t,s",    0x00000046, 0xfc0007ff, RD_t|RD_s|WR_d,         0,              I33|AL  },
{"sdl",     "t,o(b)",   0xb0000000, 0xfc000000, SM|RD_t|RD_b,           0,              I3|AL   },
{"seb",     "d,w",      0x7c000420, 0xffe007ff, WR_d|RD_t,              0,              I33|AL  },
{"seh",     "d,w",      0x7c000620, 0xffe007ff, WR_d|RD_t,              0,              I33|AL  },
{"wsbh",    "d,w",      0x7c0000a0, 0xffe007ff, WR_d|RD_t,              0,              I33|AL  },
{"wsbw",    "d,t",      0x7c0000e0, 0xffe007ff, WR_d|RD_t,              0,              AL      },
{"bitrev",  "d,t",      0x7c000520, 0xffe007ff, WR_d|RD_t,              0,              AL      },
{"bvf",     "?c,p",             0x49000000, 0xffe30000, CBD|RD_CC,      0,              AL      },
{"bvfl",    "?c,p",             0x49020000, 0xffe30000, CBL|RD_CC,      0,              AL      },
{"bvt",     "?c,p",             0x49010000, 0xffe30000, CBD|RD_CC,      0,              AL      },
{"bvtl",    "?c,p",             0x49030000, 0xffe30000, CBL|RD_CC,      0,              AL      },
{"vwb.q",   "?n3x,?o(b)",       0xf8000002, 0xfc000002, SM|RD_s|RD_C2,  0,              AL      },
{"mtv",     "t,?d0z",           0x48e00000, 0xffe0ff80, LCD|WR_t|WR_C2, 0,              AL      },
{"mfv",     "t,?d0z",           0x48600000, 0xffe0ff80, COD|RD_t|WR_CC|RD_C2, 0,        AL      },
{"mtvc",    "t,?q",             0x48e00000, 0xffe0ff00, LCD|WR_t|WR_C2, 0,              AL      },
{"mfvc",    "t,?q",             0x48600000, 0xffe0ff00, COD|RD_t|WR_CC|RD_C2, 0,        AL      },
{"vmtvc",   "?q,?s0y",          0xd0510000, 0xffff8000, WR_C2,          0,              AL      },
{"vmfvc",   "?d0z,?r",          0xd0500000, 0xffff0080, RD_C2,          0,              AL      },
{"vcmp.q",  "?f2,?s3s,?t3t",    0x6c008080, 0xff8080f0, RD_C2,          0,              AL      },
{"vcmp.q",  "?f1,?s3s",         0x6c008080, 0xffff80f0, RD_C2,          0,              AL      },
{"vcmp.q",  "?f0",              0x6c008080, 0xfffffff0, RD_C2,          0,              AL      },
{"vscmp.q", "?d3d,?s3s,?t3t",   0x6e808080, 0xff808080, RD_C2,          0,              AL      },
{"vsge.q",  "?d3d,?s3s,?t3t",   0x6f008080, 0xff808080, RD_C2,          0,              AL      },
{"vslt.q",  "?d3d,?s3s,?t3t",   0x6f808080, 0xff808080, RD_C2,          0,              AL      },
{"vi2uc.q", "?d0m,?s3w",        0xd03c8080, 0xffff8080, RD_C2,          0,              AL      },
{"vi2c.q",  "?d0m,?s3w",        0xd03d8080, 0xffff8080, RD_C2,          0,              AL      },
{"vi2us.q", "?d1m,?s3w",        0xd03e8080, 0xffff8080, RD_C2,          0,              AL      },
{"vi2s.q",  "?d1m,?s3w",        0xd03f8080, 0xffff8080, RD_C2,          0,              AL      },
{"vrndi.q", "?d3z",             0xd0218080, 0xffffff80, RD_C2,          0,              AL      },
{"vrndf1.q", "?d3z",            0xd0228080, 0xffffff80, RD_C2,          0,              AL      },
{"vrndf2.q", "?d3z",            0xd0238080, 0xffffff80, RD_C2,          0,              AL      },
{"vf2h.q",  "?d1m,?s3s",        0xd0328080, 0xffff8080, RD_C2,          0,              AL      },
{"vsrt1.q", "?d3d,?s3s",        0xd0408080, 0xffff8080, RD_C2,          0,              AL      },
{"vsrt2.q", "?d3d,?s3s",        0xd0418080, 0xffff8080, RD_C2,          0,              AL      },
{"vsrt3.q", "?d3d,?s3s",        0xd0488080, 0xffff8080, RD_C2,          0,              AL      },
{"vsrt4.q", "?d3d,?s3s",        0xd0498080, 0xffff8080, RD_C2,          0,              AL      },
{"vbfy1.q", "?d3d,?s3s",        0xd0428080, 0xffff8080, RD_C2,          0,              AL      },
{"vbfy2.q", "?d3d,?s3s",        0xd0438080, 0xffff8080, RD_C2,          0,              AL      },
{"vocp.q",  "?d3d,?s3y",        0xd0448080, 0xffff8080, RD_C2,          0,              AL      },
{"vf2in.q", "?d3m,?s3s,?b",     0xd2008080, 0xffe08080, RD_C2,          0,              AL      },
{"vf2iu.q", "?d3m,?s3s,?b",     0xd2408080, 0xffe08080, RD_C2,          0,              AL      },
{"vf2id.q", "?d3m,?s3s,?b",     0xd2608080, 0xffe08080, RD_C2,          0,              AL      },
{"vi2f.q",  "?d3d,?s3w,?b",     0xd2808080, 0xffe08080, RD_C2,          0,              AL      },
{"vcmovt.q", "?d3d,?s3s,?e",    0xd2a08080, 0xfff88080, RD_C2,          0,              AL      },
{"vcmovf.q", "?d3d,?s3s,?e",    0xd2a88080, 0xfff88080, RD_C2,          0,              AL      },
{"vqmul.q", "?v3z,?s3y,?t3x",   0xf2808080, 0xff808080, RD_C2,          0,              AL      },
{"vrot.q",  "?x3z,?s0y,?w",     0xf3a08080, 0xffe08080, RD_C2,          0,              AL      },
{"vt4444.q", "?d1z,?s3w",       0xd0598080, 0xffff8080, RD_C2,          0,              AL      },
{"vt5551.q", "?d1z,?s3w",       0xd05a8080, 0xffff8080, RD_C2,          0,              AL      },
{"vt5650.q", "?d1z,?s3w",       0xd05b8080, 0xffff8080, RD_C2,          0,              AL      },
{"vcrs.t",  "?d2d,?s2y,?t2x",   0x66808000, 0xff808080, RD_C2,          0,              AL      },
{"vcmp.t",  "?f2,?s2s,?t2t",    0x6c008000, 0xff8080f0, RD_C2,          0,              AL      },
{"vcmp.t",  "?f1,?s2s",         0x6c008000, 0xffff80f0, RD_C2,          0,              AL      },
{"vcmp.t",  "?f0",              0x6c008000, 0xfffffff0, RD_C2,          0,              AL      },
{"vscmp.t", "?d2d,?s2s,?t2t",   0x6e808000, 0xff808080, RD_C2,          0,              AL      },
{"vsge.t",  "?d2d,?s2s,?t2t",   0x6f008000, 0xff808080, RD_C2,          0,              AL      },
{"vslt.t",  "?d2d,?s2s,?t2t",   0x6f808000, 0xff808080, RD_C2,          0,              AL      },
{"vrndi.t", "?d2z",             0xd0218000, 0xffffff80, RD_C2,          0,              AL      },
{"vrndf1.t", "?d2z",            0xd0228000, 0xffffff80, RD_C2,          0,              AL      },
{"vrndf2.t", "?d2z",            0xd0238000, 0xffffff80, RD_C2,          0,              AL      },
{"vocp.t",  "?d2d,?s2y",        0xd0448000, 0xffff8080, RD_C2,          0,              AL      },
{"vf2in.t", "?d2m,?s2s,?b",     0xd2008000, 0xffe08080, RD_C2,          0,              AL      },
{"vf2iu.t", "?d2m,?s2s,?b",     0xd2408000, 0xffe08080, RD_C2,          0,              AL      },
{"vf2id.t", "?d2m,?s2s,?b",     0xd2608000, 0xffe08080, RD_C2,          0,              AL      },
{"vi2f.t",  "?d2d,?s2w,?b",     0xd2808000, 0xffe08080, RD_C2,          0,              AL      },
{"vcmovt.t", "?d2d,?s2s,?e",    0xd2a08000, 0xfff88080, RD_C2,          0,              AL      },
{"vcmovf.t", "?d2d,?s2s,?e",    0xd2a88000, 0xfff88080, RD_C2,          0,              AL      },
{"vrot.t",  "?x2z,?s0y,?w",     0xf3a08000, 0xffe08080, RD_C2,          0,              AL      },
{"vcrsp.t", "?d2z,?s2y,?t2x",   0xf2808000, 0xff808080, RD_C2,          0,              AL      },
{"vdet.p",  "?d0d,?s1s,?t1x",   0x67000080, 0xff808080, RD_C2,          0,              AL      },
{"vcmp.p",  "?f2,?s1s,?t1t",    0x6c000080, 0xff8080f0, RD_C2,          0,              AL      },
{"vcmp.p",  "?f1,?s1s",         0x6c000080, 0xffff80f0, RD_C2,          0,              AL      },
{"vcmp.p",  "?f0",              0x6c000080, 0xfffffff0, RD_C2,          0,              AL      },
{"vscmp.p", "?d1d,?s1s,?t1t",   0x6e800080, 0xff808080, RD_C2,          0,              AL      },
{"vsge.p",  "?d1d,?s1s,?t1t",   0x6f000080, 0xff808080, RD_C2,          0,              AL      },
{"vslt.p",  "?d1d,?s1s,?t1t",   0x6f800080, 0xff808080, RD_C2,          0,              AL      },
{"vi2us.p", "?d0m,?s1w",        0xd03e0080, 0xffff8080, RD_C2,          0,              AL      },
{"vi2s.p",  "?d0m,?s1w",        0xd03f0080, 0xffff8080, RD_C2,          0,              AL      },
{"vrndi.p", "?d1z",             0xd0210080, 0xffffff80, RD_C2,          0,              AL      },
{"vrndf1.p", "?d1z",            0xd0220080, 0xffffff80, RD_C2,          0,              AL      },
{"vrndf2.p", "?d1z",            0xd0230080, 0xffffff80, RD_C2,          0,              AL      },
{"vf2h.p",  "?d0m,?s1s",        0xd0320080, 0xffff8080, RD_C2,          0,              AL      },
{"vh2f.p",  "?d3d,?s1y",        0xd0330080, 0xffff8080, RD_C2,          0,              AL      },
{"vbfy1.p", "?d1d,?s1s",        0xd0420080, 0xffff8080, RD_C2,          0,              AL      },
{"vocp.p",  "?d1d,?s1y",        0xd0440080, 0xffff8080, RD_C2,          0,              AL      },
{"vsocp.p", "?d3z,?s1y",        0xd0450080, 0xffff8080, RD_C2,          0,              AL      },
{"vf2in.p", "?d1m,?s1s,?b",     0xd2000080, 0xffe08080, RD_C2,          0,              AL      },
{"vf2iu.p", "?d1m,?s1s,?b",     0xd2400080, 0xffe08080, RD_C2,          0,              AL      },
{"vf2id.p", "?d1m,?s1s,?b",     0xd2600080, 0xffe08080, RD_C2,          0,              AL      },
{"vi2f.p",  "?d1d,?s1w,?b",     0xd2800080, 0xffe08080, RD_C2,          0,              AL      },
{"vcmovt.p", "?d1d,?s1s,?e",    0xd2a00080, 0xfff88080, RD_C2,          0,              AL      },
{"vcmovf.p", "?d1d,?s1s,?e",    0xd2a80080, 0xfff88080, RD_C2,          0,              AL      },
{"vrot.p",  "?x1z,?s0y,?w",     0xf3a00080, 0xffe08080, RD_C2,          0,              AL      },
{"vcmp.s",  "?f2,?s0s,?t0t",    0x6c000000, 0xff8080f0, RD_C2,          0,              AL      },
{"vcmp.s",  "?f1,?s0s",         0x6c000000, 0xffff80f0, RD_C2,          0,              AL      },
{"vcmp.s",  "?f0",              0x6c000000, 0xfffffff0, RD_C2,          0,              AL      },
{"vscmp.s", "?d0d,?s0s,?t0t",   0x6e800000, 0xff808080, RD_C2,          0,              AL      },
{"vsge.s",  "?d0d,?s0s,?t0t",   0x6f000000, 0xff808080, RD_C2,          0,              AL      },
{"vslt.s",  "?d0d,?s0s,?t0t",   0x6f800000, 0xff808080, RD_C2,          0,              AL      },
{"vrnds.s", "?s0y",             0xd0200000, 0xffff80ff, RD_C2,          0,              AL      },
{"vrndi.s", "?d0d",             0xd0210000, 0xffffff80, RD_C2,          0,              AL      },
{"vrndf1.s", "?d0d",            0xd0220000, 0xffffff80, RD_C2,          0,              AL      },
{"vrndf2.s", "?d0d",            0xd0230000, 0xffffff80, RD_C2,          0,              AL      },
{"vh2f.s",  "?d1d,?s0y",        0xd0330000, 0xffff8080, RD_C2,          0,              AL      },
{"vsbz.s",  "?d0d,?s0s",        0xd0360000, 0xffff8080, RD_C2,          0,              AL      },
{"vsbn.s",  "?d0d,?s0s,?t0t",   0x61000000, 0xff808080, RD_C2,          0,              AL      },
{"vlgb.s",  "?d0d,?s0s",        0xd0370000, 0xffff8080, RD_C2,          0,              AL      },
{"vocp.s",  "?d0d,?s0y",        0xd0440000, 0xffff8080, RD_C2,          0,              AL      },
{"vsocp.s", "?d1z,?s0y",        0xd0450000, 0xffff8080, RD_C2,          0,              AL      },
{"vf2in.s", "?d0m,?s0s,?b",     0xd2000000, 0xffe08080, RD_C2,          0,              AL      },
{"vf2iu.s", "?d0m,?s0s,?b",     0xd2400000, 0xffe08080, RD_C2,          0,              AL      },
{"vf2id.s", "?d0m,?s0s,?b",     0xd2600000, 0xffe08080, RD_C2,          0,              AL      },
{"vi2f.s",  "?d0d,?s0w,?b",     0xd2800000, 0xffe08080, RD_C2,          0,              AL      },
{"vcmovt.s", "?d0d,?s0s,?e",    0xd2a00000, 0xfff88080, RD_C2,          0,              AL      },
{"vcmovf.s", "?d0d,?s0s,?e",    0xd2a80000, 0xfff88080, RD_C2,          0,              AL      },
{"vwbn.s",  "?d0d,?s0s,?i",     0xd3000000, 0xff008080, RD_C2,          0,              AL      },
{"vpfxs",   "?0,?1,?2,?3",      0xdc000000, 0xff000000, RD_C2,          0,              AL      },
{"vpfxt",   "?0,?1,?2,?3",      0xdd000000, 0xff000000, RD_C2,          0,              AL      },
{"vpfxd",   "?4,?5,?6,?7",      0xde000000, 0xff000000, RD_C2,          0,              AL      },
{"vfim.s",  "?t0d,?u",          0xdf800000, 0xff800000, RD_C2,          0,              AL      },
{"vnop",    "",                 0xffff0000, 0xffffffff, RD_C2,          0,              AL      },
{"vflush",  "",                 0xffff040d, 0xffffffff, RD_C2,          0,              AL      },
{"vsync",   "",                 0xffff0320, 0xffffffff, RD_C2,          0,              AL      },
{"vsync",   "i",                0xffff0000, 0xffff0000, RD_C2,          0,              AL      },


#endif

