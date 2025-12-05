# sum.c 逆向分析

## 程式功能說明
```
sum.c 這份程式讓使用者輸入數字n, 之後addUp函式會將1+2+..+n算出來的值傳給sum, 最後印出來加總的值.

**addUp 函式核心邏輯 **
    addUp 函式利用for loop 將總和從1加到參數n, 最後回傳總和.
```

## 編譯與反組譯流程
```
將sum.c 利用gcc sum.c -o sum 編譯成可執行檔
利用objdump 將sum 反組譯成sum.asm
然後輸入objdump -d sum | less 看sum這份檔案的組合語言(AT&T格式)
```

## 分析組合語言
在'less'中輸入'/addUp'跳到'addUp'函式的入口後開始分析組合語言

## addUp組合語言完整拆解
```asm
push   %rbp // 保存caller的 return address 
mov    %rsp,%rbp // 將目前stack top 指標傳給rbp這個基底指標
mov    %edi,-0x14(%rbp) // 將edi(第一個參數暫存器)也就是n 傳給基底指標的偏移位址 放局部變數n
movl   $0x0,-0x8(%rbp) // 因為sum = 0, 將0存給-0x8(%rbp), 這裡存放的是sum的初始值0
movl   $0x1,-0x4(%rbp) // 同理, i = 1 存入-0x4(%rbp)
jmp    1225 <addUp+0x25> // 跳轉到for loop的比較判斷i<=n? 
mov    -0x4(%rbp),%eax //將i存入eax暫存器
add    %eax,-0x8(%rbp) // 執行c code的 sum += i;
addl   $0x1,-0x4(%rbp) // i++
mov    -0x4(%rbp),%eax //再次將i放入eax 等等要進行比較
cmp    -0x14(%rbp),%eax // 比較n 與 i 更新flags
jle     121b // 若(ZF=1 or SF=OF) => i == (n or i < n) => i <= n 就跳轉到121b 也就是迴圈
mov    -0x8(%rbp),%eax // 迴圈結束後 將sum的值存到eax(回傳值)
pop    %rbp // 恢復caller的rbp
ret //函式回傳
```

## 技術總結

### 1.AT&T 語法操作順序
```
mov src dst //來源在前 目的在後
cmp dst src // 計算dst-src後更新flags
```

### 2.jle跳轉
jle 跳轉條件: ZF = 1 or SF != OF (i == n or i < n)

### 3.x86-64 Linux 32位元呼叫慣例
```
第一參數: %edi (32-bits)
整數傳回值: %eax (32-bits)
```