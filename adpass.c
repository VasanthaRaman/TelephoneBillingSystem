#include<stdio.h>
#include<string.h>
struct admin{
 char name[40];
 char password[40];
};
void main()
{ 
 struct admin a;
 strcpy(a.name,"admin");
 strcpy(a.password,"password");
 FILE *f;
 f=fopen("admin.bin","wb");
 fwrite(&a,sizeof(struct admin),1,f);
 fclose(f);
}
