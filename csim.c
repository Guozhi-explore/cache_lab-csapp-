/*****郭志 517021910503*******/
#include "cachelab.h"
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include <stdbool.h>

//we only have to know hit or miss,therefore it is not necessary to know block offset
typedef struct cache_line
{
	int valid;
	int tag[32];
	long time;
}cache_line;
//use a struct to store orders
typedef struct orders{
	int order_h;
	int order_v;
	int order_s;
	int order_E;
	int order_b;
	char *order_t;
}orders;

orders current_order;
cache_line *current_cache_lines;
long auto_time=0;                                                        //store sequence
int global_h_m_e[3]={0};                                                 //全局变量区

int change_char_to_int(char *ch)
{
	int base=0;
	for(int i=0;i<strlen(ch);++i)
	{
		base*=10;
		base+=(ch[i]-'0');
	}
	return base;
}
//get parameter from terminal input
void get_parameter(int argc,char *argv[])
{
	//select from rear to head
 	  current_order.order_t=argv[argc-1];
   	current_order.order_b=change_char_to_int(argv[argc-3]);
   	current_order.order_E=change_char_to_int(argv[argc-5]);
   	current_order.order_s=change_char_to_int(argv[argc-7]);
   	if(argc>8)
   	{
   		if(argv[argc-9][1]=='v')
   			current_order.order_v=1;
   		if(argv[argc-9][1]=='h')
   			current_order.order_h=1;
   	}
    return;
}

void show_help()
{
	printf("%s\n", "Usage: ./csim [-hv] -s <num> -E <num> -b <num> -t <file>");
	printf("%s\n", "Options:");
    printf("%s\n","-h         Print this help message." );
    printf("%s\n","-v         Optional verbose flag." );
    printf("%s\n","-s <num>   Number of set index bits.");
    printf("%s\n","-E <num>   Number of lines per set." );
    printf("%s\n","-b <num>   Number of block offset bits." );
    printf("%s\n","-t <file>  Trace file." );
    printf("%s\n","" );
    printf("%s\n","Examples:" );
    printf("%s\n","linux>  ./csim-ref -s 4 -E 1 -b 4 -t traces/yi.trace" );
    printf("%s\n","linux>  ./csim-ref -v -s 8 -E 2 -b 4 -t traces/yi.trace" );
}

void allocate_cache()
{
	int line_number;
	int S_number;
	S_number=1<<current_order.order_s;
  line_number=S_number*current_order.order_E;
  current_cache_lines=(cache_line*)calloc(line_number,sizeof(cache_line));
}

int change_hex_to_binary(char *hex,int index[])
{
	int site=0,number,site_temp;
	while(*(hex+site)!=',')
	{
       site++;
	}
	site_temp=site;
	while(site_temp!=0)
	{
		site_temp--;
		if(hex[site_temp]<='9')
        number=hex[site_temp]-'0';
        if(hex[site_temp]>'9')
        	number=hex[site_temp]-'a'+10;
        index[32-4*(site-site_temp)]=number/8;     //c语言中的自动类型转换是整形高于字符型
        number%=8;
        index[32-4*(site-site_temp)+1]=number/4;
        number%=4;
        index[32-4*(site-site_temp)+2]=number/2;
        number%=2;
        index[32-4*(site-site_temp)+3]=number;
	}
	return site;                                        //get the index of ',',in this way, more easy to print order
}

void load_store(int index[],int Hit_Miss_Evi[])
{
	int cache_set=0;
  int LRU_line=0;
  long LRU_time=99999999999;                                     //give latest_time a big number
	for(int i=32-(current_order.order_s+current_order.order_b);i<32-current_order.order_b;++i)
	{
		cache_set*=2;
		cache_set+=index[i];
	}
  for(int i=0;i<current_order.order_E;++i)
  {
  	bool tag_equal=true;                                                      //judge tag is equal or not
    if(current_cache_lines[current_order.order_E*cache_set+i].valid==0)
    {
      LRU_time=0;
      LRU_line=i;
    }
    else{
      if(current_cache_lines[current_order.order_E*cache_set+i].time<LRU_time)
      {
        LRU_time=current_cache_lines[current_order.order_E*cache_set+i].time;                  //get the LRU line
        LRU_line=i;
      }
    }

  	for(int j=0;j<32-current_order.order_s-current_order.order_b;++j)
  	{
  		if(current_cache_lines[current_order.order_E*cache_set+i].tag[j]!=index[j])
        tag_equal=false;
  	}
  	if(tag_equal==true)
  	{
      if(current_cache_lines[current_order.order_E*cache_set+i].valid==0)                  //标记位一致但是缓存中有效位为0
      {
        Hit_Miss_Evi[1]=1;
        current_cache_lines[current_order.order_E*cache_set+i].valid=1;
        current_cache_lines[current_order.order_E*cache_set+i].time=auto_time;
        auto_time++;
        return;                         
      }
      else
      {
        Hit_Miss_Evi[0]+=1;  
        current_cache_lines[current_order.order_E*cache_set+i].time=auto_time;
        auto_time++;                                                         //hit
        return; 
      }
  	}
  }
  if(current_cache_lines[current_order.order_E*cache_set+LRU_line].valid!=0)
  {
    Hit_Miss_Evi[2]=1;
  }
  for(int j=0;j<32-current_order.order_s-current_order.order_b;++j)
  {
    current_cache_lines[current_order.order_E*cache_set+LRU_line].tag[j]=index[j];                //LRu 替换
    current_cache_lines[current_order.order_E*cache_set+LRU_line].valid=1;
  }
  Hit_Miss_Evi[1]=1;
  current_cache_lines[current_order.order_E*cache_set+LRU_line].time=auto_time;
  return;
}

void cache_memory()
{
	FILE *fp;
  char buff[1024];
  int Hit_Miss_Evi[3]={0,0,0};
  fp= fopen(current_order.order_t, "r");
  while(fgetc(fp)!=EOF)
  {
      int index[32]={0};
      int ending_index=0;
  	  fgets(buff,1023,fp );
      ending_index=change_hex_to_binary(buff+2,index)+4;                  //根据格式打印
      for(int i=0;i<3;++i)
      {
        Hit_Miss_Evi[i]=0;
      }
      switch(buff[0])
      {
      	case 'L': case'S':
  		    load_store(index,Hit_Miss_Evi);
  		    break;
      	case 'M':
          load_store(index,Hit_Miss_Evi);
          load_store(index,Hit_Miss_Evi);
          break;
      	default:
          continue;
          break;
      }
      if(current_order.order_v==1)
      {
        for(int i=0;i<ending_index;++i)
        {
          printf("%c",buff[i]);
        }
        if(Hit_Miss_Evi[1]==1)
          printf("%s"," miss" );
        if(Hit_Miss_Evi[2]==1)
          printf("%s"," eviction" );
        if(Hit_Miss_Evi[0]==1)
          printf("%s"," hit" );
        if(Hit_Miss_Evi[0]==2)
          printf("%s"," hit hit" );
        printf("\n");
      }
      for(int i=0;i<3;++i)
      {
        global_h_m_e[i]+=Hit_Miss_Evi[i];
      }
  }
  fclose(fp);
  return;
}

int main(int argc,char *argv[])
{
    if(argc<9)
    {
    	printf("%s\n","the command is not right" );                        //read command as well as detect abnormal input
    	return 0;
    }
    get_parameter(argc,argv);
    if(current_order.order_h==1)
    {
        show_help();
        return 0;
    }
    allocate_cache();

    cache_memory();
    
    printSummary(global_h_m_e[0], global_h_m_e[1], global_h_m_e[2]);
    return 0;
}
