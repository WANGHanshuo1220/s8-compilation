
#include <stdio.h>
#include "miniccutils.h"
#include "defs.h"
#include "passe_2.h"
#include "arch.h"

void gen_code_passe_2(node_t root) {
    gen_program(root);
}

void gen_program(node_t root)
{
    create_inst_data_sec();
    if(root->opr[0])
    {
        gen_data_val_global(root->opr[0]);
    }
    gen_data_str_print(root->opr[1]);
    create_inst_text_sec();
    create_inst_label_str(root->opr[1]->opr[1]->ident);

    gen_maindecl(root->opr[1]);

}

void gen_data_val_global(node_t root)
{
    for(int i = 0; i < root->nops; i++)
    {
        if(root->opr[i]->nature == NODE_DECL)
        {
            if(root->opr[i]->opr[1])
            {
                create_inst_word(root->opr[i]->opr[0]->ident, root->opr[i]->opr[1]->value);
            }else
            {
                create_inst_word(root->opr[i]->opr[0]->ident, 0);
            }
        }else
        {
            gen_data_val_global(root->opr[i]);
        }
    }
}


void gen_data_str_print(node_t root)
{
    for(int i = 0; i < root->nops; i++)
    {
        if(root->opr[i])
        {
            if(root->opr[i]->nature == NODE_STRINGVAL)
            {
                create_inst_asciiz(NULL, root->opr[i]->str);
            }else
            {               
                gen_data_str_print(root->opr[i]);
            }
        }
    }
}

void gen_listdeclnonnull(node_t root)
{
    if(root->nature == NODE_LIST)
    {
        gen_listdeclnonnull(root->opr[0]);
        gen_vardecl(root->opr[1]);
    }else
    {
        gen_vardecl(root);
    }
}

void gen_maindecl(node_t root)
{
    // printf("in gen_maindecl\n");
    set_temporary_start_offset(root->offset);
    create_inst_stack_allocation();
    gen_block(root->opr[2]);
    // printf("000000000000\n");
    create_inst_stack_deallocation(root->offset);
    // printf("1111111111111\n");
    create_inst_ori(2, 0, 10);
    // printf("222222222222222\n");
    create_inst_syscall();
    // printf("out gen_maindecl\n");
}

void gen_vardecl(node_t root)
{
    gen_listtypedecl(root->opr[1]);
}

void gen_listtypedecl(node_t root)
{
    if(root->nature == NODE_LIST)
    {
        gen_listtypedecl(root->opr[0]);
        gen_decl(root->opr[1]);
    }else
    {
        gen_decl(root);
    }
}

void gen_decl(node_t root)
{
    // printf("in gen_decl\n");
    if(root->opr[1])
    {
        int stack_reg = get_stack_reg();
        int reg_r0 = get_r0();
        
        switch(root->opr[1]->nature)
        {
            case NODE_IDENT:
            {
                if(root->opr[1]->decl_node->global_decl)
                {
                    create_inst_lui(get_first_reg(), 0x1001);
                    create_inst_lw(get_first_reg(), root->opr[1]->decl_node->offset, get_first_reg());
                    create_inst_sw(get_first_reg(), root->opr[0]->offset, stack_reg);
                }else
                {
                    create_inst_lw(get_first_reg(), root->opr[1]->decl_node->offset, get_first_reg());
                    create_inst_sw(get_first_reg(), root->opr[0]->offset, stack_reg);
                }
                break;
            }
            case NODE_INTVAL:
            case NODE_BOOLVAL:
            {
                // printf("in gen_decl INTVAL\n");
                create_inst_ori(get_first_reg(), reg_r0, root->opr[1]->value);
                create_inst_sw(get_first_reg(), root->opr[0]->offset, stack_reg);
                break;
            }
        }
    }

    // printf("out gen_decl\n");
}

void gen_block(node_t root)
{
    if(root->opr[0])
    {
        gen_listdecl(root->opr[0]);
    }

    if(root->opr[1])
    {
        gen_listinst(root->opr[1]);
    }
}

void gen_listdecl(node_t root)
{
    if(root->opr[0])
    {
        gen_listdeclnonnull(root);
    }
}

void gen_listinst(node_t root)
{
    if(root->opr[0])
    {
        gen_listinstnonnull(root);
    }
}

void gen_listinstnonnull(node_t root)
{
    // printf("in analyse_listinstnonnull\n");
    if(root->nature == NODE_LIST)
    {
        gen_listinstnonnull(root->opr[0]);
        gen_inst(root->opr[1]);
    }else
    {
        gen_inst(root);
    }
}

void gen_inst(node_t root)
{
    // printf("in gen_inst\n");
    switch(root->nature)
    {
        case NODE_IF:
        {
            int num_label = get_new_label();
            create_inst_label(num_label);
            int num_label2 = get_new_label();
            gen_expr(root->opr[0], num_label2);
            gen_inst(root->opr[1]);
            gen_inst(root->opr[2]);
            break;
        }
        case NODE_WHILE:
        {
            gen_expr(root->opr[0], 1);
            gen_inst(root->opr[1]);
            break;
        }
        case NODE_DOWHILE:
        {
            gen_inst(root->opr[0]);
            gen_expr(root->opr[1], 1);
            break;
        }
        case NODE_FOR:
        {
            // printf("in NODE_FOR\n");
            gen_expr(root->opr[0], 0);
            // printf("000000000\n");
            int num_label = get_new_label();
            create_inst_label(num_label);
            int num_label2 = get_new_label();
            // printf("label1: %d, label2: %d", num_label, num_label2);
            // printf("eeeee\n");

            gen_expr(root->opr[1], num_label2);
            // printf("111111\n");

            gen_inst(root->opr[3]);
            // printf("22222\n");
            gen_expr(root->opr[2], 0);
            // printf("333333\n");

            create_inst_j(num_label);
            create_inst_label(num_label2);
            break;
        }
        case NODE_PRINT:
        {
            gen_listparamprint(root->opr[0]);
            break;
        }
        case NODE_BLOCK: 
        {
            gen_block(root);
            break;
        }
        default:
        {
            gen_expr(root, 0);
            break;
        }
    }
    // printf("out gen_inst\n");
}

void gen_listparamprint(node_t root)
{
    // printf("in analyse_listparamprint\n");
    if(root->nature == NODE_LIST)
    {
        gen_listparamprint(root->opr[0]);
        gen_paramprint(root->opr[1]);
    }else
    {
        gen_paramprint(root);
    }
}

void gen_paramprint(node_t root)
{
    // printf("in gen_paramprint\n");
    if(root->nature == NODE_STRINGVAL)
    {
        // printf("in gen_paramprint NODE_STRINGVAL\n");
        // printf("00000\n");
        int start_addr = get_data_sec_start_addr();
        // printf("11111\n");
        create_inst_lui(4, 0x1001);
        // printf("22222\n");
        int offset_str = add_string(root->str);
        create_inst_ori(4, 4, offset_str);
        // printf("3333333\n");
        create_inst_ori(2, 0, 4);
        // printf("444444\n");
        create_inst_syscall();
    }else
    {
        // printf("in gen_paramprint else\n");
        create_inst_lw(4, root->decl_node->offset, 29);
        create_inst_ori(2, 0, 1);
        create_inst_syscall();
    }
    // printf("out gen_paramprint\n");
}

void gen_expr(node_t root, int num_label2)
{
    // printf("in gen_expr\n");
    int stack_reg = get_stack_reg();
    switch(root->nature)
    {
        //avec deux operateurs
        case NODE_MUL:
        case NODE_DIV:
        case NODE_PLUS:
        {
            // printf("in gen_expr NDOE_PLUS\n");
            int reg[2];
            for(int i = 0; i < root->nops; i++)
            {
                switch(root->opr[i]->nature)
                {
                    case NODE_IDENT:
                    {
                        // printf("in NODE_IDENT\n");
                        if(reg_available())
                        {
                            reg[i] = get_current_reg();
                            allocate_reg();
                            // printf("1111\n");
                            if(root->opr[i]->decl_node->global_decl)
                            {
                                // printf("c'est var global\n");
                                create_inst_lw(reg[i], root->opr[i]->decl_node->offset, get_first_reg());
                            }else
                            {
                                // printf("c'est pas var global\n");
                                create_inst_lw(reg[i], root->opr[i]->decl_node->offset, stack_reg);
                            }
                            // printf("2222\n");
                        }else
                        {
                            // use pile
                        }
                        break;
                    }
                    case NODE_INTVAL:
                    {
                        // printf("in gen_expr NDOE_INTVAL\n");
                        if(reg_available())
                        {
                            reg[i] = get_current_reg();
                            allocate_reg();
                            // printf("inst_addiu\n");
                            create_inst_addiu(reg[i], 0, root->opr[i]->value);
                        }else
                        {
                            // use pile
                        }
                        break;
                    }
                    default:
                    {
                        reg[i] = get_current_reg();
                        gen_expr(root->opr[i], num_label2);
                        break;
                    }
                }
            }
            // printf("inst_addu\n");
            release_reg();
            create_inst_addu(reg[0], reg[0], reg[1]);
            
            // printf("out gen_expr NDOE_PLUS\n");
            break;
        }
        case NODE_MINUS:
        {
            // printf("in gen_expr NDOE_PLUS\n");
            int reg[2];
            for(int i = 0; i < root->nops; i++)
            {
                switch(root->opr[i]->nature)
                {
                    case NODE_IDENT:
                    {
                        // printf("in NODE_IDENT\n");
                        if(reg_available())
                        {
                            reg[i] = get_current_reg();
                            allocate_reg();
                            // printf("1111\n");
                            if(root->opr[i]->decl_node->global_decl)
                            {
                                // printf("c'est var global\n");
                                create_inst_lw(reg[i], root->opr[i]->decl_node->offset, get_first_reg());
                            }else
                            {
                                // printf("c'est pas var global\n");
                                create_inst_lw(reg[i], root->opr[i]->decl_node->offset, stack_reg);
                            }
                            // printf("2222\n");
                        }else
                        {
                            // use pile
                        }
                        break;
                    }
                    case NODE_INTVAL:
                    {
                        // printf("in gen_expr NDOE_INTVAL\n");
                        if(reg_available())
                        {
                            reg[i] = get_current_reg();
                            allocate_reg();
                            // printf("inst_addiu\n");
                            create_inst_addiu(reg[i], 0, root->opr[i]->value);
                        }else
                        {
                            // use pile
                        }
                        break;
                    }
                    default:
                    {
                        reg[i] = get_current_reg();
                        gen_expr(root->opr[i], num_label2);
                        break;
                    }
                }
            }
            // printf("inst_addu\n");
            release_reg();
            create_inst_subu(reg[0], reg[0], reg[1]);
            
            // printf("out gen_expr NDOE_PLUS\n");
            break;
        }
        
        case NODE_MOD:
        case NODE_AND:
        {
            // printf("in gen_expr NDOE_PLUS\n");
            int reg[2];
            for(int i = 0; i < root->nops; i++)
            {
                switch(root->opr[i]->nature)
                {
                    case NODE_IDENT:
                    {
                        // printf("in NODE_IDENT\n");
                        if(reg_available())
                        {
                            reg[i] = get_current_reg();
                            allocate_reg();
                            // printf("1111\n");
                            if(root->opr[i]->decl_node->global_decl)
                            {
                                // printf("c'est var global\n");
                                create_inst_lw(reg[i], root->opr[i]->decl_node->offset, get_first_reg());
                            }else
                            {
                                // printf("c'est pas var global\n");
                                create_inst_lw(reg[i], root->opr[i]->decl_node->offset, stack_reg);
                            }
                            // printf("2222\n");
                        }else
                        {
                            // use pile
                        }
                        break;
                    }
                    case NODE_INTVAL:
                    {
                        // printf("in gen_expr NDOE_INTVAL\n");
                        if(reg_available())
                        {
                            reg[i] = get_current_reg();
                            allocate_reg();
                            // printf("inst_addiu\n");
                            create_inst_addiu(reg[i], 0, root->opr[i]->value);
                        }else
                        {
                            // use pile
                        }
                        break;
                    }
                    default:
                    {
                        reg[i] = get_current_reg();
                        gen_expr(root->opr[i], num_label2);
                        break;
                    }
                }
            }
            // printf("inst_addu\n");
            release_reg();
            create_inst_and(reg[0], reg[0], reg[1]);
            
            // printf("out gen_expr NDOE_PLUS\n");
            break;
        }
        case NODE_OR:
        {
            printf("in gen_expr NDOE_PLUS\n");
            int reg[2];
            for(int i = 0; i < root->nops; i++)
            {
                switch(root->opr[i]->nature)
                {
                    case NODE_IDENT:
                    {
                        printf("in NODE_IDENT\n");
                        if(reg_available())
                        {
                            reg[i] = get_current_reg();
                            allocate_reg();
                            printf("1111\n");
                            if(root->opr[i]->decl_node->global_decl)
                            {
                                printf("c'est var global\n");
                                create_inst_lw(reg[i], root->opr[i]->decl_node->offset, get_first_reg());
                            }else
                            {
                                printf("c'est pas var global\n");
                                create_inst_lw(reg[i], root->opr[i]->decl_node->offset, stack_reg);
                            }
                            printf("2222\n");
                        }else
                        {
                            // use pile
                        }
                        break;
                    }
                    case NODE_INTVAL:
                    {
                        printf("in gen_expr NDOE_INTVAL\n");
                        if(reg_available())
                        {
                            reg[i] = get_current_reg();
                            allocate_reg();
                            printf("inst_addiu\n");
                            create_inst_addiu(reg[i], 0, root->opr[i]->value);
                        }else
                        {
                            // use pile
                        }
                        break;
                    }
                    default:
                    {
                        reg[i] = get_current_reg();
                        gen_expr(root->opr[i], num_label2);
                        break;
                    }
                }
            }
            printf("inst_addu\n");
            release_reg();
            create_inst_or(reg[0], reg[0], reg[1]);
            
            printf("out gen_expr NDOE_PLUS\n");
            break;
        }
        case NODE_BAND:
        case NODE_BOR:
        case NODE_BXOR:
        case NODE_SRL:
        case NODE_SRA:
        case NODE_SLL:

        case NODE_NE:
        case NODE_LT:
        {
            if(num_label2)
            {
                // printf("has label\n");
                // printf("%d",num_label2);
                int reg[2];
                int stack_addr = get_stack_reg();
                for(int i = 0; i < root->nops; i++)
                {
                    
                    switch(root->opr[i]->nature)
                    {
                        case NODE_IDENT:
                        {
                            if(reg_available())
                            {
                                reg[i] = get_current_reg();
                                allocate_reg();
                                create_inst_lw(reg[i], root->opr[i]->decl_node->offset, stack_addr);
                            }else
                            {
                                // use pile
                            }
                            break;
                        }
                        case NODE_INTVAL:
                        {
                            if(reg_available())
                            {
                                allocate_reg();
                                reg[i] = get_current_reg();
                                create_inst_addiu(reg[i], 0, root->value);
                            }else
                            {
                                // use pile
                            }
                            break;
                        }
                        default:
                        {
                            gen_expr(root->opr[i], num_label2);
                            break;
                        }
                    }
                }
                create_inst_slt(reg[0], reg[0], reg[1]);
                create_inst_beq(reg[0], 0, num_label2);
                release_reg();
                release_reg();
                break;
            }
        }
        case NODE_GT:
        case NODE_GE:
        case NODE_LE:
        case NODE_EQ:

        case NODE_AFFECT:
        {
            // printf("in gen_expr NODE_AFFECT\n");
            gen_expr(root->opr[1], num_label2);
            // printf("hhhhhhhhhhhhhhhhh\n");
            int reg = get_current_reg();
            if(get_current_reg() != 8)
            {
                release_reg();
                reg = get_current_reg();
            }
            create_inst_sw(reg, root->opr[0]->decl_node->offset, stack_reg);
            // printf("out gen_expr NODE_AFFECT\n");
            break;
        }

        //avec un operateur
        case NODE_UMINUS:
        case NODE_NOT:
        case NODE_BNOT:

        //sortie
        case NODE_IDENT:
        {
            if(root->decl_node->global_decl)
            {
                create_inst_lw(get_first_reg(), root->decl_node->offset, get_first_reg());
            }else
            {
                create_inst_lw(get_first_reg(), root->decl_node->offset, get_stack_reg());
            }
            break;
        }
        case NODE_INTVAL:
        case NODE_BOOLVAL:
        {

            break;
        }
    }
    // printf("out gen_expr\n");
}