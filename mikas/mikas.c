#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/serial.h>

#define BUFSIZE 4096  


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
    int speed = 0;

    /* Open and configure serial port */
    if ((fd = open(device,O_RDWR|O_NOCTTY)) == -1)
	return -1;

    speed = rate_to_constant(rate);

    if (speed == 0) {
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
    cfsetispeed(&options, speed ?: B38400);
    cfsetospeed(&options, speed ?: B38400);
    cfmakeraw(&options);
    options.c_cflag |= (CLOCAL | CREAD);
    options.c_cflag &= ~CRTSCTS;
    if (tcsetattr(fd, TCSANOW, &options) != 0)
	return -1;

    return fd;
}


int main(int argc, char **argv)
{

   // start Garmin COM-port  
   char MikasInit[1] = {0xf0};  
   char MikasStart[5] = {0x81, 0x10, 0xf1, 0x81, 0x03};  
   char MikasParams[6] = {0x82, 0x10, 0xf1, 0x21, 0x01,0xa5};  
   char MikasProbeg[6] = {0x82, 0x10, 0xf1, 0x21, 0xF3,0x97};  
   char MikasError[8] = {0x84,0x10,0xF1,0x18,0x00,0x00,0x00,0x9D};
   int BytesRead=0;  
   int i;  
   int n;  
   int fdout;  
   FILE* fout;  
   char bRead[BUFSIZE];  

    int fd;

   fd=serial_open("/dev/MIKAS",20);  
   n = write(fd,MikasInit, 1);  
   if (n<1) return;  
   close(fd);

   fd=serial_open("/dev/MIKAS",10400);  

   usleep(25000); 

   n = write(fd,MikasStart, 5);  
   if (n<5) return;  

   usleep(100000);

//   while(1){
   BytesRead = read(fd, bRead, BUFSIZE);
//    if (BytesRead != -1){
   printf("\nBytes readed %d\n", BytesRead);  
   if(bRead[9]==0xc1){
   //ECU ready
    printf("ECU ready.\n");
//    close(fd);
//    fd=serial_open("/dev/MIKAS",57600);  

    while(1){

    n = write(fd,MikasParams, 6);  
    if (n<6) return;  
    usleep(100000);
//   while(1){
    BytesRead = read(fd, bRead, BUFSIZE);
//    if (BytesRead != -1){
    printf("\nBytes readed %d\n", BytesRead);  
    if(bRead[9]==0x61){
	printf("Recived data packet OK.\n");
    }else{
	printf("Recived data packet false.\n");
      return(0);
   }

    char priznak1=bRead[14];  //13
    char priznak2=bRead[15];
    char oshibka1=bRead[16];
    char oshibka2=bRead[17];
    char oshibka3=bRead[18];
    char oshibka4=bRead[19];

    int temp=bRead[39]-40;
    printf("-- Температура ОЖ: %d\n",temp);    

   fout = fopen("/tmp/mikas/temp", "w");  // open file to output bin-data from COM-port  
   if (!fout) perror("Open/create out-file: unable to open file!");  
   fprintf(fout,"%d\n",temp);    
   fclose(fout);



    int tps=bRead[41];
    printf("-- Положение дроссельной заслонки: %d\n",tps);    

   fout = fopen("/tmp/mikas/tps", "w");  // open file to output bin-data from COM-port  
   if (!fout) perror("Open/create out-file: unable to open file!");  
   fprintf(fout,"%d\n",tps);    
   fclose(fout);


    int oboroti=(bRead[27]<<8)+bRead[28];
    printf("-- Обороты двигателя: %d\n",oboroti);    

   fout = fopen("/tmp/mikas/oboroti", "w");  // open file to output bin-data from COM-port  
   if (!fout) perror("Open/create out-file: unable to open file!");  
   fprintf(fout,"%d\n",oboroti);    
   fclose(fout);


    int oborotihh=bRead[44]*10;
    printf("-- Обороты холостого хода: %d\n",oborotihh);    


   fout = fopen("/tmp/mikas/oborotixx", "w");  // open file to output bin-data from COM-port  
   if (!fout) perror("Open/create out-file: unable to open file!");  
   fprintf(fout,"%d\n",oborotihh);    
   fclose(fout);


    int rxxcel=bRead[45];
    printf("-- РХХ целевое: %d\n",rxxcel);    


   fout = fopen("/tmp/mikas/rxxcel", "w");  // open file to output bin-data from COM-port  
   if (!fout) perror("Open/create out-file: unable to open file!");  
   fprintf(fout,"%d\n",rxxcel);    
   fclose(fout);



    int rxxtek=bRead[46];
    printf("-- РХХ текущее: %d\n",rxxtek);    

   fout = fopen("/tmp/mikas/rxxtek", "w");  // open file to output bin-data from COM-port  
   if (!fout) perror("Open/create out-file: unable to open file!");  
   fprintf(fout,"%d\n",rxxtek);    
   fclose(fout);



    float uoz=bRead[47]/2;
    printf("-- УОЗ: %f\n",uoz);    

   fout = fopen("/tmp/mikas/uoz", "w");  // open file to output bin-data from COM-port  
   if (!fout) perror("Open/create out-file: unable to open file!");  
   fprintf(fout,"%f\n",uoz);    
   fclose(fout);



    int speed=bRead[45];
    printf("-- Скорость: %d\n",speed);    

   fout = fopen("/tmp/mikas/speed", "w");  // open file to output bin-data from COM-port  
   if (!fout) perror("Open/create out-file: unable to open file!");  
   fprintf(fout,"%d\n",speed);    
   fclose(fout);

    float vcc=5.2+bRead[48]*0.05;
    printf("-- Напряжение: %f\n",vcc);    

   fout = fopen("/tmp/mikas/vcc", "w");  // open file to output bin-data from COM-port  
   if (!fout) perror("Open/create out-file: unable to open file!");  
   fprintf(fout,"%f\n",vcc);    
   fclose(fout);


    int oborxxjel=bRead[49]*10;
    printf("-- Желательные обороты ХХ: %d\n",oborxxjel);    

   fout = fopen("/tmp/mikas/oborxxjel", "w");  // open file to output bin-data from COM-port  
   if (!fout) perror("Open/create out-file: unable to open file!");  
   fprintf(fout,"%d\n",oborxxjel);    
   fclose(fout);


    float rashchas=((bRead[38]<<8)|bRead[37])/50;
    printf("-- Расход л/час: %f\n",rashchas);    

   fout = fopen("/tmp/mikas/rashchas", "w");  // open file to output bin-data from COM-port  
   if (!fout) perror("Open/create out-file: unable to open file!");  
   fprintf(fout,"%f\n",rashchas);    
   fclose(fout);


    float rashod=((bRead[43]<<8)|bRead[44])/10/100;
    printf("-- Расход л/100km: %f\n",rashod);    

   fout = fopen("/tmp/mikas/rashod", "w");  // open file to output bin-data from COM-port  
   if (!fout) perror("Open/create out-file: unable to open file!");  
   fprintf(fout,"%f\n",rashod);    
   fclose(fout);


    //Признаки
    if((priznak1&0b1)!=0){
	printf("--- Пр1.0: Выключен двигатель\n");
    }
    if((priznak1&0b10)!=0){
	printf("--- Пр1.1: Холостой ход\n");
    }
    if((priznak1&0b100)!=0){
	printf("--- Пр1.2: Обогащение по мощности\n");
    }
    if((priznak1&0b1000)!=0){
	printf("--- Пр1.3: Блокировка подачи топлива\n");
    }
    if((priznak1&0b10000)!=0){
	printf("--- Пр1.4: Регулирование ДК\n");
    }
    if((priznak1&0b100000)!=0){
	printf("--- Пр1.5: Детонация\n");
    }
    if((priznak1&0b1000000)!=0){
	printf("--- Пр1.6: Продувка адсорбера\n");
    }
    if((priznak1&0b10000000)!=0){
	printf("--- Пр1.7: Сохранение результатов обучения по датчику кислорода\n");
    }


    //Ошибки
    if((oshibka1&0b1)!=0){
	printf("---- Ош1.0: Ошибка ДКПВ\n");
    }
    if((oshibka1&0b10)!=0){
	printf("---- Ош1.1: Ошибка синхронизации времени\n");
    }
    if((oshibka1&0b100)!=0){
	printf("---- Ош1.2: Ошибка EEPROM\n");
    }
    if((oshibka1&0b1000)!=0){
	printf("---- Ош1.3: Ошибка синхронизации фаз\n");
    }
    if((oshibka1&0b10000)!=0){
	printf("---- Ош1.4: Ошибка ДФ\n");
    }
    if((oshibka1&0b100000)!=0){
	printf("---- Ош1.5: Ошибка сброса процессора\n");
    }
    if((oshibka1&0b1000000)!=0){
	printf("---- Ош1.6: Ошибка ОЗУ\n");
    }
    if((oshibka1&0b10000000)!=0){
	printf("---- Ош1.7: Ошибка ПЗУ\n");
    }

    if((oshibka2&0b1)!=0){
	printf("---- Ош2.0: Низкое бортовое напряжение\n");
    }
    if((oshibka2&0b10)!=0){
	printf("---- Ош2.1: Низкий уровень сигнала с потенциометра коррекции СО\n");
    }
    if((oshibka2&0b100)!=0){
	printf("---- Ош2.2: Низкий уровень сигнала с датчика температуры воздуха\n");
    }
    if((oshibka2&0b1000)!=0){
	printf("---- Ош2.3: Низкий уровень сигнала с ДТОЖ\n");
    }
    if((oshibka2&0b10000)!=0){
	printf("---- Ош2.4: Низкий уровень сигнала с ДК\n");
    }
    if((oshibka2&0b100000)!=0){
	printf("---- Ош2.5: Низкий уровень сигнала с ДПДЗ\n");
    }
    if((oshibka2&0b1000000)!=0){
	printf("---- Ош2.6: Низкий уровень сигнала с ДРВ\n");
    }
    if((oshibka2&0b10000000)!=0){
	printf("---- Ош2.7: Низкий уровень шума двигателя\n");
    }


    if((oshibka3&0b1)!=0){
	printf("---- Ош3.0: Высокое бортовое напряжение\n");
    }
    if((oshibka3&0b10)!=0){
	printf("---- Ош3.1: Высокий уровень сигнала с потенциометра коррекции СО\n");
    }
    if((oshibka3&0b100)!=0){
	printf("---- Ош3.2: Высокий уровень сигнала с датчика температуры воздуха\n");
    }
    if((oshibka3&0b1000)!=0){
	printf("---- Ош3.3: Высокий уровень сигнала с ДТОЖ\n");
    }
    if((oshibka3&0b10000)!=0){
	printf("---- Ош3.4: Высокий уровень сигнала с ДК\n");
    }
    if((oshibka3&0b100000)!=0){
	printf("---- Ош3.5: Высокий уровень сигнала с ДПДЗ\n");
    }
    if((oshibka3&0b1000000)!=0){
	printf("---- Ош3.6: Высокий уровень сигнала с ДРВ\n");
    }
    if((oshibka3&0b10000000)!=0){
	printf("---- Ош3.7: Высокий уровень шума двигателя\n");
    }


    if((oshibka4&0b1)!=0){
	printf("---- Ош4.0: Обрыв датчика детонации\n");
    }
    if((oshibka4&0b10)!=0){
	printf("---- Ош4.1: нет связи с иммобилайзером\n");
    }
    if((oshibka4&0b100)!=0){
	printf("---- Ош4.2: n.c. \n");
    }
    if((oshibka4&0b1000)!=0){
	printf("---- Ош4.3: n.c. \n");
    }
    if((oshibka4&0b10000)!=0){
	printf("---- Ош4.4: Нет отклика ДК при обогащении\n");
    }
    if((oshibka4&0b100000)!=0){
	printf("---- Ош4.5: Нет отклика ДК при обеднении\n");
    }
    if((oshibka4&0b1000000)!=0){
	printf("---- Ош4.6: Ошибка датчика скорости\n");
    }
    if((oshibka4&0b10000000)!=0){
	printf("---- Ош4.7: Ошибка РХХ\n");
    }


   fdout = open("/tmp/mikas/param.bin", O_RDWR);  // open file to output bin-data from COM-port  
   if (!fdout) perror("Open/create out-file: unable to open file!");  
    write(fdout,bRead,BytesRead);
    close(fdout);




    n = write(fd,MikasProbeg, 6);  
    if (n<6) return;  
    usleep(100000);
//   while(1){
    BytesRead = read(fd, bRead, BUFSIZE);
//    if (BytesRead != -1){
    printf("\nBytes readed %d\n", BytesRead);  
    if(bRead[10]==0x61){
	printf("Recived data packet OK.\n");
    }else{
	printf("Recived data packet false.\n");
      return(0);
   }


   unsigned long probeg=(bRead[17]<<24)|(bRead[18]<<16)|(bRead[19]<<8)|(bRead[20]);
   printf("-- Пробег: %lu\n",probeg);    



   fout = fopen("/tmp/mikas/probeg", "w");  // open file to output bin-data from COM-port  
   if (!fout) perror("Open/create out-file: unable to open file!");  
   fprintf(fout,"%lu\n",probeg);    
   fclose(fout);


   fdout = open("/tmp/mikas/probeg.bin", O_RDWR);  // open file to output bin-data from COM-port  
   if (!fdout) perror("Open/create out-file: unable to open file!");  
    write(fdout,bRead,BytesRead);
    close(fdout);

/*
    ////////////get errors
    n = write(fd,MikasError, 8);  
    if (n<6) return;  
    usleep(500000);
//   while(1){
    BytesRead = read(fd, bRead, BUFSIZE);
//    if (BytesRead != -1){
    printf("\nBytes readed %d\n", BytesRead);  
    if(bRead[9]==0x61){
	printf("Recived data packet OK.\n");
    }
*/

    usleep(300000);


    }

/*   fdout = open("ecu_temp", O_RDWR);  // open file to output bin-data from COM-port  
   if (!fdout) perror("Open/create out-file: unable to open file!");  
    write(fdout,ffer,10);
    close(fdout);
*/

   }else{
    printf("ECU busy!");
    return(0);
   }

//    }

/*   for (i=0; i<=BytesRead-2; i++)  // find bytes as correspondents to Garmin Messages  
   {  


      if ((bRead*==0x10 && bRead[i+1]==0x36 && bRead[i+2]==0x0C) || (bRead*==0x10 && bRead[i+1]==0x38 && bRead[i+2]==0x28))  
      {  
      printf("Bytes readed %lu, found Garmin Message %d\n", BytesRead, (int)bRead[i+1]); break;  
      }  
      else  
      {  
      printf("Bytes readed %lu, NOT found Garmin Message, EXIT!\n", BytesRead); return;  
      }  

   }  
*/

//}

    return 0;
}


