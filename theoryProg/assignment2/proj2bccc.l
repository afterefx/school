%option noyywrap

ID       [a-zA-Z]([a-zA-Z]|[0-9])*
    int myFlag=0;

%%

if              { printf("IFTOK \t\t%s\n", yytext ); myFlag=0; }
then            { printf("THENTOK \t%s\n", yytext ); myFlag=0; }
else            { printf("ELSETOK \t%s\n", yytext ); myFlag=0; }
procedure       { printf("PROC \t\t%s\n", yytext ); myFlag=0; }
function        { printf("FUNC \t\t%s\n", yytext ); myFlag=0; }
"="|"<>"|"<"|"<="|">="|">"  { printf( "RELOP \t\t%s\n", yytext ); myFlag=0; }
".."            { printf("DOTDOT \t\t%s\n", yytext ); myFlag=0; }
"+"|"-"         {
                    if(myFlag==1)
                        printf( "ADDOP \t\t%s\n", yytext );
                    else
                        printf( "UNARYOP \t%s\n", yytext );
                    myFlag=0;
                }
"*"|"/"|div|mod             { printf( "MULOP \t\t%s\n", yytext ); myFlag=0; }
or              { printf( "OROP \t\t%s\n", yytext ); myFlag=0; }
and             { printf( "ANDOP \t\t%s\n", yytext ); myFlag=0; }
not             { printf( "NOTOP \t\t%s\n", yytext ); myFlag=0; }
":="            { printf( "ASSIGNOP \t%s\n", yytext ); myFlag=0; }
"("             { printf( "LPAR \t\t%s\n", yytext ); myFlag=0; }
")"             { printf( "RPAR \t\t%s\n", yytext ); myFlag=1; }
"["             { printf( "LBRK \t\t%s\n", yytext ); myFlag=0; }
"]"             { printf( "RBRK \t\t%s\n", yytext ); myFlag=1; }
,               { printf( "COMMA \t\t%s\n", yytext ); myFlag=0; }
;               { printf( "SEMICOL \t%s\n", yytext ); myFlag=0; }
:               { printf( "COLON \t\t%s\n", yytext ); myFlag=0; }
while           { printf( "WHILETOK \t%s\n", yytext ); myFlag=0; }
do              { printf( "DOTOK \t\t%s\n", yytext ); myFlag=0; }
begin           { printf( "BEGINTOK \t%s\n", yytext ); myFlag=0; }
end             { printf( "ENDTOK \t\t%s\n", yytext ); myFlag=0; }
real            { printf( "REALTOK \t%s\n", yytext ); myFlag=0; }
integer         { printf( "INTTOK \t\t%s\n", yytext ); myFlag=0; }
boolean         { printf( "BOOLTOK \t%s\n", yytext ); myFlag=0; }
array           { printf( "ARRAYTOK \t%s\n", yytext ); myFlag=0; }
of              { printf( "OFTOK \t\t%s\n", yytext ); myFlag=0; }
var             { printf( "VARTOK \t\t%s\n", yytext ); myFlag=0; }
program         { printf( "PROGRAM \t%s\n", yytext ); myFlag=0; }
true|false      { printf( "BCONST \t\t%s\n", yytext ); myFlag=0; }
[0-9]+(\.[0-9]+)?(E("+"|"-")?[0-9]+)? {printf("NUM \t\t%s\n", yytext );  myFlag=1; }
"."            { printf("DOT \t\t%s\n", yytext ); myFlag=0; }
{ID}            { printf("ID \t\t%s\n", yytext ); myFlag=1;}
"{"[^}]*?"}" /*get rid of garbage */
[ \t\n]+          /* eat up whitespace */

.           {printf( "ERROR \t\t%s\n", yytext ); myFlag=0;}

%%
main( argc, argv )
int argc;
char **argv;
{
    if ( argc == 0 )
    {
        printf("Please provide an input file.\n");
        return 0;
    }

    yylex();
}
