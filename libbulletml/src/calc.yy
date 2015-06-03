/* 中間記法電卓 -- calc */

%{
typedef float NumType;

#define YYSTYPE float
#define YYERROR_VERBOSE

#include <cmath>
#include <cctype>
#include <cstring>

#include <vector>
#include <sstream>

#include "calc.h"
#include "formula.h"
#include "formula-variables.h"

int yyerror(const char* s);
int yylex();

const char* yyinStr;

typedef Formula<NumType> CalcFormula;
typedef Number<NumType> CalcNumber;
typedef Random<NumType> CalcRandom;
typedef Rank<NumType> CalcRank;
typedef Param<NumType> CalcParam;
typedef Operator CalcOperator;

namespace {
	CalcFormula* formula;
	std::vector<CalcFormula*> formulas;

	CalcFormula* f(float d) { return formulas[(int)d]; }

	int paramId;
}

%}

/* BISON宣言 */
%token NUM RAND RANK PARAM
%left '-' '+'
%left '*' '/'
%left NEG     /* negation--単項マイナス */
%right '^'    /* べき乗関数        */

/* 文法規則が続く */
%%
input:    /* 空文字列 */
        | input line
;

line:     '\n'
| exp '\n'  { formula = f($1); return 0; }
;

exp:    NUM {
	        $$ = formulas.size();
	        formulas.push_back(new CalcFormula(new CalcNumber($1)));
        }
        | RAND {
	        $$ = formulas.size();
            formulas.push_back(new CalcFormula(new CalcRandom()));
        }
        | RANK {
			$$ = formulas.size();
			formulas.push_back(new CalcFormula(new CalcRank()));
		}
        | PARAM {
			$$ = formulas.size();
			formulas.push_back(new CalcFormula(new CalcParam(paramId)));
		}
        | exp '+' exp {
		    $$ = formulas.size();
			formulas.push_back(new CalcFormula(f($1), op_add, f($3)));
		}
        | exp '-' exp {
		    $$ = formulas.size();
			formulas.push_back(new CalcFormula(f($1), op_sub, f($3)));
		}
        | exp '*' exp {
		    $$ = formulas.size();
			formulas.push_back(new CalcFormula(f($1), op_mul, f($3)));
		}
        | exp '/' exp {
		    $$ = formulas.size();
			formulas.push_back(new CalcFormula(f($1), op_div, f($3)));
		}
        | '-' exp  %prec NEG {
		    $$ = $2;
			f($2)->setHeadSub();
		}
        | '(' exp ')' {
		    $$ = $2;
		}
;
%%

/**
 * 字句解析器は、数値を読めば、double型の値をスタックに積んで
 * トークン「NUM」を返し、数値以外を読めば、その文字のアスキー符号を返す。
 * 空白とタブは読み飛ばされる。ファイルが終わると0を返す。
 */

#include <ctype.h>
#include <stdio.h>

int yylex ()
{
	int c;

	/* 空白類を読み飛ばす  */
	while ((c = *(yyinStr++)) == ' ' || c == '\t')
		;
	/* 数値を処理する   */
	if (c == '.' || isdigit (c))
    {
		yyinStr--;
		sscanf (yyinStr, "%f", &yylval);
		while ((c = *(++yyinStr)) == '.' || isdigit(c)) {}
		return NUM;
    }

	// 変数を処理する */
	if (c == '$') {
		if (strncmp(yyinStr, "rand", 4) == 0) {
			yyinStr += 4;
			return RAND;
		}
		else if (strncmp(yyinStr, "rank", 4) == 0) {
		    yyinStr += 4;
			return RANK;
        }
		else {
			std::istringstream iss(std::string(yyinStr).substr(0, 1));
			iss >> paramId;
			yyinStr++;
			return PARAM;
		}
	}

	/* ファイルの終わりを処理する  */
	if (c == '\0')
		return 0;
	/* 1文字を返す */
	return c;
}

int yyerror(const char* s) {
	printf("yyerror: %s\n", s);
	return 0;
}

std::auto_ptr<CalcFormula> calc(const std::string& str) {
	std::string fml = str + '\n';
	yyinStr = fml.c_str();
	yyparse();
	return std::auto_ptr<CalcFormula>(formula);
}

