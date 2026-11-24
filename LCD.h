#include"types.h"
void Writelcd(u8 dat);
void Cmdlcd(u8 cmd);
void Charlcd(u8 asciival);
void Initlcd(void);
void Strlcd(s8 *);
void u32lcd(u32);
void s32lcd(s32);
void f32lcd(f32,u8);
void BuildCGRAM(u8*,u8);

