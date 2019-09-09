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
#include<termios.h>
#define DELIM "\r\t\n "
#define del ":\n"
using namespace std;
long statuss;
long history=0;
long his=0;
vector<string>v;
map<string,string>m;
map<string,string>o;
void setmyrc_file();
void set_path_var()
{  
  int f=1;
  string PATHH="";
   FILE* fpp;
   char *token[100];
   fpp=fopen("/etc/manpath.config","r");
   char buf[1000];
   while(fgets(buf,sizeof(buf),fpp)!=NULL)
   {
      fputs(buf,fpp);
    
    token[0] =strtok(buf,DELIM);
     //printf("%s",token[0]);
    int i=1;
    while((token[i]=strtok(NULL,DELIM))!=NULL)i++;
    token[i] =NULL;
    if(strcmp(token[0],"MANPATH_MAP")==0)
    {
      f=0;
      //printf("%s",token[1]);
      PATHH=PATHH+string(token[1]);
      PATHH+=":";
    }

  }

  //cout<<"in set path"<<PATHH<<endl;
      FILE* fp=fopen("bash.txt","a+");
      if(f==0){o["PATH"]=PATHH;
      PATH=(char*)PATHH.c_str();
      PATH=(char*)PATHH.c_str();
      fprintf(fp,"PATH = %s\n",(char*)PATHH.c_str());
    }
      fclose(fp);
      fclose(fpp);
   }  



void setmyrc_file()
{   set_path_var();
   FILE *fp;
   USER=getpwuid(getuid())->pw_name;
   o["USER"]=USER;
   DIR =(getpwuid(getuid()))->pw_dir;
   o["DIR"]=DIR;
   int i=gethostname(HOSTNAME,sizeof(HOSTNAME));
   o["HOSTNAME"]=HOSTNAME;
  if(o["USER"]=="root"){o["PS1"]="#";PS1="#";}
  else {o["PS1"]="$";PS1="$";}
   fp =fopen("myrc.txt","w");
     if(fp!=NULL)
    {
       if(USER!=NULL)
       { fprintf(fp,"USER: %s\n",USER);
        fprintf(fp,"HOSTNAME: %s\n",HOSTNAME);
        fprintf(fp,"DIR: %s\n",DIR);
        string a;
    string b;
    string c;
    string d,e;
    a="/usr/bin/vlc";
    b="/usr/bin/vlc";
    c="/usr/bin/evince";
    d="/usr/bin/subl";
    e="/usr/bin/subl";
    char *aa,*bb,*cc,*dd,*ee;
    aa=(char*)a.c_str();
    bb=(char*)b.c_str();
    cc=(char*)c.c_str();
    dd=(char*)d.c_str();
    ee=(char*)e.c_str();
      fprintf(fp, "mp4:%s\n", aa);
        fprintf(fp, "mp3:%s\n", bb);
        fprintf(fp, "pdf:%s\n", cc);
           fprintf(fp, "txt:%s\n",dd);
            fprintf(fp, "cpp:%s\n",ee);
            fclose(fp);
       }
   }
  fclose(fp);

}




string print_prompt()
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
   string PROMPT=/*o["MODE"]+*/o["USER"]+o["HOSTNAME"]+s+o["PS1"];/*+o["PS1"]*/;
   return PROMPT;
   //cout<<PROMPT;
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




void do_single_redirection(char *vv,char**v)
{ int status;
  cout<<"stupid";
  pid_t pid=fork();
  if(pid<0){cout<<"error\n";exit(1);}
  else if(pid==0)
  {
   int fd =open(vv,O_WRONLY|O_CREAT|O_TRUNC,S_IWUSR|S_IWUSR);
    if(fd<0){cout<<"file isn't opening ";exit(1);}
    dup2(fd,1);
    if(execvp(*v,v)<0)
    {
      cout<<"failed\n";exit(127);
    }
  }
  else
  {  waitpid(pid,&status,0);
    //while(wait(&status)!=pid);
     bool x=WIFEXITED(status);
     if(x!=0)
     {
      statuss =WEXITSTATUS(status);
     }
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
        cout<<"failed\n";exit(127);
      }
    }

   }
   else
   {waitpid(pid,&status,0);
   // while(wait(&status)!=pid);
    bool x=WIFEXITED(status);
    if(x)statuss=WEXITSTATUS(status);
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
  //cout<<"n--"<<n<<endl;
  for(int i=0;i<n;i++)
  {
    if(strcmp(v[i],">")==0){ v[i]=NULL;do_single_redirection(v[i+1],v);}
    else if(strcmp(v[i],">>")==0){v[i]=NULL;do_double_redirection(v[i+1],v);}
  }
 }
 

string redirection2(char **v)
{
  int n=find_number_of_words_in_command(v);
  //cout<<"n--"<<n<<endl;
  for(int i=0;i<n;i++)
  {
    if(strcmp(v[i],">")==0){ v[i]=NULL;string t=v[i+1];return t;/*do_single_redirection(v[i+1],v);*/}
    else if(strcmp(v[i],">>")==0){v[i]=NULL; string t=v[i+1];return t;/*do_double_redirection(v[i+1],v);*/}
  }
 }

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
  		exit(127);
  	}
  }   
  else
  { waitpid(pid,&status_Process,0);
  	//while(wait(&status_Process)!=pid);
    bool x=WIFEXITED(status_Process);
     if(x)statuss=WEXITSTATUS(status_Process);
  }

}

void history_make(string s)
{
  v.push_back(s);
   history++; his++;

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
bool check(int c,int n)
{
  if(c<n)
    return 1;
  else return 0;
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

    FILE *fp;
    fp=fopen("alias.txt","w");

  //cout<<"ee--"<<ee<<" ";
  if(oo=='"'&&ee=='"'){m[p]=q;fprintf(fp,"%s %s\n",p.c_str(),q.c_str());}
   else if(oo=='\''&&ee=='\''){m[p]=q;m[p]=q;fprintf(fp,"%s %s\n",p.c_str(),q.c_str());}
  
  


    return ;
}


void show_history()
{
	for(int i=history-1;i>=0;i--)
    cout<<v[i]<<endl;
}

int check_re_in_pipe(char a[])
{
   string t=a;
   //cout<<"\nt--"<<t<<"size--"<<t.length();
   for(int i=0;i<t.length();i++)
   {  //cout<<"t[i]--"<<t[i]<<endl;
    if(t[i]=='>'&&t[i+1]=='>'){/*cout<<"double--"<<endl;*/return 2;}
    if(t[i]=='>'&&t[i+1]!='>'){/*cout<<"single--";*/return 1;}

   }
   return 0;
}


void execute_pipe(char **ar , int n)
{  int i=0;
  int fd[2];
  pid_t pid;
     int in=0;
  //cout<<n<<endl;
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
             char *a;
             char tt[100];
             string y1=ar[i];
             a=(char*)y1.c_str();
             strcpy(tt,a);
             cout<<"a--upar--"<<a<<endl<<"ar[i]--"<<ar[i]<<endl;
            // cout<<"i---"<<i<<" "<<ar[i]<<endl;
             parse(a,exe_pipe);
             //cout<<"parse ke niche"<<tt<<endl;
            if(i<n-1){
             if(execvp(*exe_pipe,exe_pipe) < 0)
               exit(0);}

            else if(i==n-1)
             {  int fd11;
               int bb=check_re_in_pipe(tt);
              cout<<"bb---"<<bb<<endl;

               if(bb==1)
              {    string re=redirection2(exe_pipe);
                  int  uu=find_number_of_words_in_command(exe_pipe);
                  char *rrr;
                   rrr=(char*)re.c_str();
                   char eee[100];
                   strcpy(eee,rrr);
                  //for(int k=0;k<uu;k++)cout<<"k---"<<exe_pipe[k]<<endl;
                  //cout<<"----"<<exe_pipe[uu]<<endl;
                   fd11=open(eee,O_WRONLY|O_CREAT,S_IRUSR|S_IWUSR);


              }else if(bb==2)
              {   string re=redirection2(exe_pipe);
                 int  uu=find_number_of_words_in_command(exe_pipe);
                  char *rrr;
                   rrr=(char*)re.c_str();
                   char eee[100];
                   strcpy(eee,rrr);
                 fd11=open(eee,O_WRONLY|O_CREAT|O_APPEND,S_IRUSR,S_IWUSR);


              }
              if(fd11==-1)
              {
                error_msg_exec();
                exit(1);
              }
              dup2(fd11,1);
              
              if((execvp(*exe_pipe,exe_pipe))<0)
                exit(127);

            }
              
            
           
      }
      else
      { int status;
        waitpid(pid,&status,0);
        if(WIFEXITED(status))
        { statuss = WEXITSTATUS(status);
        }
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
   {
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

bool check_for_redir(char line[])
{
      string h=line;
      for(int i=0;i<h.length();i++)
      {
        if(h[i]=='>')return true;
      }
       return false; 
   
}


bool check_for_pipe(char line[])
{
      string h=line;
      for(int i=0;i<h.length();i++)
      {
        if(h[i]=='|')return true;
      }
       return false; 
   
}
string do_change(string s)
{int j=-1;
  for(int i=0;i<s.length();i++)
  {
    if(s[i]=='~')
      {j=i;break;}
  }
  string t;
  if(j!=-1&&j<s.length()-1)
 t=s.substr((j+1),s.length());
 else if(j!=-1&&j==s.length()-1)t="";
  else t="-1";
  return t;

}
string make_path(string aa)
{int j=-1;
  for(int i=0;i<aa.length();i++)
  {
     if(aa[i]=='~')
      {j=i;break;}
  }
  string t;
  if(j!=aa.length()-1)
   t=aa.substr(j+1,aa.length());
  else
   t="";
  t=o["DIR"]+t;
 return t;
}


 string input_take()
 { char a;
    string s="";
  while((a=getchar())!='\n')
  {
     if(a==127)
     {
        printf("\r                                                                              ");
        int h=s.length();
        if(h>0){
        string kj=s.substr(0,h-1);
        s=kj;
      string yy=print_prompt();
      yy+=s;
      printf("\r%s",yy.c_str());
      //cout<<s;
     // cout<<"\r"<<yy;
        continue;}    
     } 
     else
     {
       s+=a;
     }

  }
  return s;

 }
void ex_find(string a,string b)
{
   pid_t pid;
   int status;
   pid=fork();
   if(pid<0)
   {
    error_msg_CProcess();
   }
   else if(pid==0)
   {
    char *aa=(char*)a.c_str();
    char *bb=(char*)b.c_str();
   if(execl(aa,"xdg-open",bb,(char*)0)==-1)
    {  error_msg_CProcess();
        exit(127);
    }
   }
   else
   {
        int status;
        waitpid(pid,&status,0);
        if(WIFEXITED(status))
        { statuss = WEXITSTATUS(status);
        }
   }
}
 
 void handling_media(string a)
 {
      
      int k=0;
 
     for(int i=0;i<a.length();i++)
     {
      if(a[i]!=' '){k=i;break;}
     }
     string sss=a.substr(k,a.length());
     k=0;
     string ninz=sss.substr(5,sss.length());
     for(int i=0;i<ninz.length();i++)
     {
      if(ninz[i]!=' '){k=i;break;}
     }
     a=ninz.substr(k,ninz.length());
     //cout<<"a--"<<a<<endl;
     char *man;
     char san[100],kan[100];
     man=(char*)ninz.c_str();
     strcpy(kan,man);
     char *ar[100];
     //cout<<"man---"<<man<<endl;
     ar[0]=strtok(kan,".");
     //cout<<"ar0--"<<ar[0]<<endl;
     //cout<<"ar0--"<<ar[0]<<endl;
     ar[1]=strtok(NULL,".");
     //cout<<"ar1--"<<ar[1];
      FILE* fp;
      fp=fopen("myrc.txt","r");
       if(fp!=NULL)
       {
        string hi="/home/hp/Desktop/OS_ASSIGNMENT_!/"+a;
       // cout<<"hi---"<<hi<<endl;
        while(fgets(san,sizeof(san),fp)!=NULL)
        {
          char *jan[100];
          jan[0]=strtok(san,":");
         // cout<<"jan0--"<<jan[0]<<endl;
          //cout<<"ar1--"<<ar[1]<<endl;
          if(strcmp(jan[0],ar[1])==0){
          jan[1]=strtok(NULL,":\n");
          //cout<<"jan--"<<jan[1]<<endl;
          string s;
          s=jan[1];
          //cout<<"s--"<<s<<endl<<"hi--"<<hi<<endl;
          ex_find(s,hi);
          return ;

        }
        }
       }
       else
       {
        error_msg_exec();
       }
     //cout<<ar[1]<<endl;
     fclose(fp);

 }

	int main() 
	{ 
    struct termios abc;
    tcgetattr(STDIN_FILENO,&abc);
    abc.c_lflag &=~(ICANON);
    tcsetattr(STDIN_FILENO,TCSAFLUSH,&abc);
   setmyrc_file();
   string oooo=print_prompt();
   //cout<<oooo;
  // cout<<"path "<<o["PATH"]<<endl;
		string mm,cc;
	 char *v[100],*vv[100];
	 char line_cmd[2046],ll[2046];
   const char *line;	
	  while(1)
	 { int x=0;

    
		 //cout<<"hp@-HPneerja-:~/Desktop/ngj->";
		 //break;
		 //cout<<"write a line?---";
    cout<<print_prompt();
		cc=input_take();
     
    int bg=0;
    for(x=0;x<cc.length();x++)
    {
      if(cc[x]!=' '){bg=x;break;}
    }
    
     mm=cc.substr(x,cc.length());
		history_make(mm);
		 strcpy(line_cmd,mm.c_str());
      // cout<<"line-cmd "<<line_cmd<<endl;
	     parse(line_cmd,vv);
       //cout<<"niche wala"<<line_cmd<<endl;
		   char a[100];
      
       if(strcmp(vv[0],"_open")==0)
       { 
         //string uy=mm.substr(4,mm.length());
         //cout<<uy<<endl;

         handling_media(mm);
         continue;
       }
		   else if(strcmp(vv[0],"alias")==0)
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

      //strcpy(ll,line);
	    parse(ll,v);
      
      //for(int i=0;i<r;i++)
        //cout<<v[i]<<endl;
       char j[]="cd";
      char jj[]="echo";
      char jjj[]="~";
      char aa[100];
    
      if((strcmp(j,vv[0]))==0)
      { 
         ////handling ******  ~ value 
            
            string ee = do_change(mm);
            if(ee=="-1")  
            {change_directory(mm);continue;}
          else
          {
             string ff=make_path(mm);
             chdir((char*)ff.c_str());
          }
      }
         else if((strcmp(jj,vv[0]))==0)
      {   if(strcmp("$$",vv[1])==0)
         {cout<<getpid()<<endl;
          continue;}

          else if(strcmp("$?",vv[1])==0)
            {cout<<statuss<<endl;continue;}
             string tt=mm;
             
          string gg=mm.substr(5,mm.length());
         // cout<<"gg--"<<gg<<endl;   
          eccho(gg);
          continue;
      }    

	  else if(strcmp(v[0],"exit")==0)
	    	exit(0);
	    //check_for_pipes(line_cmd,v);
          // change_directory(v);
        else if(check_for_pipe((char*)mm.c_str()))
             {
              pipes((char*)mm.c_str());
             }
      
       else if(check_for_redir((char*)mm.c_str())){
      
        int qq=find_number_of_words_in_command(v);
        
        redirection(v);
          
       }
     
             else if(mm=="")continue;
             else
                   execute(v);

        // redirect(int i=0v);
		//cout<<line;
	}
}