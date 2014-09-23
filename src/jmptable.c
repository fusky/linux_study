#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

//模拟内核默认函数
int myadd(int a,int b){return (a+b);}
int mysub(int a,int b){return (a-b);}
int mymul(int a,int b){return (a*b);}
int mydiv(int a,int b){return	b?(a/b):0;}

//模拟内核跳转表
int (*array[])(int, int) = {
	myadd, mysub, mymul, mydiv
};

int main(int argc,char **argv)
{
	int i=0;
	printf("Input a number:(1-4)");
	scanf("%d",&i);
	int index = 0;
	switch(i){
		case 1://查表操作
			index = 0;
			break;
		case 2:
			index = 1;
			break;
		case 3:
			index = 2;
			break;
		case 4:
			index = 3;
			break;
		default:break;
	}

	int x, y;
	printf("input two numbers:");
	scanf("%d %d", &x, &y);
	printf("result: %d\n", array[index](x, y));

	return 0;
}
