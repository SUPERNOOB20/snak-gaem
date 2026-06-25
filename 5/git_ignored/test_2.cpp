#include <iostream>

int g_my_global = 0;

void func() { g_my_global = 10; }

int main() { func(); printf("%d",g_my_global); return 0; }
