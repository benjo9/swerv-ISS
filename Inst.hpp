// -*- c++ -*-

#pragma once

#include <cstdint>
#include <type_traits>


namespace WdRiscv
{

  // Structures useful for encoding/decoding a risc-v instructions.

  /// Pack/unpack an r-form instruction.
  union RFormInst
  {
    /// Constructor: Either pass a valid r-form value or start with any
    /// value and then use an encode method.
    RFormInst(uint32_t inst)
    { code = inst; }

    /// Return top 5-bits of instruction (for atomic insts).
    unsigned top5() const
    { return funct7 >> 2; }

    /// Return aq field for atomic instructions.
    bool aq() const
    { return (funct7 >> 1) & 1; }

    /// Return r1 field for atomic instructions.
    bool r1() const
    { return funct7 & 1; }

    /// Encode "add rd, rs1, rs2" into this object.
    bool encodeAdd(unsigned rd, unsigned rs1, unsigned rs2);

    /// Encode "sub rd, rs1, rs2" into this object.
    bool encodeSub(unsigned rd, unsigned rs1, unsigned rs2);

    /// Encode "sll rd, rs1, rs2" into this object.
    bool encodeSll(unsigned rd, unsigned rs1, unsigned rs2);

    /// Encode "slt rd, rs1, rs2" into this object.
    bool encodeSlt(unsigned rd, unsigned rs1, unsigned rs2);

    /// Encode "sltu rd, rs1, rs2" into this object.
    bool encodeSltu(unsigned rd, unsigned rs1, unsigned rs2);

    /// Encode "xor rd, rs1, rs2" into this object.
    bool encodeXor(unsigned rd, unsigned rs1, unsigned rs2);

    /// Encode "srl rd, rs1, rs2" into this object.
    bool encodeSrl(unsigned rd, unsigned rs1, unsigned rs2);

    /// Encode "sra rd, rs1, rs2" into this object.
    bool encodeSra(unsigned rd, unsigned rs1, unsigned rs2);

    /// Encode "or rd, rs1, rs2" into this object.
    bool encodeOr(unsigned rd, unsigned rs1, unsigned rs2);

    /// Encode "and rd, rs1, rs2" into this object.
    bool encodeAnd(unsigned rd, unsigned rs1, unsigned rs2);

    /// Encode "addw rd, rs1, rs2" into this object.
    bool encodeAddw(unsigned rd, unsigned rs1, unsigned rs2);

    /// Encode "addw rd, rs1, rs2" into this object.
    bool encodeSubw(unsigned rd, unsigned rs1, unsigned rs2);

    /// Encode "sllw rd, rs1, rs2" into this object.
    bool encodeSllw(unsigned rd, unsigned rs1, unsigned rs2);

    /// Encode "srlw rd, rs1, rs2" into this object.
    bool encodeSrlw(unsigned rd, unsigned rs1, unsigned rs2);

    /// Encode "srlw rd, rs1, rs2" into this object.
    bool encodeSraw(unsigned rd, unsigned rs1, unsigned rs2);

    /// Encode "mul rd, rs1, rs2" into this object.
    bool encodeMul(unsigned rd, unsigned rs1, unsigned rs2);

    /// Encode "mulh rd, rs1, rs2" into this object.
    bool encodeMulh(unsigned rd, unsigned rs1, unsigned rs2);

    /// Encode "mulhsu rd, rs1, rs2" into this object.
    bool encodeMulhsu(unsigned rd, unsigned rs1, unsigned rs2);

    /// Encode "mulhu rd, rs1, rs2" into this object.
    bool encodeMulhu(unsigned rd, unsigned rs1, unsigned rs2);

    /// Encode "div rd, rs1, rs2" into this object.
    bool encodeDiv(unsigned rd, unsigned rs1, unsigned rs2);

    /// Encode "divu rd, rs1, rs2" into this object.
    bool encodeDivu(unsigned rd, unsigned rs1, unsigned rs2);

    /// Encode "rem rd, rs1, rs2" into this object.
    bool encodeRem(unsigned rd, unsigned rs1, unsigned rs2);

    /// Encode "remu rd, rs1, rs2" into this object.
    bool encodeRemu(unsigned rd, unsigned rs1, unsigned rs2);

    /// Encode "mulw rd, rs1, rs2" into this object.
    bool encodeMulw(unsigned rd, unsigned rs1, unsigned rs2);

    /// Encode "divw rd, rs1, rs2" into this object.
    bool encodeDivw(unsigned rd, unsigned rs1, unsigned rs2);

    /// Encode "divuw rd, rs1, rs2" into this object.
    bool encodeDivuw(unsigned rd, unsigned rs1, unsigned rs2);

    /// Encode "remw rd, rs1, rs2" into this object.
    bool encodeRemw(unsigned rd, unsigned rs1, unsigned rs2);

    /// Encode "remuw rd, rs1, rs2" into this object.
    bool encodeRemuw(unsigned rd, unsigned rs1, unsigned rs2);
    uint32_t code;

    struct
    {
      unsigned opcode : 7;
      unsigned rd     : 5;
      unsigned funct3 : 3;
      unsigned rs1    : 5;
      unsigned rs2    : 5;
      unsigned funct7 : 7;
    };
  };


  /// Pack/unpack a b-form instruction.
  union BFormInst
  {
    /// Constructor: Either pass a valid b-form value or start with any
    /// value and then use an encode method.
    BFormInst(uint32_t inst)
    { code = inst; }

    /// Sign extend the instruction immediate value to a value of type
    /// T.
    template <typename T> T immed() const
    { return T(typename std::make_signed<T>::type((imm12   << 12)  |
						  (imm11   << 11)  |
						  (imm10_5 << 5)   |
						  (imm4_1  << 1))); }

    /// Encode a "beq rs1, rs2, imm" into this object.
    bool encodeBeq(unsigned rs1, unsigned rs2, int imm);

    /// Encode a "bne rs1, rs2, imm" into this object.
    bool encodeBne(unsigned rs1, unsigned rs2, int imm);

    /// Encode a "blt rs1, rs2, imm" into this object.
    bool encodeBlt(unsigned rs1, unsigned rs2, int imm);

    /// Encode a "bge rs1, rs3, imm" into this object.
    bool encodeBge(unsigned rs1, unsigned rs2, int imm);

    /// Encode a "bltu rs1, rs2, imm" into this object.
    bool encodeBltu(unsigned rs1, unsigned rs2, int imm);

    /// Encode a "bgeu rs1, rs2, imm" into this object.
    bool encodeBgeu(unsigned rs1, unsigned rs2, int imm);

    uint32_t code;
    
    struct
    {
      unsigned opcode  : 7;
      unsigned imm11   : 1;
      unsigned imm4_1  : 4;
      unsigned funct3  : 3;
      unsigned rs1     : 5;
      unsigned rs2     : 5;
      unsigned imm10_5 : 6;
      int      imm12   : 1;  // Note: int for sign extension
    };
  };


  /// Pack/unpack a i-form instruction.
  union IFormInst
  {
    /// Constructor: Either pass a valid i-form value or start with any
    /// value and then use an encode method.
    IFormInst(uint32_t inst)
    { code = inst; }

    /// Sign extend immediate value to (signed) type T.
    template <typename T> T immed() const
    { return T(typename std::make_signed<T>::type(fields.imm)); }

    /// Return immediate value as unsigned.
    unsigned uimmed() const  // Immediate as unsigned.
    { return fields.imm & 0xfff; }

    /// Return top-7 bits of instruction.
    unsigned top7() const
    { return fields2.top7; }

    /// Return pred field (for fence instruction).
    unsigned pred() const
    { return (uimmed() >> 4) & 0xf; }

    /// Return succ field (for fence instruction).
    unsigned succ() const
    { return uimmed() & 0xf; }

    /// Return top 4-bits of instruction (for fence).
    unsigned top4() const
    { return uimmed() >> 8; }

    /// Return the rs2 bits (for sfence.vma).
    unsigned rs2() const
    { return fields2.shamt; }

    /// Encode "addi rd, rs1, imm" into this object returning
    /// true on success and false if any of the parameters are out of
    /// range.
    bool encodeAddi(unsigned rd, unsigned rs1, int imm);

    /// Encode "andi rd, rs1, imm" into this object.
    bool encodeAndi(unsigned rd, unsigned rs1, int imm);

    /// Encode "ebreak" into this object.
    bool encodeEbreak();

    /// Encode "ecall" into this object.
    bool encodeEcall();

    /// Encode "jalr rd, offset(rs1)" into this object.
    bool encodeJalr(unsigned rd, unsigned rs1, int offset);

    /// Encode "lb rd, offset(rs1)" into this object.
    bool encodeLb(unsigned rd, unsigned rs1, int offset);

    /// Encode "lh rd, offset(rs1)" into this object.
    bool encodeLh(unsigned rd, unsigned rs1, int offset);

    /// Encode "lw rd, offset(rs1)" into this object.
    bool encodeLw(unsigned rd, unsigned rs1, int offset);

    /// Encode "lbu rd, offset(rs1)" into this object.
    bool encodeLbu(unsigned rd, unsigned rs1, int offset);

    /// Encode "lhu rd, offset(rs1)" into this object.
    bool encodeLhu(unsigned rd, unsigned rs1, int offset);

    /// Encode "lwu rd, offset(rs1)" into this object.
    bool encodeLwu(unsigned rd, unsigned rs1, int offset);

    /// Encode "ld rd, offset(rs1) into this obejct.
    bool encodeLd(unsigned rd, unsigned rs1, int offset);

    /// Encode "slli rd, rs1, shamt" into this object.
    bool encodeSlli(unsigned rd, unsigned rs1, unsigned shamt);

    /// Encode "srli rd, rs1, shamt" into this object.
    bool encodeSrli(unsigned rd, unsigned rs1, unsigned shamt);

    /// Encode "srai rd, rs1, shamt" into this object.
    bool encodeSrai(unsigned rd, unsigned rs1, unsigned shamt);

    /// Encode "slti rd, rs1, imm" into this object.
    bool encodeSlti(unsigned rd, unsigned rs1, int imm);

    /// Encode "sltiu rd, rs1, imm" into this object.
    bool encodeSltiu(unsigned rd, unsigned rs1, int imm);

    /// Encode "xori rd, rs1, imm" into this object.
    bool encodeXori(unsigned rd, unsigned rs1, int imm);

    /// Encode "ori rd, rs1, imm" into this object.
    bool encodeOri(unsigned rd, unsigned rs1, int imm);

    /// Encode "addiw rd, rs1, imm" into this object returning
    /// true on success and false if any of the parameters are out of
    /// range.
    bool encodeAddiw(unsigned rd, unsigned rs1, int imm);

    /// Encode "slliw rd, rs1, shamt" into this object.
    bool encodeSlliw(unsigned rd, unsigned rs1, unsigned shamt);

    /// Encode "srliw rd, rs1, shamt" into this object.
    bool encodeSrliw(unsigned rd, unsigned rs1, unsigned shamt);

    /// Encode "sraiw rd, rs1, shamt" into this object.
    bool encodeSraiw(unsigned rd, unsigned rs1, unsigned shamt);

    uint32_t code;

    struct
    {
      unsigned opcode : 7;
      unsigned rd     : 5;
      unsigned funct3 : 3;
      unsigned rs1    : 5;
      int      imm    : 12;
    } fields;

    struct
    {
      unsigned opcode : 7;
      unsigned rd     : 5;
      unsigned funct3 : 3;
      unsigned rs1    : 5;
      unsigned shamt  : 5;
      unsigned top7   : 7;
    } fields2;
  };


  /// Pack/unpack a s-form instruction.
  union SFormInst
  {
    /// Constructor: Either pass a valid s-form value or start with any
    /// value and then use an encode method.
    SFormInst(uint32_t inst)
    { code = inst; }

    /// Sign extend immediate value to (signed) type T.
    template <typename T> T immed() const
    { return T(typename std::make_signed<T>::type((imm11_5 << 5) | imm4_0)); }

    /// Encode "sb rs2, imm(rs1)" into this object.
    bool encodeSb(unsigned rs1, unsigned rs2, int imm);

    /// Encode "sh rs2, imm(rs1)" into this object.
    bool encodeSh(unsigned rs1, unsigned rs2, int imm);

    /// Encode "sw rs2, imm(rs1)" into this object.
    bool encodeSw(unsigned rs1, unsigned rs2, int imm);

    /// Encode "sd rs2, imm(rs1)" into this object.
    bool encodeSd(unsigned rs1, unsigned rs2, int imm);

    uint32_t code;

    struct
    {
      unsigned opcode  : 7;
      unsigned imm4_0  : 5;
      unsigned funct3  : 3;
      unsigned rs1     : 5;
      unsigned rs2     : 5;
      int      imm11_5 : 7;
    };
  };


  /// Pack/unpack a u-form instruction.
  union UFormInst
  {
    /// Constructor: Either pass a valid u-form value or start with
    /// any value and then use an encode method.
    UFormInst(uint32_t inst)
    { code = inst; }

    /// Sign extend immediate value to (signed) type T.
    template <typename T> T immed() const
    { return T(typename std::make_signed<T>::type(imm << 12)); }

    /// Encode "lui rd, immed" into this object.
    bool encodeLui(unsigned rd, int immed);

    /// Encode "auipc rd, immed" into this object.
    bool encodeAuipc(unsigned rd, int immed);

    uint32_t code;

    struct
    {
      unsigned opcode  : 7;
      unsigned rd      : 5;
      int      imm     : 20;
    };
  };


  /// Pack/unpack a j-form instruction.
  union JFormInst
  {
    /// Constructor: Either pass a valid u-form value or start with
    /// any value and then use an encode method.
    JFormInst(uint32_t inst)
    { code = inst; }

    /// Sign extend immediate value to (signed) type T.
    template <typename T> T immed() const
    { return T(typename std::make_signed<T>::type((imm20 << 20)    |
						  (imm19_12 << 12) |
						  (imm11 << 11)    |
						  (imm10_1 << 1))); }

    /// Encode "jal rd, offset" into this object.
    bool encodeJal(unsigned rd, int offset);

    uint32_t code;

    struct
    {
      unsigned opcode   : 7;
      unsigned rd       : 5;
      unsigned imm19_12 : 8;
      unsigned imm11    : 1;
      unsigned imm10_1  : 10;
      int      imm20    : 1;
    };
  };


  /// Pack/unpack a cb-form instruction.
  union CbFormInst
  {
    /// Constructor: Either pass a valid cb-form value or start with
    /// any value and then use an encode method.
    CbFormInst(uint16_t inst)
    { code = inst; }

    /// Return immediate value encoded in this object.
    int immed() const
    { return (ic0 << 5) | (ic1 << 1) | (ic2 << 2) | (ic3 << 6) | (ic4 << 7) |
	(ic5 << 3) | (ic6 << 4) | int(ic7 << 8); }

    /// Encode "c.beqz rs1p, imm" into this object.
    bool encodeCbeqz(unsigned rs1p, int imm);

    /// Encode "c.bnez rs1p, imm" into this object.
    bool encodeCbnez(unsigned rs1p, int imm);

    uint32_t code;
    struct
    {
      unsigned opcode : 2;
      unsigned ic0    : 1;
      unsigned ic1    : 1;
      unsigned ic2    : 1;
      unsigned ic3    : 1;
      unsigned ic4    : 1;
      unsigned rs1p   : 3;
      unsigned ic5    : 1;
      unsigned ic6    : 1;
      int      ic7    : 1;
      unsigned funct3 : 3;
      unsigned unused : 16;
    };
  };


  /// Used to pack/unpack c.slri, c.slri64, c.srai, c.srai64, c.andi,
  /// c.sub, c.xor, c.or and c.and.
  union CaiFormInst
  {
    CaiFormInst(uint16_t inst)
    { code = inst; }
    
    int andiImmed() const
    {
      return int(ic5 << 5) | (ic4 << 4) | (ic3 << 3) | (ic2 << 2) |
	(ic1 << 1) | ic0;
    }

    unsigned shiftImmed() const
    { return unsigned(andiImmed()) & 0x1f; }

    bool encodeCsrli(unsigned rdpv, unsigned imm);

    bool encodeCsrai(unsigned rdpv, unsigned imm);

    bool encodeCandi(unsigned rdpv, int imm);

    bool encodeCsub(unsigned rdpv, unsigned rs2pv);

    bool encodeCxor(unsigned rdpv, unsigned rs2pv);

    bool encodeCor(unsigned rdpv, unsigned rs2pv);

    bool encodeCand(unsigned rdpv, unsigned rs2pv);

    uint32_t code;

    struct
    {
      unsigned opcode : 2;
      unsigned ic0    : 1;
      unsigned ic1    : 1;
      unsigned ic2    : 1;
      unsigned ic3    : 1;
      unsigned ic4    : 1;
      unsigned rdp    : 3;
      unsigned funct2 : 2;
      int      ic5    : 1;
      unsigned funct3 : 3;
      unsigned unused : 16;
    };
  };


  /// Pack-unpack ci-form compressed instructions: c.addi, c.addi16sp,
  /// c.lui, c.lwsp, c.slli, c.ebreak, c.jalr and c.jr
  union CiFormInst
  {
    CiFormInst(uint16_t inst)
    { code = inst; }

    uint32_t code;

    int addiImmed() const
    { return int(fields2.ic5 << 5) | fields2.ic4_0; }

    int addi16spImmed() const
    { return int(ic5 << 9) | (ic4 << 4) | (ic3 << 6) | (ic2 << 8) |
	(ic1 << 7) | (ic0 << 5); }

    int luiImmed() const
    { return int(fields2.ic5 << 17) | (fields2.ic4_0 << 12); }

    unsigned slliImmed() const
    { return unsigned(addiImmed()) & 0x3f; }

    unsigned lwspImmed() const
    { return (ic0 << 6) | (ic1 << 7) | (ic2 << 2) | (ic3 << 3) | (ic4 << 4) |
	((unsigned(ic5) & 1) << 5); }

    bool encodeCadd(unsigned rdv, unsigned rs2v);

    bool encodeCaddi(unsigned rdv, int imm);

    bool encodeCaddi16sp(int imm);

    bool encodeClui(unsigned rdv, int imm);

    bool encodeClwsp(unsigned rdv, unsigned imm);

    bool encodeCslli(unsigned rdv, unsigned shift);

    bool encodeCebreak();

    bool encodeCjalr(unsigned rs1);

    bool encodeCjr(unsigned rs1);

    struct
    {
      unsigned opcode : 2;
      unsigned ic0    : 1;
      unsigned ic1    : 1;
      unsigned ic2    : 1;
      unsigned ic3    : 1;
      unsigned ic4    : 1;
      unsigned rd     : 5;
      int      ic5    : 1;
      unsigned funct3 : 3;
      unsigned unused : 16;
    };

    struct
    {
      unsigned opcode : 2;
      unsigned ic4_0  : 5;
      unsigned rd     : 5;
      int      ic5    : 1;
      unsigned funct3 : 3;
      unsigned unused : 16;
    } fields2;

  };


  /// Pack/unpack cl-form instructions: c.lw, c.ld
  union ClFormInst
  {
    ClFormInst(uint16_t inst)
    { code = inst; }

    uint32_t code;

    /// Return immediate value for c.lw instruction encoded in this
    /// object.
    unsigned lwImmed() const
    { return (ic0 << 6) | (ic1 << 2) | (ic3 << 3) | (ic4 << 4) | (ic5 << 5); }

    /// Return immediate value for c.ld instruction encoded in this
    /// object.
    unsigned ldImmed() const
    { return (ic0 << 6) | (ic1 << 7) | (ic3 << 3) | (ic4 << 4) | (ic5 << 5); }

    struct
    {
      unsigned opcode : 2;
      unsigned rdp    : 3;
      unsigned ic0    : 1;
      unsigned ic1    : 1;
      unsigned rs1p   : 3;
      unsigned ic3    : 1;
      unsigned ic4    : 1;
      unsigned ic5    : 1;
      unsigned funct3 : 3;
      unsigned unused : 16;
    };
  };


  // Encode c.addi4spn
  union CiwFormInst
  {
    CiwFormInst(uint16_t inst)
    { code = inst; }

    uint32_t code;

    unsigned immed() const
    { return (ic0 << 3) | (ic1 << 2) | (ic2 << 6) | (ic3 << 7) | (ic4 << 8) |
	(ic5 << 9) | (ic6 << 4) | (ic7 << 5); }

    bool encodeCaddi4spn(unsigned rdpv, unsigned immed);

    struct
    {
      unsigned opcode : 2;
      unsigned rdp    : 3;
      unsigned ic0    : 1;
      unsigned ic1    : 1;
      unsigned ic2    : 1;
      unsigned ic3    : 1;
      unsigned ic4    : 1;
      unsigned ic5    : 1;
      unsigned ic6    : 1;
      unsigned ic7    : 1;
      unsigned funct3 : 3;
      unsigned unused : 16;
    };
  };


  /// Pack/unpack compressed cj-form instructions: c.jal and c.j
  union CjFormInst
  {
    CjFormInst(uint16_t inst)
    { code = inst; }

    uint32_t code;

    int immed() const
    { return (ic0 << 5) | (ic1 << 1) | (ic2 << 2) | (ic3 << 3) | (ic4 << 7) |
	(ic5 << 6) | (ic6 << 10) | (ic7 << 8) | (ic8 << 9) | (ic9 << 4) |
	(ic10 << 11); }

    bool encodeCjal(int imm);

    bool encodeCj(int imm);

    struct
    {
      unsigned opcode : 2;
      unsigned ic0    : 1;
      unsigned ic1    : 1;
      unsigned ic2    : 1;
      unsigned ic3    : 1;
      unsigned ic4    : 1;
      unsigned ic5    : 1;
      unsigned ic6    : 1;
      unsigned ic7    : 1;
      unsigned ic8    : 1;
      unsigned ic9    : 1;
      int ic10        : 1;   // Int used for sign extension.
      unsigned funct3 : 3;
      unsigned unused : 16;
    };
  };


  /// Pack/unpack c.swsp and similar instructions.
  union CswspFormInst
  {
    CswspFormInst(uint16_t inst)
    { code = inst; }

    uint32_t code;

    unsigned immed() const
    { return (ic0 << 6) | (ic1 << 7) | (ic2 << 2) | (ic3 << 3) | (ic4 << 4) |
	(ic5 << 5); }

    bool encodeCswsp(unsigned rs2v, unsigned imm);

    struct
    {
      unsigned opcode : 2;
      unsigned rs2    : 5;
      unsigned ic0    : 1;
      unsigned ic1    : 1;
      unsigned ic2    : 1;
      unsigned ic3    : 1;
      unsigned ic4    : 1;
      unsigned ic5    : 1;
      unsigned funct3 : 3;
      unsigned unused : 16;
    };
  };


  /// Pack/unpack c.sw and similar instructions.
  union CsFormInst
  {
    CsFormInst(uint16_t inst)
    { code = inst; }

    uint32_t code;

    unsigned swImmed() const
    { return (ic0 << 6) | (ic1 << 2) | (ic2 << 3) | (ic3 << 4) | (ic4 << 5); }

    unsigned sdImmed() const
    { return (ic0 << 7) | (ic1 << 6) | (ic2 << 3) | (ic3 << 4) | (ic4 << 5); }

    bool encodeCsw(unsigned rs1pv, unsigned rs2pv, unsigned imm);

    bool encodeCsd(unsigned rs1pv, unsigned rs2pv, unsigned imm);

    struct
    {
      unsigned opcode : 2;
      unsigned rs2p   : 3;
      unsigned ic0    : 1;
      unsigned ic1    : 1;
      unsigned rs1p   : 3;
      unsigned ic2    : 1;
      unsigned ic3    : 1;
      unsigned ic4    : 1;
      unsigned funct3 : 3;
      unsigned unused : 16;
    };
  };


  // We make all encode functions have the same signature. Instruction
  // that do not require certain arguments are passed zero for those
  // arguments.

  /// Encode "lui rd, immed" into inst: encodeLui(rd, immed, 0, inst).
  /// The third argument (x) is ignored.
  /// Return true on success and false if any of the arguments
  /// are out of bounds.
  bool encodeLui(uint32_t rd, uint32_t immed, uint32_t x, uint32_t& inst);

  /// Encode "auipc rd, immed" into inst: encodeAuipc(rd, immed, 0, inst).
  /// The third argument (x) is ignored.
  /// Return true on success and false if any of the arguments
  /// are out of bounds.
  bool encodeAuipc(uint32_t rd, uint32_t immed, uint32_t x, uint32_t& inst);

  /// Encode "jal rd, offset" into inst: encodeJal(rd, offset, 0, inst).
  /// THe third argument (x) is ignored.
  /// Return true on success and false if any of the arguments
  /// are out of bounds.
  bool encodeJal(uint32_t rd, uint32_t offset, uint32_t x, uint32_t& inst);

  /// Encode "jalr rd, offset(rs1)": encodeJalr(rd, rs1, offset, inst).
  /// The third argument (offset) is treaded as signed.
  /// Return true on success and false if any of the arguments
  /// are out of bounds.
  bool encodeJalr(uint32_t rd, uint32_t rs1, uint32_t offset, uint32_t& inst);

  /// Encode a "beq rs1, rs2, imm" into inst: encodeBeq(rs1, rs2, imm, inst).
  /// The third argument (imm) is treated as a signed value even-though
  /// the type is uint32_t.
  /// Return true on success and false if any of the arguments
  /// are out of bounds.
  bool encodeBeq(uint32_t, uint32_t, uint32_t, uint32_t& inst);

  /// Encode a "bne rs1, rs2, imm" into inst: encodeBne(rs1, rs2, imm, inst).
  /// The third argument (imm) is treated as a signed value even-though
  /// the type is uint32_t.
  /// Return true on success and false if any of the arguments
  /// are out of bounds.
  bool encodeBne(uint32_t, uint32_t, uint32_t, uint32_t& inst);

  /// Encode a "blt rs1, rs2, imm" into inst: encodeBlt(rs1, rs2, imm, inst).
  /// The third argument (imm) is treated as a signed value even-though
  /// the type is uint32_t.
  /// Return true on success and false if any of the arguments
  /// are out of bounds.
  bool encodeBlt(uint32_t, uint32_t, uint32_t, uint32_t& inst);

  /// Encode a "bge rs1, rs2, imm" into inst: encodeBge(rs1, rs2, imm, inst).
  /// The third argument (imm) is treated as a signed value even-though
  /// the type is uint32_t.
  /// Return true on success and false if any of the arguments
  /// are out of bounds.
  bool encodeBge(uint32_t, uint32_t, uint32_t, uint32_t& inst);

  /// Encode a "bltu rs1, rs2, imm" into inst: encodeBltu(rs1, rs2, imm, inst).
  /// The third argument (imm) is treated as a signed value even-though
  /// the type is uint32_t.
  /// Return true on success and false if any of the arguments
  /// are out of bounds.
  bool encodeBltu(uint32_t, uint32_t, uint32_t, uint32_t& inst);

  /// Encode a "bgeu rs1, rs2, imm" into inst: encodeBgeu(rs1, rs2, imm, inst).
  /// The third argument (imm) is treated as a signed value even-though
  /// the type is uint32_t.
  /// Return true on success and false if any of the arguments
  /// are out of bounds.
  bool encodeBgeu(uint32_t, uint32_t, uint32_t, uint32_t& inst);

  /// Encode "lb rd, offset(rs1)": encodeLb(rd, rs1, offset, inst).
  /// The third argument (offset) is treaded as signed.
  /// Return true on success and false if any of the arguments
  /// are out of bounds.
  bool encodeLb(uint32_t rd, uint32_t rs1, uint32_t offset, uint32_t& inst);

  /// Encode "lh rd, offset(rs1)": encodeLh(rd, rs1, offset, inst).
  /// The third argument (offset) is treaded as signed.
  /// Return true on success and false if any of the arguments
  /// are out of bounds.
  bool encodeLh(uint32_t rd, uint32_t rs1, uint32_t offset, uint32_t& inst);

  /// Encode "lw rd, offset(rs1)": encodeLw(rd, rs1, offset, inst).
  /// The third argument (offset) is treaded as signed.
  /// Return true on success and false if any of the arguments
  /// are out of bounds.
  bool encodeLw(uint32_t rd, uint32_t rs1, uint32_t offset, uint32_t& inst);

  /// Encode "lbu rd, offset(rs1)": encodeLbu(rd, rs1, offset, inst).
  /// The third argument (offset) is treaded as signed.
  /// Return true on success and false if any of the arguments
  /// are out of bounds.
  bool encodeLbu(uint32_t rd, uint32_t rs1, uint32_t offset, uint32_t& inst);

  /// Encode "lhu rd, offset(rs1)": encodeLhu(rd, rs1, offset, inst).
  /// The third argument (offset) is treaded as signed.
  /// Return true on success and false if any of the arguments
  /// are out of bounds.
  bool encodeLhu(uint32_t rd, uint32_t rs1, uint32_t offset, uint32_t& inst);

  /// Encode "sb rs2, imm(rs1)" into inst: encodeSb(rs1, rs2, imm, inst).
  /// Return true on success and false if any of the arguments
  /// are out of bounds.
  bool encodeSb(uint32_t rs1, uint32_t rs2, uint32_t imm, uint32_t& inst);

  /// Encode "sh rs2, imm(rs1)" into inst: encodeSh(rs1, rs2, imm, inst).
  /// Return true on success and false if any of the arguments
  /// are out of bounds.
  bool encodeSh(uint32_t rs1, uint32_t rs2, uint32_t imm, uint32_t& inst);

  /// Encode "sw rs2, imm(rs1)" into inst: encodeSw(rs1, rs2, imm, inst).
  /// Return true on success and false if any of the arguments
  /// are out of bounds.
  bool encodeSw(uint32_t rs1, uint32_t rs2, uint32_t imm, uint32_t& inst);

  /// Encode "addi rd, rs1, imm" into inst: encodeAddi(rd, rs1, imm, inst).
  /// The third argument (imm) is treated as a signed value even-though
  /// the type is uint32_t.
  /// Return true on success and false if any of the arguments
  /// are out of bounds.
  bool encodeAddi(uint32_t, uint32_t, uint32_t, uint32_t& inst);

  /// Encode "slti rd, rs1, imm" into inst: encodeSlti(rd, rs1, imm, inst).
  /// Return true on success and false if any of the arguments
  /// are out of bounds.
  bool encodeSlti(uint32_t rd, uint32_t rs1, uint32_t imm, uint32_t& inst);

  /// Encode "slti rd, rs1, imm" into inst: encodeSltiu(rd, rs1, imm, inst).
  /// Return true on success and false if any of the arguments
  /// are out of bounds.
  bool encodeSltiu(uint32_t rd, uint32_t rs1, uint32_t imm, uint32_t& inst);

  /// Encode "slti rd, rs1, imm" into inst: ecnodeSlti(rd, rs1, imm, inst).
  /// Return true on success and false if any of the arguments
  /// are out of bounds.
  bool encodeXori(uint32_t rd, uint32_t rs1, uint32_t imm, uint32_t& inst);

  /// Encode "slti rd, rs1, imm" into inst.
  /// Return true on success and false if any of the arguments
  /// are out of bounds.
  bool encodeOri(uint32_t rd, uint32_t rs1, uint32_t imm, uint32_t& inst);

  /// Encode "andi rd, rs1, imm" into inst: encodeAndi(rd, rs1, imm, inst).
  /// The third argument (imm) is treated as a signed value even-though
  /// the type is uint32_t.
  /// Return true on success and false if any of the arguments
  /// are out of bounds.
  bool encodeAndi(uint32_t, uint32_t, uint32_t, uint32_t& inst);

  /// Encode "slli rd, rs1, shamt" into inst: encodeSlli(rd, rs1, shamt, inst).
  /// Return true on success and false if any of the arguments
  /// are out of bounds.
  bool encodeSlli(uint32_t rd, uint32_t rs1, uint32_t shamt, uint32_t& inst);

  /// Encode "srli rd, rs1, shamt" into inst: encodeSrli(rd, rs1, shamt, inst).
  /// Return true on success and false if any of the arguments
  /// are out of bounds.
  bool encodeSrli(uint32_t rd, uint32_t rs1, uint32_t shamt, uint32_t& inst);

  /// Encode "srai rd, rs1, shamt" into inst: encodeSrai(rd, rs1, shamt, inst).
  /// Return true on success and false if any of the arguments
  /// are out of bounds.
  bool encodeSrai(uint32_t rd, uint32_t rs1, uint32_t shamt, uint32_t& inst);

  /// Encode "add rd, rs1, rs2" into inst: encodeAdd(rd, rs1, rs2,
  /// inst). Return true on success and false if any of the arguments
  /// are out of bounds.
  bool encodeAdd(uint32_t, uint32_t, uint32_t, uint32_t& inst);

  /// Encode "sub rd, rs1, rs2" into inst: encodeSub(rd, rs1, rs2, inst).
  /// Return true on success and false if any of the arguments
  /// are out of bounds.
  bool encodeSub(uint32_t, uint32_t, uint32_t, uint32_t& inst);

  /// Encode "sll rd, rs1, rs2" into inst: encodeSll(rd, rs1, rs2, inst).
  /// Return true on success and false if any of the arguments
  /// are out of bounds.
  bool encodeSll(uint32_t, uint32_t, uint32_t, uint32_t& inst);

  /// Encode "slt rd, rs1, rs2" into inst: encodeSlt(rd, rs1, rs2, inst).
  /// Return true on success and false if any of the arguments
  /// are out of bounds.
  bool encodeSlt(uint32_t, uint32_t, uint32_t, uint32_t& inst);

  /// Encode "sltu rd, rs1, rs2" into inst: encodeSlt(rd, rs1, rs2, inst).
  /// Return true on success and false if any of the arguments
  /// are out of bounds.
  bool encodeSltu(uint32_t, uint32_t, uint32_t, uint32_t& inst);

  /// Encode "xor rd, rs1, rs2" into inst: encodeXor(rd, rs1, rs2, inst).
  /// Return true on success and false if any of the arguments
  /// are out of bounds.
  bool encodeXor(uint32_t, uint32_t, uint32_t, uint32_t& inst);

  /// Encode "srl rd, rs1, rs2" into inst: encodeSrl(rd, rs1, rs2, inst).
  /// Return true on success and false if any of the arguments
  /// are out of bounds.
  bool encodeSrl(uint32_t, uint32_t, uint32_t, uint32_t& inst);

  /// Encode "sra rd, rs1, rs2" into inst: encodeSra(rd, rs1, rs2, inst).
  /// Return true on success and false if any of the arguments
  /// are out of bounds.
  bool encodeSra(uint32_t, uint32_t, uint32_t, uint32_t& inst);

  /// Encode "or rdv, rs1v, rs2v" into inst: encodeOr(rd, rs1, rs2, inst).
  /// Return true on success and false if any of the arguments
  /// are out of bounds.
  bool encodeOr(uint32_t, uint32_t, uint32_t, uint32_t& inst);

  /// Encode "and rd, rs1, rs2" into inst: encodeAnd(rd, rs1, rs2, inst).
  /// Return true on success and false if any of the arguments
  /// are out of bounds.
  bool encodeAnd(uint32_t, uint32_t, uint32_t, uint32_t& inst);

  /// Ecnode "fence pred, succ" into inst: encodceFence(pred, succ, 0, inst);
  /// Third parameter (x) is ignored.
  /// Return true on success and false if any of the arguments
  /// are out of bounds.
  bool encodeFence(uint32_t pred, uint32_t succ, uint32_t x, uint32_t& inst);

  /// Ecnode "fence.i" into inst: encodceFencei(0, 0, 0, inst);
  /// First 3 parameters are ignored.
  /// Return true on success and false on failure (this should not fail).
  bool encodeFencei(uint32_t, uint32_t, uint32_t, uint32_t& inst);

  /// Encode "ecall" into inst: encodeEbreak(0, 0, 0, inst). The first
  /// 3 parameters are ignored.
  /// Return true on success and false if any of the arguments
  /// are out of bounds.
  bool encodeEcall(uint32_t, uint32_t, uint32_t, uint32_t& inst);

  /// Encode "ebreak" into inst: encodeEbreak(0, 0, 0, inst). The
  /// first 3 parameters are ignored.
  /// Return true on success and false if any of the arguments
  /// are out of bounds.
  bool encodeEbreak(uint32_t, uint32_t, uint32_t, uint32_t& inst);

  bool encodeCsrrw(uint32_t rd, uint32_t csr, uint32_t rs, uint32_t& inst);
  bool encodeCsrrs(uint32_t rd, uint32_t csr, uint32_t rs, uint32_t& inst);
  bool encodeCsrrc(uint32_t rd, uint32_t csr, uint32_t rs, uint32_t& inst);
  bool encodeCsrrsi(uint32_t rd, uint32_t csr, uint32_t imm, uint32_t& inst);
  bool encodeCsrrci(uint32_t rd, uint32_t csr, uint32_t imm, uint32_t& inst);

  /// Encode "lwu rd, offset(rs1)": encodeLwu(rd, rs1, offset, inst).
  /// The third argument (offset) is treaded as signed.
  /// Return true on success and false if any of the arguments
  /// are out of bounds.
  bool encodeLwu(uint32_t rd, uint32_t rs1, uint32_t offset, uint32_t& inst);

  /// Encode "ld rd, offset(rs1)": encodeLd(rd, rs1, offset, inst).
  /// The third argument (offset) is treaded as signed.
  /// Return true on success and false if any of the arguments
  /// are out of bounds.
  bool encodeLd(uint32_t rd, uint32_t rs1, uint32_t offset, uint32_t& inst);

  /// Encode "sd rs2, imm(rs1)" into inst: encodeSd(rs1, rs2, imm, inst).
  /// Return true on success and false if any of the arguments
  /// are out of bounds.
  bool encodeSd(uint32_t rs1, uint32_t rs2, uint32_t imm, uint32_t& inst);

  /// Encode "addiw rd, rs1, imm" into inst: encodeAddiw(rd, rs1, imm, inst).
  /// The third argument (imm) is treated as a signed value even-though
  /// the type is uint32_t.
  /// Return true on success and false if any of the arguments
  /// are out of bounds.
  bool encodeAddiw(uint32_t rd, uint32_t rs1, uint32_t imm, uint32_t& inst);

  /// Encode "slliw rd, rs1, amt" into inst: encodeSlliw(rd, rs1, amt, inst).
  /// Return true on success and false if any of the arguments
  /// are out of bounds.
  bool encodeSlliw(uint32_t rd, uint32_t rs1, uint32_t amt, uint32_t& inst);

  /// Encode "srliw rd, rs1, amt" into inst: encodeSrliw(rd, rs1, amt, inst).
  /// Return true on success and false if any of the arguments
  /// are out of bounds.
  bool encodeSrliw(uint32_t rd, uint32_t rs1, uint32_t amt, uint32_t& inst);

  /// Encode "sraiw rd, rs1, amt" into inst: encodeSraiw(rd, rs1, amt, inst).
  /// Return true on success and false if any of the arguments
  /// are out of bounds.
  bool encodeSraiw(uint32_t rd, uint32_t rs1, uint32_t amt, uint32_t& inst);

  /// Encode "addw rd, rs1, rs2" into inst: encodeAddw(rd, rs1, rs2, inst).
  /// Return true on success and false if any of the arguments
  /// are out of bounds.
  bool encodeAddw(uint32_t, uint32_t, uint32_t, uint32_t& inst);

  /// Encode "subw rd, rs1, rs2" into inst: encodeSubw(rd, rs1, rs2, inst).
  /// Return true on success and false if any of the arguments
  /// are out of bounds.
  bool encodeSubw(uint32_t, uint32_t, uint32_t, uint32_t& inst);

  /// Encode "sllw rd, rs1, rs2" into inst: encodeSllw(rd, rs1, rs2, inst).
  /// Return true on success and false if any of the arguments
  /// are out of bounds.
  bool encodeSllw(uint32_t, uint32_t, uint32_t, uint32_t& inst);

  /// Encode "srlw rd, rs1, rs2" into inst: encodeSrlw(rd, rs1, rs2, inst).
  /// Return true on success and false if any of the arguments
  /// are out of bounds.
  bool encodeSrlw(uint32_t, uint32_t, uint32_t, uint32_t& inst);

  /// Encode "sraw rd, rs1, rs2" into inst: encodeSraw(rd, rs1, rs2, inst).
  /// Return true on success and false if any of the arguments
  /// are out of bounds.
  bool encodeSraw(uint32_t, uint32_t, uint32_t, uint32_t& inst);

  /// Encode "mul rd, rs1, rs2" into inst: encodeMul(rd, rs1, rs2, inst).
  /// Return true on success and false if any of the arguments
  /// are out of bounds.
  bool encodeMul(uint32_t, uint32_t, uint32_t, uint32_t& inst);

  /// Encode "mulh rd, rs1, rs2" into inst: encodeMulh(rd, rs1, rs2, inst).
  /// Return true on success and false if any of the arguments
  /// are out of bounds.
  bool encodeMulh(uint32_t, uint32_t, uint32_t, uint32_t& inst);

  /// Encode "mulhsu rd, rs1, rs2" into inst: encodeMulhsu(rd, rs1, rs2, inst).
  /// Return true on success and false if any of the arguments
  /// are out of bounds.
  bool encodeMulhsu(uint32_t, uint32_t, uint32_t, uint32_t& inst);

  /// Encode "mulhu rd, rs1, rs2" into inst: encodeMulhu(rd, rs1, rs2, inst).
  /// Return true on success and false if any of the arguments
  /// are out of bounds.
  bool encodeMulhu(uint32_t, uint32_t, uint32_t, uint32_t& inst);

  /// Encode "div rd, rs1, rs2" into inst: encodeDiv(rd, rs1, rs2, inst).
  /// Return true on success and false if any of the arguments
  /// are out of bounds.
  bool encodeDiv(uint32_t, uint32_t, uint32_t, uint32_t& inst);

  /// Encode "divu rd, rs1, rs2" into inst: encodeDivu(rd, rs1, rs2, inst).
  /// Return true on success and false if any of the arguments
  /// are out of bounds.
  bool encodeDivu(uint32_t, uint32_t, uint32_t, uint32_t& inst);

  /// Encode "rem rd, rs1, rs2" into inst: encodeRem(rd, rs1, rs2, inst).
  /// Return true on success and false if any of the arguments
  /// are out of bounds.
  bool encodeRem(uint32_t, uint32_t, uint32_t, uint32_t& inst);

  /// Encode "remu rd, rs1, rs2" into inst: encodeRemu(rd, rs1, rs2, inst).
  /// Return true on success and false if any of the arguments
  /// are out of bounds.
  bool encodeRemu(uint32_t, uint32_t, uint32_t, uint32_t& inst);

  /// Encode "mulw rd, rs1, rs2" into inst: encodeMulw(rd, rs1, rs2, inst).
  /// Return true on success and false if any of the arguments
  /// are out of bounds.
  bool encodeMulw(uint32_t, uint32_t, uint32_t, uint32_t& inst);

  /// Encode "divw rd, rs1, rs2" into inst: encodeDivw(rd, rs1, rs2, inst).
  /// Return true on success and false if any of the arguments
  /// are out of bounds.
  bool encodeDivw(uint32_t, uint32_t, uint32_t, uint32_t& inst);

  /// Encode "divuw rd, rs1, rs2" into inst: encodeDivuw(rd, rs1, rs2, inst).
  /// Return true on success and false if any of the arguments
  /// are out of bounds.
  bool encodeDivuw(uint32_t, uint32_t, uint32_t, uint32_t& inst);

  /// Encode "remw rd, rs1, rs2" into inst: encodeRemw(rd, rs1, rs2, inst).
  /// Return true on success and false if any of the arguments
  /// are out of bounds.
  bool encodeRemw(uint32_t, uint32_t, uint32_t, uint32_t& inst);

  /// Encode "remuw rd, rs1, rs2" into inst: encodeRemuw(rd, rs1, rs2, inst).
  /// Return true on success and false if any of the arguments
  /// are out of bounds.
  bool encodeRemuw(uint32_t, uint32_t, uint32_t, uint32_t& inst);

  /// Encode "c.beqz rs1p, imm" into inst: encodeCbeqz(rs1, imm, 0, inst).
  /// The third argument (x) is ignored.
  /// Return true on success and false if any of the arguments
  /// are out of bounds.
  bool encodeCbeqz(uint32_t rs1, uint32_t imm, uint32_t x, uint32_t& inst);

  /// Encode "c.bnez rs1p, imm" into inst: encodeCbnez(rs1, imm, 0, inst).
  /// The third argument (x) is ignored.
  /// Return true on success and false if any of the arguments
  /// are out of bounds.
  bool encodeCbnez(uint32_t rs1p, uint32_t imm, uint32_t x, uint32_t& inst);
}
