#include "data.h"
FILE * fp;
int lookahead;
char token[256];
set <Cond> Symbol[100];
extern void advance();

int main()
{
    fp=fopen("input.txt","r");
	if ((fp==NULL))
	{
		printf("��Դ�����ļ�����!n");
		return 0;
	}
	advance();

	program();
	fclose(fp);

    printf("\n�����ɹ���\n");
	getchar();
}
