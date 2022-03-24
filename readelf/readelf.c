/* This is a simplefied ELF reader.
 * You can contact me if you find any bugs.
 *
 * Luming Wang<wlm199558@126.com>
 */

#include "kerelf.h"
#include <stdio.h>
/* Overview:
 *   Check whether it is a ELF file.
 *
 * Pre-Condition:
 *   binary must longer than 4 byte.
 *
 * Post-Condition:
 *   Return 0 if `binary` isn't an elf. Otherwise
 * return 1.
 */
int is_elf_format(u_char *binary)
{
        Elf32_Ehdr *ehdr = (Elf32_Ehdr *)binary;
        if (ehdr->e_ident[EI_MAG0] == ELFMAG0 &&
                ehdr->e_ident[EI_MAG1] == ELFMAG1 &&
                ehdr->e_ident[EI_MAG2] == ELFMAG2 &&
                ehdr->e_ident[EI_MAG3] == ELFMAG3) {
                return 1;
        }

        return 0;
}

/* Overview:
 *   read an elf format binary file. get ELF's information
 *
 * Pre-Condition:
 *   `binary` can't be NULL and `size` is the size of binary.
 *
 * Post-Condition:
 *   Return 0 if success. Otherwise return < 0.
 *   If success, output address of every section in ELF.
 */

/*
    Exercise 1.2. Please complete func "readelf". 
*/
int readelf(u_char *binary, int size)
{
        Elf32_Ehdr *ehdr = (Elf32_Ehdr *)binary;

        int Nr;

        Elf32_Shdr *shdr = NULL;

        u_char *ptr_sh_table = NULL;
        Elf32_Half sh_entry_count;
        Elf32_Half sh_entry_size;
		
		Elf32_Half ph_entry_num;
		Elf32_Half ph_entry_size;
		Elf32_Phdr *phdr = NULL;

        // check whether `binary` is a ELF file.
        if (size < 4 || !is_elf_format(binary)) {
                printf("not a standard elf format\n");
                return 0;
        }

        // get section table addr, section header number and section header size.
		//printf("e_type:%d\n",ehdr->e_type);
		ptr_sh_table = (u_char*)ehdr + ehdr->e_phoff;
		Elf32_Half phnum = ehdr->e_phnum;
		Elf32_Half phsize = ehdr->e_phentsize;
        // for each section header, output section number and section addr. 
		phdr = (Elf32_Phdr*)ptr_sh_table;

		//for(Nr=0;Nr<shnum;Nr++)
		//{
		//	printf("%d:0x%x\n",Nr,shdr->sh_addr);
		//	shdr += 1;
		//}
        // hint: section number starts at 0.
		int i,j;
		int overlay = 0;
		int conflict = 0;
		for(i=0;i<phnum;i++)
		{
			for(j=i+1;j<phnum;j++)
			{
				if(((phdr+i)->p_vaddr+(phdr+i)->p_filesz+(phdr+i)->p_memsz)/4096 == (phdr+j)->p_vaddr/4096)
				{
					overlay = 1;
					if((phdr+i)->p_vaddr+(phdr+i)->p_filesz+(phdr+i)->p_memsz>(phdr+j)->p_vaddr)
					{
						conflict = 1;
					}
					break;
				}
			}
		}
		
		if(overlay==0&&conflict==0)
		{
			for(i=0;i<phnum;i++)
			{
				printf("%d:0x%x,0x%x\n",i,phdr->p_filesz,phdr->p_memsz);
				phdr++;
			}
		}else if(overlay==1&&conflict==0){
			printf("Overlay at page va : 0x%x\n",(phdr+j)->p_vaddr/4096<<12);
		}else if(overlay==1&&conflict==1){
			printf("Conflict at page va : 0x%x\n",(phdr+j)->p_vaddr/4096<<12);
		}

        return 0;
}

