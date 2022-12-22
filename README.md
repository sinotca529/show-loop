# show-loop
プログラム中の各ループについて，それを構成するベーシックブロックを出力する LLVM Pass ．

## ビルド方法
1. LLVM13.0.1 の入手．
    1. 自分のマシンに合った [LLVM13.0.1](https://github.com/llvm/llvm-project/releases/tag/llvmorg-13.0.1) をダウンロードする．
    2. ダウンロードしたファイルを展開し，適当な場所に置く．
2. LLVM のライブラリの場所を cmake に教える．
    1. このリポジトリ内の CMakeLists.txt を開く．
    2. 13行目の `HINTS "$ENV{LLVM13}/lib/cmake"` のパスを，1でLLVMを展開したパスをもとに書き換える．
        - あるいは環境変数 `LLVM13` を適当に export する．
3. ビルド
    1. このリポジトリの CMakeLists.txt があるディレクトリに移動する．
    2. ビルドスクリプトの生成 : `cmake -G Ninja -B build`
    3. ビルド : `cmake --build build`

## 試し方
### 1. 解析対象の用意
解析対象の C コードを適当に書く．

C コードの例 (main.c):
```c
#include <stdio.h>

int main(void) {
    for (int i = 0; i < 10; ++i) {
        printf("%d\n", i);
    }

    for (int j = 0; j < 10; ++j) {
        printf("%d\n", j);
    }
    return 0;
}
```

### 2. コンパイル時解析
コマンド :
```
clang -g -fno-legacy-pass-manager -fpass-plugin=<path to ShowLoop.so> <path to main.c>
```

結果 :
```
Visit function main
-----------------

14:                                               ; preds = %20, %13
  %15 = load i32, i32* %3, align 4, !dbg !34
  %16 = icmp slt i32 %15, 10, !dbg !36
  br i1 %16, label %17, label %23, !dbg !37

17:                                               ; preds = %14
  %18 = load i32, i32* %3, align 4, !dbg !38
  %19 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str, i64 0, i64 0), i32 %18), !dbg !40
  br label %20, !dbg !41

20:                                               ; preds = %17
  %21 = load i32, i32* %3, align 4, !dbg !42
  %22 = add nsw i32 %21, 1, !dbg !42
  store i32 %22, i32* %3, align 4, !dbg !42
  br label %14, !dbg !43, !llvm.loop !44
-----------------

4:                                                ; preds = %10, %0
  %5 = load i32, i32* %2, align 4, !dbg !17
  %6 = icmp slt i32 %5, 10, !dbg !19
  br i1 %6, label %7, label %13, !dbg !20

7:                                                ; preds = %4
  %8 = load i32, i32* %2, align 4, !dbg !21
  %9 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @.str, i64 0, i64 0), i32 %8), !dbg !23
  br label %10, !dbg !24

10:                                               ; preds = %7
  %11 = load i32, i32* %2, align 4, !dbg !25
  %12 = add nsw i32 %11, 1, !dbg !25
  store i32 %12, i32* %2, align 4, !dbg !25
  br label %4, !dbg !26, !llvm.loop !27

```
