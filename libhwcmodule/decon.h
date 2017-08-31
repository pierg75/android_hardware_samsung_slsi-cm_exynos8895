#ifndef __DECON_FB_H__
#define __DECON_FB_H__

#define MAX_DECON_WIN           6
#define MAX_DECON_EXT_WIN       3
#define MAX_BUF_PLANE_CNT       3
#define MAX_PLANE_CNT		3
#define DECON_WIN_UPDATE_IDX	(6)

typedef __u64 dma_addr_t;

struct decon_win_rect {
	int x;
	int y;
	u32 w;
	u32 h;
};

struct decon_rect {
	u32 left;
	u32 top;
	u32 right;
	u32 bottom;
};

struct s3c_fb_user_plane_alpha {
    int             channel;
    unsigned char   red;
    unsigned char   green;
    unsigned char   blue;
};

struct s3c_fb_user_chroma {
    int             enabled;
    unsigned char   red;
    unsigned char   green;
    unsigned char   blue;
};

struct s3c_fb_user_ion_client {
    int     fd[MAX_BUF_PLANE_CNT];
    int     offset;
};

enum decon_idma_type {
	IDMA_G0 = 0,	/* Dedicated to WIN5 */
	IDMA_G1,
	IDMA_VG0,
	IDMA_VG1,
	IDMA_VGF0,
	IDMA_VGF1,
	ODMA_WB,
	IDMA_G0_S,
};

enum decon_pixel_format {
	/* RGB 32bit */
	DECON_PIXEL_FORMAT_ARGB_8888 = 0,
	DECON_PIXEL_FORMAT_ABGR_8888,
	DECON_PIXEL_FORMAT_RGBA_8888,
	DECON_PIXEL_FORMAT_BGRA_8888,
	DECON_PIXEL_FORMAT_XRGB_8888,
	DECON_PIXEL_FORMAT_XBGR_8888,
	DECON_PIXEL_FORMAT_RGBX_8888,
	DECON_PIXEL_FORMAT_BGRX_8888,
	/* RGB 16 bit */
	DECON_PIXEL_FORMAT_RGBA_5551,
	DECON_PIXEL_FORMAT_RGB_565,
	/* YUV422 2P */
	DECON_PIXEL_FORMAT_NV16,
	DECON_PIXEL_FORMAT_NV61,
	/* YUV422 3P */
	DECON_PIXEL_FORMAT_YVU422_3P,
	/* YUV420 2P */
	DECON_PIXEL_FORMAT_NV12,
	DECON_PIXEL_FORMAT_NV21,
	DECON_PIXEL_FORMAT_NV12M,
	DECON_PIXEL_FORMAT_NV21M,
	/* YUV420 3P */
	DECON_PIXEL_FORMAT_YUV420,
	DECON_PIXEL_FORMAT_YVU420,
	DECON_PIXEL_FORMAT_YUV420M,
	DECON_PIXEL_FORMAT_YVU420M,
	/* YUV - support for single plane */
	DECON_PIXEL_FORMAT_NV12N,
	DECON_PIXEL_FORMAT_NV12N_10B,

	DECON_PIXEL_FORMAT_MAX,
};

enum decon_blending {
    DECON_BLENDING_NONE = 0,
    DECON_BLENDING_PREMULT = 1,
    DECON_BLENDING_COVERAGE = 2,
    DECON_BLENDING_MAX = 3,
};

enum dpp_flip {
    DPP_FLIP_NONE = 0x0,
    DPP_FLIP_X,
    DPP_FLIP_Y,
    DPP_FLIP_XY,
};

enum dpp_csc_eq {
    /* eq_mode : 6bits [5:0] */
    CSC_STANDARD_SHIFT = 0,
    CSC_BT_601 = 0,
    CSC_BT_709 = 1,
    CSC_BT_2020 = 2,
    CSC_DCI_P3 = 3,
    /* eq_mode : 3bits [8:6] */
    CSC_RANGE_SHIFT = 6,
    CSC_RANGE_LIMITED = 0x0,
    CSC_RANGE_FULL = 0x1,
};

struct dpp_params {
	dma_addr_t addr[MAX_PLANE_CNT];
	enum dpp_flip flip;
	enum dpp_csc_eq eq_mode;
	enum dpp_comp_src comp_src;
};

struct decon_frame {
	int x;
	int y;
	u32 w;
	u32 h;
	u32 f_w;
	u32 f_h;
};

struct decon_win_config {
    enum {
        DECON_WIN_STATE_DISABLED = 0,
        DECON_WIN_STATE_COLOR,
        DECON_WIN_STATE_BUFFER,
        DECON_WIN_STATE_UPDATE,
    } state;
    
    union {
        __u32 color;
        struct {
            int                             fd_idma[3];
            int                             fence_fd;
            int                             plane_alpha;
            enum decon_blending             blending;
            enum decon_idma_type            idma_type;
            enum decon_pixel_format         format;
            struct dpp_params               dpp_parm;
            /* no read area of IDMA */
            struct decon_win_rect           block_area;
            struct decon_win_rect           transparent_area;
            struct decon_win_rect           opaque_area;
            /* source framebuffer coordinates */
            struct decon_frame              src;
        };
    };
    
    /* destination OSD coordinates */
    struct decon_frame dst;
    bool protection;
    bool compression;
};

struct decon_win_config_data {
    int     fence;
    int     fd_odma;
    struct decon_win_config config[MAX_DECON_WIN + 1];
};


#define MAX_DPP_LOG	10
/* IOCTL commands */
#define S3CFB_WIN_POSITION              _IOW('F', 203, \
struct decon_user_window)
#define S3CFB_WIN_SET_PLANE_ALPHA       _IOW('F', 204, \
struct s3c_fb_user_plane_alpha)
#define S3CFB_WIN_SET_CHROMA            _IOW('F', 205, \
struct s3c_fb_user_chroma)
#define S3CFB_SET_VSYNC_INT             _IOW('F', 206, __u32)

#define S3CFB_GET_ION_USER_HANDLE       _IOWR('F', 208, \
struct s3c_fb_user_ion_client)
#define S3CFB_WIN_CONFIG                _IOW('F', 209, \
struct decon_win_config_data)
#define S3CFB_WIN_PSR_EXIT              _IOW('F', 210, int)

#define S3CFB_SET_VSYNC_INT		_IOW('F', 206, __u32)
#define S3CFB_DECON_SELF_REFRESH	_IOW('F', 207, __u32)
#define S3CFB_WIN_CONFIG		_IOW('F', 209, \
						struct decon_win_config_data)

#define S3CFB_START_CRC			_IOW('F', 270, u32)
#define S3CFB_SEL_CRC_BITS		_IOW('F', 271, u32)
#define S3CFB_GET_CRC_DATA		_IOR('F', 272, u32)

#define EXYNOS_GET_DISPLAYPORT_CONFIG		_IOW('F', 300, \
						struct exynos_displayport_data)
#define EXYNOS_SET_DISPLAYPORT_CONFIG		_IOW('F', 301, \
						struct exynos_displayport_data)
#define EXYNOS_DPU_DUMP		_IOW('F', 302, \
						struct decon_win_config_data)
#define S3CFB_POWER_MODE		_IOW('F', 223, __u32)
enum doze_mode {
	DECON_PWR_OFF = 0,
	DECON_PWR_DOZE,
	DECON_PWR_NORMAL,
	DECON_PWR_DOZE_SUSPEND,
	DECON_PWR_MAX,
};

#define V4L2_EVENT_DECON				(V4L2_EVENT_PRIVATE_START + 1000)
#define V4L2_EVENT_DECON_FRAME_START	(V4L2_EVENT_DECON + 1)
#define V4L2_EVENT_DECON_FRAME_DONE		(V4L2_EVENT_DECON + 2)
#define V4L2_EVENT_DECON_VSYNC			(V4L2_EVENT_DECON + 3)

#ifdef CONFIG_SUPPORT_DSU
int dsu_win_config(struct decon_device *decon, struct decon_win_config *windata, struct decon_reg_data *regs);
void dpu_dump_winconfig(struct decon_device *decon, struct decon_win_config *windata);
int dpu_set_dsu_update_config(struct decon_device *decon, struct decon_reg_data *regs);
void decon_set_dsu_update(u32 id, enum decon_dsi_mode dsi_mode, struct decon_param *p);
#endif

#endif
