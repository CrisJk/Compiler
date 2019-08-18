#include "data.h"
extern FILE *fp ;
extern char token[256];
bool Isalpha(char ch)
{
	return 'a' <= ch && ch <= 'z' || 'A' <= ch && ch <= 'Z';
}
bool Isdigit(char ch)
{
    return '0'<=ch && ch <='9' ;
}
char* word[20] = {"","begin","as","sub","end","if","then","else","call","while","do",
                    "integer","float","input","output","var"};
int getToken()
{
    int tokenType=ERROR;
    char ch=getc(fp);
    if(ch!=EOF){
        while(ch==' ' || ch == '\n' || ch=='\t') {
            ch=getc(fp);
        }
        if(ch == EOF) return FEOF;
        if(Isalpha(ch)){
            int j=0;
            token[j++] = ch;
            ch = getc(fp);
			while (Isalpha(ch)||Isdigit(ch)|| ch=='$')  //�������ĸ����ϱ�ʶ��������������ʶ����Ͻ���
			{
				token[j++] = ch; //��ϵı�ʶ��������token��
				ch = getc(fp);  //����һ���ַ�
				if(ch==EOF){
                    break;
				}
			}
			token[j] = '\0';     //��ʶ����Ͻ���
			//cout<<token<<endl;
			tokenType = id;
			if(ch!=EOF)fseek(fp, -1, 1);   //����һ���ַ�
            for (int i = 1; i <= 15; i++){
				if (!strcmp(token, word[i])){
					tokenType = i;
					break;
				}
            }
            if(!strcmp(token,"and")){
                tokenType=AND;
            }
            if(!strcmp(token,"or")){
                tokenType=OR;
            }
        }
        else if (Isdigit(ch))//���ִ���
		{
			int j = 0;
			token[j++] = ch;
			ch = getc(fp);  //����һ���ַ�
			while (Isdigit(ch))  //�����������������������������������Ͻ���
			{
				token[j++] = ch;  //�������������token��
				ch = getc(fp);  //����һ���ַ�
			}
			int flag=1;
			if(ch=='.'){
                flag=0;
                ch=getc(fp);
                while(isdigit(ch)){
                    token[j++]=ch;
                    ch=getc(fp);
                }
                tokenType=decimal;
			}
			if(flag){
                tokenType=digits;
			}
			token[j] = '\0';
			if (ch != EOF) fseek(fp, -1, 1);   //����һ���ַ�
		}
		else if (ch == '+')
		{
			tokenType = plus;
		}
		else if (ch == '-')
		{
			tokenType = minus;
		}
		else if (ch == '*')
		{
			tokenType = multiply;
		}
		else if (ch == '/')
		{
		    ch=getc(fp);
		    int flag=0;
		    if(ch=='/'){
                flag=1;
                while(ch!='\n'){
                    ch=getc(fp);
                    if(ch==EOF){
                        break;
                    }
                }
                if(ch!=EOF){
                    return getToken();
                }
                else{
                    tokenType=FEOF;
                }

		    }
		    else if(ch=='*'){
                flag=2;
                ch=getc(fp);
                while(1){
                    if(ch=='*'){
                        ch=getc(fp);
                        if(ch=='/'){
                            break;
                        }
                        else{
                            fseek(fp,-1,1);
                        }
                    }
                    if(ch==EOF){
                        tokenType=FEOF;
                        break;
                    }
                    ch=getc(fp);
                }
                return getToken();
		    }
		    if(flag==0){
                tokenType = div;
                if(ch!=EOF){
                    fseek(fp,-1,1);
                }
		    }
		}
		else if (ch == '(')
		{
			tokenType = LP;
		}
		else if (ch == ')')
		{
			tokenType = RP;
		}
		else if (ch == '=')
		{
		    ch=getc(fp);
		    if(ch=='='){
                tokenType=EE;
		    }
		    else{
                tokenType = equ;
                fseek(fp,-1,1);
		    }
		}
		else if (ch == ',')
		{
			tokenType = comma;
		}
		else if (ch == ';')
		{
			tokenType = semi;
		}
		else if (ch == '>')
		{
		    ch=getc(fp);
		    if(ch=='='){
                tokenType=BOE;
		    }
		    else{
                tokenType = ge;
                fseek(fp,-1,1);
		    }
		}
		else if (ch == '<')
		{
			ch=getc(fp);
		    if(ch=='='){
                tokenType=LOE;
		    }
		    else if(ch=='>'){
                tokenType=BL;
		    }
		    else{
                tokenType = le;
                fseek(fp,-1,1);
		    }
		}

	}
	else if(ch==EOF){
        return FEOF;
	}
    return tokenType;
}
