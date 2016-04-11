#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/serial.h>

#define BUFSIZE 4096  

typedef unsigned char uint8;
typedef unsigned short uint16;

//char string_lcd[500];
char result_string[500]; //Строка в 20 символов
int result_string_len_old=0;
char result_stringn[500]; //Строка в 20 символов
char result_group[20];
char stri[500];
int oldstat=-1;
char refreshbitrate=1;
char action[500]; //Строка в 20 символов
int result_action_len_old=0;

int playerico=0;

int say40=0;
int say98=0;

int say60=0;
int say70=0;
int say80=0;
int say90=0;
int say100=0;
int say110=0;
int say120=0;
int say130=0;
int say140=0;

int toj=0;
int toj_old=0;
int ts=0;
int ts_old=0;
int speed=0;
int speed_old=0;
int oboroti_old=0;

int reset_key=0;
int reset_key_timer=0;

char mode=0;

int num=0;
   int fd;

FILE *fp;

time_t now;
struct tm local_date_time;
 char c_time_string[50];
char rev=0;

 long probeg;
 long sutochn;
 long savedprobeg;
 long timergas=0;

unsigned char power=1;


void tty_set_param(char* param,int value){
       usleep(20000);
       write(fd,param,1);
       sprintf(stri,"%d",value);
       write(fd,stri,strlen(stri));
       write(fd,"\n",1);
       usleep(10000);
}

void tty_set_paramf(char* param,float value){
       usleep(20000);
       write(fd,param,1);
       sprintf(stri,"%6.2f",value);
       write(fd,stri,strlen(stri));
       write(fd,"\n",1);
       usleep(10000);
}


void tty_set_str(char* param,char* value){
       usleep(20000);
       write(fd,param,1);
       write(fd,value,strlen(value));
       write(fd,"\n",1);
       usleep(10000);
}

static int rate_to_constant(int baudrate) {
#define B(x) case x: return B##x
    switch(baudrate) {
	B(50);     B(75);     B(110);    B(134);    B(150);
	B(200);    B(300);    B(600);    B(1200);   B(1800);
	B(2400);   B(4800);   B(9600);   B(19200);  B(38400);
	B(57600);  B(115200); B(230400); B(460800); B(500000); 
	B(576000); B(921600); B(1000000);B(1152000);B(1500000); 
    default: return 0;
    }
#undef B
}    


int serial_open(const char *device, int rate)
{
    struct termios options;
    struct serial_struct serinfo;
    int fd;
    int speeds = 0;

    /* Open and configure serial port */
    if ((fd = open(device,O_RDWR|O_NOCTTY)) == -1)
	return -1;

    speeds = rate_to_constant(rate);

    if (speeds == 0) {
	/* Custom divisor */
	serinfo.reserved_char[0] = 0;
	if (ioctl(fd, TIOCGSERIAL, &serinfo) < 0)
	    return -1;
	serinfo.flags &= ~ASYNC_SPD_MASK;
	serinfo.flags |= ASYNC_SPD_CUST;
	serinfo.custom_divisor = (serinfo.baud_base + (rate / 2)) / rate;
	if (serinfo.custom_divisor < 1) 
	    serinfo.custom_divisor = 1;
	if (ioctl(fd, TIOCSSERIAL, &serinfo) < 0)
	    return -1;
	if (ioctl(fd, TIOCGSERIAL, &serinfo) < 0)
	    return -1;
	if (serinfo.custom_divisor * rate != serinfo.baud_base) {
	    warnx("actual baudrate is %d / %d = %d",
	          serinfo.baud_base, serinfo.custom_divisor,
	          serinfo.baud_base / serinfo.custom_divisor);
	}
    }

    fcntl(fd, F_SETFL, 0);
    tcgetattr(fd, &options);
    cfsetispeed(&options, speeds ?: B38400);
    cfsetospeed(&options, speeds ?: B38400);
    cfmakeraw(&options);
    options.c_cflag |= (CLOCAL | CREAD);
    options.c_cflag &= ~CRTSCTS;
    if (tcsetattr(fd, TCSANOW, &options) != 0)
	return -1;

    return fd;
}



int readnumstation(){
   int num;
    int pr;
    FILE *tmp;
    tmp=fopen("/automedia/oldstation.db","r");
//    tmp=fopen("/sys/devices/platform/sunxi-i2c.1/i2c-1/1-0068/nvram","r");

    if(tmp == NULL)
    {
	return;
    }


    fscanf(tmp,"%i\n",  &pr);
    fclose(tmp);
    return pr;
}


int readintparam(char* fname){
   int num;
    int pr;
    FILE *tmp;
//    tmp=fopen("/automedia/oldstation.db","r");
    tmp=fopen(fname,"r");

    if(tmp == NULL)
    {
	return;
    }


    fscanf(tmp,"%i\n",  &pr);
    fclose(tmp);
    return pr;
}


long readlongparam(char* fname){
   long num;
    long pr;
    FILE *tmp;
    tmp=fopen(fname,"r");

    if(tmp == NULL)
    {
	return;
    }


    fscanf(tmp,"%lu\n",  &pr);
    fclose(tmp);
    return pr;
}


char* readstringparam(char* fname){
   char* num;
    char* pr;
    FILE *tmp;
    tmp=fopen(fname,"r");

    if(tmp == NULL)
    {
	return;
    }


    fscanf(tmp,"%s\n",  &pr);
    fclose(tmp);
    return pr;
}



void writelongparam(char* fname,long parm){
    FILE *tmp;
    tmp=fopen(fname,"w");

    if(tmp == NULL)
    {
	return;
    }


    fprintf(tmp,"%lu\n",  parm);
    fclose(tmp);
}


float readfloatparam(char* fname){
   float num;
    float pr;
    FILE *tmp;
    tmp=fopen(fname,"r");

    if(tmp == NULL)
    {
 	printf("Error '%s'",tmp);
	return;
    }


    fscanf(tmp,"%f",  &pr);
    fclose(tmp);
    return pr;
}





int readbitrate(){
   int num;
    int pr;
    FILE *tmp;
    tmp=fopen("/tmp/bitrate","r");

    if(tmp == NULL)
    {
	return;
    }


    fscanf(tmp,"%i\n",  &pr);
    fclose(tmp);
    return pr;
}


int readpingtest(){
   int num;
    int pr;
    FILE *tmp;
    tmp=fopen("/tmp/pingtest","r");

    if(tmp == NULL)
    {
	return;
    }


    fscanf(tmp,"%i\n",  &pr);
    fclose(tmp);
    return pr;
}



void getstring(){
FILE *file; 
    char *fname = "/tmp/title.cp1251";
 
    file = fopen(fname,"r");
    
    int xd;
    for(xd=0;xd<500;xd++){
    result_string[xd]=0;
    }
 
    if(file == NULL)
    {
	return;
    }
 
    int i=0;
    char *real_tail;

    
    while(fgets(result_string+strlen(result_string),sizeof(result_string),file))
    {
	real_tail="";
        
/*        int k;       

        for(k=strlen(result_string);k<strlen(result_string)+17;k++){
           result_string[k]=" ";
        }
           result_string[k]=0x00;
*/
	if(result_string[strlen(result_string)-1] == '\n')//проверяем является ли последний элемент в строке символом её окончания
	{
	    real_tail="\\n";
	    result_string[strlen(result_string)-1]=' ';
	};// эта часть кода добавлена лишь для отображения символа конца строки в консоль без перевода на новую строку	
 
 }

    fclose(file);
}

void getaction(){
FILE *file; 
    char *fname = "/tmp/action.cp866";
 
    file = fopen(fname,"r");
    
    int xd;
    for(xd=0;xd<500;xd++){
    action[xd]=0;
    }
 
    if(file == NULL)
    {
	return;
    }
 
    int i=0;
    char *real_tail;

    
    while(fgets(action+strlen(action),sizeof(action),file))
    {
	real_tail="";
        
/*        int k;       

        for(k=strlen(result_string);k<strlen(result_string)+17;k++){
           result_string[k]=" ";
        }
           result_string[k]=0x00;
*/
	if(action[strlen(action)-1] == '\n')//проверяем является ли последний элемент в строке символом её окончания
	{
	    real_tail="\\n";
	    action[strlen(action)-1]=' ';
	};// эта часть кода добавлена лишь для отображения символа конца строки в консоль без перевода на новую строку	
 
 }

    fclose(file);
}



void getnamestation(){
FILE *file; 
    char *fname = "/tmp/streamname.cp1251";
 
    file = fopen(fname,"r");

    result_stringn[0]=0x00;

 
    if(file == NULL)
    {
        fclose(file);
	return;
    }
 
    int i=0;
    char *real_tail;


    
    while(fgets(result_stringn+strlen(result_stringn),sizeof(result_stringn),file))
    {
	real_tail="";
        
/*        int k;       

        for(k=strlen(result_string);k<strlen(result_string)+17;k++){
           result_string[k]=" ";
        }
           result_string[k]=0x00;
*/
	if(result_stringn[strlen(result_stringn)-1] == '\n')//проверяем является ли последний элемент в строке символом её окончания
	{
	    real_tail="\\n";
	    result_stringn[strlen(result_stringn)-1]=' ';
	};// эта часть кода добавлена лишь для отображения символа конца строки в консоль без перевода на новую строку	
 
 }
   
    fclose(file);
//           remove("/tmp/streamname.cp1251");

}


void getgroup(){
    FILE *tmp;
    char tmpstr[20];
    tmp=fopen("/automedia/radiolist.grp","r");

    if(tmp == NULL)
    {
	return;
    }
    
    int x=0;

    for(x=0;x!=num;x++){
        fscanf(tmp,"%s",  &tmpstr);
    }

    fscanf(tmp,"%s",  &result_group);

    if(result_group[0]==0x0){result_group[0]=' ';result_group[1]=' ';result_group[2]=' ';result_group[3]=' ';result_group[4]=' ';result_group[5]=' ';result_group[6]=' ';}
    if(result_group[1]==0x0){result_group[1]=' ';result_group[2]=' ';result_group[3]=' ';result_group[4]=' ';result_group[5]=' ';result_group[6]=' ';}
    if(result_group[2]==0x0){result_group[2]=' ';result_group[3]=' ';result_group[4]=' ';result_group[5]=' ';result_group[6]=' ';}
    if(result_group[3]==0x0){result_group[3]=' ';result_group[4]=' ';result_group[5]=' ';result_group[6]=' ';}
    if(result_group[4]==0x0){result_group[4]=' ';result_group[5]=' ';result_group[6]=' ';}
    if(result_group[5]==0x0){result_group[5]=' ';result_group[6]=' ';}
    if(result_group[6]==0x0){result_group[6]=' ';}
    result_group[7]=0x0;
 
    fclose(tmp);
}


void eventsjob(){

          //Чтение освещенности
	  if(power==1){
          if(readlongparam("/tmp/cubard/flight")==1){
	    //LCD_BL(0);
	  }
	  else {
	    //LCD_BL(1);
          }
	  }


          //Чтение газоанализатора
	  int gas=readlongparam("/tmp/cubard/gas");
          
	  if(timergas>0){timergas++;}
	  if(gas<50){timergas=0;};
          if(timergas==50){timergas=0;};
          if(gas>=70){
            if(timergas==0){timergas++;}
	    if(timergas==1){
	    if(gas<150){
		    //LCD_BL(0);
                       system( "/automedia/says/saywav.sh lowgas.wav  > /tmp/saylog.log &");
		    //LCD_BL(1);
	    }else{
		    //LCD_BL(0);
                       system( "/automedia/says/saywav.sh highgas.wav > /tmp/saylog.log  &");
		    //LCD_BL(1);
	  }
	  }
	  }




       //Здесь сообщения о превышениях скоростей
       int spd=readlongparam("/tmp/mikas/speed");
       
       if(spd!=0){
       if(spd<55){say60=0;say70=0;say80=0;say90=0;say100=0;say110=0;say120=0;say130=0;say140=0;}
       if(spd<65){say70=0;say80=0;say90=0;say100=0;say110=0;say120=0;say130=0;say140=0;}
       if(spd<75){say80=0;say90=0;say100=0;say110=0;say120=0;say130=0;say140=0;}
       if(spd<85){say90=0;say100=0;say110=0;say120=0;say130=0;say140=0;}
       if(spd<95){say100=0;say110=0;say120=0;say130=0;say140=0;}
       if(spd<105){say110=0;say120=0;say130=0;say140=0;}
       if(spd<115){say120=0;say130=0;say140=0;}
       if(spd<125){say130=0;say140=0;}
       if(spd<135){say140=0;}
       }

       if(say60==0){
            if(spd>60){
    	    say60=1;
            system( "/automedia/says/saywav.sh 60.wav  > /tmp/saylog.log &");
            }
	}

       if(say70==0){
            if(spd>70){
    	    say70=1;
            system( "/automedia/says/saywav.sh 70.wav  > /tmp/saylog.log &");
            }
	}

       if(say80==0){
            if(spd>80){
    	    say80=1;
            system( "/automedia/says/saywav.sh 80.wav  > /tmp/saylog.log &");
            }
	}

       if(say90==0){
            if(spd>90){
    	    say90=1;
            system( "/automedia/says/saywav.sh 90.wav  > /tmp/saylog.log &");
            }
	}

       if(say100==0){
            if(spd>100){
    	    say100=1;
            system( "/automedia/says/saywav.sh 100.wav > /tmp/saylog.log  &");
            }
	}

       if(say110==0){
            if(spd>110){
    	    say110=1;
            system( "/automedia/says/saywav.sh 110.wav > /tmp/saylog.log  &");
            }
	}

       if(say120==0){
            if(spd>120){
    	    say120=1;
            system( "/automedia/says/saywav.sh 120.wav > /tmp/saylog.log  &");
            }
	}

       if(say130==0){
            if(spd>130){
    	    say130=1;
            system( "/automedia/says/saywav.sh 130.wav > /tmp/saylog.log  &");
            }
	}

       if(say140==0){
            if(spd>140){
    	    say140=1;
            system( "/automedia/says/saywav.sh 140.wav > /tmp/saylog.log  &");
            }
	}



       //Здесь температура салона
       ts=readlongparam("/tmp/ard1/temp");
       if(ts_old!=ts){
	   ts_old=toj;
           tty_set_param("t",ts);       
       }


       //Здесь сообщения о превышениях температур
       toj=readlongparam("/tmp/mikas/temp");
       if(toj_old!=toj){
	   toj_old=toj;
           tty_set_param("g",toj);       
       }

       if(toj!=0){
       if(toj<35){say40=0;say98=0;}
       if(toj<95){say98=0;}
       }

       if(say40==0){
            if(toj>40){
    	    say40=1;
            system( "/automedia/says/saywav.sh temp.wav > /tmp/saylog.log  &");
            }
	}

       if(say98==0){
            if(toj>98){
    	    say98=1;
            system( "/automedia/says/saywav.sh peregrev.wav > /tmp/saylog.log &");
            }
	}	


}



void tty_write_string(int nstr,char* str,char r,char g,char b){
   usleep(20000);
   if(r==0){r=1;};
   if(g==0){g=1;};
   if(b==0){b=1;};
   if(nstr==1){
   write(fd,"q", 1);  
   }
   if(nstr==2){
   write(fd,"w", 1);  
   }
   if(nstr==3){
   write(fd,"e", 1);  
   }
   write(fd,&r, 1);  
   write(fd,&g, 1);  
   write(fd,&b, 1);
   int strlena=0;
   if(strlen(str)>46){
      strlena=46;
   }else{
   strlena=strlen(str);
   }
   
   write(fd,str, strlena);  
   write(fd,"\n", 1);  
   usleep(20000);
}


int main (int argc, char *argv[]) { 
   int i=0;
   int st_init=1;

   fd=serial_open("/dev/ttyDISP",115200);  
   usleep(2000000);

   int n;
//=write(fd,"мы\n", 18); 
//   write(fd,"j83\n", 4); 
     
//   if (n<1) return;  
   tty_write_string(1,"Старт системы",255,0,255);
//   char bRead[BUFSIZE];  
//   BytesRead = read(fd, bRead, BUFSIZE);

          //Инициализация констант
          savedprobeg=readlongparam("/automedia/savedprobeg.const");

//          system( "/automedia/says/start.sh &");

	while(1){
           
	   if(refreshbitrate==1){
           //рисуем битрейт    
           int bitrate=readbitrate();
           if((bitrate<128)&(bitrate>0)){
//	    LCD_Goto(16,1);
//            LCD_Write_Char(0x06);
            refreshbitrate=0;
           }
           if(bitrate==128){
//	    LCD_Goto(16,1);
//            LCD_Write_Char(0x05);
            refreshbitrate=0;
           }
           //рисуем битрейт    
           if(bitrate>128){
//	    LCD_Goto(16,1);
//            LCD_Write_Char(0x04);
            refreshbitrate=0;
           }
           //рисуем битрейт    
           if(bitrate==0){
//	    LCD_Goto(16,1);
//            LCD_Write_Char(' ');
           }
	}


       eventsjob();

         probeg=readlongparam("/tmp/mikas/probeg")-14341980;
         sutochn=probeg-savedprobeg;
         tty_set_paramf("z",(float)probeg/100);
         tty_set_paramf("x",(float)sutochn/100);
//         LCD_Goto(8,1);
//	 LCD_Write_Int2Dec(8,probeg,2);
//         LCD_Goto(8,2);
//	 LCD_Write_Int2Dec(8,sutochn,2);
//         usleep(300000);


         speed=readlongparam("/tmp/mikas/speed");
         if(speed_old!=speed){
             speed_old=speed;
             tty_set_param("s",speed);       
         }

         int oboroti=readlongparam("/tmp/mikas/oboroti");
         if(oboroti_old!=oboroti){
             oboroti_old=oboroti;
             tty_set_param("o",oboroti);       
         }
//         LCD_Goto(12,1);
//         LCD_Write_String("   ");
//         LCD_Goto(12,1);
//	 LCD_Write_Int(speed);
//         LCD_Goto(12,2);
//         LCD_Write_String("    ");
//         LCD_Goto(12,2);
//	 LCD_Write_Int(oboroti);
//         usleep(300000);


         float volt=readfloatparam("/tmp/cubard/voltage");
         float amp=readfloatparam("/tmp/cubard/ampers");
//         LCD_Goto(12,1);
//         LCD_Write_String("   ");
//         LCD_Goto(12,1);
//	 LCD_Write_Int2Dec(4,(long)(volt*100),2);
//         LCD_Goto(12,2);
//         LCD_Write_String("    ");
//         LCD_Goto(12,2);
//	 LCD_Write_Int2Dec(4,(long)(amp*100),2);
//         usleep(300000);

//	eventsjob();
 
if ((fp = fopen("/tmp/newchannel.flg", "r")) != NULL){
  remove("/tmp/newchannel.flg");
  refreshbitrate=1;
}

//         Пишем номер станции
//	    LCD_Goto(1,1);
//	    num=readnumstation();
//	    LCD_Write_Int(num);
	    //Состояние интернета
//	    LCD_Goto(2,2);
	    //Источник звука
//	    LCD_Goto(4,1);
//            LCD_Write_Char(0x00);

	//Выведем время
//        printf("%s\n", buf);
    
//        LCD_Goto(12,2);
//        LCD_Write_String(buf);
          
         //Выведем RSSI
//         getgroup();
//         LCD_Goto(4,2);
//         LCD_Write_String(result_group);
    


         
        //Выведем радиостанцию
	getstring();
        if(result_string_len_old!=strlen(result_string)){
	    result_string_len_old=strlen(result_string);
            tty_write_string(2,"                                              ",0,0,255);
            tty_write_string(2,result_string,0,0,255);
	}
	    num=readnumstation();
            sprintf((char*)result_stringn,"%d.   ",num);
//            tty_write_string(3," ",0,128,255);
            tty_write_string(3,result_stringn,0,128,255);

        //Выведем событие
	getaction();
        if(result_action_len_old!=strlen(action)){
	    result_action_len_old=strlen(action);
            tty_write_string(1,"                                              ",0,0,255);
            tty_write_string(1,action,255,0,0);
	}


	//Выведем время
        char buf[100];
        time_t t = time(NULL);
        strftime(buf, BUFSIZ, "%T", localtime(&t));
        tty_set_str("m",&buf);

        int des=100;
        des++;
        if(des==101){
          des=0;
        t = time(NULL);
        strftime(buf, BUFSIZ, "%D", localtime(&t));
        tty_set_str("d",&buf);
        }

	    //Состояние интернета
	    int net=readpingtest();
	    if(oldstat!=net){
	    oldstat=net;
	    if(net==0){
		if(rev==0){
			write(fd,"i21\n", 4);  
		};
		if(rev==2){
			write(fd,"i21\n", 4);  
		};
		
	    }else{
			write(fd,"i45\n", 4);  
	    }
	    }

       
        //Обработка файлов статусов
        if (reset_key==20){
	     writelongparam("/automedia/savedprobeg.const",probeg);
	     savedprobeg=probeg;
	     reset_key=0;
        }

        if ((fp = fopen("/tmp/mplayeroff", "r")) != NULL){
           remove("/tmp/mplayeroff");
           write(fd,"j86\n", 4); 
        }

        if ((fp = fopen("/tmp/mplayeron", "r")) != NULL){
               remove("/tmp/mplayeron");
               write(fd,"j83\n", 4); 
        }


        if ((fp = fopen("/tmp/lirc/mute", "r")) != NULL){
           remove("/tmp/lirc/mute");
        }

        if ((fp = fopen("/tmp/lirc/power", "r")) != NULL){
           remove("/tmp/lirc/power");
        }

        if ((fp = fopen("/tmp/lirc/0", "r")) != NULL){
           remove("/tmp/lirc/0");
        }

        if ((fp = fopen("/tmp/lirc/1", "r")) != NULL){
           remove("/tmp/lirc/1");
        }

        if ((fp = fopen("/tmp/lirc/2", "r")) != NULL){
           remove("/tmp/lirc/2");
        }

        if ((fp = fopen("/tmp/lirc/3", "r")) != NULL){
           remove("/tmp/lirc/3");
        }
        if ((fp = fopen("/tmp/lirc/4", "r")) != NULL){
           remove("/tmp/lirc/4");
        }


        if ((fp = fopen("/tmp/lirc/5", "r")) != NULL){
           remove("/tmp/lirc/5");
	}

        if ((fp = fopen("/tmp/lirc/6", "r")) != NULL){
           remove("/tmp/lirc/6");
	}

        if ((fp = fopen("/tmp/lirc/7", "r")) != NULL){
           remove("/tmp/lirc/7");
	}

        if ((fp = fopen("/tmp/lirc/8", "r")) != NULL){
           remove("/tmp/lirc/8");
	}

        if ((fp = fopen("/tmp/lirc/9", "r")) != NULL){
           remove("/tmp/lirc/9");
	}

        if ((fp = fopen("/tmp/lirc/source", "r")) != NULL){
           remove("/tmp/lirc/source");
	   reset_key++;
	   reset_key_timer=0;
        }
        
        reset_key_timer++;
	if(reset_key_timer>=300){
	    reset_key_timer=0;
	    reset_key=0;
	}

         usleep(100000);

	}


return 0;
}


