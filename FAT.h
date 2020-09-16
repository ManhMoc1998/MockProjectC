#ifndef _FAT_H
#define _FAT_H

typedef struct
{
	uint16_t bytesPerSector;
	uint8_t sectorsPerCluster;
	uint8_t numberFAT;
	uint16_t maxEntryOfDir;
	uint16_t totalSector;
	uint16_t sectorsPerFAT;
	uint16_t indexSectorOfFAT;
	uint16_t indexSectorDirRoot;
	uint16_t indexSectorDataRegion;
} BootSector_t;
typedef struct
{
	char fileName[9];
	char extension[4];
	bool isFile;
	bool isSubEntry;
	uint8_t dayCreate;
	uint8_t monthCreate;
	uint8_t yearCreate;
	uint8_t secondCreate;
	uint8_t minuteCreate;
	uint8_t hourCreate;
	uint16_t dateLastMd;
	uint16_t timeLastMd;
	uint16_t clusterStart;
	uint32_t fileSize;
	struct EntryFile_t *parent;
	struct EntryFile_t *next;
} EntryFile_t;
void FAT_setBootSector(FILE *f, BootSector_t *bs);
uint16_t FAT_clusterNext(FILE *f, uint16_t clusterStart);
void FAT_setEntryFile(char inforEntry[32], EntryFile_t *entry);
#endif /*_FAT_H*/

