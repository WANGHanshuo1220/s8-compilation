
.data

start: .word 0
end: .word 5
.asciiz "sum: "
.asciiz "\n"
.asciiz "sum: "
.asciiz "\n"

.text

main:
    addiu $29, $29, -16
    lui   $8, 0x1001
    lw    $8, 0($8)
    sw    $8, 4($29)
    lui   $8, 0x1001
    lw    $8, 4($8)
    sw    $8, 8($29)
    ori   $8, $0, 0xa
    sw    $8, 12($29)
    lw    $8, 4($29)
    sw    $8, 0($29)
_L1:
    lw    $8, 0($29)
    lw    $9, 8($29)
    slt   $8, $8, $9
    beq   $8, $0, _L2
_L3:
    sw    $8, 12($29)
    lw    $8, 12($29)
    lw    $9, 0($29)
    subu  $8, $8, $9
    sw    $8, 12($29)
    lui   $4, 0x1001
    ori   $4, $4, 0x8
    ori   $2, $0, 0x4
    syscall
    lw    $4, 12($29)
    ori   $2, $0, 0x1
    syscall
    lui   $4, 0x1001
    ori   $4, $4, 0xe
    ori   $2, $0, 0x4
    syscall
    lw    $8, 12($29)
    addiu $9, $0, 1
    addu  $8, $8, $9
    sw    $8, 12($29)
    lw    $8, 0($29)
    addiu $9, $0, 1
    addu  $8, $8, $9
    sw    $8, 0($29)
    j     _L1
_L2:
    lui   $4, 0x1001
    ori   $4, $4, 0x10
    ori   $2, $0, 0x4
    syscall
    lw    $4, 12($29)
    ori   $2, $0, 0x1
    syscall
    lui   $4, 0x1001
    ori   $4, $4, 0x16
    ori   $2, $0, 0x4
    syscall
    addiu $29, $29, 16
    ori   $2, $0, 0xa
    syscall
