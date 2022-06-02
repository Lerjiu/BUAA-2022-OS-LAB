/*
 * operations on IDE disk.
 */

#include "fs.h"
#include "lib.h"
#include <mmu.h>

int time_read()
{
	u_int update;
	syscall_read_dev((u_int)(&update), 0x15000000, 4);
	u_int time;
	syscall_read_dev((u_int)(&time), 0x15000010, 4);
	return time;
}

void raid0_write(u_int secno, void *src, u_int nsecs)
{
	int i;
	u_int offset = 0;
	for(i = secno; i <secno + nsecs; i++)
	{
		if((i % 2) == 0)
		{
			ide_write(1, i/2, src + offset, 1);
		} else {
			ide_write(2, i/2, src + offset, 2);
		}
		offset += 0x200;
	}
}

void raid0_read(u_int secno, void *dst, u_int nsecs)
{
	int i;
    u_int offset = 0;
    for(i = secno; i <secno + nsecs; i++)
    {
        if((i % 2) == 0)
        {
            ide_read(1, i/2, dst + offset, 1);
        } else {
            ide_read(2, i/2, dst + offset, 2);
        }
        offset += 0x200;
    }
}

int raid4_valid(u_int diskno)
{
	u_int ret;
	u_int zero = 0;

	syscall_write_dev((u_int)(&diskno), 0x13000010, 4);
	
	syscall_write_dev((u_int)(&zero), 0x13000000, 4);

	syscall_write_dev((u_int)(&zero), 0x13000020, 1);

	syscall_read_dev((u_int)(&ret), 0x13000030, 1);
	if(ret == 0)
	{
		return 0;
	}else{
		return 1;
	}
}

int raid4_write(u_int blockno, void *src)
{
	int i;
	int invalid_num = 0;
	for(i = 1; i < 5; i++)
	{
		if(raid4_valid(i))
		{
			ide_write(i, blockno*2, src+(i-1)*0x200, 1);
			ide_write(i, blockno*2+1, src+(i+3)*0x200, 1);
		}else{
			invalid_num++;
		}
	}
	
	if(!raid4_valid(5))
	{
		invalid_num++;
		return invalid_num;
	}

	u_int disk5_1[128] = {0};
	u_int disk5_2[128] = {0};
	u_int *tmp = NULL;
	tmp = (u_int*)src;
	for(i = 0; i < 128; i++)
	{
		disk5_1[i] = tmp[i] ^ tmp[128+i] ^ tmp[256+i] ^ tmp[128*3+i];
		disk5_2[i] = tmp[512+i] ^ tmp[512+128+i] ^ tmp[512+256+i] ^ tmp[512+128*3+i];
	}

	ide_write(5, blockno*2, (void*)disk5_1, 1);
	ide_write(5, blockno*2+1, (void*)disk5_2, 1);
	return invalid_num;
}

int raid4_read(u_int blockno, void *dst)
{
	int invalid_num = 0;
	int invalid_disk = 0;
	int i;
	for(i = 1; i <= 5; i++)
	{
		if(!raid4_valid(i))
		{
			invalid_num++;
			invalid_disk = i;
		}
	}

	if(invalid_num == 0)
	{
		for(i = 1; i < 5; i++)
		{
			ide_read(i, blockno*2, dst+(i-1)*0x200, 1);
            ide_read(i, blockno*2+1, dst+(i+3)*0x200, 1);
		}
		u_int disk5_1[128] = {0};
   		u_int disk5_2[128] = {0};
    	u_int *tmp = NULL;
    	tmp = (u_int*)dst;
    	for(i = 0; i < 128; i++)
    	{
        	disk5_1[i] = tmp[i] ^ tmp[128+i] ^ tmp[256+i] ^ tmp[128*3+i];
        	disk5_2[i] = tmp[512+i] ^ tmp[512+128+i] ^ tmp[512+256+i] ^ tmp[512+128*3+i];
    	}
		u_int disk51[128] = {0};
		u_int disk52[128] = {0};
		ide_read(5, blockno*2, (void*)disk51, 1);
		ide_read(5, blockno*2+1, (void*)disk52, 1);
		
		for(i = 0; i < 128; i++)
		{
			if((disk5_1[i]!=disk51[i]) || (disk5_2[i]!=disk52[i]))
				return -1;
		}
		return 0;
	}

	if(invalid_num == 1)
	{
		if(invalid_disk == 5)
		{
			for(i = 1; i < 5; i++)
        	{
            	ide_read(i, blockno*2, dst+(i-1)*0x200, 1);
            	ide_read(i, blockno*2+1, dst+(i+3)*0x200, 1);
        	}
		}else{
			for(i = 1; i < 5; i++)
            {
                if(i == invalid_disk) continue;
                ide_read(i, blockno*2, dst+(i-1)*0x200, 1);
                ide_read(i, blockno*2+1, dst+(i+3)*0x200, 1);
            }
			u_int invalid1[128] = {0};
			u_int invalid2[128] = {0};
			u_int valid11[128] = {0};
			u_int valid12[128] = {0};
			u_int valid21[128] = {0};
			u_int valid22[128] = {0};
			u_int valid31[128] = {0};
			u_int valid32[128] = {0};
			u_int valid41[128] = {0};
			u_int valid42[128] = {0};
			if(invalid_disk == 1)
			{
				
			}
		}
		return 1;
	}
	
	if(invalid_num > 1) return invalid_num;
}

// Overview:
// 	read data from IDE disk. First issue a read request through
// 	disk register and then copy data from disk buffer
// 	(512 bytes, a sector) to destination array.
//
// Parameters:
//	diskno: disk number.
// 	secno: start sector number.
// 	dst: destination for data read from IDE disk.
// 	nsecs: the number of sectors to read.
//
// Post-Condition:
// 	If error occurrs during the read of the IDE disk, panic.
//
// Hint: use syscalls to access device registers and buffers
/*** exercise 5.2 ***/
void
ide_read(u_int diskno, u_int secno, void *dst, u_int nsecs)
{
	// 0x200: the size of a sector: 512 bytes.
	int offset_begin = secno * 0x200;
	int offset_end = offset_begin + nsecs * 0x200;
	int offset = 0;

	u_int offset_cur;
	u_int zero = 0;

	while (offset_begin + offset < offset_end) {
		// Your code here
		// error occurred, then panic.
		offset_cur = offset_begin + offset;
		if(syscall_write_dev((u_int)(&diskno), 0x13000010, 4) < 0)
		{
			user_panic("ide_read panic1");
		}

		if(syscall_write_dev((u_int)(&offset_cur), 0x13000000, 4) < 0)
		{
			user_panic("ide_read panic2");
		}

		if(syscall_write_dev((u_int)(&zero), 0x13000020, 1) < 0)
		{
			user_panic("ide_read panic3");
		}

		u_int ret;
		if(syscall_read_dev((u_int)(&ret), 0x13000030, 1) < 0)
		{
			user_panic("ide_read panic4");
		}
		
		if(ret == 0)
		{
			user_panic("ide_read panic5");
		}

		if(syscall_read_dev((u_int)(dst + offset), 0x13004000, 0x200) < 0)
		{
			user_panic("ide_read panic6");
		}

		offset += 0x200;
	}
}


// Overview:
// 	write data to IDE disk.
//
// Parameters:
//	diskno: disk number.
//	secno: start sector number.
// 	src: the source data to write into IDE disk.
//	nsecs: the number of sectors to write.
//
// Post-Condition:
//	If error occurrs during the read of the IDE disk, panic.
//
// Hint: use syscalls to access device registers and buffers
/*** exercise 5.2 ***/
void
ide_write(u_int diskno, u_int secno, void *src, u_int nsecs)
{
	// Your code here

	// DO NOT DELETE WRITEF !!!
	writef("diskno: %d\n", diskno);

	// while ( < ) {
		// copy data from source array to disk buffer.

		// if error occur, then panic.
	u_int offset_begin = secno * 0x200;
	u_int offset_end = offset_begin + nsecs * 0x200;
	u_int offset = 0;
	u_int dev_addr = 0x13000000;
	u_char status = 0;
	u_char write_value = 1;

	writef("diskno: %d\n", diskno);

	while (offset_begin + offset < offset_end) {
		u_int now_offset = offset_begin + offset;
		if (syscall_write_dev((u_int)&diskno, dev_addr + 0x10, 4) < 0)
		{
			user_panic("ide_write error!");
		}
		if (syscall_write_dev((u_int)&now_offset, dev_addr + 0x0, 4) < 0)
		{
			user_panic("ide_write error!");
		}
		if (syscall_write_dev((u_int)(src + offset), dev_addr + 0x4000, 0x200) < 0)
		{
			user_panic("ide_write error!");
		}
		if (syscall_write_dev((u_int)&write_value, dev_addr + 0x20, 1) < 0)
		{
			user_panic("ide_write error!");
		}
		status = 0;
		if (syscall_read_dev((u_int)&status, dev_addr + 0x30, 1) < 0)
		{
			user_panic("ide_write error!");
		}
		if (status == 0)
		{
			user_panic("ide write faild!");
		}
		offset += 0x200;
	}
}
