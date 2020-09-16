#include<stdio.h>
#include<stdint.h>
#include<stdbool.h>
#include"FAT.h"

#define LITTLE_ENDIAN_TO_DEC(a,b) (a)

static void FAT_getClusterStart(char inforEntry[32], EntryFile_t * entry);
static void FAT_getDate(uint16_t date, EntryFile_t *entry);
static void FAT_getTime(uint16_t time, EntryFile_t *entry);
static void FAT_getFileName(char inforEntry[32], EntryFile_t * entry);
static void FAT_getFileExtension(char inforEntry[32], EntryFile_t *entry);
void FAT_setBootSector(FILE *f, BootSector_t *bs)
{
	fseek(f,0xb,SEEK_SET);/*0x00b*/
	fread(&(bs->bytesPerSector),sizeof(uint16_t),1,f);
	fread(&(bs->sectorsPerCluster),sizeof(uint8_t),1,f);
	fseek(f,0x10,SEEK_SET);/*0x010*/
	fread(&(bs->numberFAT),sizeof(uint8_t),1,f);
	fread(&(bs->maxEntryOfDir),sizeof(uint16_t),1,f);
	fread(&(bs->totalSector),sizeof(uint16_t),1,f);
	fseek(f,0x16,SEEK_SET);/*0x016*/
	fread(&(bs->sectorsPerFAT),sizeof(uint16_t),1,f);
	
	bs->indexSectorOfFAT=1;
	bs->indexSectorDirRoot=bs->indexSectorOfFAT + (bs->numberFAT)*(bs->sectorsPerFAT);
	bs->indexSectorDataRegion=bs->indexSectorDirRoot + (bs->maxEntryOfDir*32/512);
}
void FAT_setEntryFile(char inforEntry[32], EntryFile_t *entry)
{
	uint16_t date;
	uint16_t time;
	
	date=( (uint8_t)inforEntry[0x18]+ ((uint8_t)inforEntry[0x19])*16*(uint16_t)16);
	time=( (uint8_t)inforEntry[0x16]+ ((uint8_t)inforEntry[0x17])*16*(uint16_t)16);
	FAT_getClusterStart(inforEntry, entry);
    FAT_getDate(date, entry);
    FAT_getTime(time, entry);
    FAT_getFileName(inforEntry, entry);
    FAT_getFileExtension(inforEntry,entry);
}
void FAT_getDate(uint16_t date, EntryFile_t *dr)
{
	uint8_t day;
	uint8_t month;
	uint8_t year;
	
	day=date&(0x001F);
    month= (uint8_t)((date&(0x01E0))>>5);
    year=((date&(0xFE00))>>9) +(uint16_t)1980;
    dr->dayCreate=day;
    dr->monthCreate=month;
    dr->yearCreate=year;
}
void FAT_getTime(uint16_t time, EntryFile_t *dr)
{
	uint8_t second;
	uint8_t minute;
	uint8_t hour;
	
	second=time&(0x001F);
    minute= (uint8_t)((time&(0x07E0))>>5);
    hour=(time&(0xF800))>>11;
    dr->secondCreate=second;
    dr->minuteCreate=minute;
    dr->hourCreate=hour;
}
void FAT_getFileName(char inforEntry[32], EntryFile_t * entry)
{
	uint8_t index;

	for(index=0;index<8;index++)
	{
		entry->fileName[index]=inforEntry[index];
		if(inforEntry[index]==' ') break;
	}
	entry->fileName[index]=0;
	if(inforEntry[0x0b]==0x10)
	{
		entry->isFile=false;
	}
	else
	{
		entry->isFile=true;
	}
	/*check subEntry*/
	if(inforEntry[0x0b]==0x0F)
	{
		entry->isSubEntry=true;
	}
	else
	{
		entry->isSubEntry=false;
	}
}
void FAT_getClusterStart(char inforEntry[32], EntryFile_t * entry)
{
	uint16_t clusterStart;
	
	clusterStart=( (uint16_t)inforEntry[0x1a]& 0x00FF )| ( ((uint16_t)inforEntry[0x1b] & 0x00FF)<<8) ;
	entry->clusterStart=clusterStart;
}
void FAT_getFileExtension(char inforEntry[32], EntryFile_t *entry)
{
	uint8_t index;
	
	for(index=8;index<=10;index++)
	{
		entry->extension[index-8]=inforEntry[index];
		if(inforEntry[index]==' ') break;
	}
	entry->extension[index]=0;
}
uint16_t FAT_clusterNext(FILE *f, uint16_t clusterStart)
{
	uint16_t retVal;
	uint16_t indexByteOfFAT;
	char arr[6]={0};
	
	indexByteOfFAT=(clusterStart/4)*6;
	fseek(f,indexByteOfFAT+512,SEEK_SET);
	fread(arr,sizeof(char),6,f);
	if(clusterStart%4==0)
	{
		retVal=(uint8_t)arr[0];
		retVal|=( ((uint16_t)arr[1] & 0x000F)<<8 );
	}
	else if(clusterStart%4==1)
	{
		retVal=(uint8_t)arr[2];
		retVal=retVal<<4;
		retVal|= ( ((uint16_t)arr[1] &0x00F0) >> 4);
	}
	else if(clusterStart%4==2)
	{
		retVal=(uint8_t)arr[3];
		retVal|=( ((uint16_t)arr[4] & 0x000F)<<8 );
	}
	else
	{
		retVal=(uint8_t)arr[5];
		retVal=retVal<<4;
		retVal|= ( ((uint16_t)arr[4] &0x00F0) >> 4);
	}

	return retVal;
}
