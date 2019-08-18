#include "data.h"

int temp=0; //临时变量
extern FILE *fp ;
extern int lookahead;
extern char token[256];
extern set <Cond> Symbol[100];
Pro Process[100];
int True[100],False[100];
const string emty="_";
struct Intertable{
    string op;
    string left;
    string right;
    string result;
}intertable[1000];
int ein=0,hin=0;
int Nxq;
int res;
int next;
int jp;
stack <string> oper,arg,st; ///定义符号栈和操作数栈
stack <int> exp[1000],huo;
///错误信息
char* err_msg[] =
{

	/*  0 */    "",
	/*  1 */    "缺少：begin，或begin拼写错误！",
	/*  2 */    "缺少：as，或as拼写错误！",
	/*  2 */    "缺少：sub，或sub拼写错误！",
	/*  2 */    "缺少：end，或end拼写错误！",
	/*  2 */    "缺少：if，或if拼写错误！",
	/*  2 */    "缺少：then，或then拼写错误！",
	/*  2 */    "缺少：else，或else拼写错误！",
	/*  2 */    "缺少：call，或call拼写错误！",
	/*  2 */    "缺少：do，或do拼写错误！",
	/*  3 */    "缺少：integer，或integer拼写错误！",
	/*  2 */    "缺少：float，或float拼写错误！",
	/*  2 */    "缺少：input，或input拼写错误！",
	/*  2 */    "缺少：output，或output拼写错误！",
	/*  2 */    "缺少：var，或var拼写错误！",
	/*  2 */    "缺少：标识符，或标识符拼写错误！",
	/*  2 */    "缺少：整数，或整数拼写错误！",
	/*  7 */    "缺少：+，或+拼写错误！",
	/*  2 */    "缺少：-，或-拼写错误！",
	/*  2 */    "缺少：*，或*拼写错误！",
	/*  2 */    "缺少：/，或/拼写错误！",
	/*  2 */    "缺少：(，或(拼写错误！",
	/*  2 */    "缺少：)，或)拼写错误！",
	/*  2 */    "缺少：=，或=拼写错误！",
	/*  2 */    "缺少：,，或,拼写错误！",
	/*  2 */    "缺少：;，或;拼写错误！",
	/*  2 */    "缺少：>，或>拼写错误！",
	/*  2 */    "缺少：<，或<拼写错误！",
	/*  2 */    "缺少：程序尾部出现多余单词",
	/*  2 */    "非法语句",
	/*  2 */    "缺少：==，或==拼写错误！",
	/*  2 */    "缺少: <=，或<=拼写错误！",
	/*  2 */    "缺少：>=，或>=；拼写错误！",
	/*  2 */    "缺少：小数，或小数拼写错误！",
	/*  2 */    "缺少：与，或与拼写错误！",
	/*  2 */    "缺少：或，或或拼写错误！",
	/*  2 */    "缺少：<>，或<>拼写错误！",
};
void program();
void func();
void parameterList();
void statement();
void Complex();
void definitions();
int dataType();
void Input();
void Output();
void Assignment();
void Call();
void IF_statement();
void loop();
void Expression();
void Passing(int x);
void Item();
void Factor();
void Bool();
string Relations();
string relation();
///报错
void error (int n)
{
    printf("%s\n",err_msg[n]);
    fclose(fp);
    getchar();
    exit(0);
}

void advance()
{
    lookahead=getToken();
}

bool match(int Tok)
{
    bool matched=true;
    if(lookahead != Tok){
        //cout<<Tok<<endl;
        //cout<<token<<endl;
        matched=false;
        error(Tok);
    }
    advance();
    return matched;
}

///1.<程序>—> <过程> [<过程>]
void program() {
    ein=0;
    Nxq=0;
    res=1;
    func();
    temp++;
	while (lookahead == sub){
		func();
		temp++;
	}
	if(lookahead!=FEOF) {
		error(unexpectedFileEnd);
	}
	for(int i=0;i<Nxq;i++){
        cout<<i<<".("<<intertable[i].op<<","<<intertable[i].left<<","<<intertable[i].right<<","<<intertable[i].result<<")"<<endl;
	}
	cout<<Nxq<<".(....)"<<endl;
}
///<过程>—> sub id  <参数列表> <语句> end sub
void func()
{
    //cout<<token<<endl;
    match(sub);
    //cout<<token<<endl;
    string str;
    str.clear();
    int len=strlen(token);
    for(int i=0;i<len;i++){
        str.push_back(token[i]);
    }

    for(int i=0;i<temp;i++){
        if(str==Process[i].str){
            printf("错误 ：过程 %s 重复定义\n",token);
            exit(0);
        }
    }
    Process[temp].str=str;
    match(id);

    parameterList();   ///参数列表
    //cout<<token<<endl;
    statement();       ///语句
    //cout<<token<<endl;
    match(END);
    match(sub);
    string Result;
}
///<参数列表>—> ( id as <数据类型> [，id as <数据类型>] )
void parameterList()
{
    int amount=0;
    string str;
    Cond ss;
    set<Cond>::iterator it;
    if(lookahead==LP){
        match(LP);
        //cout<<token<<endl;
        if(lookahead==id){
            int len=strlen(token);
            str.clear();
            for(int i=0;i<len;i++){
                str.push_back(token[i]);
            }
            ss.str=str;
            it=Symbol[temp].find(ss);
            if(it!=Symbol[temp].end()){
                printf("错误：%s重复定义\n",token);
                exit(0);
            }
        }
        match(id);
        match(as);
        int type=dataType();  ///数据类型
        ss.type=type;
        Symbol[temp].insert(ss);
        Process[temp].pname[amount]=type;
        amount++;
        //cout<<type<<endl;
        //cout<<lookahead<<endl;
        while(lookahead==comma){
            advance();
            if(lookahead==id){
                int len=strlen(token);
                str.clear();
                for(int i=0;i<len;i++){
                    str.push_back(token[i]);
                }
                ss.str=str;
                it=Symbol[temp].find(ss);
                if(it!=Symbol[temp].end()){
                    printf("错误：%s重复定义\n",token);
                    exit(0);
                }
            }
            match(id);
            match(as);
            type=dataType();
            ss.type=type;
            Symbol[temp].insert(ss);
            Process[temp].pname[amount]=type;
            amount++;
        }
        Process[temp].amount=amount;
        //cout<<lookahead<<endl;
        match(RP);
    }
}
/**<语句>—> <分支语句>
             | <赋值语句>
             | <循环语句>
             | <输入语句>
             | <输出语句>
             | <函数调用语句>
             | <变量定义语句>
             | <复合语句>
             |ε
**/
void statement()
{
    while(lookahead == IF || lookahead == id || lookahead == WHILE || lookahead ==input|| lookahead == output
          || lookahead ==  call || lookahead == var || lookahead ==BEGIN){
        if(lookahead == IF){
            IF_statement();
        }
        else if(lookahead == id){
            Assignment();
        }
        else if(lookahead == WHILE){
            loop();
            string Result;
            intertable[Nxq].op="j";
            intertable[Nxq].left="_";
            intertable[Nxq].right="_";
            int x=jp+1;
            Result.clear();
            while(x){
                if(x==jp+1){
                    x--;
                }
                Result.push_back(x%10+'0');
                x/=10;
            }
            reverse(Result.begin(),Result.end());
            intertable[Nxq].result=Result;
            Nxq++;
            intertable[Nxq].op="jz";
            intertable[Nxq].left="_";
            intertable[Nxq].right="_";
            x=Nxq+1;
            Result.clear();
            while(x){
                Result.push_back(x%10+'0');
                x/=10;
            }
            reverse(Result.begin(),Result.end());
            intertable[Nxq].result=Result;
            Nxq++;
        }
        else if(lookahead == input){
            Input();
        }
        else if(lookahead == output){
            Output();
        }
        else if(lookahead == call){
            Call();   ///调用
        }
        else if(lookahead == var){
            definitions();  ///变量定义
        }
        else if(lookahead == BEGIN){
            Complex();      ///复合语句
        }
    }
}

///<复合语句>—> begin  <语句>  [ ,  <语句> ] end
void Complex()
{
    //cout<<token<<endl;
    match(BEGIN);
    statement();
    //cout<<lookahead<<endl;
    while(lookahead==comma){
        advance();
        statement();
    }
    //cout<<token<<endl;
    match(END);
}
///<变量定义语句>—> var id [ , id ] as <数据类型> ；
void definitions(){

    //cout<<token<<endl;
    match(var);
    int index=0;
    Cond ss;
    string sem[100];
    string str;
    set<Cond> ::iterator it;
    if(lookahead==id){
        int len=strlen(token);
        str.clear();
        for(int i=0;i<len;i++){
            str.push_back(token[i]);
        }
        ss.str=str;

        it=Symbol[temp].find(ss);
        if(it!=Symbol[temp].end()){
            printf("错误：%s重复定义\n",token);
            exit(0);
        }
        sem[index++]=str;
    }
    match(id);
    //cout<<lookahead<<endl;
    while(lookahead == comma){
        advance();
        if(lookahead==id){
            int len=strlen(token);
            str.clear();
            for(int i=0;i<len;i++){
                str.push_back(token[i]);
            }
            for(int i=0;i<index;i++){
                if(str==sem[i]){
                    printf("错误：%s重复定义\n",token);
                    exit(0);
                }
            }
            ss.str=str;
            it=Symbol[temp].find(ss);
            if(it!=Symbol[temp].end()){
                printf("错误：%s重复定义\n",token);
                exit(0);
            }
            sem[index++]=str;
        }
        //cout<<token<<endl;
        match(id);
    }
    match(as);
    int type=dataType();
    //cout<<lookahead<<endl;
    for(int i=0;i<index;i++){
        ss.str=sem[i];
        ss.type=type;
        Symbol[temp].insert(ss);
    }
    match(semi);
}
///<数据类型>—> integer | float
int dataType()
{
    if(lookahead==integer){
        advance();
        //cout<<lookahead<<endl;
        return integer;
    }
    else if(lookahead==FLOAT){
        advance();
        return FLOAT;
    }
    else{
        match(integer);
    }
}
///<输入语句>—> input  id  [ , id ] ；
void Input(){
    match(input);
    if(lookahead==id){
        Cond ss;
        int len=strlen(token);
        ss.str.clear();
        for(int i=0;i<len;i++){
            ss.str.push_back(token[i]);
        }
        set<Cond>::iterator it;
        it=Symbol[temp].find(ss);
        if(it==Symbol[temp].end()){
            printf("错误：变量%s未定义\n",token);
            exit(0);
        }
    }
    match(id);
    while(lookahead == comma){
        advance();
        match(id);
    }
    match(semi);
}
///<输出语句>—> output  <表达式>  [ ,  <表达式> ] ；
void Output(){
    match(output);
    Expression();
    while(lookahead == comma){
        Expression();
    }
    match(semi);
}
///<赋值语句>—> id  =  <表达式> ；
void Assignment()
{

    while(!oper.empty()){
        oper.pop();
    }
    while(!arg.empty()){
        arg.pop();
    }
    if(lookahead==id){
        Cond ss;
        int len=strlen(token);
        ss.str.clear();
        for(int i=0;i<len;i++){
            ss.str.push_back(token[i]);
        }
        set<Cond>::iterator it;
        it=Symbol[temp].find(ss);
        if(it==Symbol[temp].end()){
            printf("错误：变量%s未定义\n",token);
            exit(0);
        }
        arg.push(ss.str);
    }
    match(id);
    //cout<<"ferw"<<lookahead<<endl;
    match(equ);
    oper.push("=");
    Expression();
    //cout<<lookahead<<endl;
    match(semi);
    while(!oper.empty()){
        string op=oper.top();
        oper.pop();
        //cout<<arg.size()<<endl;
        string you=arg.top();
        arg.pop();
        string zuo=arg.top();
        arg.pop();
        if(op=="="){
            intertable[Nxq].left=zuo;
            intertable[Nxq].right=emty;
            intertable[Nxq].op="=";
            intertable[Nxq].result=you;
            Nxq++;
        }
        else{
            intertable[Nxq].left=zuo;
            intertable[Nxq].right=you;
            intertable[Nxq].op=op;
            string Result;
            int x=res;
            while(x){
               if(x==res){
                        x-=1;
                }
                Result.push_back(x%10+'0');
                x/=10;
            }
            Result.push_back('T');
            reverse(Result.begin(),Result.end());
            intertable[Nxq].result=Result;
            arg.push(Result);
            res++;
            Nxq++;
        }
    }

}
///<函数调用语句>—> call id (  <传递参数>  ) ；
void Call(){
    match(call);
    int ii=-1;
    if(lookahead==id){
        string str;
        int len=strlen(token);
        str.clear();
        for(int i=0;i<len;i++){
            str.push_back(token[i]);
        }
        for(int i=0;i<=temp;i++){
            if(str==Process[i].str){
                ii=i;
            }
        }
        if(ii==-1){
            printf("错误：过程%s未定义\n",token);
            exit(0);
        }
    }
    match(id);
    match(LP);
    Passing(ii);
    match(RP);
    match(semi);
}
///<传递参数>—> id [ ,  id ]
void Passing(int ii)
{
    int amount=0;
    if(lookahead==id){
        Cond ss;
        int len=strlen(token);
        ss.str.clear();
        for(int i=0;i<len;i++){
            ss.str.push_back(token[i]);
        }
        set<Cond>::iterator it;
        it=Symbol[temp].find(ss);
        if(it==Symbol[temp].end()){
            printf("错误：变量%s未定义\n",token);
            exit(0);
        }
        if(Process[ii].pname[amount]!=it->type){
            printf("传递参数%s类型错误\n",token);
            exit(0);
        }
        amount++;
    }
    match(id);
    while(lookahead==comma){
        advance();
        if(lookahead==id){
            Cond ss;
            int len=strlen(token);
            ss.str.clear();
            for(int i=0;i<len;i++){
                ss.str.push_back(token[i]);
            }
            set<Cond>::iterator it;
            it=Symbol[temp].find(ss);
            if(it==Symbol[temp].end()){
                printf("错误：变量%s未定义\n",token);
                exit(0);
            }
            len=strlen(token);
            ss.str.clear();
            for(int i=0;i<len;i++){
                ss.str.push_back(token[i]);
            }
            it=Symbol[temp].find(ss);
            if(it==Symbol[temp].end()){
                printf("错误：变量%s未定义\n",token);
                exit(0);
            }
            if(Process[ii].pname[amount]!=it->type){
                printf("传递参数%s类型错误\n",token);
                exit(0);
            }
            amount++;
        }
        match(id);
    }
    if(amount!=Process[ii].amount){
        printf("错误：传参数错误\n");
    }
}
/**<分支语句>—> if  <布尔表达式> then  <语句>
                 |  if  <布尔表达式> then  <语句> else <语句>
**/
void IF_statement()
{
    ein++;
    int sss=ein;
    while(!huo.empty()){
        huo.pop();
    }
    True[sss]=-1;
    False[sss]=-1;
    match(IF);
    Bool();
    True[sss]=Nxq;
    match(then);


    statement();

    False[sss]=Nxq;
    string Result;
    if(lookahead==ELSE){
        advance();
        intertable[Nxq].op="j";
        intertable[Nxq].left=intertable[Nxq].right="_";
        int ss=Nxq;
        Nxq++;
        False[sss]=Nxq;
        statement();
        int x=Nxq;
        Result.clear();
        while(x){
            Result.push_back(x%10+'0');
            x/=10;
        }
        reverse(Result.begin(),Result.end());
        intertable[ss].result=Result;
    }

    next=-1;
    //cout<<exp[sss].size()<<" "<<st.size()<<endl;
    while(!exp[sss].empty()){
        int d=exp[sss].top();
        exp[sss].pop();
        if(next==-1){
            next=d;
            Result.clear();
            int re=False[sss];
            while(re){
                Result.push_back(re%10+'0');
                re/=10;
            }
            reverse(Result.begin(),Result.end());
            intertable[d].result=Result;
            d=exp[sss].top();
            exp[sss].pop();
            Result.clear();
            re=True[sss];
            while(re){
                Result.push_back(re%10+'0');
                re/=10;
            }
            reverse(Result.begin(),Result.end());
            intertable[d].result=Result;
        }
        else if(!st.empty()){
            //cout<<st.size()<<endl;
            string op=st.top();
            st.pop();
            if(op=="and"){
                Result.clear();
                int re;
                if(huo.empty()){
                  re=False[sss];
                }
                else{
                    re=huo.top();
                }
                while(re){
                    Result.push_back(re%10+'0');
                    re/=10;
                }
                reverse(Result.begin(),Result.end());
                intertable[d].result=Result;
                d=exp[sss].top();
                exp[sss].pop();
                Result.clear();
                re=next-1;
                while(re){
                    Result.push_back(re%10+'0');
                    re/=10;
                }
                reverse(Result.begin(),Result.end());
                intertable[d].result=Result;
                next=d;
            }
            else if(op=="or"){
                huo.push(next-1);
                int re=huo.top();
                Result.clear();
                while(re){
                    Result.push_back(re%10+'0');
                    re/=10;
                }
                reverse(Result.begin(),Result.end());
                intertable[d].result=Result;
                d=exp[sss].top();
                exp[sss].pop();
                Result.clear();
                re=True[sss];
                while(re){
                    Result.push_back(re%10+'0');
                    re/=10;
                }
                reverse(Result.begin(),Result.end());
                intertable[d].result=Result;
                next=d+1;
            }
        }
    }
}

///<循环语句>—> while  <布尔表达式> do <语句>
void loop()
{
    ein++;
    int sss=ein;
    while(!huo.empty()){
        huo.pop();
    }
    True[sss]=-1;
    False[sss]=-1;
    match(WHILE);
    jp=Nxq;
    Bool();
    True[sss]=Nxq;
    match(DO);
    statement();
    False[sss]=Nxq;
    string Result;
    next=-1;
    while(!exp[sss].empty() && !st.empty()){
        int d=exp[sss].top();
        exp[sss].pop();
        if(next==-1){
            next=d;
            Result.clear();
            int re=False[sss];
            while(re){
                Result.push_back(re%10+'0');
                re/=10;
            }
            reverse(Result.begin(),Result.end());
            intertable[d].result=Result;
            d=exp[sss].top();
            exp[sss].pop();
            Result.clear();
            re=True[sss];
            while(re){
                Result.push_back(re%10+'0');
                re/=10;
            }
            reverse(Result.begin(),Result.end());
            intertable[d].result=Result;
        }
        else{
            //cout<<st.size()<<endl;
            string op=st.top();
            st.pop();
            if(op=="and"){
                Result.clear();
                int re;
                if(huo.empty()){
                  re=False[sss];
                }
                else{
                    re=huo.top();
                }
                while(re){
                    Result.push_back(re%10+'0');
                    re/=10;
                }
                reverse(Result.begin(),Result.end());
                intertable[d].result=Result;
                d=exp[sss].top();
                exp[sss].pop();
                Result.clear();
                re=next-1;
                while(re){
                    Result.push_back(re%10+'0');
                    re/=10;
                }
                reverse(Result.begin(),Result.end());
                intertable[d].result=Result;
                next=d;
            }
            else if(op=="or"){
                huo.push(next-1);
                int re=huo.top();
                Result.clear();
                while(re){
                    Result.push_back(re%10+'0');
                    re/=10;
                }
                reverse(Result.begin(),Result.end());
                intertable[d].result=Result;
                d=exp[sss].top();
                exp[sss].pop();
                Result.clear();
                re=True[sss];
                while(re){
                    Result.push_back(re%10+'0');
                    re/=10;
                }
                reverse(Result.begin(),Result.end());
                intertable[d].result=Result;
                next=d+1;
            }
        }
    }
}
///<表达式>—> <项> [ +|－ <项> ]
void Expression()
{
    Item();
    while(lookahead==plus || lookahead == minus){
        string str;
        if(lookahead==plus){
            str="+";
        }
        else{
            str="-";
        }
        if(!oper.empty()){
            string ss=oper.top();
            while(ss=="*" || ss=="/"){

                if(oper.empty()){
                    break;
                }

                string you=arg.top();
                arg.pop();
                string zuo=arg.top();
                arg.pop();
                //cout<<zuo<<" "<<you<<endl;
                intertable[Nxq].left=zuo;
                intertable[Nxq].right=you;
                intertable[Nxq].op=ss;
                string Result;
                int x=res;
                while(x){
                   if(x==res){
                        x-=1;
                   }
                    Result.push_back(x%10+'0');
                    x/=10;
                }
                Result.push_back('T');
                reverse(Result.begin(),Result.end());
                intertable[Nxq].result=Result;
                arg.push(Result);
                oper.pop();
                ss=oper.top();
                Nxq++;
                res++;
            }
        }
        oper.push(str);
        advance();
        Item();
    }
}
///<项>—> <因子> [ *|/  <因子> ]
void Item()
{
    Factor();
    while(lookahead == multiply || lookahead == div){
        string str;
        str.clear();
        if(lookahead == multiply){
            str.push_back('*');
        }
        else if(lookahead == div){
            str.push_back('/');
        }
        /*int len=strlen(token);
        for(int i=0;i<len;i++){
            str.push_back(token[i]);
        }*/
        oper.push(str);
        advance();
        Factor();
    }

}
///<因子>—> id | con | deci |  (  <表达式>  )
void Factor()
{
    if(lookahead == id || lookahead == digits || lookahead == decimal){
        Cond ss;
        int len=strlen(token);
        ss.str.clear();
        for(int i=0;i<len;i++){
            ss.str.push_back(token[i]);
        }
        set<Cond>::iterator it;
        it=Symbol[temp].find(ss);
        if(it==Symbol[temp].end()){
            printf("错误：变量%s未定义\n",token);
            exit(0);
        }
        arg.push(ss.str);
        advance();
    }
    else{
        match(LP);

        oper.push("(");
        Expression();
        //cout<<lookahead<<endl;
        match(RP);
        string str=oper.top();
        while(str!="("){
            if(oper.empty()){
                break;
            }
            string you=arg.top();
            arg.pop();
            string zuo=arg.top();
            arg.pop();
            if(str=="="){
                intertable[Nxq].left=zuo;
                intertable[Nxq].right=emty;
                intertable[Nxq].op=str;
                intertable[Nxq].result=you;
                Nxq++;
            }
            else{
                intertable[Nxq].left=zuo;
                intertable[Nxq].right=you;
                intertable[Nxq].op=str;
                string Result;
                int x=res;
                while(x){
                   if(x==res){
                        x-=1;
                   }
                    Result.push_back(x%10+'0');
                    x/=10;
                }
                Result.push_back('T');
                reverse(Result.begin(),Result.end());
                arg.push(Result);
                intertable[Nxq].result=Result;
                res++;
                Nxq++;

            }
            oper.pop();
            str=oper.top();
        }
        //cout<<oper.top()<<endl;
        if(!oper.empty()){
            oper.pop();
        }
        //cout<<lookahead<<endl;
    }
}
///<布尔表达式>—> <关系表达式> [ and | or  <布尔表达式> ]
void Bool()
{
    string str=Relations();
    while(lookahead == AND || lookahead ==OR)
    {
        if(lookahead==AND){
            st.push("and");
        }
        else{
            st.push("or");
        }
        advance();
        Bool();
    }
    //cout<<str<<endl;
}
///<关系表达式>—> <表达式> <关系> <表达式>
void jisuan()
{
    while(!oper.empty()){
         string op=oper.top();
        oper.pop();
        //cout<<arg.size()<<endl;
        string you=arg.top();
        arg.pop();
        string zuo=arg.top();
        arg.pop();
        if(op=="="){
            intertable[Nxq].left=zuo;
            intertable[Nxq].right=emty;
            intertable[Nxq].op="=";
            intertable[Nxq].result=you;
            Nxq++;
        }
        else{
            intertable[Nxq].left=zuo;
            intertable[Nxq].right=you;
            intertable[Nxq].op=op;
            string Result;
            int x=res;
            while(x){
                if(x==res){
                    x-=1;
                }
                Result.push_back(x%10+'0');
                x/=10;
            }
            Result.push_back('T');
            reverse(Result.begin(),Result.end());
            intertable[Nxq].result=Result;
            arg.push(Result);
            res++;
            Nxq++;
        }
    }
}
string Relations()
{
    Expression();
    jisuan();
    string relationType=relation();
    jisuan();
    Expression();
    string you=arg.top();
    arg.pop();
    string zuo=arg.top();
    arg.pop();
    intertable[Nxq].left=zuo;
    intertable[Nxq].right=you;
    intertable[Nxq].op='j'+relationType;
    intertable[Nxq].result="_";
    exp[ein].push(Nxq);
    Nxq++;
    intertable[Nxq].left="_";
    intertable[Nxq].right="_";
    intertable[Nxq].op="j";
    intertable[Nxq].result="_";
    exp[ein].push(Nxq);
    Nxq++;
    return relationType;
}
///<关系>—> < | <= | > | >= | == | <>
string relation()
{
    if(lookahead==le || lookahead == LOE || lookahead == ge || lookahead == BOE || lookahead == EE || lookahead==BL){
        string op;
        switch (lookahead){
            case le:op="<" ; break;
            case LOE:op="<=";break;
            case ge:op=">";break;
            case BOE:op=">=";break;
            case EE:op="==";break;
            case BL:op="<>";break;
            default:break;
        }
        match(lookahead);
        return op;
    }
}
