#include<iostream>
#include<string>
#include "profile.h"
#include<bits/stdc++.h>
#include<sys/types.h> 
#include<sys/wait.h> 
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include<pwd.h>
using namespace std;
long statuss;
long history=0;
long his=0;
vector<string>v;
map<string,string>m;
using namespace std;
map<string,string>o;
void setmyrc_file()
{ 
   FILE *fp;
   USER=getpwuid(getuid())->pw_name;
   o["USER"]=USER;
   DIR =(getpwuid(getuid()))->pw_dir;
   o["DIR"]=DIR;
   int i=gethostname(HOSTNAME,sizeof(HOSTNAME));
   o["HOSTNAME"]=HOSTNAME;
   fp =fopen("bash.txt","w");
     if(fp!=NULL)
    {
       if(USER!=NULL)
       { fprintf(fp,"USER: %s\n",USER);
        fprintf(fp,"HOSTNAME: %s\n",HOSTNAME);
        fprintf(fp,"DIR: %s\n",DIR);
       }
   }
  fclose(fp);

}

void print_prompt()
{  /*if(size_t found = line.find("sudo"))
   {o["MODE"]="root@";
    o["PS1"]="#";
   }
   else
    {
      o["MODE"]="";
      o["PS1"]="$";
     }*/
   char p[100];
   char *w=getcwd(p,sizeof(p));
   string s=p;
   string PROMPT=/*o["MODE"]+*/o["USER"]+o["HOSTNAME"]+s/*+o["PS1"]*/;
   cout<<PROMPT;
}

int find_number_of_words_in_command(char **v)
{
	int k=0;
	while(v[k]!=NULL)
	{
		k++;
	}
	return k;
}
void error_msg_CProcess()
{
	cout<<"can't create child process\n";
}


void error_msg_exec()
{
	cout<<"execution falied\n";
}

void parse(char *line_cmd,char **v)
{
	for(int i=strlen(line_cmd)-1;i>=0;i--)
	{if(line_cmd[i]==' ')line_cmd[i]='\0';
     else
     	break;

	}
	while(*line_cmd!='\0'){
	while(*line_cmd==' '|| *line_cmd =='\t'|| *line_cmd=='\n')
	  *line_cmd++='\0';
		
	
	    *v++=line_cmd;
 
	    while(*line_cmd!='\0'&&*line_cmd!=' '&& *line_cmd!='\t'&& *line_cmd!='\n')
	    	line_cmd++;

      }
      *v=NULL;
}

/*void check_for_pipes(char line[],char **v)
{   char s;
	for(int i=0;i<strlen(line);i++)
	{  
      if(line[i]=='|')
      {
      	pipes(line);
      }
	}
  
}


void pipes(char line[])
{
	int n;
	pid_t pid;
    char line[40];
     char* val[40];
     int i=0;
     val[0]= strtok(line, "|");
      while (val[i] != NULL) {
      	i++;
        val[i] = strtok(NULL, "|"); 
       }  
      val[i]=NULL;
      int m=i;
      char *v[40];
      char *p[40];
      v[0] = strtok(val[0], " ");
      int j=0;
       while (p[j] != NULL) {
      	j++;
        p[j] = strtok(NULL, " "); 
       } 
       int k=0;
       p[0]=strtok(val[1]," ");
         while (p[k] != NULL) {
      	k++;
        p[k] = strtok(NULL, " "); 
       }
   
    int fd[2];
    pipe(fd);
    if(pipe(fd)<2)
    cout<<"error";
    if((pid=fork())<0)
    cout<<"fork error";
    else if(pid>0)
    {   dup2(fd[1],1);
    	close(fd[0]);


    }
    else if(pid==0)
    {
    	close(fd[1]);
    }
}

 void redirect(char **v)
 {  
 	pid_t pid;
    int status_Process;
    
 	int countt= find_number_of_words_in_command(v);
    char ar[100],vr[100];
 	strcpy(ar,v[countt-1]);
 	int fd=open(ar,O_WRONLY|O_TRUNC |S_IWGRP | S_IWUSR);
 	//strcpy(vr,v[0]);
 	//cout<<ar<<endl;
     v[countt-1]=NULL;
     v[countt-2]=NULL;
    
      
 	
    if((pid=fork())<0)
    {
    	
  	error_msg_CProcess();
  	exit(1);
     
     }
    
   else if(pid==0)
  {  cout<<"kjh";  
    dup2(fd,1);
  	   close(fd);
  	  cout<<"ninja";
  
  	if(execvp(*v,v)<0)
  	{
  		error_msg_exec();
  		exit(1);
  	}
  	
  }   
  else
  {
  	while(wait(&status_Process)!=pid);
  }
    
 	
     

 } 
 void change_directory(char **v)
 {
           int countt=find_number_of_words_in_command(v);
           char ar[100];
           strcpy(ar,v[countt-1]);
           if(ar=="..")
           	chdir("..");
           else
           {
           	 chdir(ar);
           }
 }


*/

void execute(char **v)
{

  pid_t pid;
  int status_Process;
  if((pid=fork())<0)
  {
  	error_msg_CProcess();
  	exit(1);
  }
  else if(pid==0)
  {
  	if(execvp(*v,v)<0)
  	{
  		error_msg_exec();
  		exit(1);
  	}
  }   
  else
  {
  	while(wait(&status_Process)!=pid){statuss=status_Process;};
  }

}

void history_make(string s)
{ FILE *fp;
  fp =fopen("history.txt","w");
  char ar[100];
  strcpy(ar,s.c_str());
  if(fp!=NULL)
  fprintf(fp,"%s\n",ar);
  v.push_back(s);
   history++; his++;
   fclose(fp);

}
void history_find_previous()
{   his--;

	if(his<0)
	cout<<v[his-1];
    else
    	cout<<"underflow";

	

}
void history_find_next()
{ if(his>=history){
	cout<<"overflow";
	return ;}
  cout<<v[his];
  his++;

}

void find_map(string a)
{  
    string s=a;int f=0;
    int i=0;
    while(s[i]!=' '&&i<s.length()){i++;}
    string p="";
         
    while(s[i]==' '&&i<s.length())i++;
 
    while(s[i]!=' '&&s[i]!='='&&i<s.length())
    {
       p+=s[i];
        i++;
    }
    if(s[i]==' ')
    {
      cout<<"error\n";return; 
    }
   
    /*while(s[i]==' '&&i<s.length())
    {
        i++;
    }*/
    int j=0,g=0,x=0;
    if(s[i]=='=')i++;
    char o,e,oo,ee;
    o='\'';
    if(s[i]==' '){ cout<<"error\n";return ;}
     oo=s[i];
     if(s[i]!='"'||s[i]!=o){g=1;}
      string q="";
    while(i<s.length())
    {   
      if(s[i]!='"'&&s[i]!='\'') 
        q+=s[i];
        i++;
    }
    ee=s[i-1];
  //cout<<"ee--"<<ee<<" ";
  if(oo=='"'&&ee=='"')m[p]=q;
   else if(oo=='\''&&ee=='\''){m[p]=q;}
  
  


    return ;
}



void do_single_redirection(char *vv,char**v)
{ int status;
  pid_t pid=fork();
  if(pid<0){cout<<"error\n";exit(1);}
  else if(pid==0)
  {
   int fd =open(vv,O_WRONLY|O_CREAT|O_TRUNC,S_IWUSR|S_IWUSR);
    if(fd<0){cout<<"file isn't opening ";exit(1);}
    dup2(fd,1);
    if(execvp(*v,v)<0)
    {
      cout<<"failed\n";exit(1);
    }
  }
  else
  {
    while(wait(&status)!=pid){statuss=status;};
  }
}



void do_double_redirection(char *vv,char **v)
{
   int status;
   pid_t pid=fork();
   if(pid<0){cout<<"error\n";exit(1);}
   else if(pid==0)
   {
    int fd=open(vv,O_WRONLY|O_CREAT|O_APPEND,S_IRUSR|S_IWUSR);
    if(fd<0){cout<<"failed\n";exit(1);}
    else
    {
      dup2(fd,1);
      if(execvp(*v,v)<0)
      {
        cout<<"failed\n";exit(1);
      }
    }

   }
   else
   {
    while(wait(&status)!=pid){statuss=status;};
   }

}

void change_directory(string line)
{
  char str[100];
  char *ar[100];
  strcpy(str,line.c_str());
  parse(str,ar);
  if(chdir(ar[1])!=0)
    perror("chdir failed");
}
void redirection(char **v)
{
  int n=find_number_of_words_in_command(v);
  for(int i=0;i<n;i++)
  {
    if(strcmp(v[i],">")==0){*(v+i)=NULL;do_single_redirection(v[i+1],v);}
    else if(strcmp(v[i],">>")==0){*(v+i)=NULL;do_double_redirection(v[i+1],v);}
  }
 }
 

bool check(int c,int n)
{
  if(c<n)
    return 1;
  else return 0;
}

void execute_pipe(char **ar , int n)
{  int i=0;
  int fd[2];
  pid_t pid;
     int in=0;
  //cout<<"n--"<<n<<endl;
   while(i<n)
   {
    //int fd[2];
     
     pipe(fd);
     pid=fork();
     //close(fd[1]);
     if(pid<0)
     {
      error_msg_exec();
      exit(1);
     }
     else if(pid==0)
     {   dup2(in,0);
            //close(fd[1]);
            //cout<<"i+1"<<(i+1)<<n<<endl;
             if(check(i+1,n))
            { dup2(fd[1],1);}
              close(fd[0]);
             char *exe_pipe[100];
             char a[100];
             strcpy(a,ar[i]);
            // cout<<"i---"<<i<<" "<<ar[i]<<endl;
             parse(a,exe_pipe);
            
             if(execvp(*exe_pipe,exe_pipe) < 0)
              exit(0);

      }
      else
      {
        wait(NULL);
        close(fd[1]);
        in=fd[0];
        i++;
      }
     

    }
}



void pipes(char * line)
{   char *ar[100];
 // cout<<"line--"<<line<<endl;
    ar[0] = strtok(line,"|");
    int i=1;
   // cout<<"ar0--"<<ar[0]<<endl;
    //cout<<"i--"<<i<<endl;
    while((  ar[i]=strtok(NULL,"|"))!=NULL)
    { 
   
     //cout<<"i--"<<i<<" ar[i]--"<<ar[i]<<endl;
     i++;
     }
     //cout<<"i--"<<i<<endl;
     ar[i]=NULL;   
    //cout<<"countt---"<<i<<endl; 
   int countt=i;
   //cout<<"countt--"<<countt;

   execute_pipe(ar,countt);
}
void show_history()
{
	for(int i=history-1;i>=0;i--)
    cout<<v[i]<<endl;
}

void print_val_double_quotes(string ss)
{
  int k=0;
   for(int i=0;i<ss.length();i++)
  {
  if(ss[i]!=' ')
  {
    k=i;
    break;
  }
  
 }
 //cout<<"k--"<<k<<endl;
 int dd=ss.length();
  string s=ss.substr(k+1,dd-2);
  //cout<<"double ke andr--"<<s<<endl;
 // cout<<"s---"<<s<<" s.len---"<<s.length()<<endl;
  string p="";
   for(int i=0;i<s.length();i++)
   {
    if(s[i]=='$')
    { string f="";
     //cout<<"s[i]--"<<i<<" "<<s[i];
      for(int j=i+1;s[j]!=' '&&j<s.length();j++,i++)
      {
         f+=s[j];
         //cout<<"s[j]--"<<s[j]<<endl;
      }


     // cout<<"f--"<<f<<endl;
      //cout<<"o---"<<o[f]<<endl;
      if(o[f]=="")p+='$'+f;
      else
       { p+=o[f];}
    }
    else
      p+=s[i];
      
  }
  cout<<p<<endl;
}

void print_val_single_quotes(string ss)
{
  /*int k=0;
   for(int i=0;i<ss.length();i++)
  {
  if(ss[i]!='\'')
  {
    k=i;
    break;
  }
  
 }*/
  string s=ss.substr(1,ss.length()-2);
  string p="";
  //cout<<"ss--"<<s<<s.length()<<endl;
  for(int i=0;i<s.length();i++)
  {
    if(s[i]=='$')
    { string f="";
      for(int j=i+1;s[j]!=' '&&j<s.length();j++,i++)
      {
         f+=s[j];

      }
      if(o[f]=="")p+='$'+f;
      else{
        p+=o[f];
      
      }
    }
    p+=s[i];
      
  }
  cout<<p<<endl;
}
string print_without_quote(string ss)
{
  int k=0;
   for(int i=0;i<ss.length();i++)
  {
  if(ss[i]!=' ')
  {
    k=i;
    break;
  }
  
 }
 if(ss[k]=='\'')k++;
 //cout<<"ss---"<<ss<<"k--"<<k<<endl;
  string s=ss.substr(k,ss.length());
  string p="";
   //cout<<"s--"<<s<<endl;
  for(int i=0;i<s.length();i++)
  {
    if(s[i]=='$')
    { string f="";
      for(int j=i+1;s[j]!=' '&&j<s.length();j++,i++)
      {
         f+=s[j];
      }

      if(o[f]=="")p+='$'+f;
      else
        p+=o[f];
    }
    else
      p+=s[i];
      
  }
  return p; 
}
void eccho(string ss)
{  //cout<<"ss--"<<ss<<endl;
  int k=0;
   for(int i=0;i<ss.length();i++)
  {
  if(ss[i]!=' ')
  {
    k=i;
    break;
  }
  
 }
 //cout<<"k--"<<k<<endl;
 //cout<<"s--"<<ss<<endl;
 string s=ss.substr(k,ss.length());
 //cout<<"len---"<<s<<"  "<<s.length()<<endl;
 //cout<<"echo ke andr--"<<s<<k<<endl;
  if(s[0]=='"'&&s[s.length()-1]=='"')
   {// cout<<"double quotes";
     print_val_double_quotes(s); 
   } 
   else if(s[0]=='\''&&s[s.length()-1]=='\'')
   {
    print_val_single_quotes(s);
   }
  
   else if((s[0]!='\"'&&s[s.length()-1]!='\"')||(s[0]!='\''&&s[s.length()-1]!='\''))
   {
    string p=print_without_quote(s);//cout<<"yaha";
    if(p.length()==s.length())
      cout<<p<<endl;
    else
      cout<<"syntax error\n";
   } 
   
}

	int main()
	{ setmyrc_file();
    cout<<"$";
		string mm,oo;
	char *v[100],*vv[100];
	char line_cmd[2046],ll[2046];
  const char *line;	
	while(1)
	{
	  print_prompt();
		//break;
		//cout<<"write a line?---";
		getline(cin,mm);
   /* if(size_t found = mm.find("sudo"))
   { 
     o.insert("PS1","#");
   }
    else
    {
    
      o.insert("PS1"="$");
     }
     cout<<o["PS1"];*/
    
    for(int i=0;i<mm.length();i++)
    {
        if(mm[i]!=' ')
        {
          mm=mm.substr(i,mm.length());
          break;
        }
    }

		history_make(mm);
   
		strcpy(line_cmd,mm.c_str());
	    parse(line_cmd,vv);

		char a[100];
		   if(strcmp(vv[0],"alias")==0)
	     {  find_map(mm);
	    	  continue;
	     }  	
	       	
	  	
		strcpy(a,mm.c_str());

		if(strcmp(a,"history")==0)
		{
			show_history();
			continue;
		}
      string h,b,w;
      b=line_cmd;
      char *token=strtok(line_cmd," ");
      string d=token;

      //cout<<"m.size---"<<m.size();
      int f=0;
        for(auto it=m.begin();it!=m.end();it++)
      {  
             h= it->first;

             if(h==d)
             {f=1;
              w=it->second;
               line=w.c_str();
               break;
             }

       }
      cout<<"jhd";
      cout<<"line--"<<line<<endl;
      string y="";
       if(f==1)
      { //cout<<"w--"<<w<<endl;
        y=y+w+mm.substr(d.length(),mm.length());
        //cout<<"y--"<<y<<endl;

        strcpy(ll,y.c_str());
        //cout<<"ll--"<<ll<<endl;
      }
      else
        strcpy(ll,mm.c_str());
       cout<<"ll--"<<ll<<endl;
      // strcpy(ll,line);
	//     parse(ll,v);
 //      cout<<"ll---"<<vv[0]<<endl;
 //      //for(int i=0;i<r;i++)
 //        //cout<<v[i]<<endl;
 //        execute(v);   
	// /* if(strcmp(vv[0],"exit")==0)
	//     	exit(1);*/
 //       //cout<<"ll--"<<ll<<endl;



      ///------- yeh tabhi chalega jab tera v off hoga**********////////

    //  pipes(ll);
      cout<<"hii--";
      /*char j[]="cd";
      char jj[]="echo";
      char aa[100];
      if((strcmp(j,vv[0]))==0)
      {change_directory(mm);}
      else if((strcmp(jj,vv[0]))==0)
      {   if(strcmp("$$",vv[1])==0)
         {cout<<getuid()<<endl;
          continue;}
          else if(strcmp("$?",vv[1])==0)
            {cout<<statuss<<endl;continue;}
          string tt=mm;
   
          string gg=mm.substr(5,mm.length());
          //cout<<"gg--"<<gg<<endl;   
          eccho(gg);
      }
          /*else  if(strcmp("|",v[1])==0)
            check_for_pipes(line_cmd,v);
*/
          execute(vv);
      //redirection(v);
	    //check_for_pipes(line_cmd,v);
          // change_directory(v);
        //  
        // redirect(int i=0		//cout<<line;
	}
}