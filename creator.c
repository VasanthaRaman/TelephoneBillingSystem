#include<stdio.h>
 struct plan{
int code;
char name[30];
char data[5];
int isd,loc,std,msg;
float cost;
};
void main()
{
 struct plan p[5];
 FILE *ff;
 ff=fopen("plan_layout.bin","wb");
 printf("Enter the details of  3plan:\n");
 for(int i=0;i<3;i++)
{
 printf("Code:");
 scanf("%d",&p[i].code);
 printf("Name:");
 scanf("%s",p[i].name);
 printf("data:");
 scanf("%s",p[i].data);
 printf("isd loc std msg ");
 scanf("%d %d %d %d",&p[i].isd,&p[i].loc,&p[i].std,&p[i].msg);
 printf("cost:");
 scanf("%f",&p[i].cost);
 fwrite(&p[i],sizeof(struct plan),1,ff);
}
 fclose(ff);
}
 
 
 
 
