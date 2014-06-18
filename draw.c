#include "launchelf.h"
#include "font5200.c"

itoGsEnv screen_env;

// ELISA100.FNTに存在しない文字
const uint16 font404[] = {
	0xA2AF, 11,
	0xA2C2, 8,
	0xA2D1, 11,
	0xA2EB, 7,
	0xA2FA, 4,
	0xA3A1, 15,
	0xA3BA, 7,
	0xA3DB, 6,
	0xA3FB, 4,
	0xA4F4, 11,
	0xA5F7, 8,
	0xA6B9, 8,
	0xA6D9, 38,
	0xA7C2, 15,
	0xA7F2, 13,
	0xA8C1, 720,
	0xCFD4, 43,
	0xF4A5, 1030,
	0,0
};

// ASCIIとSJISの変換用配列
const unsigned char sjis_lookup_81[256] = {
  0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,  // 0x00
  0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,  // 0x10
  0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,  // 0x20
  0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,  // 0x30
  ' ', ',', '.', ',', '.', 0xFF,':', ';', '?', '!', 0xFF,0xFF,'ｴ', '`', 0xFF,'^',   // 0x40
  0xFF,'_', 0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,'0', 0xFF,'-', '-', 0xFF,0xFF,  // 0x50
  0xFF,0xFF,0xFF,0xFF,0xFF,'\'','\'','"', '"', '(', ')', 0xFF,0xFF,'[', ']', '{',   // 0x60
  0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,'+', '-', 0xFF,'*', 0xFF,  // 0x70
  '/', '=', 0xFF,'<', '>', 0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,'ｰ', 0xFF,0xFF,'ｰ', 0xFF,  // 0x80
  '$', 0xFF,0xFF,'%', '#', '&', '*', '@', 0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,  // 0x90
  0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,  // 0xA0
  0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,  // 0xB0
  0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,'&', '|', '!', 0xFF,0xFF,0xFF,0xFF,0xFF,  // 0xC0
  0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,  // 0xD0
  0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,  // 0xE0
  0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,  // 0xF0
};
const unsigned char sjis_lookup_82[256] = {
  0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,  // 0x00
  0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,  // 0x10
  0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,  // 0x20
  0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,  // 0x30
  0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,'0',   // 0x40
  '1', '2', '3', '4', '5', '6', '7', '8', '9', 0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,  // 0x50
  'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',   // 0x60
  'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,  // 0x70
  0xFF,'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o',   // 0x80
  'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', 0xFF,0xFF,0xFF,0xFF,0xFF,  // 0x90
  0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,  // 0xA0
  0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,  // 0xB0
  0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,  // 0xC0
  0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,  // 0xD0
  0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,  // 0xE0
  0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,  // 0xF0
};


////////////////////////////////////////////////////////////////////////
// 画面表示のテンプレート
void setScrTmp(const char *msg0, const char *msg1)
{
	int x, y;
	
	// バージョン表記
	x = SCREEN_MARGIN;
	y = SCREEN_MARGIN;
	printXY(" ■ LaunchELF v3.48 ■",
		SCREEN_WIDTH-SCREEN_MARGIN-FONT_WIDTH*22, y/2, setting->color[1], TRUE);
	y += FONT_HEIGHT+4;
	
	// メッセージ
	printXY(msg0, x, y/2, setting->color[2], TRUE);
	y += FONT_HEIGHT;
	
	// 枠
	drawFrame(SCREEN_MARGIN, y/2,
		SCREEN_WIDTH-SCREEN_MARGIN-1,
		(SCREEN_HEIGHT-SCREEN_MARGIN-FONT_HEIGHT-2)/2,
		setting->color[1]);
	
	// 操作説明
	x = SCREEN_MARGIN;
	y = SCREEN_HEIGHT-SCREEN_MARGIN-FONT_HEIGHT;
	printXY(msg1, x, y/2, setting->color[2], TRUE);
}

////////////////////////////////////////////////////////////////////////
// メッセージ描画
void drawMsg(const char *msg)
{
	itoSprite(setting->color[0], 0, (SCREEN_MARGIN+FONT_HEIGHT+4)/2,
		SCREEN_WIDTH, (SCREEN_MARGIN+FONT_HEIGHT+4+FONT_HEIGHT)/2, 0);
	printXY(msg, SCREEN_MARGIN, (SCREEN_MARGIN+FONT_HEIGHT+4)/2,
		setting->color[2], TRUE);
	drawScr();
}

////////////////////////////////////////////////////////////////////////
// setup ito
void setupito(void)
{
	itoInit();

	// screen resolution
	screen_env.screen.width		= 512;
	screen_env.screen.height	= 480;
	screen_env.screen.psm		= ITO_RGBA32;

	// These setting work best with my tv, experiment for youself
	screen_env.screen.x			= setting->screen_x; 
	screen_env.screen.y			= setting->screen_y;
	
	screen_env.framebuffer1.x	= 0;
	screen_env.framebuffer1.y	= 0;
	
	screen_env.framebuffer2.x	= 0;
	screen_env.framebuffer2.y	= 480;

	// zbuffer
	screen_env.zbuffer.x		= 0;
	screen_env.zbuffer.y		= 480*2;
	screen_env.zbuffer.psm		= ITO_ZBUF32;
	
	// scissor 
	screen_env.scissor_x1		= 0;
	screen_env.scissor_y1		= 0;
	screen_env.scissor_x2		= 512;
	screen_env.scissor_y2		= 480;
	
	// misc
	screen_env.dither			= TRUE;
	screen_env.interlace		= setting->interlace;
	screen_env.ffmode			= ITO_FRAME;
	screen_env.vmode			= ITO_VMODE_AUTO;
	
	itoGsEnvSubmit(&screen_env);
	
	itoSetBgColor(setting->color[0]);
}

////////////////////////////////////////////////////////////////////////
// 画面のクリア
void clrScr(uint64 color)
{
	itoSprite(color, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
}

////////////////////////////////////////////////////////////////////////
// 画面の描画
void drawScr(void)
{
	itoGsFinish();
	itoVSync();
	itoSwitchFrameBuffers();
}

////////////////////////////////////////////////////////////////////////
// 枠の描画
void drawFrame(int x1, int y1, int x2, int y2, uint64 color)
{
	itoLine(color, x1, y1, 0, color, x2, y1, 0);
	
	itoLine(color, x2, y1, 0, color, x2, y2, 0);
	itoLine(color, x2-1, y1, 0, color, x2-1, y2, 0);
	
	itoLine(color, x2, y2, 0, color, x1, y2, 0);
	
	itoLine(color, x1, y2, 0, color, x1, y1, 0);
	itoLine(color, x1+1, y2, 0, color, x1+1, y1, 0);
}

////////////////////////////////////////////////////////////////////////
// draw a char using the system font (8x8)
void drawChar(unsigned char c, int x, int y, uint64 colour)
{
	unsigned int i, j;
	unsigned char cc;

	for(i=0; i<8; i++)
	{
		cc = font5200[(c-32)*8+i];
		for(j=0; j<8; j++)
		{
			if(cc & 0x80) itoPoint(colour, x+j, y+i, 0);
			cc = cc << 1;
		}
	}
}

////////////////////////////////////////////////////////////////////////
// draw a char using the ELISA font (8x8)
void drawChar2(int32 n, int x, int y, uint64 colour)
{
	unsigned int i, j;
	uint8 b;
	
	for(i=0; i<8; i++)
	{
		b = elisaFnt[n+i];
		for(j=0; j<8; j++)
		{
			if(b & 0x80) {
				itoPoint(colour, x+j, y+i, 0);
			}
			b = b << 1;
		}
	}
}

////////////////////////////////////////////////////////////////////////
// draw a string of characters
int printXY(const unsigned char *s, int x, int y, uint64 colour, int draw)
{
	int32 n;
	unsigned char ascii;
	uint16 code;
	int i, j, tmp;
	
	i=0;
	while(s[i]){
		if(s[i] & 0x80) {
			// SJISコードの生成
			code = s[i++];
			code = (code<<8) + s[i++];
			
			switch(code){
			// ○
			case 0x819B:
				if(draw){
					drawChar(160, x, y, colour);
					drawChar(161, x+8, y, colour);
				}
				x+=16;
				break;
			// ×
			case 0x817E:
				if(draw){
					drawChar(162, x, y, colour);
					drawChar(163, x+8, y, colour);
				}
				x+=16;
				break;
			// □
			case 0x81A0:
				if(draw){
					drawChar(164, x, y, colour);
					drawChar(165, x+8, y, colour);
				}
				x+=16;
				break;
			// △
			case 0x81A2:
				if(draw){
					drawChar(166, x, y, colour);
					drawChar(167, x+8, y, colour);
				}
				x+=16;
				break;
			// ■
			case 0x81A1:
				if(draw){
					drawChar(168, x, y, colour);
					drawChar(169, x+8, y, colour);
				}
				x+=16;
				break;
			default:
				if(elisaFnt!=NULL){
					tmp=y;
					if(code<=0x829A) tmp++;
					// SJISからEUCに変換
					if(code >= 0xE000) code-=0x4000;
					code = ((((code>>8)&0xFF)-0x81)<<9) + (code&0x00FF);
					if((code & 0x00FF) >= 0x80) code--;
					if((code & 0x00FF) >= 0x9E) code+=0x62;
					else code-=0x40;
					code += 0x2121 + 0x8080;
					
					// EUCから恵梨沙フォントの番号を生成
					n = (((code>>8)&0xFF)-0xA1)*(0xFF-0xA1)
						+ (code&0xFF)-0xA1;
					j=0;
					while(font404[j]) {
						if(code >= font404[j]) {
							if(code <= font404[j]+font404[j+1]-1) {
								n = -1;
								break;
							} else {
								n-=font404[j+1];
							}
						}
						j+=2;
					}
					n*=8;
					
					if(n>=0 && n<=55008) {
						if(draw) drawChar2(n, x, tmp, colour);
						x+=9;
					}else{
						if(draw) drawChar('_', x, y, colour);
						x+=8;
					}
				}else{
					ascii=0xFF;
					if(code>>8==0x81)
						ascii = sjis_lookup_81[code & 0x00FF];
					else if(code>>8==0x82)
						ascii = sjis_lookup_82[code & 0x00FF];
					if(ascii!=0xFF){
						if(draw) drawChar(ascii, x, y, colour);
					}else{
						if(draw) drawChar('_', x, y, colour);
					}
					x+=8;
				}
				break;
			}
		}else{
			if(draw) drawChar(s[i], x, y, colour);
			i++;
			x += 8;
		}
		if(x > SCREEN_WIDTH-SCREEN_MARGIN-FONT_WIDTH){
			//x=16; y=y+8;
			return x;
		}
	}
	
	return x;
}
