#include <aquarius.h>
#include "aq.h"

void write_help_str(char *cmd, char *descr)
{
	attr(CYAN);
	Write(cmd);
	attr(WHITE);
	Write("\t");
	Write(descr);
	Write("\n");
}

void help_general()
{
	write_help_str("help", "show this help");
	write_help_str("version", "show os version");
	write_help_str("echo", "echo args");
	write_help_str("exec", "start a new process");
	Write("\nUse ");
	attr(CYAN);
	Write("help cmd");
	attr(WHITE);
	Write(" to get help on command 'cmd'.\n");
}

void help_command(char *cmd, char *fmt, char *descr)
{
	attr(MAGENTA);
	Write("COMMAND:\n\t");
	attr(CYAN);
	Write(cmd);
	attr(MAGENTA);
	Write("\nFORMAT:\n\t");
	attr(CYAN);
	Write(cmd);
	Write(" ");
	attr(BROWN);
	Write(fmt);
	attr(MAGENTA);
	Write("\nDESCRIPTION:\n\t");
	attr(WHITE);
	Write(descr);
	Write("\n");
}

void help_subj(char *s)
{
	if (!strcmp(s, "help")) {
		help_command("help", "[command]", "Describes a given command or gives a list of supported ones, if nothing specified.");
	} else if (!strcmp(s, "echo")) {
		help_command("echo", "text", "Echoes a given text.");
	} else if (!strcmp(s, "version")) {
		help_command("version", "", "Display OS version.");
	} else if (!strcmp(s, "exec")) {
		help_command("exec", "file", "Start a new process. For example: exec /initrd/hello");
	} else {
		help_general();
	}
}

char kbuffer[256];
int knext = 0;
void keypress_handler(int scancode, char ascii)
{
	char s[2] = {0};
	s[0] = ascii;
	Write(s);
	kbuffer[knext++] = ascii;
	if (ascii == 8) {
		knext -= 2;
	}
	if (ascii == '\n') {
		char *cmd = kbuffer;
		char *arg = strchr(kbuffer, ' ');
		if (arg) {
			*arg = 0;
			arg++;
			arg[strlen(arg) - 1] = 0;
		}
		if (cmd[strlen(cmd) - 1] == '\n') cmd[strlen(cmd) - 1] = 0;
		if (!strcmp(cmd, "version")) {
			Write("Aquarius Alpha by Alex Orlenko\n");
		} else if (!strcmp(cmd, "help")) {
			if (!arg) {
				help_general();
			} else {
				help_subj(arg);
			}
		} else if (!strcmp(cmd, "echo")) {
			Write(arg);
		} else if (!strcmp(cmd, "exec")) {
			LoadProcess(arg);
		} else if (!strcmp(cmd, "game")) {
			LoadProcess("/initrd/game");
			for (;;) ;
		} else if (!strcmp(cmd, "")) {
		} else {
			Write("Unknown command: ");
			Write(cmd);
			Write("\nTry "); attr(CYAN); Write("help"); attr(WHITE); Write(" to get a list of supported commands\n");
		}

		int i;
		for (i = 0; i < knext; i++)
			kbuffer[i] = 0;
		knext = 0;
		Write("\n$ ");
	}
}

void keyrelease_handler(int scancode, char ascii)
{
}

int main()
{
	Write("Aquarius Alpha\nCopyright (c) 2011-2013 Alex Orlenko\n");
	Write("\n$ ");

	bool kbd;
	for (;;) {
		SystemCall(2, 0, &kbd);
		if (kbd) {
			keyboard();
		}
	}
	return 0;
}
