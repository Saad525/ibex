# Reset PMP CSRs
#define RESET_PMP       \
    csrw pmpcfg0, x0;   \
    csrw pmpcfg1, x0;   \
    csrw pmpcfg2, x0;   \
    csrw pmpcfg3, x0;   \
    csrw pmpaddr0, x0;  \
    csrw pmpaddr1, x0;  \
    csrw pmpaddr2, x0;  \
    csrw pmpaddr3, x0;  \
    csrw pmpaddr4, x0;  \
    csrw pmpaddr5, x0;  \
    csrw pmpaddr6, x0;  \
    csrw pmpaddr7, x0;  \
    csrw pmpaddr8, x0;  \
    csrw pmpaddr9, x0;  \
    csrw pmpaddr10, x0; \
    csrw pmpaddr11, x0; \
    csrw pmpaddr12, x0; \
    csrw pmpaddr13, x0; \
    csrw pmpaddr14, x0; \
    csrw pmpaddr15, x0; \
    csrw CSR_MSECCFG, x0;

# Calculate NAPOT addr by applying mask
#define SET_NAPOT_ADDR(addr, gran) \
    li t0, gran >> 3;              \
    not t2, t0;                    \
    la t1, addr;                   \
    srli t1, t1, 2;                \
    and t1, t1, t2;                \
    addi t0, t0, -1;               \
    or t1, t1, t0;

# Set pmp configuration CSR depending upon region
#define SET_PMP_CFG(pmp_cfg, pmp_region) \
    li t0, (8*pmp_region) & 0x1f;        \
    li t1, pmp_region;                   \
    li t2, 3;                            \
    bgt t1, t2, 1f;                      \
    li t1, 0xff;                         \
    sll t1, t1, t0;                      \
    csrc pmpcfg0, t1;                    \
    li t1, pmp_cfg;                      \
    sll t1, t1, t0;                      \
    csrs pmpcfg0, t1;                    \
    j 4f;                                \
1:                                       \
    li t2, 7;                            \
    bgt t1, t2, 2f;                      \
    li t1, 0xff;                         \
    sll t1, t1, t0;                      \
    csrc pmpcfg1, t1;                    \
    li t1, pmp_cfg;                      \
    sll t1, t1, t0;                      \
    csrs pmpcfg1, t1;                    \
2:                                       \
    li t2, 11;                           \
    bgt t1, t2, 3f;                      \
    li t1, 0xff;                         \
    sll t1, t1, t0;                      \
    csrc pmpcfg2, t1;                    \
    li t1, pmp_cfg;                      \
    sll t1, t1, t0;                      \
    csrs pmpcfg2, t1;                    \
3:                                       \
    li t2, 15;                           \
    bgt t1, t2, 4f;                      \
    li t1, 0xff;                         \
    sll t1, t1, t0;                      \
    csrc pmpcfg3, t1;                    \
    li t1, pmp_cfg;                      \
    sll t1, t1, t0;                      \
    csrs pmpcfg3, t1;                    \
4:

#define SET_PMP_NAPOT(addr, gran, pmp_cfg, pmp_region) \
    SET_NAPOT_ADDR(addr, gran);                        \
    csrw pmpaddr ## pmp_region, t1;                    \
    SET_PMP_CFG(pmp_cfg, pmp_region);

#define SET_PMP_TOR(addr_high, addr_low, pmp_cfg, pmp_region_high, pmp_region_low) \
    la t0, addr_low;                                          \
    srli t0, t0, 2;                                           \
    csrw pmpaddr ## pmp_region_low, t0;                       \
    la t0, addr_high;                                         \
    srli t0, t0, 2;                                           \
    csrw pmpaddr ## pmp_region_high, t0;                      \
    SET_PMP_CFG(pmp_cfg, pmp_region_high);

#define SET_PMP_TOR_ADDR_FROM_REG(addr_high, addr_low, pmp_cfg, pmp_region_high, pmp_region_low) \
    srli t0, addr_high, 2;                                                                       \
    csrw pmpaddr ## pmp_region_low, t0;                                                          \
    srli t0, addr_low, 2;                                                                        \
    csrw pmpaddr ## pmp_region_high, t0;                                                         \
    SET_PMP_CFG(pmp_cfg, pmp_region_high);

#define SKIP_PC      \
    csrr t0, mepc;   \
    lb t1, 0(t0);    \
    li t2, 0x3;      \
    and t1, t1, t2;  \
    bne t1, t2, 1f;  \
    addi t0, t0, 2;  \
1:                   \
    addi t0, t0, 2;  \
    csrw mepc, t0;
