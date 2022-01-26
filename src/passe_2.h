#ifndef _PASSE_2_H_
#define _PASSE_2_H_

#include "defs.h"

void gen_code_passe_2(node_t root);
void gen_program(node_t root);
void gen_data_val_global(node_t root);
void gen_data_str_print(node_t root);
void gen_data(node_t root);
void gen_listdeclnonnull(node_t root);
void gen_maindecl(node_t root);
void gen_vardecl(node_t root);
void gen_listtypedecl(node_t root);
void gen_decl(node_t root);
void gen_block(node_t root);
void gen_listdecl(node_t root);
void gen_listinst(node_t root);
void gen_listinstnonnull(node_t root);
void gen_inst(node_t root);
void gen_listparamprint(node_t root);
void gen_paramprint(node_t root);
void gen_expr(node_t root, int has_block);


#endif

