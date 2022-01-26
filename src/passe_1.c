#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "defs.h"
#include "passe_1.h"
#include "miniccutils.h"

extern int trace_level;

void analyse_passe_1(node_t root/*, node_nature parent_nature, ...*/)
{
    analyse_program(root);
}

void analyse_program(node_t root)
{
    // printf("in analyse_program\n");
    if(root->opr[0])
    {
        push_global_context();
        analyse_listdeclnonnull(root->opr[0], 1);
        analyse_maindecl(root->opr[1]);
    }else
    {
        analyse_maindecl(root->opr[1]);
    }
}

void analyse_listdeclnonnull(node_t root, bool global)
{
    // printf("in analyse_listdeclnonnull\n");
    if(root->nature == NODE_LIST)
    {
        analyse_listdeclnonnull(root->opr[0], global);
        analyse_vardecl(root->opr[1], global);
    }else
    {
        analyse_vardecl(root, global);
    }
}

void analyse_maindecl(node_t root)
{
    // printf("in analyse_maindecl\n");
    reset_env_current_offset();
    analyse_type(root->opr[0], root->nature);
    // if(root->opr[1]->ident != "main")
    if( 0 != strcmp(root->opr[1]->ident, "main"))
    {
        printf("error in line %d, NODE_FUNC name should be main!\n", root->lineno);
        exit(1);
    }
    analyse_block(root->opr[2]);

    int offset = get_env_current_offset();
    if(offset >= 0)
    {
        root->offset = offset;
    }else
    {
        printf("error in line %d, offset of NODE_FUNC can not be negetive!\n", root->lineno);
        exit(1);
    }
}

void analyse_vardecl(node_t root, bool global)
{
    // printf("in analyse_vardecl\n");
    set_ident_type(root->opr[1], root->opr[0]->type, global);
    analyse_type(root->opr[0], root->nature);
    analyse_listtypedecl(root->opr[1]);
}

void analyse_type(node_t root, node_nature nature)
{
    // printf("in analyse_type\n");
    if(nature == NODE_FUNC)
    {
        if(root->type != TYPE_VOID)
        {
            printf("error in line %d, fonc type should be void!\n", root->lineno);
            exit(1);
        }
    }else
    {
        if(root->type == TYPE_VOID)
        {
            printf("error in line %d, variable type should not be void!\n", root->lineno);
            exit(1);
        }
    }
}

void analyse_listtypedecl(node_t root)
{
    // printf("in analyse_listtypedecl\n");
    if(root->nature == NODE_LIST)
    {
        analyse_listtypedecl(root->opr[0]);
        analyse_decl(root->opr[1]);
    }else
    {
        analyse_decl(root);
    }
}

void analyse_decl(node_t root)
{
    // printf("in analyse_decl\n");
    if(!root->opr[1])
    {
        analyse_ident(root->opr[0]);
    }else
    {
        analyse_ident(root->opr[0]);
        analyse_expr(root->opr[1]);
        if(root->opr[0]->type != root->opr[1]->type)
        {
            printf("error in line %d, variable should be the same type!\n", root->lineno);
            exit(1);
        }
    }
}

void analyse_block(node_t root)
{
    // printf("in analyse_block\n");
    bool a = 0;
    if(root->opr[0])
    {
        push_context();
        analyse_listdecl(root->opr[0], 0);
        a = 1;
    }
    if(root->opr[1])
    {
        analyse_listinst(root->opr[1]);
    }
    if(a)
    {
        pop_context();
    }
    // printf("out analyse_block\n");
}

void analyse_listdecl(node_t root, bool global)
{
    // printf("in analyse_listdecl\n");
    if(root->opr[0])
    {
        analyse_listdeclnonnull(root, global);
    }
}

void analyse_listinst(node_t root)
{
    // printf("in analyse_listinst\n");
    if(root->opr[0])
    {
        analyse_listinstnonnull(root);
    }
}

void analyse_listinstnonnull(node_t root)
{
    // printf("in analyse_listinstnonnull\n");
    if(root->nature == NODE_LIST)
    {
        analyse_listinstnonnull(root->opr[0]);
        analyse_inst(root->opr[1]);
    }else
    {
        analyse_inst(root);
    }
}

void analyse_inst(node_t root)
{
    // printf("in analyse_inst\n");
    switch(root->nature)
    {
        case NODE_IF:
        {
            // printf("in NODE_IF\n");
            analyse_expr(root->opr[0]);
            analyse_inst(root->opr[1]);
            analyse_inst(root->opr[2]);
            // printf("out NODE_IF\n");
            break;
        }
        case NODE_WHILE:
        {
            analyse_expr(root->opr[0]);
            analyse_inst(root->opr[1]);
            break;
        }
        case NODE_DOWHILE:
        {
            analyse_inst(root->opr[0]);
            analyse_expr(root->opr[1]);
            break;
        }
        case NODE_FOR:
        {
            // printf("in NODE_FOR\n");
            analyse_expr(root->opr[0]);
            analyse_expr(root->opr[1]);
            analyse_expr(root->opr[2]);
            analyse_inst(root->opr[3]);
            // printf("out NODE_FOR\n");
            break;
        }
        case NODE_PRINT:
        {
            analyse_listparamprint(root->opr[0]);
            break;
        }
        case NODE_BLOCK: 
        {
            analyse_block(root);
            break;
        }
        default:
        {
            analyse_expr(root);
            break;
        }
    }
}

void analyse_listparamprint(node_t root)
{
    // printf("in analyse_listparamprint\n");
    if(root->nature == NODE_LIST)
    {
        analyse_listparamprint(root->opr[0]);
        analyse_paramprint(root->opr[1]);
    }else
    {
        analyse_paramprint(root);
    }
}

void analyse_paramprint(node_t root)
{
    // printf("in analyse_paramprint\n");
    if(root->nature == NODE_IDENT)
    {
        check_ident_decl(root);
    }
}

void analyse_ident(node_t root)
{
    // printf("in analyse_ident\n");
    int offset = env_add_element(root->ident, root);
    if(offset >= 0)
    {
        root->offset = offset;
    }else
    {
        printf("error in line %d, variable has already been defined!\n", root->lineno);
        exit(1);
    }
}

void analyse_expr(node_t root)
{
    // printf("in analyse_expr\n");
    switch(root->nature)
    {
        //avec deux operateurs
        case NODE_MUL:
        case NODE_DIV:
        case NODE_PLUS:
        case NODE_MINUS:
        case NODE_MOD:
        case NODE_AND:
        case NODE_OR:
        case NODE_BAND:
        case NODE_BOR:
        case NODE_BXOR:
        case NODE_SRL:
        case NODE_SRA:
        case NODE_SLL:
        {
            // printf("in expr deux oprs\n");
            analyse_expr(root->opr[0]);
            analyse_expr(root->opr[1]);
            // printf("hhhhh\n");
            if(root->opr[0]->type != root->opr[1]->type)
            {
                printf("error in line %d, variable should be the same type!\n", root->lineno);
                exit(1);
            }else
            {
                root->type = root->opr[0]->type;
            }
            // printf("11111111\n");
            break;
        }

        case NODE_NE:
        case NODE_LT:
        case NODE_GT:
        case NODE_GE:
        case NODE_LE:
        case NODE_EQ:
        {
            // printf("in expr bool\n");
            analyse_expr(root->opr[0]);
            analyse_expr(root->opr[1]);
            if(root->opr[0]->type != root->opr[1]->type)
            {
                printf("error in line %d, variable should be the same type!\n", root->lineno);
                exit(1);
            }else
            {
                root->type = TYPE_BOOL;
            }
            break;
        }

        case NODE_AFFECT:
        {
            // printf("in expr affect\n");
            check_ident_decl(root->opr[0]);
            analyse_expr(root->opr[1]);
            if(root->opr[0]->type != root->opr[1]->type)
            {
                printf("error in line %d, variables should be the same type!\n", root->lineno);
                exit(1);
            }else
            {
                root->type = root->opr[0]->type;
            }
            // printf("out NODE_AFFECT\n");
            break;
        }

        //avec un operateur
        case NODE_UMINUS:
        case NODE_NOT:
        case NODE_BNOT:
        {
            analyse_expr(root->opr[0]);
            root->type = root->opr[0]->type;
            break;
        }

        //sortie
        case NODE_IDENT:
        {
            check_ident_decl(root);
            break;
        }
        case NODE_INTVAL:
        {
            root->type = TYPE_INT;
            break;
        }
        case NODE_BOOLVAL:
        {
            root->type = TYPE_BOOL;
            break;
        }
    }
}

void check_ident_decl(node_t root)
{
    // printf("in check_ident_decl\n");
    // node_t node = (node_t)malloc(sizeof(node_s));
    node_t node = (node_t) get_decl_node(root->ident);
    if(node)
    {
        root->decl_node = node;
        root->type = node->type;
    }else
    {
        printf("error in line %d, variable should be declared!\n", root->lineno);
        exit(1);
    }
}


void set_ident_type(node_t root, node_type type, bool global)
{
    // printf("in set_ident_type\n");
    for(int i = 0; i < root->nops; i++)
    {
        if(root->nature == NODE_DECL)
        {
            root->opr[0]->type = type;
            root->opr[0]->global_decl = global;
        }else
        {
            set_ident_type(root->opr[i], type, global);
        }
    }
}
