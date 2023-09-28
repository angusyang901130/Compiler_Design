%{
#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

extern int32_t line_num;  /* declared in scanner.l */
extern char buffer[];     /* declared in scanner.l */
extern FILE *yyin;        /* declared by lex */
extern char *yytext;      /* declared by lex */

extern int yylex(void); 
static void yyerror(const char *msg);
%}

%token ID INT FLOAT SCIENTIFIC OCT STRING
%token MOD ASSIGN LT GT EQ NLT NGT NEQ AND OR NOT
%token KWvar KWarray KWof KWboolean KWinteger KWreal KWstring KWtrue KWfalse
%token KWdef KWreturn KWbegin KWend KWwhile KWdo KWif KWthen KWelse KWfor KWto KWprint KWread

%union {
    int     value;
    double  dval;
    char*   text;
}

%start Program

%left '+' '-'
%left '*' '/'

%type <value> expr 

%%

Program: ProgramName
         DeclareList
         FunctionList
         CompoundStmt
         KWend 
;

Stmt: CompoundStmt | SimpleStmt | ReturnStmt | ifStmt | FunctCallStmt | WhileLoop | ForLoop
;

CompoundStmt: KWbegin 
              DeclareList 
              StmtList
              KWend 
;

SimpleStmt: VarRef ASSIGN expr ';'
          | KWprint expr ';'
          | KWread VarRef ';'
          ;

expr: FunctCall
    | VarRef
    | '(' expr ')' 
    | expr '+' expr 
    | expr '-' expr 
    | expr '*' expr 
    | expr '/' expr 
    | expr MOD expr 
    | expr AND expr
    | expr OR expr
    | NOT expr
    | '-' expr %prec '*' 
    | expr Relop expr
    | Num
    | STRING 
;

VarRef: ID ArrayRef
;

ArrayRef: 
        | AR
;

AR: '[' expr ']'
  | AR '[' expr ']'
;

ReturnStmt: KWreturn expr ';'
;

ifStmt: KWif expr KWthen
        CompoundStmt
        KWelse
        CompoundStmt
        KWend KWif
      | KWif expr KWthen
        CompoundStmt
        KWend KWif
;

FunctCallStmt: FunctCall ';'
;

FunctCall: ID '(' ParamList ')'
;

ParamList: 
         | PL 
;

PL: expr
  | PL ',' expr
;

WhileLoop: KWwhile expr KWdo
           CompoundStmt
           KWend KWdo
;

ForLoop: KWfor ID ASSIGN INT KWto INT KWdo
         CompoundStmt
         KWend KWdo
;

StmtList: 
        | SL
;

SL: Stmt
  | SL Stmt 
;

ProgramName: ID ';'
;

DeclareList:
           | DL 
;

DL: VarConstDeclaration
  | DL VarConstDeclaration

VarConstDeclaration: VarDeclaration
                   | ConstDeclaration
;

VarDeclaration: KWvar Argument ';' 
;

ConstDeclaration: KWvar IDList ':' Value ';' 
;

FunctionList:
            | FL;

FL: FunctDefDec
  | FL FunctDefDec
;

FunctDefDec: FunctDefinition
           | FunctDeclaration
;

FunctDeclaration: ID '(' ArgumentList ')' ':' ScalarType ';' 
                | ID '(' ArgumentList ')' ';'
;

FunctDefinition: ID '(' ArgumentList ')' ':' ScalarType 
                 CompoundStmt
                 KWend 
               | ID '(' ArgumentList ')'
                 CompoundStmt
                 KWend
;

ArgumentList: 
            | AL
;

AL: Argument
  | AL ';' Argument  
;

Argument: IDList ':' Type 
;

IDList: ID 
      | IDList ',' ID
;

Type: ScalarType 
    | ArrayType
;

ArrayType: MulDimArray ScalarType 
;

MulDimArray: KWarray INT KWof
           | MulDimArray KWarray INT KWof  
;

ScalarType: KWreal    
          | KWstring  
          | KWboolean 
          | KWinteger 
;

Value: Num 
     | STRING 
     | KWtrue 
     | KWfalse 
;

Num: INT
   | FLOAT
   | OCT
   | SCIENTIFIC
;

Relop: LT 
     | GT 
     | EQ 
     | NLT 
     | NGT 
     | NEQ 
;

%%

void yyerror(const char *msg) {
    fprintf(stderr,
            "\n"
            "|-----------------------------------------------------------------"
            "---------\n"
            "| Error found in Line #%d: %s\n"
            "|\n"
            "| Unmatched token: %s\n"
            "|-----------------------------------------------------------------"
            "---------\n",
            line_num, buffer, yytext);
    exit(-1);
}

int main(int argc, const char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: ./parser <filename>\n");
        exit(-1);
    }

    yyin = fopen(argv[1], "r");
    if (yyin == NULL) {
        perror("fopen() failed:");
    }

    yyparse();

    printf("\n"
           "|--------------------------------|\n"
           "|  There is no syntactic error!  |\n"
           "|--------------------------------|\n");
    return 0;
}
