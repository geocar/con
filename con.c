#include "k.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/poll.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <termios.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <ctype.h>
#include <sys/ioctl.h>

ZI D,W,L,Y,X,p,f,c,n[5],j,hp,Q='q';ZC b[4096];ZK x,y,h;static struct pollfd P[2]={0};static struct termios G0,G1;ZS q[]={
  "q){[x];a:.q.show;b:neg .z.w;.q.show::{x .Q.s y;}[b];@[{.q.show value x;};x;{x\"'\",y,\"\\n\"}[b]];.q.show::a;}",
  "k){a:.q.show;b:-.z.w;.q.show::{x .Q.s y;}[b];@[{x (-3!.:y),\"\\n\"}[b];x;{x\"'\",y,\"\\n\"}[b]];.q.show::a;}"
};
ZI cc(I x){R (x>='0'&&x<='9')?'0':(x>='a'&&x<='z')?'a':(x>='A'&&x<='Z')?'a':x;} ZS sd(S p,I n){if(n>=10)p=sd(p,n/10);*p='0'+(n%10);R p+1;}
ZI ox(I f,S x,I n){I r;while(n>0)if((r=write(f,x,n))<=0)break;else x+=r,n-=r;R 1;}ZI o1(S x,I n){ox(1,x,n);}ZI o1c(I c){C b[4]={c,0,0,0};R o1(b,1);}
ZI oxs(I f,S x){R ox(f,x,strlen(x));}ZI o1s(S x){R oxs(1,x);}ZI mc(I y,I x){C*p,b[99]={27,'['};p=sd(b+2,y);*p=';';p=sd(p+1,x);*p='H';p[1]=0;o1s(b);}
ZV rp(V){o1s("\x1b[0J");if('k'==Q)o1s("  ");else o1(xC,2);} ZK nk(V){K x=ktn(KC,2);kC(x)[1]=')';kC(x)[0]=Q;R x;}
ZI oh(S x,I m){I f,d;S h=getenv("HOME");d=open(".",O_RDONLY);if(h)chdir(h);f=open(x,m,0666);fchdir(d);close(d);R f;}ZI af(I f,I b){I o=P[f].events;P[f].events=POLLIN|POLLHUP;R o&POLLIN;}
ZV sw(V){k(-f,"k){.:\"\\\\c \",\" \"/:$x,y}",ki(L),ki(W),0);}ZI i0(V){C b[4];if(read(0,b,1)==1)R *b;R -1;}
ZV gw(I _){struct winsize ws={0};ioctl(0,TIOCGWINSZ,&ws);if(!ws.ws_col)ioctl(1,TIOCGWINSZ,&ws);L=ws.ws_row;W=ws.ws_col;if(L>0&&W>0)sw();signal(SIGWINCH,gw);}
#define hn h->n
//position fd(kdb) char displaymode
int main(int argc,char *argv[]){
  if(argc == 1){oxs(2,"Usage: ");oxs(2,*argv);oxs(2," [host] port\n");R 1;}
  if(-1==(f=khpu(argc==2?"127.0.0.1":argv[1],atoi(argv[argc-1]),getenv("USER")))){oxs(2,"kdb: ");oxs(2,strerror(errno));oxs(2,"\n");R 2;}
  h=ktn(0,0);if(-1!=(c=oh(".conhistory",O_RDONLY))){struct stat sb;if(-1!=fstat(c,&sb)){x=ktn(KC,sb.st_size);for(p=0;p<xn;p+=j)if(0>=(j=read(c,xC+p,xn-p)))break;h=d9(x);}close(c);}
  tcgetattr(0,&G0);G1=G0;G1.c_iflag&=~(BRKINT|ICRNL|INPCK|ISTRIP|IXON);G1.c_cflag|=CS8;G1.c_lflag&=~(ECHO|ICANON|IEXTEN|ISIG);G1.c_cc[VMIN]=1;G1.c_cc[VTIME]=0;
  tcsetattr(0,TCSAFLUSH,&G1); c=1;setsockopt(f,SOL_SOCKET,SO_KEEPALIVE,&c,sizeof(c));gw(69);

  D=0;P->fd=0;P[1].fd=f;af(0,1);af(1,0);r1(h);hp=hn;Y=X=0;x=nk();p=2;o1s("\r\x1b[6n");while(-1!=poll(P,2,-1))
    if(P[0].revents&POLLIN)switch(c=i0()){
      case 3:case -1:shutdown(f,SHUT_WR);break;
      case 12:redraw:if(D)mc(Y+1,1);else mc(Y,X);rp();o1(xC+2,xn-2);af(1,1);gotoxy:if(p!=xn){I cx=X,cy=Y;if(D)cx=1,cy++;if(W>0)mc(cy+(p/W),cx+(p%W));else{mc(cy,cx);rp();o1(xC+2,p-2);}}break;
      case 8:case 127:if(p>2){if(p==xn)o1s("\b \b"),p--,--xn;else{o1s("\x1b[D");o1(xC+p,xn-p);o1c(' ');--p;goto del;}}break;//delete
      case 11:xn=p;o1s("\x1b[0J");break;case 4:del:if(p==xn)break;memmove(xC+p,xC+p+1,xn-(p+1));xn--;o1(xC+p,xn-p);if(xn==p)o1s(" \b");else{o1c(' ');goto gotoxy;}
      case 1:p=2;if(D)mc(Y+1,3);else mc(Y,X+2);break;case 2:back:if(p>2){--p;o1s("\x1b[D");}break;case 6:forward:if(p<xn){++p;o1s("\x1b[C");}break;
      case 16:up:if(!hp)break;if(hp==hn)jk(&h,r1(x));else kK(h)[hp]=x;--hp;x=kK(h)[hp];goto redraw; case 14:down:if((hp+1)>=hn)break;kK(h)[hp]=x;++hp;x=kK(h)[hp];goto redraw;
      case 27:switch((c=i0())){
        case 'b':if(p==2)break;--p;while(p>2&&cc(xC[p])==cc(xC[p-1]))--p;goto gotoxy; case 'f':if(p==xn)break;++p;while(p<xn&&cc(xC[p])==cc(xC[p+1]))++p;goto gotoxy;
        case '[':memset(n,0,sizeof(n));j=0;do{while((c=i0())>='0'&&c<='9'&&j<5)n[j]=(c-'0')+(n[j]*10);++j;}while(c==';');switch(c){
          case 'R':Y=n[0],X=n[1];goto redraw;case 'D':goto back;case 'C':goto forward;case '~':if(n[0]==3)goto del; case 'A':goto up;case 'B':goto down;
        }};break;
      case '\r':case '\n':c=(xn==3&&xC[2]=='\\');if(!c&&xn>2)k(-f,q[Q=='k'],r1(x),0,0);if(hp==hn)jk(&h,r1(x));else kK(h)[hp]=x; hp=hn; if(c)Q=*q[Q!='k']; x=nk();p=2;af(1,0);break;
      default:if(p==xn){ja(&x,b);xC[p]=c;o1c(c);++p;break;}ja(&x,b);memmove(xC+p+1,xC+p,xn-(p+1));xC[p]=c;o1(xC+p,1+(xn-p));++p;goto gotoxy;
    } else if(P[1].revents&POLLIN){if(0>=(c=read(f,b,4096)))break;mc(Y,X);o1(b,c);if(b[c-1]=='\n')o1s("\r\x1b[6n"),D=0;else o1s("\x1b[6n\r\n"),D=1;af(1,0);}
  o1s("\r\n");tcsetattr(0,TCSAFLUSH,&G0);x=b9(0,h);ox(oh(".conhistory",O_RDWR|O_CREAT),xC,xn);R 0;}
