%option noyywrap

DIGIT    [0-9]

%%

{DIGIT}+    { printf( "NUM \t%s\n", yytext ); }
"+"|"-"   printf( "ADDOP \t%s\n", yytext );
"("   printf( "LPAR \t%s\n", yytext );
")"   printf( "RPAR \t%s\n", yytext );
"*"|"/"  printf( "MULOP \t%s\n", yytext );

[ \t\n]+          /* remove whitespace */

.           printf( "ERROR \t%s\n", yytext );

%%

int main( argc, argv )
{
    yyin = stdin;
    yylex();
    return 0;
}

