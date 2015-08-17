#include "nemu.h"

#include "stdlib.h"
/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <sys/types.h>
#include <regex.h>

enum {
	NOTYPE = 256, EQ,DATA

	/* TODO: Add more token types */

};

static struct rule {
	char *regex;
	int token_type;
} rules[] = {

	/* TODO: Add more rules.
	 * Pay attention to the precedence level of different rules.
	 */

	{" +",	NOTYPE},				// spaces
	{"[0-9]+", DATA},					// plus

    {"\\(", '('},						// mul
    {"\\)", ')'},						// mul

	{"\\+", '+'},					// plus
  //  {"-", '-'},						  // sub
	{"==", EQ},						// equal
    {"\\*", '*'},						// mul

    //{"==", EQ}						// equa
};

#define NR_REGEX (sizeof(rules) / sizeof(rules[0]) )

static regex_t re[NR_REGEX];

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
			Assert(ret != 0, "regex compilation failed: %s\n%s", error_msg, rules[i].regex);
		}
	}
}

typedef struct token {
	int type;
	char str[32];
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
			if (regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0) {
				char *substr_start = e + position;
				int substr_len = pmatch.rm_eo;
				
                 
				Log("match rules[%d] = \"%s\" at position %d with len %d: %.*s", i, rules[i].regex, position, substr_len, substr_len, substr_start);
			

				/* TODO: Now a new token is recognized with rules[i]. Add codes
				 * to record the token in the array ``tokens''. For certain 
				 * types of tokens, some extra actions should be performed.
				 */
                //add by tiger
                strncpy(tokens[nr_token].str,e+position,substr_len);
				tokens[nr_token++].type=rules[i].token_type;
				position += substr_len;

				//
				//switch(rules[i].token_type) {
				//	default: ;//panic("please implement me");
		//		}

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

bool check_parentheses(int p,int q)
{
	printf("p=%d,q=%d\n",p,q);

if (tokens[p].type=='(' && tokens[q].type==')')
	return true;
return false;
}

int eval(int p,int q)
{
  int op,val1,val2;
    if(p > q) {
		printf("p=%d,q=%d\n",p,q);
        panic("Bad expression");/* Bad expression */
    }
    else if(p == q) { 
		
		return  atoi(tokens[p].str);
		
        /* Single token.
         * For now this token should be a number. 
         * Return the value of the number.
         */ 
    }
    else if(check_parentheses(p, q) == true) {
        /* The expression is surrounded by a matched pair of parentheses. 
         * If that is the case, just throw away the parentheses.
         */
         printf("p=%d,q=%d\n",p,q);
        return eval(p + 1, q - 1); 
    }
    else {
        int i,j;op=p;
		for (i=4;i<NR_REGEX;i++)
		  for (j=p;j<q;j++)
		  	if (rules[i].token_type==tokens[j].type)
		  		{
		  		  op=j;
				  j=p+1;
				  i=NR_REGEX;
		  		}
			//assert(tokens[op].type=='+');
        //op = 1;//the position of dominant operator in the token expression;
        val1 = eval(p, op - 1);
        val2 = eval(op + 1, q);

        switch(tokens[op].type) {
            case '+': return val1 + val2;
            case '-': return val1 - val2;/* ... */
            case '*': return val1 * val2;/* ... */
            case '/': /* ... */
            default: assert(0);
        }
    }
}



uint32_t expr(char *e, bool *success) {
	int result=0;
	

	

		if(!make_token(e)) {
	   	  panic("please implement me");

		*success = false;
		return 0;
	}

    
	/* TODO: Insert codes to evaluate the expression. */

     if (nr_token>0)
    {
          result =  eval(0,nr_token-1); 
    }
	//panic("please implement me");
	return result;
}


	

