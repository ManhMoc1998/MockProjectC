#include<stdio.h>
#include<stdbool.h>
#include<stdint.h>
#include"FAT.h"
#include"HAL.h"

FILE *f;
FILE* File_Init(char *filename)
{
	f=fopen(filename,"rb");
	return f;
}
void Read_File(FILE *f,EntryFile_t *entry,BootSector_t *bs)
{
	uint16_t temp=0;
	uint16_t index;
	uint16_t clusterStart;
	bool check=true;
	char inforSector[512];
	clusterStart=(uint16_t)entry->clusterStart;
   	while(check)
	{
		if(clusterStart >= 0xFFE)
		{
			check=false;
		}
		else
		{
			temp=((clusterStart-2) * bs->sectorsPerCluster)+ bs->indexSectorDataRegion;
		    fseek(f,temp*512,SEEK_SET);
			fread(inforSector,sizeof(char),512,f);
			for(index=0;index<512;index++)
			{
				printf("%c",inforSector[index]);
			}
			break;
			clusterStart=FAT_clusterNext(f,clusterStart);
		}
	}
}
EntryFile_t* transTree(FILE *f,BootSector_t *bs,uint16_t clusterStart, EntryFile_t *parent)
{
	uint16_t index=0;
	EntryFile_t * Head=NULL;
	EntryFile_t * Tail=NULL;
	EntryFile_t * nodeEntry=NULL;
	char inforEntry[32];
	bool check=true;
	
	//Head=(EntryFile_t*)malloc(sizeof(EntryFile_t));
	
	if(clusterStart==0)
	{
		index=bs->indexSectorDirRoot;
		fseek(f,index*512,SEEK_SET);
	}
	else
	{
		index=bs->indexSectorDataRegion + (clusterStart-2)*bs->sectorsPerCluster;
		fseek(f,index*512,SEEK_SET);
	}	
	while(check)
	{
		nodeEntry=(EntryFile_t*)(malloc(sizeof(EntryFile_t)));
		fread(inforEntry,sizeof(char),32,f);
		if(inforEntry[0]==0) /*End list*/
		{
			free(nodeEntry);
			nodeEntry=NULL;
			break;
		}
		FAT_setEntryFile(inforEntry,nodeEntry);
		if(nodeEntry->isSubEntry) continue;/*Sub entry*/
		else if(inforEntry[0]==0x2e && inforEntry[1]==0x2e)
		{
		//	nodeEntry->clusterStart=clusterParent;
			nodeEntry->parent=parent;
			nodeEntry->fileName[0]='.';
			nodeEntry->fileName[1]='.';
			nodeEntry->fileName[2]=0;
		//	nodeEntry->extension[0]=' ';
			//nodeEntry->extension[1]=0;
		}
		else if(inforEntry[0]==0x2e)
		{
			nodeEntry->fileName[0]='.';
			nodeEntry->fileName[1]=0;
			//nodeEntry->extension[0]=' ';
			//nodeEntry->extension[1]=0;
		}
		if(Head==NULL)
		{
			Head=nodeEntry;
			Tail=nodeEntry;
			Tail->next=NULL;
		}
		else
		{
			Tail->next=nodeEntry;
			Tail=nodeEntry;
			Tail->next=NULL;
		}
	}
	return Head;
}

