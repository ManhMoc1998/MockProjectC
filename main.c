#include<stdio.h>
#include<stdint.h>
#include<stdbool.h>
#include"FAT.h"
#include"HAL.h"

void Menu()
{
	
}

int main()
{
	FILE *f;
	char *s1="Copy.img";
	BootSector_t bs;//=(BootSector_t*)malloc(sizeof(BootSector_t));
	EntryFile_t *entry;
	uint16_t index=0;
	EntryFile_t *temp;
	char c;
	uint16_t clusterParent=0;
	
	f=File_Init(s1);
	FAT_setBootSector(f,&bs);
	entry=transTree(f,&bs,0,NULL);
	while(1)
	{
		temp=entry;
		index=0;
		while(temp!=NULL)
		{
		
			printf("%d %s",index,temp->fileName);
			if(temp->fileName[0]=='.'){}
			else if(temp->isFile)
			{
				//printf("%s  File	\n",index,temp->extension);
			}
			else
			{
				printf(" 	Folder	");
			}
			printf("\n");
			index++;
			temp=temp->next;
		}
		printf("Enter:");
		fflush(stdin);
		scanf("%c",&c);
		
			//entry=transTree(f,&bs,temp->clusterParent,0);
		EntryFile_t *en1=entry;
		en1=en1->next;
		//entry=transTree(f,&bs,en->clusterStart,clusterParent);
	
		
		{
			EntryFile_t *en=entry;
			uint16_t count=0;
			while(count<(uint16_t)(c-'0'))
			{
				en=en->next;
				count++;
			}
			if(en->fileName[0]==0x2e && en->fileName[1]==0x2e)
			{
			//	entry=transTree(f,&bs,en->clusterStart,clusterParent);
			// Tao lai list
				entry=en->parent;
			}
			if(en->isFile) Read_File(f,en,&bs);
			else
			{
				entry=transTree(f,&bs,en->clusterStart,entry);
			}
		}
	}
	return 0;
}
