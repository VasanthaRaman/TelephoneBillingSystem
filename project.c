#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<time.h>
const long long inv_constant=12345678987;
struct admin{
 char name[40];
 char password[40];
};
struct query{
  char question[200];
  char answer[200];
};

struct bill{
//period of billing
char company[100];
long int invoice;
float cost,gst,final;//sorry no vat and discount in our bill
};
struct plan{
int code;
char name[30];
char data[5];
int isd,loc,std,msg;
float cost;
};
struct user{
  char name[100];
  long long number;
  char password[100];
  char plan[200];
  struct bill b;
  struct plan p;
  struct tm date;
  struct query q;
  long int days;/*by default initialised to zero;
  It refers to the day on which the plan was subcribed*/
};
void first_screen(struct user *u,char *pl);
void welcome_user();
void gen_bill(struct user *u);
void welcome();
void admin_first_screen();

void query_view()
{
 FILE *ff;struct user y;
 ff=fopen("user_details.bin","rb+");
 if(!ff)
  printf("No users yet!\n");
 else
{
 while(fread(&y,sizeof(struct user),1,ff))
 {
   if(y.q.question!='\0')
   {  printf("Customer name: %s\n",y.name);
      printf("Phone number: %lld\n",y.number); 
      printf("Query: %s\n\n",y.q.question);
      printf("Answer to query:\n");
      scanf(" %s",y.q.answer); 
      fseek(ff,((-1)*sizeof(struct user)),SEEK_CUR);   
      fwrite(&y,sizeof(struct user),1,ff);
   }
 }
printf("That is all the queries for today!\n");
}
fclose(ff);
}

void del_record()
{ long long n; FILE *op,*t;struct user y;
 printf("Enter the number to be deleted:");
 scanf("%lld",&n);
 op=fopen("user_details.bin","rb");
 t=fopen("temp.bin","wb");
 if(!op)
  printf("No users yet!\n");
 else
 {
  while(fread(&y,sizeof(struct user),1,op))
 if(!(y.number==n))
   fwrite(&y,sizeof(struct user),1,op);
 fclose(op);
 fclose(t);
 remove("user_details.bin");
 rename("temp.bin","user_details.bin");
 printf("Deleted successfully!\n");
}
} 
void plan_change()
{FILE *f;f=fopen("plan_layout.bin","rb+");
 struct plan p;int cg;
 printf("Current plans are:\n");
 printf("Code\t\t\tName\t\t\t\tISD\tSTD\tLOC\t\tNo.of messages\t\tData pack\t\tplan cost\n");
 while(fread(&p,sizeof(struct plan),1,f))
   printf("%-22d %-32s %-7d %-7d %-10d %-12d %-5s%-3.2f\n",p.code,p.name,p.isd,p.std,p.loc,p.msg,p.data,p.cost); 
 printf("\n Enter your choice:\n1.Add new plan\n2.Delete a plan\nPress any other key to exit\n");
 scanf("%d",&cg); 
 if(cg==1)
 { 
   printf("Enter the details of new plan:\n");
   printf("plan code:");
   scanf("%d",&p.code);
   printf("plan name:");
   scanf(" %s",p.name);
   printf("No.of ISD STD and LOCAL calls:");
   scanf("%d %d %d",&p.isd,&p.std,&p.loc);
   printf("No.of messages:");
   scanf("%d",&p.msg);
   printf("Amount of data:");
   scanf(" %s",p.data);
   printf("Cost of the plan:");
   scanf("%f",&p.cost);
   fseek(f,0,SEEK_END);
   fwrite(&p,sizeof(struct plan),1,f);
   printf("New plan added successfully\n");
}
 else if(cg==2)
 { int pcode;FILE *t;
   printf("Enter the plan code to be deleted:");
   scanf("%d",&pcode);
   t=fopen("teemp.bin","wb");
   fseek(f,0,SEEK_SET);
   while(fread(&p,sizeof(struct plan),1,f))
    if(!(p.code==pcode))
       fwrite(&p,sizeof(struct plan),1,f);
   remove("plan_layout.bin");
   rename("teemp.bin","plan_layout.bin");
   printf("Updated Successfully!\n");
}
else
 return;
}
void admin_first_screen()
{int ch;
do{ 
printf("Welcome admin.Enter your choice:\n1.Queries\n2.Remove customers\n3.Change tarrif plans\n4.Exit");
 scanf("%d",&ch);
 switch(ch)
{ 
 case 1: query_view();
         break;
 case 2: del_record();
         break;
 case 3: plan_change();
         break;
 case 4: welcome();
         break;
 default: printf("Try again with correct choice\n"); 
          break;
}
}while(ch!=4);
}

void admin_login()
{ struct admin ad;char aname[40],apassword[40];
  printf("Welcome administrator!\n Enter your username and password to continue:");
  printf("Username:");
  scanf(" %s",aname);
  printf("Password:");
  scanf(" %s",apassword);
  FILE *fp;
  fp=fopen("admin.bin","rb");
  if(!fp)
  printf("No such file!\n");
  else
  { 
    fread(&ad,sizeof(struct admin),1,fp);
    if((strcmp(aname,ad.name)==0)&&(strcmp(apassword,ad.password)==0))
    admin_first_screen();
    else
   { printf("Username and password does not match!Press any key to exit\n");
    getchar();
    welcome();  }
  }
 fclose(fp);
}
void plan_read(struct user *u)
{ long int validity=0;
  FILE *fptr;int v;int fll=1;
  struct plan p1;
  fptr=fopen("plan_layout.bin","rb");
  printf("Code\t\t\tName\t\t\t\t\tISD\tSTD\tLOC\t\t\t\tNo.of messages\t\tData pack\t\tplan cost\n");
  while(fread(&p1,sizeof(struct plan),1,fptr)){
    printf("%-2d %-9s %-2d %-2d %-2d %-2d %-3s %-3.2f",p1.code,p1.name,p1.isd,p1.std,p1.loc,p1.msg,p1.data,p1.cost);
    /*u->p.code=p1.code;strcpy(u->p.name,p1.name);u->p.isd=p1.isd;u->p.std=p1.std;u->p.loc=p1.loc;u->p.msg=p1.msg;
    strcpy(u->p.data,p1.data);u->p.cost=p1.cost;*/      //u->p=p1; wont this work??
    }
    if(!(u->days)){//i.e u->days==0
    time_t sec;
    sec = time (NULL);
    validity=sec/(3600*24)-u->days;
    }
  do{
    if(validity!=0)
    { printf("New plans cannot be subscribed until your current plan expires.!Press 0 to go to homescreen\n");
      scanf("%d",&v); }
    else
    { 
      printf("Enter the code of choice of your plan or press 0 to exit:");
      scanf("%d",&v);//fseek(fp, 0, SEEK_END);
      fseek(fptr,0,SEEK_SET);
      while(fread(&p1,sizeof(struct plan),1,fptr))
       { if(p1.code==v)
     {u->p.code=p1.code;strcpy(u->p.name,p1.name);u->p.isd=p1.isd;u->p.std=p1.std;u->p.loc=p1.loc;u->p.msg=p1.msg;
      strcpy(u->p.data,p1.data);u->p.cost=p1.cost; //u->p=p1; wont this work??
      printf("You have subscribed to plan %d succcessfully",v);
      time_t T= time(NULL);
      u->date = *localtime(&T);
      time_t sec;
      sec = time (NULL);
      u->days=sec/(3600*24);
      fll=2;
      }
      }
      if((fll==1)&&(v!=0))
      printf("Plan you entered is not available!\n");
}         
}while(v!=0);
fclose(fptr);
}

void gen_bill(struct user *u)
{  struct user rt;
    time_t T= time(NULL);
    struct  tm tm = *localtime(&T);
   printf("%20s\n",u->name);
   printf("%lld\n",u->number);
   printf("Invoice Number: %lld\n",inv_constant-(u->number));
   printf("Period of Billing: %02d/%02d/%04d to %02d/%02d/%04d\n",u->date.tm_mday,u->date.tm_mon+1,u->date.tm_year+1900,tm.tm_mday, tm.tm_mon+1, tm.tm_year+1900);
   printf("Plan Name: %s\n",(u->p.name));
   printf("Cost: %.2f\n",(u->p.cost));
   printf("GST: %0.2f\n",0.05*(u->p.cost));
   printf("Final Amount(in ₹): %0.2f\n\n",(u->p.cost)*1.05);
}

void ask_query(struct user *u)
{FILE *f; struct user t;
 printf("Enter your Query:\n");
 scanf(" %s",u->q.question);
 f=fopen("user_details.bin","rb+");
 while(fread(&t,sizeof(struct user),1,f))
 { if(u->number==t.number)
   { fseek(f,(-1)*sizeof(struct user),SEEK_CUR);
     fwrite(u,sizeof(struct user),1,f);
     printf("Query submitted successfully.You will recieve a solution any time soon\n");
     break;
   }
 }
 fclose(f);
}
void view_sol(struct user *u)
{
 if(u->q.question!='\0')
 {printf("Your Query: %s",u->q.question);
  if(u->q.answer!='\0')
   printf("Answer to your query: %s",u->q.answer);
  else
   printf("Sorry.Your Query has not been ansered yet.Check again later!\n");
 }
 else
   printf("YOu have not submitted any queries!\n");
}

void first_screen(struct user *u,char *pl)
{int ch,g=1;
 printf("Welcome %s\n",u->name);
 printf("Your number: %lld\n",u->number);
 printf("Currently active plans in your account\n");
 if(*pl!='\0')
 printf("Validity: %s\n",pl);//include validity	
 else
 printf("No plans Active!\n");
 do{
 printf("Enter Your choice:");
 printf("\n1. New plans\n2. Generate bill\n3.Submit a Query\n4.View solution to previous query\n5. Log out\n");//include query
 scanf("%d",&ch);
 switch(ch)
 {
  case 1: plan_read(u);
          break;
  case 2: gen_bill(u);
          break;
  case 3: ask_query(u);
          break;
  case 4: view_sol(u);
          break;
  case 5: welcome();
          break;
 
           
}
}while(ch!=5);
}

int sign_in_check(struct user *u,char ch,char *pl){
  FILE *p;struct user t;
  if(ch=='S'){
    p=fopen("user_details.bin","rb");
    int flag=0;
    while(p){
      fread(&t, sizeof(struct user), 1, p);
      if(t.number==u->number && strcmp(u->name,t.name)==0 && strcmp(u->password,t.password)==0){
        flag=1;
        strcpy(pl,u->plan);
        break;
      }
    }
    fclose(p);
    return flag;
  }
  else if((ch=='R')||(ch=='r')){
     
    p=fopen("user_details.bin","ab");
    fwrite(u, sizeof(struct user), 1, p);
    fclose(p);
    return -1;  //-1 returned after adding data to file
  }
  printf("Entered character other than S and R\n");
  return 2; //user has entered character other than S and R
}
void welcome(){
  printf("\t\t***************************************************\n");
  printf("\t\t\tWELCOME TO TELEPHONE BILLING SYSTEM\n");
  printf("\t\t***************************************************\n\n");
  printf("Press C to continue\nE to exit\n");
  char i;
  scanf(" %c",&i);
  if((i=='E')||(i=='e')){
  printf("Thank You for using the telephone billing system!\n");
  exit(0);
  }
  printf("\n");
}
void signup_register(struct user *u,char *pl){
  int check;
  char c;
  printf("Enter choice U-user A-Admin ");
  scanf(" %c",&c);
  if((c=='U')||(c='u')){
  do
 { char ch;
  
  
  printf("Sign in - S\nRegister - R\n");
  scanf(" %c",&ch);
    printf("Name: ");
    scanf(" %s",u->name);
    printf("Telephone Number: ");
    scanf("%lld",&(u->number));
    printf("Password: ");
    scanf(" %s",u->password);
    //check if data entered is correct 
     check=sign_in_check(u,ch,pl);
  if(check==-1){
  printf("Registeration successful!\n");
  //first_screen(u,pl);
  }
  else if(!check)
  printf("Incorrect details! Try again\n");
  //else if(check==1)
  //first_screen(u,pl);
  else if(check==2)
  welcome();
} while(check==0);
}
else
admin_login();
}
int main(void) {
  //struct user u;
  char pl;
  welcome();
  //clear screen
  while(1){
  struct user u;
  signup_register(&u,&pl);
  first_screen(&u,&pl);
  }
  //printf("Thank You for using the telephone billing system!\n");
  return 0;
}
