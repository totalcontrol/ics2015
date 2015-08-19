#include "nemu.h"

#include "stdlib.h"
/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <sys/types.h>
#include <regex.h>

enum {
	NOTYPE = 256, EQ,DATA,EAX,EBX,ECX,EDX,EBP,ESP,EIP,ESI,EDI,REG,COMPUTE,BRACKET,LEFT
		,RIGHT,HEX,DEC

	/* TODO: Add more token types */

};

static struct rule {
	char *regex;
	int token_type;
	int type1;
} rules[] = {

	/* TODO: Add more rules.
	 * Pay attention to the precedence level of different rules.
	 */

	{" +",	NOTYPE,NOTYPE},				// spaces
	{"0x[0-9,a-f,A-F]+", HEX,DATA},		// plus
	{"[0-9]+", DEC,DATA},					// plus

	{"%eax", EAX,REG}, 					// mul
	{"%ebx", EBX,REG},						// mul
	{"%ecx", ECX,REG},						// mul
	{"%edx", EDX,REG},						// mul
	{"%esi", ESI,REG},						// mul
	{"%edi", EDI,REG}, 					// mul
	{"%ebp", EBP,REG},						// mul
	{"%esp", ESP,REG},						// mul
	{"%eip", EIP,REG},						// mul
    {"\\(", '(',BRACKET},						// mul
    {"\\)", ')',BRACKET},						// mul
	
	{"\\+", '+',COMPUTE},					// plus
    {"-", '-',COMPUTE},						  // sub
	{"==", EQ,COMPUTE},						// equal
    {"\\*", '*',COMPUTE},						// mul


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
	int type1;
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
                if  (e[position]!=' ')  //delete space char
				{strncpy(tokens[nr_token].str,e+position,substr_len);
				 tokens[nr_token].type1=rules[i].type1;
				 tokens[nr_token++].type=rules[i].token_type;

				}
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
bool check_expr_pair(int p,int q)
{
    int pair_count=0;int i;
	if (p>q) return false;
	for ( i=p;i<q;i++)
    {
		if (tokens[i].type=='(')
			pair_count++;
		if (tokens[i].type==')')
            pair_count--;
		if (pair_count<0)
			return false;
			
	}
	return true;

}
bool check_parentheses(int p,int q)
{
    
	//printf("p=%d,q=%d\n",p,q);
    if (p>q) return false;
	if (check_expr_pair(p,q)==false) return false;
	

if (tokens[p].type=='(' && tokens[q].type==')')
{
    if (check_expr_pair(p+1,q-1)==true)
	 return true;
}
return false;
}

int eval(int p,int q)
{
  int op,val1,val2;
  //printf("p=%d,q=%d\n",p,q);
	
	if(p > q) {
		printf("Bad expression");/* Bad expression */
		return 0;
    }
    else if(p == q) { 
		if (tokens[p].type1==DATA)
			{ 
			if (tokens[p].type==DEC)
   		     return  strtoul(tokens[p].str,NULL,10);
			else return  strtoul(tokens[p].str,NULL,16);
			}
		else if (tokens[p].type1==REG)
			{
			  switch (tokens[p].type)
              	{
                   case EAX:return cpu.eax;
				   case EBX:return cpu.ebx;
                   case ECX:return cpu.ecx;
				   case EDX:return cpu.edx;
                   case EIP:return cpu.eip;
				   case EBP:return cpu.ebp;
                   case ESP:return cpu.esp;
				   case ESI:return cpu.esi;
				   case EDI:return cpu.edi;
				   default:assert(0);;
			  }
	         
		}
		return 1;
		 /* Single token.
         * For now this token should be a number. 
         * Return the value of the number.
         */ 
    }
    else if(check_parentheses(p, q) == true) {
        /* The expression is surrounded by a matched pair of parentheses. 
         * If that is the case, just throw away the parentheses.
         */
        // printf("pair matched p=%d,q=%d\n",p,q);
        return eval(p + 1, q - 1); 
    }
    else {
        int i,j;int paircount=0;op=p;
		for (i=0;i<NR_REGEX;i++)
		{
		  if (rules[i].type1!=COMPUTE) continue;
		  for (j=p;j<q;j++)
		  	{
			  if (tokens[j].type=='(')
			  	paircount++;
			  if (tokens[j].type==')')
			  	paircount--;

		  	if (rules[i].token_type==tokens[j].type && paircount==0 )
		  		{
		  		  op=j;
				  //printf("___opcode=%d__%d\n",tokens[op].type,j);
				  j=p+1;
				  i=NR_REGEX;
		  		}
		  	}
		}
			//assert(tokens[op].type=='+');
        //op = 1;//the position of dominant operator in the token expression;
        val1 = eval(p, op - 1);
        val2 = eval(op + 1, q);

        switch(tokens[op].type) {
            case '+': return val1 + val2;
            case '-': return val1 - val2;/* ... */
            case '*': return val1 * val2;/* ... */
            case EQ:  return val1==val2; /* ... */
            default: assert(0);
			return 0;
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


	

