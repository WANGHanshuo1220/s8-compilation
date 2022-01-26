
.data

c: .word 10
.asciiz "a: "

.text

main:
    addiu $29, $29, -8
    ori   $8, $0, 0x1
    sw    $8, 0($29)
    ori   $8, $0, 0x2
    sw    $8, 4($29)
    addiu $8, $0, 1
    addiu $9, $0, 2
    addu  $8, $8, $9
    addiu $9, $0, 3
    addu  $8, $8, $9
    addiu $9, $0, 4
    addu  $8, $8, $9
    addiu $9, $0, 5
    addu  $8, $8, $9
    sw    $8, 0($29)
    lui   $4, 0x1001
    ori   $4, $4, 0x4
    ori   $2, $0, 0x4
    syscall
    lw    $4, 0($29)
    ori   $2, $0, 0x1
    syscall
    addiu $29, $29, 8
    ori   $2, $0, 0xa
    syscall
