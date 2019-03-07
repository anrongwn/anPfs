#pragma once
#include "uv.h"
#define DATA2_LEN	(1024)

class CanReadReq :
	public uv_fs_t
{
public:
	CanReadReq();
	~CanReadReq();

public:
	char data2[DATA2_LEN];
};

