typedef unsigned int DWORD;

void ExitProcess(DWORD code);

void _start(void) {
    ExitProcess(1);
}
