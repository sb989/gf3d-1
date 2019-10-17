#include <stdio.h>
#include <string.h>
#include "gf3d_create_obj.h"
#include "simple_logger.h"
//#include "vertex.h"

void gf3d_create_obj_simple_box(char * d, char * w, char * h,char * filename)
{
  int i;
  slog("w is %s",w);
  slog("d is %s",d);
  slog("h is %s",h);
  char vec[9][20000];
  /*char vec1[sizeof(w)+8];// w,0,0
  char vec2[sizeof(d)+4];// 0,d,0
  char vec3[sizeof(h)+4];// 0,0,h
  char vec4[sizeof(w)+sizeof(h)+3];// w,0,h
  char vec5[sizeof(w)+sizeof(d)+3];// w,d,0
  char vec6[sizeof(d)+sizeof(h)+3];// 0,d,h
  char vec7[sizeof(w)+sizeof(d)+sizeof(h)];// w,d,h
  char vec8[sizeof(char)*6] = "0 0 0";*/
  strcpy(vec[1],w);
  strcat(vec[1]," 0 0");
  strcpy(vec[2],"0 ");
  strcat(vec[2],d);
  strcat(vec[2]," 0");
  strcpy(vec[3],"0 0 ");
  strcat(vec[3],h);
  strcpy(vec[4],w);
  strcat(vec[4]," 0 ");
  strcat(vec[4],h);
  strcpy(vec[5],w);
  strcat(vec[5]," ");
  strcat(vec[5],d);
  strcat(vec[5]," 0");
  strcpy(vec[6],"0 ");
  strcat(vec[6],d);
  strcat(vec[6]," ");
  strcat(vec[6],h);
  strcat(vec[7],w);
  strcat(vec[7]," ");
  strcat(vec[7],d);
  strcat(vec[7]," ");
  strcat(vec[7],h);
  strcpy(vec[8],"0 0 0");
  slog("%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n",vec[1],vec[2],vec[3],vec[4],vec[5],vec[6],vec[7],vec[8]);
  char texture[] = {"vt 1.000000 0.000000\nvt 0.000000 1.000000\nvt 0.000000 0.000000\nvt 1.000000 0.000000\nvt 0.000000 1.000000\nvt 0.000000 0.000000\nvt 1.000000 0.000000\nvt 0.000000 1.000000\nvt 0.000000 0.000000\nvt 1.000000 0.000000\nvt 0.000000 1.000000\nvt 0.000000 0.000000\nvt 1.000000 0.000000\nvt 0.000000 1.000000\nvt 0.000000 0.000000\nvt 1.000000 1.000000\nvt 1.000000 1.000000\nvt 1.000000 1.000000\nvt 1.000000 1.000000\nvt 1.000000 1.000000"};
  char normal[]  = {"vn -1.0000 0.0000 0.0000\nvn 0.0000 0.0000 -1.0000\nvn 1.0000 0.0000 0.0000\nvn 0.0000 0.0000 1.0000\nvn 0.0000 -1.0000 0.0000\nvn 0.0000 1.0000 0.0000"};
  char face[] = {"f 2/1/1 3/2/1 1/3/1\nf 4/4/2 7/5/2 3/6/2\nf 8/7/3 5/8/3 7/9/3\nf 6/10/4 1/11/4 5/12/4\nf 7/13/5 1/11/5 3/6/5\nf 4/4/6 6/14/6 8/15/6\nf 2/1/1 4/16/1 3/2/1\nf 4/4/2 8/17/2 7/5/2\nf 8/7/3 6/18/3 5/8/3\nf 6/10/4 2/19/4 1/11/4\nf 7/13/5 5/20/5 1/11/5\nf 4/4/6 2/19/6 6/14/6"};
  //FILE* tmp = tmpfile();
  slog("made it");
  char  name [50], filepath[50];

  slog("count :%d",count);
  sprintf(name,"bound_box_%d",count);
  slog(name);
  sprintf(filepath,"models/%s.obj",name);
  slog(name);
  slog(filepath);
  FILE* tmp = fopen(filepath,"w");
    if (!tmp)
    {
        puts("Unable to create temp file");
        return;
    }
    slog("here");
    for (i =1;i<9;i++)
    {
      fputs("v ",tmp);
      fputs(vec[i],tmp);
      fputs("\n",tmp);
    }
    slog("here1");
    fputs(texture,tmp);
    fputs("\n",tmp);
    fputs(normal,tmp);
    fputs("\n",tmp);
    fputs(face,tmp);
    fputs("\n",tmp);
    //gf3d_model_load(tmp);
    rewind(tmp);
    /*while (!feof(tmp))
        putchar(fgetc(tmp));
    rewind(tmp);
    */fclose(tmp);
    //return tmp;
  /*  char target_file[25];
    sprintf(target_file,"images/bound_box_%d.png",count);
    FILE *source, *target;
    unsigned char ch;
    source = fopen("images/red_frame.png", "rb");
    target = fopen(target_file, "wb");
    while( ( ch = fgetc(source) ) != EOF )
        fputc(ch, target);*/
    printf("File copied successfully.\n");
    //fclose(source);
    //fclose(target);
    strcpy(filename,name);


}
