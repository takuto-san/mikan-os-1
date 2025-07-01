; asmfunc.asm:
;   アセンブリでしか書けない処理をまとめたファイル
;
; System V AMD64 Calling Convention
; Registers: RDI, RSI, RDX, RCX, R8, R9

bits 64
section .text

; day06b
; IoOut32:
;   CPUから特定のハードウェア部品に対して、命令やデータを送りつけるための関数
;   I/Oポートマッピング方式でデバイスと通信する際に使用する
global IoOut32  ; void IoOut32(uint16_t addr, uint32_t data);
IoOut32:
    mov dx, di      ; 第1引数(di)のポートアドレスをdxレジスタに格納
    mov eax, esi    ; 第2引数(esi)の出力データをeaxレジスタに格納
    out dx, eax     ; dxレジスタが示すI/Oポートに、eaxレジスタのデータ(32bit)を出力
    ret             ; 呼び出し元に戻る

; IoIn32:
;   CPUが特定のハードウェア部品から、現在の状態や値を読み取るための関数
;   I/Oポートマッピング方式でデバイスと通信する際に使用する
global IoIn32   ; uint32_t IoIn32(uint16_t addr);
IoIn32:
    mov dx, di      ; 第1引数(di)のポートアドレスをdxレジスタに格納
    in eax, dx      ; dxレジスタが示すI/Oポートからデータ(32bit)を読み込み、eaxレジスタに格納
    ret             ; 戻り値(eax)を返して呼び出し元に戻る

; day07a
; GetCS:
;   CPUが現在どのような権限やモードでプログラムを実行しているかを示す
;   コードセグメント(CS)レジスタの値を取得するための関数
global GetCS    ; uint16_t GetCS(void);
GetCS:
    xor eax, eax    ; eaxレジスタを0でクリア (xor命令は高速。raxの上位32bitもクリアされる)
    mov ax, cs      ; 現在のコードセグメント(cs)の値をax(eaxの下位16bit)にコピー
    ret             ; 戻り値(ax)を返して呼び出し元に戻る

; LoadIDT:
;   IDT(Interrupt Descriptor Table)をCPUのIDTRレジスタにロードする
;   IDTは割り込みや例外が発生した際のジャンプ先を定義したテーブル
global LoadIDT  ; void LoadIDT(uint16_t limit, uint64_t offset);
LoadIDT:
    push rbp            ; ベースポインタをスタックに保存
    mov rbp, rsp        ; 新しいベースポインタを設定
    sub rsp, 10         ; IDTRのための10バイトをスタックに確保 (limit: 2バイト, offset: 8バイト)
    mov [rsp], di       ; 第1引数(di)の limit 値をスタックにコピー
    mov [rsp + 2], rsi  ; 第2引数(rsi)の offset 値をスタックにコピー
    lidt [rsp]          ; スタック上のデータを使ってIDTRをロード
    mov rsp, rbp        ; スタックポインタを復元
    pop rbp             ; ベースポインタを復元
    ret                 ; 呼び出し元に戻る

; day08b
; LoadGDT:
;   GDTをCPUのGDTRレジスタにロードする
;   GDTの大きさと場所をレジスタに登録する
;   これでCPUが新しいGDTの方を見にいくようになる
global LoadGDT  ; void LoadGDT(uint16_t limit, uint64_t offset);
LoadGDT:
    push rbp            ; ベースポインタをスタックに保存
    mov rbp, rsp        ; 新しいベースポインタを設定
    sub rsp, 10         ; GDTRのための10バイトをスタックに確保 (limit: 2バイト, offset: 8バイト)
    mov [rsp], di       ; 第1引数(di)の limit 値をスタックにコピー
    mov [rsp + 2], rsi  ; 第2引数(rsi)の offset 値をスタックにコピー
    lgdt [rsp]          ; スタック上のデータを使ってGDTRをロード
    mov rsp, rbp        ; スタックポインタを復元
    pop rbp             ; ベースポインタを復元
    ret                 ; 呼び出し元に戻る

; SetCSSS:
;   コードセグメント(CS)とスタックセグメント(SS)を更新する
;   CSにはUEFIが設定した値が入ったままなので、CSとSSに値を設定する
;   retf命令を使ってCSとRIPを同時に変更する
global SetCSSS  ; void SetCSSS(uint16_t cs, uint16_t ss);
SetCSSS:
    push rbp            ; ベースポインタをスタックに保存
    mov rbp, rsp        ; 新しいベースポインタを設定
    mov ss, si          ; 第2引数(si)の値でスタックセグメント(SS)を更新
    mov rax, .next      ; .nextラベルのアドレス(ジャンプ先のRIP)をraxにロード
    push rdi            ; 第1引数(di)の新しいCSセレクタをスタックにプッシュ
    push rax            ; ジャンプ先のRIPをスタックにプッシュ
    o64 retf            ; far return: スタックからCSとRIPをポップし、ジャンプする
.next:
    mov rsp, rbp        ; スタックポインタを復元
    pop rbp             ; ベースポインタを復元
    ret                 ; 呼び出し元に戻る

; SetDSAll:
;   4つのセグメントレジスタ(DS, ES, FS, GS)に引数をコピーする
;   セグメントレジスタとは現在CPUが使用しているメモリ区画（セグメント）を指し示す特殊レジスタ
global SetDSAll  ; void SetDSAll(uint16_t value);
SetDSAll:
    mov ds, di          ; データセグメント(DS)に値を設定
    mov es, di          ; エクストラセグメント(ES)に値を設定
    mov fs, di          ; FSセグメントに値を設定
    mov gs, di          ; GSセグメントに値を設定
    ret                 ; 呼び出し元に戻る

; SetCR3:
;   ページングで使用するページテーブルの物理アドレスをCR3レジスタに設定する
;   PLM4テーブルの物理アドレスをCR3レジスタに設定する
;   CR3を書き換えた直後からCPUは新しい階層ページング構造を使ってアドレス変換するようになる
global SetCR3  ; void SetCR3(uint64_t value);
SetCR3:
    mov cr3, rdi        ; 第1引数(rdi)の値をCR3レジスタにロード
    ret                 ; 呼び出し元に戻る

; C言語などで書かれたカーネル本体のスタック領域(外部シンボル)
extern kernel_main_stack
; C言語などで書かれたカーネル本体の関数(外部シンボル)
extern KernelMainNewStack

; KernelMain:
;   スタックを切り替えつつKernelMainNewStackを呼び出す関数
;   UEFIはこれからはC++の関数ではなく、新しくアセンブリで書いた関数を呼び出してくれるようになる
global KernelMain
KernelMain:
    ; RSPを設定。設定している値はmain.cppで用意したメモリ領域kernel_main_stackの末尾のアドレス
    mov rsp, kernel_main_stack + 1024 * 1024
    ; main.cppのKernekMainNewStackを呼び出す
    call KernelMainNewStack
.fin:
    ; KernelMainNewStackがもしリターンした場合、CPUを停止させて暴走を防ぐための無限ループ
    hlt
    jmp .fin