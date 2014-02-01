#include "aq.h"

char scancodes[]={
	0,
	0,	//esc
	'1','2','3','4','5','6','7','8','9','0','-','=',
	8,	//backspace
	'\t', //tab
	'q','w','e','r','t','y','u','i','o','p','[',']',
	'\n', //enter
	0, //ctrl
	'a','s','d','f','g','h','j','k','l',';','\'','`',
	0, //left shift
	'\\',
	'z','x','c','v','b','n','m',',','.','/',
	0, //right shift
	'*', //numpad
	0, //alt
	' ',
	0, //caps lock
	0,0,0,0,0,0,0,0,0,0, //f1 - f10
	0, //num lock
	0, //scroll lock
	0, //home
	0,
	0, //page up
	'-', //numpad
	0, 0,
	0, //(r)
	'+', //numpad
	0, //end
	0,
	0, //page down
	0, //ins
	0, //del
	0, //sys rq
	0,
	0,0, //f11, f12
	0,
	0,0,0, //f13 - 15
	0,0,0,0,0,0,0,0,0, //f16 - f24
	0,0,0,0,0,0,0,0
};

char scancodes_shifted[]={
	0,
	0,	//ESC
	'!','@','#','$','%','^','&','*','(',')','_','+',
	8,	//BACKSPACE
	'\t', //TAB
	'Q','W','E','R','T','Y','U','I','O','P','{','}',
	'\n', //ENTER
	0, //CTRL
	'A','S','D','F','G','H','J','K','L',':','"','~',
	0, //LEFT SHIFT
	'|',
	'Z','X','C','V','B','N','M','<','>','?',
	0, //RIGHT SHIFT
	'*', //NUMPAD
	0, //ALT
	' ',
	0, //CAPS LOCK
	0,0,0,0,0,0,0,0,0,0, //F1 - F10
	0, //NUM LOCK
	0, //SCROLL LOCK
	0, //HOME
	0,
	0, //PAGE UP
	'-', //NUMPAD
	0, 0,
	0, //(R)
	'+', //NUMPAD
	0, //END
	0,
	0, //PAGE DOWN
	0, //INS
	0, //DEL
	0, //SYS RQ
	0,
	0,0, //F11, F12
	0,
	0,0,0, //F13 - 15
	0,0,0,0,0,0,0,0,0, //F16 - F24
	0,0,0,0,0,0,0,0
};

typedef struct {
    int escaped;
    bool shift;
    bool ctrl;
    bool alt;
    bool num_lock;
    bool caps_lock;
    bool scroll_lock;
    bool leds_changed;
    unsigned char leds;
    bool busy;
} kbd_state_t;

kbd_state_t kbd_state;

void keyboard(void)
{
    unsigned char scancode, ascii, creg;
    scancode = inportb(0x60);
    switch(scancode) {
        case 0x36:
        case 0x2a:
            kbd_state.shift = true;
			ascii = 0;
            break;
            
        case 0x36+0x80:
        case 0x2a+0x80:
            kbd_state.shift = false;
			ascii = 0;
            break;
            
        case 0x1D:
            kbd_state.ctrl = true;
			ascii = 0;
            break;
            
        case 0x1D+0x80:
            kbd_state.ctrl = false;
			ascii = 0;
            break;
            
        case 0x38:
            kbd_state.alt = true;
			ascii = 0;
            break;
            
        case 0x38+0x80:
            kbd_state.alt = false;
			ascii = 0;
            break;
            
        default:

	    if (kbd_state.escaped) {

		switch (kbd_state.escaped) {

		    case 0xE0:
			switch (scancode) {
			    case 0x48:		//up
				ascii = 0x1C;
				break;

			    case 0x4B:		//left
				ascii = 0x1D;
				break;

			    case 0x4D:		//right
				ascii = 0x1E;
				break;

			    case 0x50:		//down
				ascii = 0x1F;
				break;
			}
			break;
		    
		    default:
			break;
		}
		
		kbd_state.escaped = 0;
	    } else {
		if (scancode == 0xE0) {
		    kbd_state.escaped = 0xE0;
		} else {
		    if(kbd_state.shift) {
			ascii = scancodes_shifted[scancode];
		    } else {
			ascii = scancodes[scancode];
		    }
		}
	    }
            break;
    }
    creg = inportb(0x61);
    creg |= 1;
    outportb(0x61, creg);
    if (scancode < 0x80) {
	keypress_handler (scancode, ascii);
    } else {
	keyrelease_handler (scancode - 0x80, ascii);
    }
}

