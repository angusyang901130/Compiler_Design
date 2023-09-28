%{
#include <stdint.h>

#define LIST                concatenateString(yytext)
#define TOKEN(t)            { LIST; if (opt_tok) printf("<%s>\r\n", #t); }
#define TOKEN_CHAR(t)       { LIST; if (opt_tok) printf("<%c>\r\n", (t)); }
#define TOKEN_STRING(t, s)  { LIST; if (opt_tok) printf("<%s: %s>\r\n", #t, (s)); }
#define ENTER               print_enter()  

#define MAX_LINE_LENG       512
uint32_t line_num = 1;
static char buffer[MAX_LINE_LENG];

static uint32_t opt_src = 1;
static uint32_t opt_tok = 1;
static uint32_t empty_str = 0;
static char *buffer_ptr = buffer;

static char* parseString(const char* yytext_ptr);
static void concatenateString(const char *yytext_ptr);
static void print_enter();

%}

digit [0-9]
letter [a-zA-Z]

INT ([1-9]{digit}*)|{digit}
FLOAT {INT}\.({digit}*[1-9]|[0])
SCI (({INT}\.({digit}*[1-9]|[0]))|{INT})([Ee])([-+]?){INT}

%x STRING
%x COMMENT0
%x COMMENT1
%x PCOMMENT
%x PCOMMENTGET

%%

<INITIAL>\" { LIST; empty_str = 1; BEGIN STRING; }
<INITIAL>"/*" { LIST; BEGIN COMMENT0; }
<INITIAL>"//" { LIST; BEGIN COMMENT1; }
<INITIAL>"//&" { LIST; BEGIN PCOMMENT; }

<STRING>([^\"]|(\"\"))* { empty_str = 0; char* new_str = parseString(yytext); TOKEN_STRING(string, new_str); }
<STRING>\" { if(empty_str) TOKEN_STRING(string, "") else LIST; empty_str = 0; BEGIN INITIAL; }

<COMMENT0>\n { ENTER; BEGIN COMMENT0; }
<COMMENT0>. { LIST; BEGIN COMMENT0; }
<COMMENT0>"*/" { LIST; BEGIN INITIAL; }

<COMMENT1>. { LIST; BEGIN COMMENT1; }
<COMMENT1>\n { ENTER; BEGIN INITIAL; }

<PCOMMENT>"S+" { LIST; opt_src = 1; BEGIN PCOMMENTGET; }
<PCOMMENT>"S-" { LIST; opt_src = 0; BEGIN PCOMMENTGET; }
<PCOMMENT>"T+" { LIST; opt_tok = 1; BEGIN PCOMMENTGET; }
<PCOMMENT>"T-" { LIST; opt_tok = 0; BEGIN PCOMMENTGET; }

<PCOMMENTGET>\n   { ENTER; BEGIN INITIAL; }
<PCOMMENTGET>.    { LIST; BEGIN PCOMMENTGET; }

"(" { TOKEN_CHAR('('); }

")" { TOKEN_CHAR(')'); }

"," { TOKEN_CHAR(','); }

";" { TOKEN_CHAR(';'); }

":" { TOKEN_CHAR(':'); }

"[" { TOKEN_CHAR('['); }

"]" { TOKEN_CHAR(']'); }

"+" { TOKEN_CHAR('+'); }

"-" { TOKEN_CHAR('-'); }

"*" { TOKEN_CHAR('*'); }

"/" { TOKEN_CHAR('/'); }

"mod" { TOKEN(mod); }

":=" { TOKEN(:=); }

"<" { TOKEN_CHAR('<'); }

"<=" { TOKEN(<=); }

"<>" { TOKEN(<>); }

">=" { TOKEN(>=); }

">" { TOKEN_CHAR('>'); }

"=" { TOKEN_CHAR('='); }

"and" { TOKEN(and); }

"or" { TOKEN(or); }

"not" { TOKEN(not); }

"var" { TOKEN(KWvar); }

"def" { TOKEN(KWdef); }

"array" { TOKEN(KWarray); }

"of" { TOKEN(KWof); }

"boolean" { TOKEN(KWboolean); }

"integer" { TOKEN(KWinteger); }

"real" { TOKEN(KWreal); }

"string" { TOKEN(KWstring); }

"true" { TOKEN(KWtrue); }

"false" { TOKEN(KWfalse); }

"while" { TOKEN(KWwhile); }

"do" { TOKEN(KWdo); }

"if" { TOKEN(KWif); }

"then" { TOKEN(KWthen); }

"else" { TOKEN(KWelse); }

"for" { TOKEN(KWfor); }

"to" { TOKEN(KWto); }

"begin" { TOKEN(KWbegin); }

"end" { TOKEN(KWend); }

"print" { TOKEN(KWprint); }

"read" { TOKEN(KWread); }

"return" { TOKEN(KWreturn); }

{letter}({letter}|{digit})* { TOKEN_STRING(id, yytext); }

{INT}|("(-"{INT}")") { TOKEN_STRING(integer, yytext); }

0[0-7]+ { TOKEN_STRING(oct_integer, yytext); }

{FLOAT}|("(-"{FLOAT}")") { TOKEN_STRING(float, yytext); }

{SCI}|("(-"{SCI}")") { TOKEN_STRING(scientific, yytext); }

\r { LIST; }

" " { LIST; }

\t { LIST; }

\n { ENTER; }

    /* Catch the character which is not accepted by rules above */
. {
    printf("Error at line %d: bad character \"%s\"\r\n", line_num, yytext);
    exit(-1);
}

%%

static char* parseString(const char* yytext_ptr){
    char* new_str = calloc(strlen(yytext_ptr), sizeof(char));
    int pos = 0;

    while(*yytext_ptr){
        if(*yytext_ptr == '\"'){
            *(new_str+pos) = '\"';
            yytext_ptr += 2;
        }else{
            *(new_str+pos) = *yytext_ptr;
            yytext_ptr++;           
        }
        pos++;
    }
    *(new_str+pos) = '\0';

    return new_str;
}

static void print_enter(){
    if (opt_src) {
        printf("%d: %s\n", line_num, buffer);
    }
    ++line_num;
    buffer[0] = '\0';
    buffer_ptr = buffer;
}

static void concatenateString(const char *yytext_ptr) {
    while (*yytext_ptr) {
        *buffer_ptr = *yytext_ptr;
        ++buffer_ptr;
        ++yytext_ptr;
    }
    *buffer_ptr = '\0';
}

int main(int argc, const char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage: ./scanner [filename]\n");
        exit(-1);
    }

    yyin = fopen(argv[1], "r");
    if (yyin == NULL) {
        fprintf(stderr, "Open file error\n");
        exit(-1);
    }

    yylex();

    return 0;
}
