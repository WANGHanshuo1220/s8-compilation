
#ifndef _PASSE_1_
#define _PASSE_1_

#include "defs.h"


void analyse_passe_1(node_t root);

void analyse_program(node_t root);
void analyse_listdeclnonnull(node_t root, bool global);
void analyse_maindecl(node_t root);
void analyse_vardecl(node_t root, bool global);
void analyse_type(node_t root, node_nature nature);
void analyse_listtypedecl(node_t root);
void analyse_decl(node_t root);
void analyse_block(node_t root);
void analyse_listdecl(node_t root, bool global);
void analyse_listinst(node_t root);
void analyse_listinstnonnull(node_t root);
void analyse_inst(node_t root);
void analyse_listparamprint(node_t root);
void analyse_paramprint(node_t root);
void analyse_ident(node_t root);
void analyse_expr(node_t root);
void check_ident_decl(node_t root);
void set_ident_type(node_t root, node_type type, bool global);

#endif

