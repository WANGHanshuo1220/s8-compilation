
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#include <inttypes.h>
#include <unistd.h>
#include <getopt.h>

#include "defs.h"
#include "common.h"
#include "arch.h"


extern char * infile;
extern char * outfile;
int32_t trace_level = DEFAULT_TRACE_LEVEL;
extern bool stop_after_syntax;
extern bool stop_after_verif;


void parse_args(int argc, char ** argv) {
    // A implementer (la ligne suivante est a changer)
    if(argc == 2)
    {
        char *p = &argv[1][strlen(argv[1])-1];
        if(!strcmp(p, "h"))
        {
            infile = NULL;
            outfile = NULL;
            printf("------------------------------------------------------------------------------------------------\n");
            printf("Usage:                                                                                         |\n");
            printf("-> -b : Affiche une bannière indiquant le nom du compilateur et des membres du binôme          |\n");
            printf("-> -o <filename> : Définit le nom du fichier assembleur produit (défaut : out.s).              |\n");
            printf("-> -t <int> : Définit le niveau de trace à utiliser entre 0 et 5                               |\n");
            printf("    (0 = pas de trace ; 5 = toutes les traces.defaut = 0).                                     |\n");
            printf("-> -r <int> : Définit le nombre maximum de registres à utiliser, entre 4 et 8 (défaut : 8)     |\n");
            printf("-> -s : Arrêter la compilation après l’analyse syntaxique (défaut = non).                      |\n");
            printf("-> -v : Arrêter la compilation après la passe de vérifications (défaut = non).                 |\n");
            printf("-> -h : Afficher la liste des options (fonction d’usage) et arrêter le parsing des arguments.  |\n");
            printf("------------------------------------------------------------------------------------------------\n");
        }else if(!strcmp(p, "b"))
        {
            infile = NULL;
            outfile = NULL;
            printf("------------------------------------------\n");
            printf("|         Compilation binome:            |\n");
            printf("|      WANG Hanshuo && WEI Yongtao       |\n");
            printf("------------------------------------------\n");
        }else
        {
            if(!strcmp(p, "c"))
            {
                infile = argv[1];
            }else
            {
                printf("commande erreur!\n");
                exit(1);
            }
        }
    }else if(argc == 3)
    {
        int check = 0;
        for(int i = 1; i < argc; i++)
        {
            char *p = &argv[i][strlen(argv[i])-1];
            if(!strcmp(p, "c"))
            {
                infile = argv[i];
                check++;
            }else if(!strcmp(p, "s"))
            {
                stop_after_syntax = true;
                check++;
            }
        }
        if(check != 2)
        {
            printf("commande erreur!\n");
            exit(1);
        }
    }else if(argc == 4)
    {
        int check = 0;
        for(int i = 1; i < argc; i++)
        {
            char *p = &argv[i][strlen(argv[i])-1];
            if(!strcmp(p, "c"))
            {
                infile = argv[i];
                check++;
            }else if(!strcmp(p, "o"))
            {
                char *p1 = &argv[i+1][strlen(argv[i+1])-1];
                if(!strcmp(p1, "s"))
                {
                    check+=2;
                }else
                {
                    printf("commande erreur!\n");
                    exit(1);
                } 
            }
        }
        // printf("hhhhhh\n");
        if(check != 3)
        {
            // printf("%d",check);
            printf("commande erreur!\n");
            exit(1);
        }else
        {
            printf("%d",check);
        }
    }else if(argc == 5)
    {
        int check = 0;
        for(int i = 1; i < argc; i++)
        {
            char *p = &argv[i][strlen(argv[i])-1];
            if(!strcmp(p, "c"))
            {
                infile = argv[i];
                check++;
            }else if(!strcmp(p, "o"))
            {
                char *p1 = &argv[i+1][strlen(argv[i+1])-1];
                if(!strcmp(p1, "s"))
                {
                    check+=2;
                }else
                {
                    printf("commande erreur!\n");
                    exit(1);
                } 
            }else if(!strcmp(p, "v"))
            {
                stop_after_verif = true;
                check++;
            }
        }
        // printf("hhhhhh\n");
        if(check != 4)
        {
            // printf("%d",check);
            printf("commande erreur!\n");
            exit(1);
        }else
        {
            printf("%d",check);
        }
    }else if(argc == 8)
    {
        int check = 0;
        for(int i = 1; i < argc; i++)
        {
            char *p = &argv[i][strlen(argv[i])-1];
            if(!strcmp(p, "c"))
            {
                infile = argv[i];
                check++;
            }else if(!strcmp(p, "o"))
            {
                char *p1 = &argv[i+1][strlen(argv[i+1])-1];
                if(!strcmp(p1, "s"))
                {
                    check+=2;
                }else
                {
                    printf("commande erreur!\n");
                    exit(1);
                } 
            }else if(!strcmp(p, "t"))
            {
                int p = atoi(argv[i+1]);
                if(p>= 0 && p <=5)
                {
                    trace_level = p;
                    i++;
                    check += 2;
                }else
                {
                    printf("commande erreur!\n");
                    exit(1);
                } 
            }else if(!strcmp(p, "r"))
            {
                int p = atoi(argv[i+1]);
                if(p>= 4 && p <= 8)
                {
                    set_max_registers(p);
                    i++;
                    check += 2;
                }else
                {
                    printf("commande erreur!\n");
                    exit(1);
                } 
            }
        }
        // printf("hhhhhh\n");
        if(check != 7)
        {
            // printf("%d",check);
            printf("commande erreur!\n");
            exit(1);
        }
    }
}

void free_nodes(node_t n) {
    // A implementer
    if(n->nops)
    {
        for(int i = 0; i < n->nops; i++)
        {
            if(n->opr[i])
            {
                free_nodes(n->opr[i]);
            }
        }
        free(n);
    }else
    {
        free(n);
    }
}


char * strdup1(char * s) {
    char * r = malloc(strlen(s) + 1);
    strcpy(r, s);
    return r;
}


static int32_t dump_tree2dot_rec(FILE * f, node_t n, int32_t node_num) {

    if (n == NULL) {
        fprintf(f, "    N%d [shape=record, label=\"{{NULL}}\"];\n", node_num);
        return node_num;
    }

    switch (n->nature) {
        case NODE_IDENT:
            {
                node_t decl_node = n->decl_node;
                fprintf(f, "    N%d [shape=record, label=\"{{NODE %s|Type: %s}|{<decl>Decl      |Ident: %s|Offset: %d}}\"];\n", node_num, node_nature2string(n->nature), node_type2string(n->type), n->ident, n->offset);
                if (decl_node != NULL && decl_node != n) {
                    fprintf(f, "    edge[tailclip=false];\n");
                    fprintf(f, "    \"N%d\":decl:c -> \"N%d\" [style=dashed]\n", node_num, decl_node->node_num);
                }
                break;
            }
        case NODE_INTVAL:
        case NODE_BOOLVAL:
            fprintf(f, "    N%d [shape=record, label=\"{{NODE %s|Type: %s}|{Value: %" PRId64 "}}\"];\n", node_num, node_nature2string(n->nature), node_type2string(n->type), n->value);
            break;
        case NODE_STRINGVAL:
            {
                char str[32];
                int32_t i = 1;
                while (true) {
                    str[i - 1] = n->str[i];
                    i += 1;
                    if (n->str[i] == '"') {
                        str[i - 1] = '\0';
                        break;
                    }
                }
                fprintf(f, "    N%d [shape=record, label=\"{{NODE %s|Type: %s}|{val: %s}}\"];\n", node_num, node_nature2string(n->nature), node_type2string(n->type), str);
            }
            break;
        case NODE_TYPE:
            fprintf(f, "    N%d [shape=record, label=\"{{NODE %s|Type: %s}}\"];\n", node_num, node_nature2string(n->nature), node_type2string(n->type));
            break;
        case NODE_LIST:
            fprintf(f, "    N%d [shape=record, label=\"{{NODE LIST}}\"];\n", node_num);
            break;
        case NODE_PROGRAM:
        case NODE_BLOCK:
        case NODE_DECLS:
        case NODE_DECL:
        case NODE_IF:
        case NODE_WHILE:
        case NODE_FOR:
        case NODE_DOWHILE:
        case NODE_PRINT:
            fprintf(f, "    N%d [shape=record, label=\"{{NODE %s|Nb. ops: %d}}\"];\n", node_num, node_nature2string(n->nature), n->nops);
            break;
        case NODE_FUNC:
            fprintf(f, "    N%d [shape=record, label=\"{{NODE %s|Nb. ops: %d}|{offset: %d}}\"];\n", node_num, node_nature2string(n->nature), n->nops, n->offset);
            break;
        case NODE_PLUS:
        case NODE_MINUS:
        case NODE_MUL:
        case NODE_DIV:
        case NODE_MOD:
        case NODE_LT:
        case NODE_GT:
        case NODE_LE:
        case NODE_GE:
        case NODE_EQ:
        case NODE_NE:
        case NODE_AND:
        case NODE_OR:
        case NODE_BAND:
        case NODE_BOR:
        case NODE_BXOR:
        case NODE_SRA:
        case NODE_SRL:
        case NODE_SLL:
        case NODE_NOT:
        case NODE_BNOT:
        case NODE_UMINUS:
        case NODE_AFFECT:
            fprintf(f, "    N%d [shape=record, label=\"{{NODE %s|Type: %s|Nb. ops: %d}}\"];\n", node_num, node_nature2string(n->nature), node_type2string(n->type), n->nops);
            break;
        default:
            printf("*** Error in %s: unknow nature : %s\n", __func__, node_nature2string(n->nature));
            assert(false);
    }

    n->node_num = node_num;

    int32_t curr_node_num = node_num + 1;
    for (int32_t i = 0; i < n->nops; i += 1) {
        int32_t new_node_num = dump_tree2dot_rec(f, n->opr[i], curr_node_num);

        fprintf(f, "    edge[tailclip=true];\n");
        fprintf(f, "    N%d -> N%d\n", node_num, curr_node_num);
        curr_node_num = new_node_num + 1;
    }

    return curr_node_num - 1;
}



static void dump_tree2dot(FILE * f, node_t root) {
    assert(root->nature == NODE_PROGRAM);

    int32_t curr_node_num = 1;
    dump_tree2dot_rec(f, root, curr_node_num);
}


void dump_tree(node_t prog_root, const char * dotname) {

    FILE * f;

    f = fopen(dotname, "w");
    fprintf(f, "digraph global_vars {\n");
    dump_tree2dot(f, prog_root);
    fprintf(f, "}");    
    fclose(f);
}


const char * node_type2string(node_type t) {
    switch (t) {
        case TYPE_NONE:
            return "TYPE NONE";
        case TYPE_INT:
            return "TYPE INT";
        case TYPE_BOOL:
            return "TYPE BOOL";
        case TYPE_VOID:
            return "TYPE VOID";
        case TYPE_STRING:
            return "TYPE STRING";
        default:
            assert(false);
    }
}


const char * node_nature2string(node_nature t) {
    switch (t) {
        case NONE:
            return "NONE";
        case NODE_PROGRAM:
            return "PROGRAM";
        case NODE_BLOCK:
            return "BLOCK";
        case NODE_LIST:
            return "LIST";
        case NODE_DECLS:
            return "DECLS";
        case NODE_DECL:
            return "DECL";
        case NODE_IDENT:
            return "IDENT";
        case NODE_TYPE:
            return "TYPE";
        case NODE_INTVAL:
            return "INTVAL";
        case NODE_BOOLVAL:
            return "BOOLVAL";
        case NODE_STRINGVAL:
            return "STRINGVAL";
        case NODE_FUNC:
            return "FUNC";
        case NODE_IF:
            return "IF";
        case NODE_WHILE:
            return "WHILE";
        case NODE_FOR:
            return "FOR";
        case NODE_DOWHILE:
            return "DOWHILE";
        case NODE_PLUS:
            return "PLUS";
        case NODE_MINUS:
            return "MINUS";
        case NODE_MUL:
            return "MUL";
        case NODE_DIV:
            return "DIV";
        case NODE_MOD:
            return "MOD";
        case NODE_LT:
            return "LT";
        case NODE_GT:
            return "GT";
        case NODE_LE:
            return "LE";
        case NODE_GE:
            return "GE";
        case NODE_EQ:
            return "EQ";
        case NODE_NE:
            return "NE";
        case NODE_AND:
            return "AND";
        case NODE_OR:
            return "OR";
        case NODE_BAND:
            return "BAND";
        case NODE_BOR:
            return "BOR";
        case NODE_BXOR:
            return "BXOR";
        case NODE_SLL:
            return "SLL";
        case NODE_SRA:
            return "SRA";
        case NODE_SRL:
            return "SRL";
        case NODE_NOT:
            return "NOT";
        case NODE_BNOT:
            return "BNOT";
        case NODE_UMINUS:
            return "UMINUS";
        case NODE_AFFECT:
            return "AFFECT";
        case NODE_PRINT:
            return "PRINT";
        default:
            fprintf(stderr, "*** Error in %s: Unknown node nature: %d\n", __func__, t);
            exit(1);
    }
}




const char * node_nature2symb(node_nature t) {
    switch (t) {
        case NONE:
        case NODE_PLUS:
            return "+";
        case NODE_MINUS:
            return "-";
        case NODE_MUL:
            return "*";
        case NODE_DIV:
            return "/";
        case NODE_MOD:
            return "%";
        case NODE_LT:
            return "<";
        case NODE_GT:
            return ">";
        case NODE_LE:
            return "<=";
        case NODE_GE:
            return ">=";
        case NODE_EQ:
            return "==";
        case NODE_NE:
            return "!=";
        case NODE_AND:
            return "&&";
        case NODE_OR:
            return "||";
        case NODE_BAND:
            return "&";
        case NODE_BOR:
            return "|";
        case NODE_BXOR:
            return "^";
        case NODE_SRA:
            return ">>";
        case NODE_SRL:
            return ">>>";
        case NODE_SLL:
            return "<<";
        case NODE_NOT:
            return "!";
        case NODE_BNOT:
            return "~";
        case NODE_UMINUS:
            return "-";
        default:
            fprintf(stderr, "*** Error in %s: Unknown node nature: %d\n", __func__, t);
            exit(1);
    }
}




