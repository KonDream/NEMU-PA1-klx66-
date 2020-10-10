#include "nemu.h"

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <sys/types.h>
#include <regex.h>

enum {
	NOTYPE = 256, EQ, UEQ, AND, OR, NOT, hex, num, var, reg, neg

	/* TODO: Add more token types */

};

static struct rule {
	char *regex;
	int token_type;
	int priority;
} rules[] = {

	/* TODO: Add more rules.
	 * Pay attention to the precedence level of different rules.
	 */

	{" +",	NOTYPE, 0},				// spaces
	{"\\+", '+', 4},					// plus
	{"==", EQ, 3},						// equal
	
	{"\\*", '*', 5}, 		// multiplication
	{"-", '-', 4}, 		// subtraction
	{"/", '/', 5}, 		// division
	{"\\(", '(', 7},		// left parenthesis
	{"\\)", ')', 7}, 		// right parenthesis
	{"!=", UEQ, 3}, 		// not equal	
	{"&&", AND, 2}, 		// logical AND
	{"\\|\\|", OR, 1},		// logical OR
	{"!", NOT, 6}, 		// logical NOT
	{"0[xX][A-Fa-f0-9]{1,8}", hex, 0}, //hex
	{"[0-9]{1,10}", num, 0},	// num
	{"[a_zA_Z_][a-zA-Z0-9_]*", var, 0},	// var
	{"\\$[a-dA-D][hlHL]|\\$[eE]?(ax|dx|cx|bx|bp|si|di|sp)", reg, 0}  //reg
};

#define NR_REGEX (sizeof(rules) / sizeof(rules[0]) )

static regex_t re[NR_REGEX];
static uint32_t eval(int p, int q);
static uint32_t find_dominant_operator(int p, int q);
static bool check_parentheses(int p, int q);

/* Rules are used for many times.
 * Therefore we compile them only once before any usage.
 */
void init_regex() {
	int i;
	char error_msg[128];
	int ret;

	for(i = 0; i < NR_REGEX; i ++) {
		ret = regcomp(&re[i], rules[i].regex, REG_EXTENDED);
		if(ret != 0) {
			regerror(ret, &re[i], error_msg, 128);
			Assert(ret == 0, "regex compilation failed: %s\n%s", error_msg, rules[i].regex);
		}
	}
}

typedef struct token {
	int type;
	char str[32];
	int priority;
} Token;

Token tokens[32];
int nr_token;

static bool make_token(char *e) {
	int position = 0;
	int i;
	regmatch_t pmatch;
	
	nr_token = 0;

	while(e[position] != '\0') {
		/* Try all rules one by one. */
		for(i = 0; i < NR_REGEX; i ++) {
			if(regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0) { 	// first position
				char *substr_start = e + position;
//				char *tmp = e + position + 1;
				int substr_len = pmatch.rm_eo;
					
				Log("match rules[%d] = \"%s\" at position %d with len %d: %.*s", i, rules[i].regex, position, substr_len, substr_len, substr_start);
				position += substr_len;

				/* TODO: Now a new token is recognized with rules[i]. Add codes
				 * to record the token in the array `tokens'. For certain types
				 * of tokens, some extra actions should be performed.
				 */
				
			switch(rules[i].token_type) {
				case 256: break;
				default:
					tokens[nr_token].type = rules[i].token_type;
					tokens[nr_token].priority = rules[i].priority;
					strncpy(tokens[nr_token].str, substr_start, substr_len);
					tokens[nr_token].str[substr_len] = '\0';
					nr_token ++;
				}
			//	position += substr_len;
				break;
			}
		}

		if(i == NR_REGEX) {
			printf("no match at position %d\n%s\n%*.s^\n", position, e, position, "");
			return false;
		}
	}

	return true; 
}

static bool check_parentheses(int p, int q)
{
	int cnt = 0;
	int i;
	int flag = 1;
	if(tokens[p].type == 40 && tokens[q].type == 41)
	{
	for(i = p + 1; i < q; i ++)
	{
		if(tokens[i].type == 40)
		{
			cnt ++; 
			flag = 0;
		}
		else if(tokens[i].type == 41)
		{
			cnt --;
			flag = 0;
		}
		if(cnt < 0)
		return false;
	}
	if(cnt == 0 && flag)
	return true;
	return false;
	}
	return false;
}// (1+2)

static uint32_t find_dominant_operator(int p, int q)
{
	int op = 1;
	int i, j;
	int min_pri = 15;
	for(i = p; i <= q; i ++)
	{
		if(tokens[i].type == num || tokens[i].type == hex || tokens[i].type == reg || tokens[i].type == var)
			continue;
		int flag = 0;
		bool klx = true;
		for(j = i - 1; j >= p; j --)
		{
			if(tokens[j].type == '(' && !flag)
			{
				klx = false;
				break;
			}
			if(tokens[j].type == '(')
				flag --;
			if(tokens[j].type == ')')
				flag ++;
		}// 2/-1
		if(!klx)
		continue;
		if(tokens[i].priority <= min_pri)
		{
			min_pri = tokens[i].priority;
			op = i;
		}
	}
	return op;
}

static uint32_t eval(int p, int q)
{
	if(p > q)
	{
		printf("Bad Expression\n");
		assert(0);
	}
	else if(p == q)
	{
		int klx;
		sscanf(tokens[p].str, "%d", &klx);
		return klx;
	}
	else if(check_parentheses(p, q) == true)
		return eval(p + 1, q - 1);
	else
	{
		int op;
		op = find_dominant_operator(p, q);
		printf("%d ", op);	
		if(/*p == op || */tokens[op].type == neg)
		{
			uint32_t k1 = eval(p + 1, q);
			switch(tokens[p].type)
			{
				case neg: return -k1;
				default: assert(0);
			}
	
		}		

		uint32_t k_left = eval(p, op - 1);
		uint32_t k_right = eval(op + 1, q);
		switch(tokens[op].type)
		{
			case 43: return k_left + k_right;
			case 45: return k_left - k_right;
			case 42: return k_left * k_right;
			case 47: 
		//		if(k_right == 0)
		//		printf("Illegal Expression\n");
			//	assert(k_right != 0);
				
				return k_left / k_right;
			default:
				assert(0);
				break;
		}
	}
	assert(1);
	return -123;
}

uint32_t expr(char *e, bool *success) {
	if(!make_token(e)) {
		*success = false;
		return 0;
	}
	/* TODO: Insert codes to evaluate the expression. */
	*success = true;
	int i;
	for(i = 0; i < nr_token; i ++)
	{
		if(tokens[i].type == '-' && ((i == 0) || (tokens[i - 1].type != num && tokens[i - 1].type != hex && tokens[i - 1].type != reg && tokens[i - 1].type != var && tokens[i - 1].type != ')')))
		{
			tokens[i].type = neg;
			tokens[i].priority = 6;
		}
	}
	return eval(0, nr_token - 1);
	panic("please implement me");
	return 0;
}

