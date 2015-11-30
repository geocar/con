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

ZI D,W,L,p,f,c,n[5],j,hp,Q='q';ZC b[4096];ZK x,y,z,h;static struct pollfd P[2]={0};static struct termios G0,G1;ZS q[]={
  "q){[x];a:.q.show;b:neg .z.w;.q.show::{[x;y] x .Q.s y;}[b];@[{.q.show value x;};x;{x\"'\",y,\"\\n\"}[b]];.q.show::a;}",
  "k){a:.q.show;b:-.z.w;.q.show::{x .Q.s y;}[b];@[{x (-3!.:y),\"\\n\"}[b];x;{x\"'\",y,\"\\n\"}[b]];.q.show::a;}"
};
ZI cc(I x){R (x>='0'&&x<='9')?'0':(x>='a'&&x<='z')?'a':(x>='A'&&x<='Z')?'a':x;} ZS sd(S p,I n){if(n>=10)p=sd(p,n/10);*p='0'+(n%10);R p+1;}
ZI ox(I f,S x,I n){I r;while(n>0)if((r=write(f,x,n))<=0)break;else x+=r,n-=r;R 1;}ZI o1(S x,I n){ox(1,x,n);}ZI o1c(I c){C b[4]={c,0,0,0};R o1(b,1);}
ZI oxs(I f,S x){R ox(f,x,strlen(x));}ZI o1s(S x){R oxs(1,x);}
ZV mr(I y,I x){C*p,b[99]={27,'['};p=sd(b+2,x+1);*p++='G';if(y>0){*p++=27;*p++='[';p=sd(p,y);*p++='B';}*p=0;o1s(b);}ZV mh(V){C b[4]={27,8,0,0};b[3]=D*'\n';o1s(b);}
ZV rp(V){if('k'==Q)o1s("  ");else o1(xC,2);} ZV nk(V){x=ktn(KC,p=2);kC(x)[1]=')';kC(x)[0]=Q;}
ZI oh(S x,I m){I f,d;S h=getenv("HOME");d=open(".",O_RDONLY);if(h)chdir(h);f=open(x,m,0666);fchdir(d);close(d);R f;}
ZV sw(V){k(-f,"k){.:\"\\\\c \",\" \"/:$x,y}",ki(L),ki(W),0);}ZI i0(V){C b[4];if(read(0,b,1)==1)R *b;R -1;}
ZV gw(I _){struct winsize ws={0};ioctl(0,TIOCGWINSZ,&ws);if(!ws.ws_col)ioctl(1,TIOCGWINSZ,&ws);L=ws.ws_row;W=ws.ws_col;if(L>0&&W>0)sw();signal(SIGWINCH,gw);}
ZK gk(K*y){/*little-endian*/K x=*y,r;I m;if(xn<8)R 0;if(xn<(m=xI[1]))R 0;if(xn==m){*y=r1(ktn(KC,0));R x;}r=ktn(xt,m);memcpy(r->G0,xC,m);memmove(xC,xC+m,xn-m);xn-=m;R r;}
ZI o1k(K x){o1(xC,xn);R xC[xn-1]=='\n';}
#define hn h->n
//position fd(kdb) char displaymode
int main(int argc,char *argv[]){
  if(argc == 1){oxs(2,"Usage: ");oxs(2,*argv);oxs(2," [host] port\n");R 1;}
  if(-1==(f=khpu(argc==2?"127.0.0.1":argv[1],atoi(argv[argc-1]),getenv("USER")))){oxs(2,"kdb: ");oxs(2,strerror(errno));oxs(2,"\n");R 2;}
  h=ktn(0,0);if(-1!=(c=oh(".conhistory",O_RDONLY))){struct stat sb;if(-1!=fstat(c,&sb)){x=ktn(KC,sb.st_size);for(p=0;p<xn;p+=j)if(0>=(j=read(c,xC+p,xn-p)))break;h=d9(x);}close(c);}
  tcgetattr(0,&G0);G1=G0;G1.c_iflag&=~(BRKINT|ICRNL|INPCK|ISTRIP|IXON);G1.c_cflag|=CS8;G1.c_lflag&=~(ECHO|ICANON|IEXTEN|ISIG);G1.c_cc[VMIN]=1;G1.c_cc[VTIME]=0;
  tcsetattr(0,TCSAFLUSH,&G1); c=1;setsockopt(f,SOL_SOCKET,SO_KEEPALIVE,&c,sizeof(c));gw(69);

  D=1;P->fd=0;P[1].fd=f;DO(2,P[i].events=POLLIN|POLLHUP);r1(h);hp=hn;nk();o1s("\r\033[0J\0337");rp();z=r1(ktn(KC,0));while(-1!=poll(P,2,-1))
    if(P[0].revents&POLLIN)switch(c=i0()){
      case 3:case -1:shutdown(f,SHUT_WR);break;
      case 12:redraw:mh();o1s("\033[0J");rp();o1(xC+2,xn-2);if(p==xn)break; gotoxy:mh();mr(p/W,p%W);break;
      case 8:case 127:if(p==2)break;if(((p-1)/W)==(p/W))o1s("\033[D");else{o1s("\033[A");mr(0,W-1);} --p;goto del;//delete
      case 11:xn=p;o1s("\033[0J");break;case 4:del:if(p==xn)break;memmove(xC+p,xC+p+1,xn-(p+1));xn--;o1(xC+p,xn-p);o1c(' ');goto gotoxy;
      case 1:if(p<W){mr(0,p=2);break;} p=2;goto gotoxy;
      case 2:back:if(p==2)break;if(((p-1)/W)==(p/W)){o1c('\b');--p;break;} --p;goto gotoxy; case 6:forward:if(p==xn)break;o1c(xC[p]);++p;break;
      case 16:up:if(!hp)break;if(hp==hn)jk(&h,r1(x));else kK(h)[hp]=x;--hp;x=kK(h)[hp];p=xn;goto redraw; case 14:down:if((hp+1)>=hn)break;kK(h)[hp]=x;++hp;x=kK(h)[hp];p=xn;goto redraw;
      case 27:switch((c=i0())){
        case 'b':if(p==2)break;--p;while(p>2&&cc(xC[p])==cc(xC[p-1]))--p;goto gotoxy; case 'f':if(p==xn)break;++p;while(p<xn&&cc(xC[p])==cc(xC[p+1]))++p;goto gotoxy;
        case '[':switch(c=i0()){case 'D':goto back;case 'C':goto forward;case '3':if(i0()=='~')goto del;break; case 'A':goto up;case 'B':goto down;}};break;
      case '\r':case '\n':c=(xn==3&&xC[2]=='\\');if(!c&&xn>2)k(-f,q[Q=='k'],r1(x),0,0);if(hp==hn)jk(&h,r1(x));else kK(h)[hp]=x; hp=hn; if(c)Q=*q[Q!='k']; nk();o1s("\n\0337");D=1;rp();break;
      default:if(p==xn){ja(&x,b);xC[p]=c;o1c(c);++p;break;}ja(&x,b);memmove(xC+p+1,xC+p,xn-(p+1));xC[p]=c;o1(xC+p,1+(xn-p));++p;goto gotoxy;
    } else if(P[1].revents&POLLIN){if(0>=(c=read(f,b,4096)))break; jv(&z,kpn(b,c));while((y=gk(&z)))switch((y=d9(y))->t){
      case KC: if(y->n){o1s("\0338\033[0J");D=o1k(y);o1s("\0337");goto redraw;}}}
  o1c('\n');tcsetattr(0,TCSAFLUSH,&G0);x=b9(0,h);ox(oh(".conhistory",O_RDWR|O_CREAT),xC,xn);R 0;}
