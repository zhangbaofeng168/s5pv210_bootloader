
#ifndef _SETUP_H_
#define _SETUP_H_

#include "hardware.h"

/* The list ends with an ATAG_NONE node. */
#define ATAG_NONE	0x00000000

struct tag_header {
	u32 size;
	u32 tag;
};

/* The list must start with an ATAG_CORE node */
#define ATAG_CORE	0x54410001

struct tag_core {
	u32 flags;		/* bit 0 = read-only */
	u32 pagesize;
	u32 rootdev;
};

/* it is allowed to have multiple ATAG_MEM nodes */
#define ATAG_MEM	0x54410002

struct tag_mem32 {
	u32	size;
	u32	start;	/* physical start address */
};

/* VGA text type displays */
#define ATAG_VIDEOTEXT	0x54410003

struct tag_videotext {
	u8	x;
	u8	y;
	u16	video_page;
	u8	video_mode;
	u8	video_cols;
	u16	video_ega_bx;
	u8	video_lines;
	u8	video_isvga;
	u16	video_points;
};

/* describes how the ramdisk will be used in kernel */
#define ATAG_RAMDISK	0x54410004

struct tag_ramdisk {
	u32 flags;	/* bit 0 = load, bit 1 = prompt */
	u32 size;	/* decompressed ramdisk size in _kilo_ bytes */
	u32 start;	/* starting block of floppy-based RAM disk image */
};

/* describes where the compressed ramdisk image lives (virtual address) */
/*
 * this one accidentally used virtual addresses - as such,
 * its depreciated.
 */
#define ATAG_INITRD		0x54410005

/* describes where the compressed ramdisk image lives (physical address) */
#define ATAG_INITRD2	0x54420005

struct tag_initrd {
	u32 start;	/* physical start address */
	u32 size;	/* size of compressed ramdisk image in bytes */
};

/* board serial number. "64 bits should be enough for everybody" */
#define ATAG_SERIAL		0x54410006

struct tag_serialnr {
	u32 low;
	u32 high;
};

/* board revision */
#define ATAG_REVISION	0x54410007

struct tag_revision {
	u32 rev;
};

/* initial values for vesafb-type framebuffers. see struct screen_info
 * in include/linux/tty.h
 */
#define ATAG_VIDEOLFB	0x54410008

struct tag_videolfb {
	u16	lfb_width;
	u16	lfb_height;
	u16	lfb_depth;
	u16	lfb_linelength;
	u32	lfb_base;
	u32	lfb_size;
	u8	red_size;
	u8	red_pos;
	u8	green_size;
	u8	green_pos;
	u8	blue_size;
	u8	blue_pos;
	u8	rsvd_size;
	u8	rsvd_pos;
};

/* command line: \0 terminated string */
#define ATAG_CMDLINE	0x54410009

struct tag_cmdline {
	char	cmdline[1];	/* this is the minimum size */
};

/* acorn RiscPC specific information */
#define ATAG_ACORN		0x41000101

struct tag_acorn {
	u32	memc_control_reg;
	u32	vram_pages;
	u8	sounddefault;
	u8	adfsdrives;
};

/* footbridge memory clock, see arch/arm/mach-footbridge/arch.c */
#define ATAG_MEMCLK		0x41000402

struct tag_memclk {
	u32	fmemclk;
};

#define ATAG_MTDPART    0x41001099

struct tag_mtdpart {
	u32	mtd_part_size[3];
};

struct tag {
    struct tag_header hdr;
    union { 
        struct tag_core         core;
        struct tag_mem32        mem;
        struct tag_videotext	videotext;
        struct tag_ramdisk      ramdisk;
        struct tag_initrd       initrd;
        struct tag_serialnr     serialnr;
        struct tag_revision     revision;
        struct tag_videolfb     videolfb;
        struct tag_cmdline      cmdline;       
     	/*
        	* Acorn specific
        	*/
        struct tag_acorn        acorn;
     	/*
         	* DC21285 specific
         	*/
        struct tag_memclk       memclk;        
        struct tag_mtdpart      mtdpart_info;
    }u;
};

struct tagtable {
	u32 tag;
	int (*parse)(const struct tag *);
};

#define __tag __attribute__((unused, __section__(".taglist")))

#define __tagtable(tag, fn) \
	static struct tagtable __tagtable_##fn __tag = { tag, fn }

#define tag_member_present(tag, member)				\
	((unsigned long)(&((struct tag *)0L)->member+1) <= (tag)->hdr.size*4)
	
#define tag_next(t) ((struct tag *)((u32 *)(t) + (t)->hdr.size))

#define tag_size(type) ((sizeof(struct tag_header)+sizeof(struct type))>>2)

#define for_each_tag(t, base) for(t = base; t->hdr.size; t = tag_next(t))


#endif
