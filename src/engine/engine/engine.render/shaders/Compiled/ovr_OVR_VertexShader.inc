#if 0
//
// Generated by Microsoft (R) D3D Shader Disassembler
//
//
///
// Input signature:
//
// Name                 Index   Mask Register SysValue  Format   Used
// -------------------- ----- ------ -------- -------- ------- ------
// POSITION                 0   xyz         0     NONE   float   xyz 
// COLOR                    0   xyzw        1     NONE   float   xyzw
// TEXCOORD                 0   xy          2     NONE   float   xy  
//
//
// Output signature:
//
// Name                 Index   Mask Register SysValue  Format   Used
// -------------------- ----- ------ -------- -------- ------- ------
// SV_Position              0   xyzw        0      POS   float   xyzw
// COLOR                    0   xyzw        1     NONE   float   xyzw
// TEXCOORD                 0   xy          2     NONE   float   xy  
//
//
// Constant buffer to DX9 shader constant mappings:
//
// Target Reg Buffer  Start Reg # of Regs        Data Conversion
// ---------- ------- --------- --------- ----------------------
// c1         cb0             4        12  ( FLT, FLT, FLT, FLT)
//
//
// Runtime generated constant mappings:
//
// Target Reg                               Constant Description
// ---------- --------------------------------------------------
// c0                              Vertex Shader position offset
//
//
// Level9 shader bytecode:
//
    vs_2_0
    dcl_texcoord v0
    dcl_texcoord1 v1
    dcl_texcoord2 v2
    mul r0, v0.y, c6
    mad r0, c5, v0.x, r0
    mad r0, c7, v0.z, r0
    add r0, r0, c8
    mul r1, r0.y, c2
    mad r1, c1, r0.x, r1
    mad r1, c3, r0.z, r1
    mad r0, c4, r0.w, r1
    mul r1, r0.y, c10
    mad r1, c9, r0.x, r1
    mad r1, c11, r0.z, r1
    mad r0, c12, r0.w, r1
    mad oPos.xy, r0.w, c0, r0
    mov oPos.zw, r0
    mov oT0, v1
    mov oT1.xy, v2

// approximately 16 instruction slots used
vs_4_0
dcl_constantbuffer cb0[16], immediateIndexed
dcl_input v0.xyz
dcl_input v1.xyzw
dcl_input v2.xy
dcl_output_siv o0.xyzw, position
dcl_output o1.xyzw
dcl_output o2.xy
dcl_temps 2
mul r0.xyzw, v0.yyyy, cb0[9].xyzw
mad r0.xyzw, cb0[8].xyzw, v0.xxxx, r0.xyzw
mad r0.xyzw, cb0[10].xyzw, v0.zzzz, r0.xyzw
add r0.xyzw, r0.xyzw, cb0[11].xyzw
mul r1.xyzw, r0.yyyy, cb0[5].xyzw
mad r1.xyzw, cb0[4].xyzw, r0.xxxx, r1.xyzw
mad r1.xyzw, cb0[6].xyzw, r0.zzzz, r1.xyzw
mad r0.xyzw, cb0[7].xyzw, r0.wwww, r1.xyzw
mul r1.xyzw, r0.yyyy, cb0[13].xyzw
mad r1.xyzw, cb0[12].xyzw, r0.xxxx, r1.xyzw
mad r1.xyzw, cb0[14].xyzw, r0.zzzz, r1.xyzw
mad o0.xyzw, cb0[15].xyzw, r0.wwww, r1.xyzw
mov o1.xyzw, v1.xyzw
mov o2.xy, v2.xyxx
ret 
// Approximately 0 instruction slots used
#endif

const BYTE ovr_OVR_VertexShader[] =
{
     68,  88,  66,  67, 118, 134, 
    175, 197, 114, 203, 125, 246, 
    175, 160, 213, 107, 202, 114, 
    218, 240,   1,   0,   0,   0, 
    244,   4,   0,   0,   4,   0, 
      0,   0,  48,   0,   0,   0, 
    176,   1,   0,   0,  16,   4, 
      0,   0, 128,   4,   0,   0, 
     65, 111, 110,  57, 120,   1, 
      0,   0, 120,   1,   0,   0, 
      0,   2, 254, 255,  68,   1, 
      0,   0,  52,   0,   0,   0, 
      1,   0,  36,   0,   0,   0, 
     48,   0,   0,   0,  48,   0, 
      0,   0,  36,   0,   1,   0, 
     48,   0,   0,   0,   4,   0, 
     12,   0,   1,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   2, 254, 255,  31,   0, 
      0,   2,   5,   0,   0, 128, 
      0,   0,  15, 144,  31,   0, 
      0,   2,   5,   0,   1, 128, 
      1,   0,  15, 144,  31,   0, 
      0,   2,   5,   0,   2, 128, 
      2,   0,  15, 144,   5,   0, 
      0,   3,   0,   0,  15, 128, 
      0,   0,  85, 144,   6,   0, 
    228, 160,   4,   0,   0,   4, 
      0,   0,  15, 128,   5,   0, 
    228, 160,   0,   0,   0, 144, 
      0,   0, 228, 128,   4,   0, 
      0,   4,   0,   0,  15, 128, 
      7,   0, 228, 160,   0,   0, 
    170, 144,   0,   0, 228, 128, 
      2,   0,   0,   3,   0,   0, 
     15, 128,   0,   0, 228, 128, 
      8,   0, 228, 160,   5,   0, 
      0,   3,   1,   0,  15, 128, 
      0,   0,  85, 128,   2,   0, 
    228, 160,   4,   0,   0,   4, 
      1,   0,  15, 128,   1,   0, 
    228, 160,   0,   0,   0, 128, 
      1,   0, 228, 128,   4,   0, 
      0,   4,   1,   0,  15, 128, 
      3,   0, 228, 160,   0,   0, 
    170, 128,   1,   0, 228, 128, 
      4,   0,   0,   4,   0,   0, 
     15, 128,   4,   0, 228, 160, 
      0,   0, 255, 128,   1,   0, 
    228, 128,   5,   0,   0,   3, 
      1,   0,  15, 128,   0,   0, 
     85, 128,  10,   0, 228, 160, 
      4,   0,   0,   4,   1,   0, 
     15, 128,   9,   0, 228, 160, 
      0,   0,   0, 128,   1,   0, 
    228, 128,   4,   0,   0,   4, 
      1,   0,  15, 128,  11,   0, 
    228, 160,   0,   0, 170, 128, 
      1,   0, 228, 128,   4,   0, 
      0,   4,   0,   0,  15, 128, 
     12,   0, 228, 160,   0,   0, 
    255, 128,   1,   0, 228, 128, 
      4,   0,   0,   4,   0,   0, 
      3, 192,   0,   0, 255, 128, 
      0,   0, 228, 160,   0,   0, 
    228, 128,   1,   0,   0,   2, 
      0,   0,  12, 192,   0,   0, 
    228, 128,   1,   0,   0,   2, 
      0,   0,  15, 224,   1,   0, 
    228, 144,   1,   0,   0,   2, 
      1,   0,   3, 224,   2,   0, 
    228, 144, 255, 255,   0,   0, 
     83,  72,  68,  82,  88,   2, 
      0,   0,  64,   0,   1,   0, 
    150,   0,   0,   0,  89,   0, 
      0,   4,  70, 142,  32,   0, 
      0,   0,   0,   0,  16,   0, 
      0,   0,  95,   0,   0,   3, 
    114,  16,  16,   0,   0,   0, 
      0,   0,  95,   0,   0,   3, 
    242,  16,  16,   0,   1,   0, 
      0,   0,  95,   0,   0,   3, 
     50,  16,  16,   0,   2,   0, 
      0,   0, 103,   0,   0,   4, 
    242,  32,  16,   0,   0,   0, 
      0,   0,   1,   0,   0,   0, 
    101,   0,   0,   3, 242,  32, 
     16,   0,   1,   0,   0,   0, 
    101,   0,   0,   3,  50,  32, 
     16,   0,   2,   0,   0,   0, 
    104,   0,   0,   2,   2,   0, 
      0,   0,  56,   0,   0,   8, 
    242,   0,  16,   0,   0,   0, 
      0,   0,  86,  21,  16,   0, 
      0,   0,   0,   0,  70, 142, 
     32,   0,   0,   0,   0,   0, 
      9,   0,   0,   0,  50,   0, 
      0,  10, 242,   0,  16,   0, 
      0,   0,   0,   0,  70, 142, 
     32,   0,   0,   0,   0,   0, 
      8,   0,   0,   0,   6,  16, 
     16,   0,   0,   0,   0,   0, 
     70,  14,  16,   0,   0,   0, 
      0,   0,  50,   0,   0,  10, 
    242,   0,  16,   0,   0,   0, 
      0,   0,  70, 142,  32,   0, 
      0,   0,   0,   0,  10,   0, 
      0,   0, 166,  26,  16,   0, 
      0,   0,   0,   0,  70,  14, 
     16,   0,   0,   0,   0,   0, 
      0,   0,   0,   8, 242,   0, 
     16,   0,   0,   0,   0,   0, 
     70,  14,  16,   0,   0,   0, 
      0,   0,  70, 142,  32,   0, 
      0,   0,   0,   0,  11,   0, 
      0,   0,  56,   0,   0,   8, 
    242,   0,  16,   0,   1,   0, 
      0,   0,  86,   5,  16,   0, 
      0,   0,   0,   0,  70, 142, 
     32,   0,   0,   0,   0,   0, 
      5,   0,   0,   0,  50,   0, 
      0,  10, 242,   0,  16,   0, 
      1,   0,   0,   0,  70, 142, 
     32,   0,   0,   0,   0,   0, 
      4,   0,   0,   0,   6,   0, 
     16,   0,   0,   0,   0,   0, 
     70,  14,  16,   0,   1,   0, 
      0,   0,  50,   0,   0,  10, 
    242,   0,  16,   0,   1,   0, 
      0,   0,  70, 142,  32,   0, 
      0,   0,   0,   0,   6,   0, 
      0,   0, 166,  10,  16,   0, 
      0,   0,   0,   0,  70,  14, 
     16,   0,   1,   0,   0,   0, 
     50,   0,   0,  10, 242,   0, 
     16,   0,   0,   0,   0,   0, 
     70, 142,  32,   0,   0,   0, 
      0,   0,   7,   0,   0,   0, 
    246,  15,  16,   0,   0,   0, 
      0,   0,  70,  14,  16,   0, 
      1,   0,   0,   0,  56,   0, 
      0,   8, 242,   0,  16,   0, 
      1,   0,   0,   0,  86,   5, 
     16,   0,   0,   0,   0,   0, 
     70, 142,  32,   0,   0,   0, 
      0,   0,  13,   0,   0,   0, 
     50,   0,   0,  10, 242,   0, 
     16,   0,   1,   0,   0,   0, 
     70, 142,  32,   0,   0,   0, 
      0,   0,  12,   0,   0,   0, 
      6,   0,  16,   0,   0,   0, 
      0,   0,  70,  14,  16,   0, 
      1,   0,   0,   0,  50,   0, 
      0,  10, 242,   0,  16,   0, 
      1,   0,   0,   0,  70, 142, 
     32,   0,   0,   0,   0,   0, 
     14,   0,   0,   0, 166,  10, 
     16,   0,   0,   0,   0,   0, 
     70,  14,  16,   0,   1,   0, 
      0,   0,  50,   0,   0,  10, 
    242,  32,  16,   0,   0,   0, 
      0,   0,  70, 142,  32,   0, 
      0,   0,   0,   0,  15,   0, 
      0,   0, 246,  15,  16,   0, 
      0,   0,   0,   0,  70,  14, 
     16,   0,   1,   0,   0,   0, 
     54,   0,   0,   5, 242,  32, 
     16,   0,   1,   0,   0,   0, 
     70,  30,  16,   0,   1,   0, 
      0,   0,  54,   0,   0,   5, 
     50,  32,  16,   0,   2,   0, 
      0,   0,  70,  16,  16,   0, 
      2,   0,   0,   0,  62,   0, 
      0,   1,  73,  83,  71,  78, 
    104,   0,   0,   0,   3,   0, 
      0,   0,   8,   0,   0,   0, 
     80,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      3,   0,   0,   0,   0,   0, 
      0,   0,   7,   7,   0,   0, 
     89,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      3,   0,   0,   0,   1,   0, 
      0,   0,  15,  15,   0,   0, 
     95,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   0,   0, 
      3,   0,   0,   0,   2,   0, 
      0,   0,   3,   3,   0,   0, 
     80,  79,  83,  73,  84,  73, 
     79,  78,   0,  67,  79,  76, 
     79,  82,   0,  84,  69,  88, 
     67,  79,  79,  82,  68,   0, 
     79,  83,  71,  78, 108,   0, 
      0,   0,   3,   0,   0,   0, 
      8,   0,   0,   0,  80,   0, 
      0,   0,   0,   0,   0,   0, 
      1,   0,   0,   0,   3,   0, 
      0,   0,   0,   0,   0,   0, 
     15,   0,   0,   0,  92,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   3,   0, 
      0,   0,   1,   0,   0,   0, 
     15,   0,   0,   0,  98,   0, 
      0,   0,   0,   0,   0,   0, 
      0,   0,   0,   0,   3,   0, 
      0,   0,   2,   0,   0,   0, 
      3,  12,   0,   0,  83,  86, 
     95,  80, 111, 115, 105, 116, 
    105, 111, 110,   0,  67,  79, 
     76,  79,  82,   0,  84,  69, 
     88,  67,  79,  79,  82,  68, 
      0, 171
};