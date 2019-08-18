#include "data.h"

int temp=0; //��ʱ����
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
stack <string> oper,arg,st; ///�������ջ�Ͳ�����ջ
stack <int> exp[1000],huo;
///������Ϣ
char* err_msg[] =
{

	/*  0 */    "",
	/*  1 */    "ȱ�٣�begin����beginƴд����",
	/*  2 */    "ȱ�٣�as����asƴд����",
	/*  2 */    "ȱ�٣�sub����subƴд����",
	/*  2 */    "ȱ�٣�end����endƴд����",
	/*  2 */    "ȱ�٣�if����ifƴд����",
	/*  2 */    "ȱ�٣�then����thenƴд����",
	/*  2 */    "ȱ�٣�else����elseƴд����",
	/*  2 */    "ȱ�٣�call����callƴд����",
	/*  2 */    "ȱ�٣�do����doƴд����",
	/*  3 */    "ȱ�٣�integer����integerƴд����",
	/*  2 */    "ȱ�٣�float����floatƴд����",
	/*  2 */    "ȱ�٣�input����inputƴд����",
	/*  2 */    "ȱ�٣�output����outputƴд����",
	/*  2 */    "ȱ�٣�var����varƴд����",
	/*  2 */    "ȱ�٣���ʶ�������ʶ��ƴд����",
	/*  2 */    "ȱ�٣�������������ƴд����",
	/*  7 */    "ȱ�٣�+����+ƴд����",
	/*  2 */    "ȱ�٣�-����-ƴд����",
	/*  2 */    "ȱ�٣�*����*ƴд����",
	/*  2 */    "ȱ�٣�/����/ƴд����",
	/*  2 */    "ȱ�٣�(����(ƴд����",
	/*  2 */    "ȱ�٣�)����)ƴд����",
	/*  2 */    "ȱ�٣�=����=ƴд����",
	/*  2 */    "ȱ�٣�,����,ƴд����",
	/*  2 */    "ȱ�٣�;����;ƴд����",
	/*  2 */    "ȱ�٣�>����>ƴд����",
	/*  2 */    "ȱ�٣�<����<ƴд����",
	/*  2 */    "ȱ�٣�����β�����ֶ��൥��",
	/*  2 */    "�Ƿ����",
	/*  2 */    "ȱ�٣�==����==ƴд����",
	/*  2 */    "ȱ��: <=����<=ƴд����",
	/*  2 */    "ȱ�٣�>=����>=��ƴд����",
	/*  2 */    "ȱ�٣�С������С��ƴд����",
	/*  2 */    "ȱ�٣��룬����ƴд����",
	/*  2 */    "ȱ�٣��򣬻��ƴд����",
	/*  2 */    "ȱ�٣�<>����<>ƴд����",
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
///����
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

///1.<����>��> <����> [<����>]
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
///<����>��> sub id  <�����б�> <���> end sub
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
            printf("���� ������ %s �ظ�����\n",token);
            exit(0);
        }
    }
    Process[temp].str=str;
    match(id);

    parameterList();   ///�����б�
    //cout<<token<<endl;
    statement();       ///���
    //cout<<token<<endl;
    match(END);
    match(sub);
    string Result;
}
///<�����б�>��> ( id as <��������> [��id as <��������>] )
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
                printf("����%s�ظ�����\n",token);
                exit(0);
            }
        }
        match(id);
        match(as);
        int type=dataType();  ///��������
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
                    printf("����%s�ظ�����\n",token);
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
/**<���>��> <��֧���>
             | <��ֵ���>
             | <ѭ�����>
             | <�������>
             | <������>
             | <�����������>
             | <�����������>
             | <�������>
             |��
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
            Call();   ///����
        }
        else if(lookahead == var){
            definitions();  ///��������
        }
        else if(lookahead == BEGIN){
            Complex();      ///�������
        }
    }
}

///<�������>��> begin  <���>  [ ,  <���> ] end
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
///<�����������>��> var id [ , id ] as <��������> ��
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
            printf("����%s�ظ�����\n",token);
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
                    printf("����%s�ظ�����\n",token);
                    exit(0);
                }
            }
            ss.str=str;
            it=Symbol[temp].find(ss);
            if(it!=Symbol[temp].end()){
                printf("����%s�ظ�����\n",token);
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
///<��������>��> integer | float
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
///<�������>��> input  id  [ , id ] ��
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
            printf("���󣺱���%sδ����\n",token);
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
///<������>��> output  <���ʽ>  [ ,  <���ʽ> ] ��
void Output(){
    match(output);
    Expression();
    while(lookahead == comma){
        Expression();
    }
    match(semi);
}
///<��ֵ���>��> id  =  <���ʽ> ��
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
            printf("���󣺱���%sδ����\n",token);
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
///<�����������>��> call id (  <���ݲ���>  ) ��
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
            printf("���󣺹���%sδ����\n",token);
            exit(0);
        }
    }
    match(id);
    match(LP);
    Passing(ii);
    match(RP);
    match(semi);
}
///<���ݲ���>��> id [ ,  id ]
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
            printf("���󣺱���%sδ����\n",token);
            exit(0);
        }
        if(Process[ii].pname[amount]!=it->type){
            printf("���ݲ���%s���ʹ���\n",token);
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
                printf("���󣺱���%sδ����\n",token);
                exit(0);
            }
            len=strlen(token);
            ss.str.clear();
            for(int i=0;i<len;i++){
                ss.str.push_back(token[i]);
            }
            it=Symbol[temp].find(ss);
            if(it==Symbol[temp].end()){
                printf("���󣺱���%sδ����\n",token);
                exit(0);
            }
            if(Process[ii].pname[amount]!=it->type){
                printf("���ݲ���%s���ʹ���\n",token);
                exit(0);
            }
            amount++;
        }
        match(id);
    }
    if(amount!=Process[ii].amount){
        printf("���󣺴���������\n");
    }
}
/**<��֧���>��> if  <�������ʽ> then  <���>
                 |  if  <�������ʽ> then  <���> else <���>
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

///<ѭ�����>��> while  <�������ʽ> do <���>
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
///<���ʽ>��> <��> [ +|�� <��> ]
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
///<��>��> <����> [ *|/  <����> ]
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
///<����>��> id | con | deci |  (  <���ʽ>  )
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
            printf("���󣺱���%sδ����\n",token);
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
///<�������ʽ>��> <��ϵ���ʽ> [ and | or  <�������ʽ> ]
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
///<��ϵ���ʽ>��> <���ʽ> <��ϵ> <���ʽ>
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
///<��ϵ>��> < | <= | > | >= | == | <>
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
