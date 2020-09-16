#ifndef _HAL_H
#define _HAL_H

FILE* File_Init(char *filename);
void Read_Sector(FILE *f,uint16_t clusterStart);
void Read_Multi_Sector(FILE *f,uint16_t clusterStart);
void Read_File(FILE *f,EntryFile_t *entry,BootSector_t *bs);
void detailDirRoot(FILE *f,BootSector_t *bs);
EntryFile_t* transTree(FILE *f,BootSector_t *bs,uint16_t clusterStart, EntryFile_t *parent);
bool File_Deinit(FILE **f);
#endif /*_HAL_H*/
