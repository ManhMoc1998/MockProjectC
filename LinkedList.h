#ifndef _LINKED_LIST_H_
#define _LINKED_LIST_H_
typedef struct
{
	char fileName[9];
	char extension[4];
	bool isFile;
	uint16_t clusterStart;
} List;
#endif /*_LINKED_LIST_H_*/
