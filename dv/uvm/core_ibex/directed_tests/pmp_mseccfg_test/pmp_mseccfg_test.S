#include "riscv_test.h"
#include "test_macros.h"
#include "custom_macros.h"

#define PMP_GRAN 0x1000

# Accesses at start, mid and end of PMP range
#define RW_ACCESSES(pmp_addr, gran) \
  la s0, pmp_addr;                  \
  lw s1, 0(s0);                     \
  sw s1, 0(s0);                     \
  li s1, gran/2;                    \
  add s2, s0, s1;                   \
  lw s1, 0(s2);                     \
  sw s1, 0(s2);                     \
  li s1, gran-4;                    \
  add s2, s0, s1;                   \
  lw s1, 0(s2);                     \
  sw s1, 0(s2);

#define SET_PMP_AND_ACCESS_TOR(pmp_permissions, region_high, region_low)                            \
  SET_PMP_TOR(pmp_region_end, pmp_region_start, pmp_permissions | PMP_TOR, region_high, region_low) \
  RW_ACCESSES(pmp_region_start, PMP_GRAN)

#define SET_PMP_AND_ACCESS_NAPOT(pmp_permissions, region)                        \
  SET_PMP_NAPOT(pmp_region_start, PMP_GRAN, pmp_permissions | PMP_NAPOT, region) \
  RW_ACCESSES(pmp_region_start, PMP_GRAN)

#define SET_MSECCFG(val) \
  li t1, val;            \
  csrw CSR_MSECCFG, t1;

RVTEST_RV64M
RVTEST_CODE_BEGIN

  # setting machine handler
  la   t0, mtvec_handler
  csrw mtvec, t0

  RESET_PMP

  # MML 0
  SET_PMP_AND_ACCESS_NAPOT(PMP_R, 15)
  SET_PMP_AND_ACCESS_NAPOT(PMP_W, 14)
  SET_PMP_AND_ACCESS_NAPOT(PMP_X, 13)
  SET_PMP_AND_ACCESS_NAPOT(PMP_R | PMP_W, 12)
  SET_PMP_AND_ACCESS_NAPOT(PMP_R | PMP_X, 10)
  SET_PMP_AND_ACCESS_NAPOT(PMP_R | PMP_W | PMP_X, 9)

  SET_PMP_AND_ACCESS_NAPOT(PMP_L | PMP_R, 8)
  SET_PMP_AND_ACCESS_NAPOT(PMP_L | PMP_W, 7)
  SET_PMP_AND_ACCESS_NAPOT(PMP_L | PMP_X, 6)
  SET_PMP_AND_ACCESS_NAPOT(PMP_L | PMP_R | PMP_W, 5)
  SET_PMP_AND_ACCESS_NAPOT(PMP_L | PMP_R | PMP_X, 4)
  SET_PMP_AND_ACCESS_NAPOT(PMP_L | PMP_R | PMP_W | PMP_X, 3)

  # MML 1
  SET_PMP_TOR(pmp_region_start, _start, PMP_L | PMP_R | PMP_X | PMP_TOR, 0, 0)
  li t0, 0x90000000
  la t1, pmp_region_end
  SET_PMP_TOR_ADDR_FROM_REG(t0, t1, PMP_L | PMP_R | PMP_W | PMP_TOR, 2, 1)

  SET_MSECCFG(MSECCFG_MML)
  SET_MSECCFG(MSECCFG_RLB)
  SET_MSECCFG(MSECCFG_MML | MSECCFG_MMWP)

  j pass

  TEST_PASSFAIL

.balign 256
mtvec_handler:
  SKIP_PC
  mret

RVTEST_CODE_END

  .data
RVTEST_DATA_BEGIN

.balign 0x1000
pmp_region_start: .zero 0x1000
pmp_region_end:
  TEST_DATA

RVTEST_DATA_END
